#include <iostream>
#include <opencv2/core/utils/logger.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

#include <opencv2/opencv.hpp>
#include <cmath>

//.\task0402 C:\c++\misis2025s-3-improc\prj.lab\lab04\output\collage\collage.png C:\c++\misis2025s-3-improc\prj.lab\lab04\output\collage\collageOUT.json

int estimateBlurSize(const cv::Mat& image) {
    if (image.empty()) return -1;

    cv::Mat gray;
    if (image.channels() > 1) {
        cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    }
    else {
        gray = image.clone();
    }

    if (gray.depth() == CV_32F || gray.depth() == CV_64F) {
        gray = gray * 255.0;
        gray.convertTo(gray, CV_8U);
    }

    cv::Mat laplacian;
    cv::Laplacian(gray, laplacian, CV_64F);

    cv::Scalar mean, stddev;
    cv::meanStdDev(laplacian, mean, stddev);
    double variance = stddev[0] * stddev[0];

    double sharpness = std::min(variance, 1000.0);
    double blur_strength = 250.0 / (sharpness + 1e-6);
    int kernel_size = static_cast<int>(std::round(blur_strength));

    if (kernel_size % 2 == 0) kernel_size += 1;
    kernel_size = std::max(1, std::min(kernel_size, 99));

    return kernel_size;
}



int main(int argc, char** argv) {
    cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_SILENT);
    if (argc != 3) {
        std::cerr << "Invalid input argument:" << "\n";
        std::cerr << "1 - image path" << "\n";
        std::cerr << "2 - config.json pass" << "\n";
        return -1;
    }

    cv::Mat inputImage = cv::imread(argv[1], cv::IMREAD_GRAYSCALE);
    std::string jsonPath = argv[2];


    cv::FileStorage fsJSON(jsonPath, cv::FileStorage::WRITE | cv::FileStorage::FORMAT_JSON);
    if (!fsJSON.isOpened()) {
        std::cerr << "Can not open jsonFile: " << jsonPath << std::endl;
    }

    cv::Mat mask = (inputImage != 1);
    cv::Scalar mean, stddev;
    cv::meanStdDev(inputImage, mean, stddev);
    int noise_std = stddev[0];

    fsJSON << "blur_size" << estimateBlurSize(inputImage)
            << "colors" << "{"
            << "bg_color" << 0
            << "elps_color" << 255
            << "}"
            << "noise_std" << noise_std
            << "size_of_collage" << 255
            << "objects" << "[";



    // обработка
    // 1 бинаризация

    cv::Mat binaryCollage;
    cv::threshold(inputImage, binaryCollage, 80, 255, cv::THRESH_BINARY);
    cv::imshow("Binary image", binaryCollage);
    cv::waitKey(0);


    int kernelSize = 5;
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(2 * kernelSize + 1, 2 * kernelSize + 1), cv::Point(kernelSize, kernelSize));
    cv::Mat openedImage;
    morphologyEx(binaryCollage, openedImage, cv::MORPH_OPEN, kernel);
    cv::imshow("Opened image", openedImage);
    cv::waitKey(0);

    cv::Mat labels;
    cv::Mat stats;
    cv::Mat centroids;
    int numComponents = connectedComponentsWithStats(openedImage, labels, stats, centroids, 8, CV_32S);

    cv::Mat output;
    cvtColor(openedImage, output, cv::COLOR_GRAY2BGR);

    for (int i = 1; i < numComponents; ++i) {
        cv::Mat mask = labels == i;
        std::vector<std::vector<cv::Point>> contours;
        findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
        if (!contours.empty() && contours[0].size() >= 5) {
            cv::RotatedRect ellipse = fitEllipse(contours[0]);
            cv::ellipse(output, ellipse, cv::Scalar(0, 255, 0), 2);
            fsJSON << "{"
                << "elps_angle" << ellipse.angle
                << "elps_width" << static_cast<int>(ellipse.size.width)
                << "elps_height" << static_cast<int>(ellipse.size.height)
                << "elps_x" << static_cast<int>(ellipse.center.x)
                << "elps_y" << static_cast<int>(ellipse.center.y)
                << "}"
                << "pic_coordinates" << "{"
                << "col" << i % numComponents * 256
                << "row" << i / numComponents * 256
                << "}";
        }
    }
    fsJSON << "]";
    fsJSON.release();
    imshow("Ellipses", output);
    cv::waitKey(0);

}
