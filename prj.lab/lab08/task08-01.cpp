#include <iostream>
#include <opencv2/opencv.hpp>
#include "task.cpp"

int main() {
    cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_SILENT);
    std::string path = "C:\\c++\\misis2025s-3-improc\\assests\\lab3\\102.jpg";
    cv::Mat image = cv::imread(path, cv::IMREAD_COLOR);

    if (image.empty()) {
        std::cerr << "Error: Could not load image!" << std::endl;
        return -1;
    }

    cv::Mat grayWorldImage = grayWorld(image);

    cv::imshow("Original Image", image);
    cv::imshow("gray_world Image", grayWorldImage);
    cv::waitKey(0);

    cv::Mat notTrivialImage = advancedColorCorrection(image);
    cv::imshow("notTrivialImage Image", notTrivialImage);
    cv::waitKey(0);

    cv::Mat worldHist = colorDistribution(grayWorldImage);
    cv::imshow("gray world", worldHist);
    cv::waitKey(0);

    cv::Mat notTrivialHist = colorDistribution(notTrivialImage);
    cv::imshow("not trivial hist", notTrivialHist);
    cv::waitKey(0);

    std::cout << "Statistics gray image correction" << std::endl;
    evaluate(image, grayWorldImage);
    std::cout << std::endl;
    std::cout << "Statistics not trivial gray image correction" << std::endl;
    evaluate(image, notTrivialImage);

    return 0;
}



