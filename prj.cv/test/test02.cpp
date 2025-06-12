#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>
#include <iostream>
#include <libname/include/libname.hpp>

int main(int argc, char** argv) {
	if (argc > 3 ) {

		std::cerr << "Wrong input format" << std::endl;
		std::cerr << " -- image path" << std::endl;
		return 0;
	}
	bool flag = false;

	std::string input_image= std::string(argv[1]);
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_SILENT);


	cv::Mat image = cv::imread(input_image, cv::IMREAD_COLOR);

	cv::Mat gray; 
	if (image.channels() == 3)
		cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
	else
		gray = image.clone();

	if (argc == 3) {
		flag = true;
	}

	int ans = count_money(gray, flag);

	std::cout << "Count of coins : " << ans;

}
