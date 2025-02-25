#include "../semcv/semcv.hpp"
#include <iostream>
#include <opencv2/core/utils/logger.hpp>
#include <opencv2/highgui.hpp>

int main() {
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_SILENT);
	cv::Mat img = gen_tgtimg00(90, 127, 165);

	cv::namedWindow("Image", cv::WINDOW_AUTOSIZE);
	cv::imshow("Image", img);
	cv::waitKey(0);




	img = add_noise_gau(img, 15);

	cv::namedWindow("Image", cv::WINDOW_AUTOSIZE);
	cv::imshow("Image", img);
	cv::waitKey(0);



}