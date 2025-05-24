#include <string>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>   
#include <opencv2/opencv.hpp>
#include <cmath>
#include <iostream>
#include <opencv2/highgui.hpp>
#include <vector>
#include <filesystem>
#include <fstream> 
#include <semcv/semcv.hpp>
#include <opencv2/core/types.hpp>
#include <set>
#include <vector>
#include <iostream>
#include "opencv2/imgcodecs.hpp"


void imageStatistics(cv::Mat& img, cv::Mat& noiseImg) {

	uchar x1 = img.at<uchar>(0, 0);
	uchar x2 = img.at<uchar>(24, 24);
	uchar x3 = img.at<uchar>(128, 128);

	std::vector<uchar> scalars;
	std::vector<int> statI;
	std::vector<int> statN;

	scalars.push_back(x1);
	scalars.push_back(x2);
	scalars.push_back(x3);


	for (int i = 0; i < 3; i++) {
		cv::Scalar mean_value_i, stddev_value_i;
		cv::Scalar mean_value_n, stddev_value_n;
		cv::Mat mask = (img == scalars[i]);
		cv::meanStdDev(img, mean_value_i, stddev_value_i, mask);
		cv::meanStdDev(noiseImg, mean_value_n, stddev_value_n, mask);

	
		statI.push_back(mean_value_i[0]);
		statI.push_back(stddev_value_i[0]);

		statN.push_back(mean_value_n[0]);
		statN.push_back(stddev_value_n[0]);

	}
	std::cout << "Characteristics" << std::endl;
	std::cout << "mean 1   " << "std 1	" << "mean 2	" << "std 2	" << "mean 3	" << "std 3		" << std::endl;
	for (int i = 0; i < 6; i++) {
		std::cout << statI[i]<< "	";
	}
	std::cout << std::endl;
	for (int i = 0; i < 6; i++) {
		std::cout << statN[i] << "	";
	}
	cv::waitKey(0);

}

void imageStatistics_rgb(cv::Mat& img) {
	std::cout << "Image statistics" << std::endl;
	cv::Scalar mean_value, std_value;
		cv::meanStdDev(img, mean_value, std_value);
		std::cout << mean_value << "	" << std_value << std::endl;
	
}


std::string converting_numbers(int number, const int n) {
	int temp = number;
	int count = 0;
	while (temp > 0) {
		temp = temp / 10;
		count += 1;
	}
	std::string result;
	if (count != n) {
		for (int i = 0; i < (n - count); i++) {
			result += '0';
		}
		result += std::to_string(number);
	}
	else {
		result = std::to_string(number);
	}
	return result;
}

void saveImage(const cv::Mat& img) {
	std::string folder = "output_images";
	std::filesystem::create_directories(folder);
	std::string baseName = "test_image";
	std::string pngPath = folder + "/" + baseName + ".png";
	std::string tiffPath = folder + "/" + baseName + ".tiff";
	std::string jpgPath = folder + "/" + baseName + ".jpg";
	cv::imwrite(pngPath, img);
	cv::imwrite(tiffPath, img);
	cv::imwrite(jpgPath, img);
	std::cout << "- Images saved in the directory	output_images" << std::endl;

}

void createFile(const std::string& folderPath) {
	std::string path = folderPath + '/' + "task01.lst";
	std::ofstream outfile(path);
	for (const auto& entry : std::filesystem::directory_iterator(folderPath))
		if (entry.path().filename().string() != "task01.lst") {
			outfile << entry.path().filename().string() << std::endl;
		}
	outfile.close();
	std::cout << "- File saved by the pass " << path << std::endl;
}


std::string strid_from_mat(const cv::Mat& img, const int n) {
	std::string height = converting_numbers(img.rows, n);
	std::string width = converting_numbers(img.cols, n);
	std::string channels = std::to_string(img.channels());

	int type = img.type();
	std::string typeStr;
	uchar depth = type & CV_MAT_DEPTH_MASK;
	uchar chans = 1 + (type >> CV_CN_SHIFT);
	switch (depth) {
	case CV_8U:  typeStr = "uint08"; break;
	case CV_8S:  typeStr = "sint08"; break;
	case CV_16U: typeStr = "uint16"; break;
	case CV_16S: typeStr = "sint16"; break;
	case CV_32S: typeStr = "sint32"; break;
	case CV_32F: typeStr = "real32"; break;
	case CV_64F: typeStr = "real64"; break;
	default:     typeStr = "unknown"; break;
	}
	std::string result = width + 'x' + height + '.' + channels + '.' + typeStr;

	saveImage(img);
	createFile("output_images");
	return result;
}

std::vector<std::filesystem::path> get_list_of_file_paths(const std::filesystem::path& path_lst) {
	std::vector<std::filesystem::path> paths;
	std::ifstream in(path_lst, std::ios_base::in);
	std::string line;
	while (getline(in, line)) {
		paths.push_back(std::filesystem::absolute(line));
	}
	in.close();
	std::cout << "###Images wrote in the file###" << std::endl;
	for (int i = 0; i < paths.size(); i++) {
		std::cout << paths[i] << std::endl;
	}

	return paths;
}

cv::Mat greyImage() {
	cv::Mat img(30, 768, CV_8U, cv::Scalar(0));
	int value = 0;
	int width = 768;
	int height = 30;
	int stripeWidth = 3;

	for (int col = 0; col < width; col += stripeWidth) {
		for (int i = 0; i < stripeWidth; i++) {
			for (int row = 0; row < height; row++) {
				img.at<uchar>(row, col + i) = static_cast<uchar>(value);
			}
		}
		value++;
	}

	cv::namedWindow("Striped Image", cv::WINDOW_AUTOSIZE);
	cv::imshow("Striped Image", img);
	cv::waitKey(0);
	return img;
}

cv::Mat gammaCorr(const cv::Mat& img, double& gamma) {
	cv::Mat normalized;
	img.convertTo(normalized, CV_32F, 1.0 / 255.0);
	cv::Mat gammaCorrected;
	cv::pow(normalized, gamma, gammaCorrected);
	gammaCorrected.convertTo(gammaCorrected, CV_8U, 255.0);

	return gammaCorrected;
}

cv::Mat gen_tgtimg00(const int lev0, const int lev1, const int lev2) {
	cv::Mat tgtimg(256, 256, CV_8U,cv::Scalar(lev0));
	cv::rectangle(tgtimg, cv::Point(23, 23), cv::Point(232, 232), cv::Scalar(lev1), -1);
	cv::circle(tgtimg, cv::Point(128, 128), 83, cv::Scalar(lev2), -1);
  return tgtimg;
}

cv::Mat add_noise_gau(const cv::Mat& img, const int std) {
	double mean = 0;
	cv::Mat noise(img.size(), CV_32F);
	cv::randn(noise, cv::Scalar(mean), cv::Scalar(std));
	cv::Mat img_float;
	img.convertTo(img_float, CV_32F);
	cv::Mat noisy_img;
	cv::add(img_float, noise, noisy_img);
	cv::Mat noisy_img_8u; 
	noisy_img.convertTo(noisy_img_8u, CV_8U); 

	return noisy_img_8u;
}

 cv::Mat buildHist( cv::Mat& src) {
	cv::Mat hist;
	int histSize = 256;
	float range[] = { 0, 256 };
	const float* histRange = { range };

	cv::calcHist(&src, 1, 0, cv::Mat(), hist, 1, &histSize, &histRange);
	int hist_w = 256, hist_h = 256;
	cv::Mat histImage(hist_h, hist_w, CV_8UC1, cv::Scalar(255));
	cv::normalize(hist, hist, 0, histImage.rows-5, cv::NORM_MINMAX, -1, cv::Mat());

	for (int i = 1; i < histSize; i++) {
		cv::line(histImage, cv::Point((i - 1) * hist_w / histSize, hist_h - cvRound(hist.at<float>(i - 1))),
			cv::Point(i * hist_w / histSize, hist_h - cvRound(hist.at<float>(i))),
			cv::Scalar(0), 2, 8, 0);
	}

	return histImage;
}

 cv::Mat buildHistColor(cv::Mat& src) {
	 cv::Mat b_hist, g_hist, r_hist;
	 int histSize = 256;
	 float range[] = { 0, 256 };
	 const float* histRange = { range };

	 std::vector<cv::Mat> bgr_planes;
	 cv::split(src, bgr_planes);
	 cv::calcHist(&bgr_planes[0], 1, 0, cv::Mat(), b_hist, 1, &histSize, &histRange);
	 cv::calcHist(&bgr_planes[1], 1, 0, cv::Mat(), g_hist, 1, &histSize, &histRange); 
	 cv::calcHist(&bgr_planes[2], 1, 0, cv::Mat(), r_hist, 1, &histSize, &histRange); 
	 int hist_w = 256, hist_h = 256;
	

	 cv::Mat histImage(hist_h, hist_w, CV_8UC3, cv::Scalar(0,0,0));

	 cv::normalize(b_hist, b_hist, 0, histImage.rows - 5, cv::NORM_MINMAX, -1, cv::Mat());
	 cv::normalize(g_hist, g_hist, 0, histImage.rows - 5, cv::NORM_MINMAX, -1, cv::Mat());
	 cv::normalize(r_hist, r_hist, 0, histImage.rows - 5, cv::NORM_MINMAX, -1, cv::Mat());


	 for (int i = 1; i < histSize; i++) {
		 cv::line( histImage, 
			 
			 cv::Point((i - 1) * hist_w / histSize, hist_h - cvRound(b_hist.at<float>(i - 1))),
			
			 cv::Point(i * hist_w / histSize, hist_h - cvRound(b_hist.at<float>(i))),
			 
			 cv::Scalar(255, 0, 0), 2, 8, 0);

		 cv::line(histImage, 

			 cv::Point((i - 1) * hist_w / histSize, hist_h - cvRound(g_hist.at<float>(i - 1))),

			 cv::Point(i * hist_w / histSize, hist_h - cvRound(g_hist.at<float>(i))),

			 cv::Scalar(0, 255, 0), 2, 8, 0);

		 cv::line(histImage, 

			 cv::Point((i - 1) * hist_w / histSize, hist_h - cvRound(r_hist.at<float>(i - 1))),

			 cv::Point(i * hist_w / histSize, hist_h - cvRound(r_hist.at<float>(i))), 

			 cv::Scalar(0, 0, 255), 2, 8, 0); 

	 }

	 return histImage;
 }

cv::Mat autocontrast(const cv::Mat& img, const double q_black, const double q_white, bool flag ) {
	if (img.type() != CV_8UC1) {
		std::cerr << "Image is not grey" << std::endl;
	}
	cv::Mat image = img;
    cv::Mat hist =buildHist(image);
	double minVal, maxVal;
	cv::minMaxLoc(img, &minVal, &maxVal);
	double blackLevel = minVal + (maxVal - minVal) * q_black;
	double whiteLevel = minVal + (maxVal - minVal) * q_white;
	cv::Mat result;
	double scale = 255.0 / (whiteLevel - blackLevel);
	double offset = -blackLevel * scale;
	img.convertTo(result, CV_8UC1, scale, offset);
	cv::Mat histR = buildHist(result);
	if (flag == true) {
		cv::imshow("Img", img);
		cv::waitKey(0);
		cv::imshow("hist", hist);
		cv::waitKey(0);
		cv::imshow("rez", result);
		cv::waitKey(0);
		cv::imshow("histR", histR);
		cv::waitKey(0);
	}
	return result;
 }

cv::Mat autocontrastColor(const cv::Mat& img, const double q_blue, const double q_red, const double q_green, bool flag) {
	if (img.channels() != 3) {
		std::cerr << "Image must have 3 channels (BGR)." << std::endl;
		return cv::Mat();
	}
	cv::Mat image = img;
	std::vector<cv::Mat> bgr_planes;
	cv::split(img, bgr_planes);
	double minValB, maxValB, minValG, maxValG, minValR, maxValR;
	cv::minMaxLoc(bgr_planes[0], &minValB, &maxValB);
	cv::minMaxLoc(bgr_planes[1], &minValG, &maxValG);
	cv::minMaxLoc(bgr_planes[2], &minValR, &maxValR);

	double blackLevelB = minValB + (maxValB - minValB) * q_blue;
	double whiteLevelB = minValB + (maxValB - minValB) * (1 - q_blue);

	double blackLevelR = minValR + (maxValR - minValR) * q_red;
	double whiteLevelR = minValR + (maxValR - minValR) * (1 - q_red);

	double blackLevelG = minValG + (maxValG - minValG) * q_green;
	double whiteLevelG = minValG + (maxValG - minValG) * (1 - q_green);


	double scaleB = 255.0 / (whiteLevelB - blackLevelB);
	double offsetB = -blackLevelB * scaleB;

	double scaleG = 255.0 / (whiteLevelG - blackLevelG);
	double offsetG = -blackLevelG * scaleG;

	double scaleR = 255.0 / (whiteLevelR - blackLevelR);
	double offsetR = -blackLevelR * scaleR;


	cv::Mat resB, resG, resR;
	bgr_planes[0].convertTo(resB, CV_8UC1, scaleB, offsetB);
	bgr_planes[1].convertTo(resG, CV_8UC1, scaleG, offsetG);
	bgr_planes[2].convertTo(resR, CV_8UC1, scaleR, offsetR);

	std::vector<cv::Mat> resultChannels = { resB, resG, resR };
	cv::Mat resultImage;
	cv::merge(resultChannels, resultImage);


	if (flag) {
		cv::imshow("Original Image", img);
		cv::waitKey(0);

		cv::Mat hist = buildHistColor(image);
		cv::imshow("Original Histogram", hist);
		cv::waitKey(0);

		cv::imshow("Autocontrasted Image", resultImage);
		cv::waitKey(0);

		cv::Mat histR = buildHistColor(resultImage);
		cv::imshow("Autocontrasted Histogram", histR);
		cv::waitKey(0);
	}

	return resultImage;
}


cv::Mat autocontrast_rgb(const cv::Mat& img, const double q_black, const double q_white, bool flag) {
	if (img.channels() != 3) {
		std::cerr << "Image not 3 channels" << std::endl;
	}
	cv::Mat image = img;
	cv::Mat hist = buildHistColor(image);

	std::vector<cv::Mat> bgr_planes;
	cv::split(img, bgr_planes);

	double minValB, maxValB, minValG, maxValG, minValR, maxValR;
	cv::minMaxLoc(bgr_planes[0], &minValB, &maxValB);
	cv::minMaxLoc(bgr_planes[1], &minValG, &maxValG);
	cv::minMaxLoc(bgr_planes[2], &minValR, &maxValR);

	double blueLevel = minValB + (maxValB - minValB) * q_black;
	double whiteLevelB = minValB + (maxValB - minValB) * q_white;

	double redLevel = minValR + (maxValR - minValR) * q_black;
	double whiteLevelR = minValR + (maxValR - minValR) * q_white;


	double greenLevel = minValG + (maxValG - minValG) * q_black;
	double whiteLevelG = minValG + (maxValG - minValG) * q_white;

	double scaleB = 255.0 / (whiteLevelB - blueLevel);
	double scaleG = 255.0 / (whiteLevelG - greenLevel);
	double scaleR = 255.0 / (whiteLevelR - redLevel);

	double offsetB = -blueLevel * scaleB;
	double offsetG = -greenLevel * scaleG;
	double offsetR = -redLevel * scaleR;

	cv::Mat resB, resG, resR;

	bgr_planes[0].convertTo(resB, CV_8UC1, scaleB, offsetB);
	bgr_planes[1].convertTo(resG, CV_8UC1, scaleG, offsetG);
	bgr_planes[2].convertTo(resR, CV_8UC1, scaleR, offsetR);
	std::vector<cv::Mat> result = { resB, resG, resR };
	cv::Mat resultImage;
	cv::merge(result, resultImage);

	cv::Mat histR = buildHistColor(resultImage);

	if (flag) {
		//cv::imshow("Img", img);
		//cv::waitKey(0);
		//cv::imshow("hist", hist);
		//cv::waitKey(0);

		cv::imshow("Autocontrasted limited Image", resultImage);  
		cv::waitKey(0);

		cv::imshow("Autocontrasted limited Histogram", histR); 
		cv::waitKey(0);
	}
	return img;

}


 

