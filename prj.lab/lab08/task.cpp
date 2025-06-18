#include <iostream>
#include <opencv2/opencv.hpp>

cv::Mat grayWorld(cv::Mat image) {
    std::vector<cv::Mat> channels;
    cv::split(image, channels);

    cv::Scalar meanB = cv::mean(channels[0]);
    cv::Scalar meanG = cv::mean(channels[1]);
    cv::Scalar meanR = cv::mean(channels[2]);

    float normB = 128.0f / meanB[0];
    float normG = 128.0f / meanG[0];
    float normR = 128.0f / meanR[0];


    channels[0] = channels[0] * normB;
    channels[1] = channels[1] * normG;
    channels[2] = channels[2] * normR;

    cv::Mat norm;
    cv::merge(channels, norm);
    return norm;
}

cv::Mat advancedColorCorrection(const cv::Mat& input) {

    cv::Mat img = input.clone();
    cv::Mat lab;
    cv::cvtColor(img, lab, cv::COLOR_BGR2Lab);
    std::vector<cv::Mat> labChannels;
    cv::split(lab, labChannels);
    cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE();
    clahe->setClipLimit(2.0);
    clahe->apply(labChannels[0], labChannels[0]);
    cv::Mat hsv;
    cv::cvtColor(img, hsv, cv::COLOR_BGR2HSV);
    std::vector<cv::Mat> hsvChannels;
    cv::split(hsv, hsvChannels);
    cv::Mat saturationMask;
    cv::threshold(hsvChannels[1], saturationMask, 64, 255, cv::THRESH_BINARY);
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5));
    cv::morphologyEx(saturationMask, saturationMask, cv::MORPH_CLOSE, kernel);
    hsvChannels[1] = hsvChannels[1] + 0.3 * hsvChannels[1].mul(saturationMask / 255.0);
    cv::merge(labChannels, lab);
    cv::cvtColor(lab, img, cv::COLOR_Lab2BGR);
    cv::merge(hsvChannels, hsv);
    cv::Mat hsvCorrected;
    cv::cvtColor(hsv, hsvCorrected, cv::COLOR_HSV2BGR);
    cv::Mat result;
    cv::addWeighted(img, 0.7, hsvCorrected, 0.3, 0, result);
    cv::Mat blurred;
    cv::GaussianBlur(result, blurred, cv::Size(0, 0), 3);
    cv::addWeighted(result, 1.5, blurred, -0.5, 0, result);
    result.convertTo(result, CV_8UC3);

    return result;
}

cv::Mat colorDistribution(cv::Mat image) {
    std::vector<cv::Mat>imageChannels;
    cv::split(image, imageChannels);

    int histSize = 256;
    float range[] = { 0,256 };
    const float* histRange = { range };

    cv::Mat histB;
    cv::Mat histG;
    cv::Mat histR;
    cv::calcHist(&imageChannels[0], 1, 0, cv::Mat(), histB, 1, &histSize, &histRange, true, false);
    cv::calcHist(&imageChannels[1], 1, 0, cv::Mat(), histG, 1, &histSize, &histRange, true, false);
    cv::calcHist(&imageChannels[2], 1, 0, cv::Mat(), histR, 1, &histSize, &histRange, true, false);

    cv::normalize(histB, histB, 0, 256, cv::NORM_MINMAX);
    cv::normalize(histG, histG, 0, 256, cv::NORM_MINMAX);
    cv::normalize(histR, histR, 0, 256, cv::NORM_MINMAX);

    int binWidth = round(512 / 256);
    cv::Mat histogram(256, 512, CV_8UC3, cv::Scalar(20, 20, 20));
    for (int i = 1; i < 256; i++) {
        cv::line(histogram, cv::Point(binWidth * (i - 1), 256 - cvRound(histB.at<float>(i - 1))),
            cv::Point(binWidth * (i), 256 - cvRound(histB.at<float>(i))),
            cv::Scalar(256, 0, 0), 2);
        cv::line(histogram, cv::Point(binWidth * (i - 1), 256 - cvRound(histG.at<float>(i - 1))),
            cv::Point(binWidth * (i), 256 - cvRound(histG.at<float>(i))),
            cv::Scalar(0, 256, 0), 2);
        cv::line(histogram, cv::Point(binWidth * (i - 1), 256 - cvRound(histR.at<float>(i - 1))),
            cv::Point(binWidth * (i), 256 - cvRound(histR.at<float>(i))),
            cv::Scalar(0, 0, 256), 2);
    }
    return histogram;
}

    void evaluate(cv::Mat image, cv::Mat corrImage) {
        std::vector<cv::Mat> channels;
        cv::split(image, channels);

        cv::Scalar mBimage = cv::mean(channels[0]);
        cv::Scalar mGimage = cv::mean(channels[1]);
        cv::Scalar mRimage = cv::mean(channels[2]);

        cv::split(corrImage, channels);

        cv::Scalar mBhist = cv::mean(channels[0]);
        cv::Scalar mGhist = cv::mean(channels[1]);
        cv::Scalar mRhist = cv::mean(channels[2]);

        std::cout << "Evaluate statistics       " << "Image     " << "Corrected image" << std::endl;
        std::cout << "Channel BLUE                " << mBimage[0] << "        " << mBhist[0] << std::endl;
        std::cout << "Channel GREEN               " << mGimage[0] << "        " << mGhist[0] << std::endl;
        std::cout << "Channel RED                 " << mRimage[0] << "        " << mRhist[0] << std::endl;

    }

