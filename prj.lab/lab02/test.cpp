#include "../semcv/semcv.hpp"
#include <iostream>
#include <opencv2/core/utils/logger.hpp>
#include <opencv2/highgui.hpp>

int main() {
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_SILENT);
	cv::Mat img = gen_tgtimg00(90, 127, 165);
	/*if (cv::imwrite(".\\greyImage.png", img)) {
		std::cout << "OKEY" << std::endl;
	}
	else {
		std::cout << "NOT OKEY" << std::endl;
	}

	cv::imshow("Image", img);
	cv::waitKey(0);
	buildHist(img);*/

	cv::Mat noiseImg;
	noiseImg = add_noise_gau(img, 15);
//	if (cv::imwrite(".\\greyNoiseImage.png", noiseImg)){
//	std::cout << "OKEY" << std::endl;
//}
//	else {
//		std::cout << "NOT OKEY" << std::endl;
//	}
//	std::cout << "Comparing two pictures" << std::endl;
	cv::imshow("Noise Image", noiseImg);
	imageStatistics(img, noiseImg);
	buildHist(noiseImg);






}