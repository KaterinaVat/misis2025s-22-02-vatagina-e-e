#include <iostream> 
#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>
#include "boundaries.cpp"

int main(/*char argc, char** argv*/) {

	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_SILENT);
	//if (argc < 2) {
	//	std::cerr << "Usage: task0702 <path>";
	//	return -1;
	//}
	//cv::Mat image = cv::imread(argv[1], cv::IMREAD_GRAYSCALE);
	std::string path = "C:\\c++\\misis2025s-3-improc\\prj.lab\\lab04\\output\\collage\\collage.png";

//мое
	cv::Mat image = cv::imread(path, cv::IMREAD_GRAYSCALE);
	cv::Mat myBoundaries = getBoundaries(image);
//опенсв
	cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
	cv::Mat dilated, eroded;
	cv::dilate(image, dilated, kernel);
	cv::erode(image, eroded, kernel);

	cv::Mat opencvBoundaries = dilated - eroded;

	cv::imshow("op", opencvBoundaries);
	cv::waitKey(0);

//еще опенсв
	cv::Mat edges;
	cv::Canny(image, edges, 0, 256);  
	cv::imshow("Canny", edges);
	cv::waitKey(0);

}