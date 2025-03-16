#include ".\semcv\semcv.hpp"
#include <opencv2/core/utils/logger.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>

int main() {
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_SILENT);
	//cv::Mat image = cv::imread("C:\\c++\\misis2025s-3-improc\\assests\\lab3\\1005.jpg", cv::IMREAD_GRAYSCALE);
	//cv::Mat resImage = autocontrast(image, 0.25, 0.95, true); 
	std::cout << "Original image" << std::endl;
	cv::Mat imageBGR = cv::imread("C:\\c++\\misis2025s-3-improc\\assests\\lab3\\picRGB.jpeg", cv::IMREAD_COLOR);
	imageStatistics_rgb(imageBGR);
	std::cout << std::endl;
	std::cout << "Native image" << std::endl;
	cv::Mat resImageBGR = autocontrastColor(imageBGR, 0.05, 0.05, 0.05, true);
	imageStatistics_rgb(resImageBGR);
	std::cout << std::endl;

	std::cout << "Limited image" << std::endl;
	cv::Mat resImage_rgb = autocontrast_rgb(imageBGR,0.05, 0.95, true);
	imageStatistics_rgb(resImage_rgb);



}