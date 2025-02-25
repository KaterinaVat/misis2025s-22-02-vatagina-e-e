#include "../semcv/semcv.hpp"
#include <iostream>
#include <vector>
#include <opencv2/core/utils/logger.hpp>
#include <opencv2/highgui.hpp>
#include <filesystem> 

int main(int argc, char* argv[]) {

	if (argc != 2) {
		std::cerr << "Load pass to save noise image" << std::endl;
		std::cerr << "Use: " << argv[0] << " <pass_to_folder>" << std::endl;
	}

	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_SILENT);
	std::vector<std::vector<int>> brightLevel = { {0, 127, 255}, {20,127, 235}, {55, 127, 200}, {90, 127, 165} };
	cv::Mat currentImg;

	cv::Mat collage(256, 256 * 4, CV_8U, cv::Scalar(0));
	for (int i = 0; i < brightLevel.size(); i++) {
		currentImg = gen_tgtimg00(brightLevel[i][0], brightLevel[i][1], brightLevel[i][2]); 
		cv::Rect roi(256*i, 0, currentImg.cols, currentImg.rows);
		currentImg.copyTo(collage(roi));
	}
	cv::imshow("image", collage);
	cv::waitKey(0);

	cv::Mat final_collage(256 * 4, 256 * 4, CV_8U);
	cv::Rect roi(0, 0, collage.cols, collage.rows);
	collage.copyTo(final_collage(roi));

	std::vector<int> noise_std = { 3,7,15 };


	for (int i = 0; i < noise_std.size(); i++) {
		cv::Mat noisy_collage = add_noise_gau(collage, noise_std[i]);  // Применяем шум к исходному коллажу
		cv::Rect roi_noisy(0, 256 * (i + 1), noisy_collage.cols, noisy_collage.rows);
		noisy_collage.copyTo(final_collage(roi_noisy)); // Копируем шумовой коллаж в соответствующую строку
	}

	cv::imshow("image", final_collage);
	cv::waitKey(0);

	std::string folderPath = argv[1];
	if (!std::filesystem::exists(folderPath)) {
	std::cerr << "Following folder " << folderPath << " is not exist" << std::endl;
	}
	std::string folder = argv[1] + std::string("\\") + std::string("noiseCollage.png");
	
	if (cv::imwrite(folder, final_collage)) {
		std::cout << "Image was successfully saved by the pass " << folder << std::endl;
	}
	else {
		std::cout << "Image was not saved by the pass " << folder << std::endl;
	}
}