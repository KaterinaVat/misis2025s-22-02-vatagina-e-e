#include <iostream>
#include <opencv2/core.hpp>
#include <random>
#include "task04.h"

//генерирование тестовых данных
//  .\task0401.exe C:\c++\misis2025s-3-improc\prj.lab\lab04\input\config1.json C:\c++\misis2025s-3-improc\prj.lab\lab04\output\collage\collage.png --seed 42



int main(int argc, char** argv) {
	std::string configFilePath = argv[1];
	std::string generateImagePath = std::string(argv[2]);
	std::string maskPath = argv[3];
	unsigned int seed = std::random_device{}(); 
	if (argc == 6 && std::string(argv[4]) == "--seed") {
		try {
			seed = static_cast<unsigned int>(std::stoul(argv[5]));
		}
		catch (const std::exception& e) {
			std::cerr << "Invalid seed value" << argv[5] << std::endl;
		}
	}
	else {
		seed = 42;
	}

	std::mt19937 rng(seed);
	cv::FileStorage fsJSON(configFilePath, cv::FileStorage::WRITE | cv::FileStorage::FORMAT_JSON);
	if (!fsJSON.isOpened()) {
		std::cerr << "Can not open jsonFile: " << configFilePath << std::endl;
	}

	std::uniform_int_distribution<int> distN(2, 5);

	std::uniform_int_distribution<int> distNOISE(1, 15);
	std::uniform_int_distribution<int> distBLUR(1, 15);
	std::uniform_int_distribution<int> distMAXW(5, 150);
	std::uniform_int_distribution<int> distMINW(5, 150);
	std::uniform_int_distribution<int> distMAXH(5, 150);
	std::uniform_int_distribution<int> distMINH(5, 150);
	



	int n = distN(rng);

	int noise_std = distNOISE(rng);
	int blur_size = distBLUR(rng);
	int max_elps_width = distMAXW(rng);
	int min_elps_width = distMINW(rng);
	int max_elps_height = distMAXH(rng);
	int min_elps_height = distMINH(rng);




	fsJSON << "output_path" << generateImagePath
		<< "n" << n
		<< "bg_color" << 0
		<< "elps_color" << 255
		<< "noise_std" << noise_std
		<< "blur_size" << blur_size
		<< "min_elps_width" << min_elps_width
		<< "max_elps_width" << max_elps_width
		<< "min_elps_height" << min_elps_height
		<< "max_elps_height" << max_elps_height;
	fsJSON.release();
	std::cout << "Configuration has beem written in file.\n";

	imageGeneration(configFilePath);

	
}