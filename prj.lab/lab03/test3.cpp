#include ".\semcv\semcv.hpp"
#include <iostream>
#include <opencv2/core/utils/logger.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>


int main(int argc, char** argv) {
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_SILENT); 
	if (argc != 6) {
		std::cerr << "Format of input data" << std::endl;
		std::cerr << " - auto contrast type [naive|rgb]" << std::endl;
		std::cerr << " - path to input image(gray or rgb)" << std::endl;
		std::cerr << " - black quantile" << std::endl;
		std::cerr<< " - white quantile" << std::endl;
		std::cerr << " - path to output image " << std::endl;
		return -1;
	}

	double q_black = std::stod(argv[3]);
	double q_white = std::stod(argv[4]);
	std::string folderPath = std::string(argv[5]) +	std::string("autocontrastImage.png");


	if (argv[1] == "naive") {
		cv::Mat image = cv::imread(argv[2], cv::IMREAD_GRAYSCALE);
		if (image.empty()) {
			std::cerr << "ERROR: Could not open or find the image" << std::endl;
			return -1;
		}
		if (image.channels() != 1) {
			std::cerr << "ERROR: Naive autocontr availible for 1 channel image" << std::endl;
			return -1;
		}
		cv::Mat resImage = autocontrast(image, q_black, q_white);

		if (!cv::imwrite(folderPath, resImage)) {
			std::cerr << "ERROR: Could not save the image" << std::endl;
			return -1;
		}
	}
	else if (argv[1] == "rgb") {
		cv::Mat image = cv::imread(argv[2], cv::IMREAD_COLOR); 
		if (image.empty()) {
			std::cerr << "ERROR: Could not open or find the image" << std::endl;
			return -1;
		}
		if (image.channels() != 3) {
			std::cerr << "ERROR: Naive autocontr availible for 3 channel image" << std::endl;
			return -1;
		}
		cv::Mat resImage = autocontrast_rgb(image, q_black, q_white);

		if (!cv::imwrite(folderPath, resImage)) {
			std::cerr << "ERROR: Could not save the image" << std::endl;
			return -1;
		}
	}
	else {
		std::cerr<< "ERROR: unknown auto contrast type [naive|rgb] " << std::endl;
	}
	
	

}