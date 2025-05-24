#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>
#include <iostream>
#include "../prj.lab/semcv/semcv.hpp"

cv::Mat filterI(cv::Mat collage, cv::Mat kernel) {
	cv::Mat filteredImage;

	cv::filter2D(collage, filteredImage, CV_32F, kernel);
	double minVal, maxVal;
	cv::minMaxLoc(filteredImage, &minVal, &maxVal);
	cv::Mat normalized;
	filteredImage.convertTo(normalized, CV_8U, 255.0 / (maxVal - minVal), -minVal * 255.0 / (maxVal - minVal));

	return normalized;
}

int main() {
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_SILENT);
	cv::Mat image = cv::imread("C:\\c++\\misis2025s-3-improc\\prj.cv\\examples\\IMG_6448.png", cv::IMREAD_COLOR);

	cv::Mat gray;
	//беру входное изображение и конвертирую его в серое изображение
	if (image.channels() == 3)
		cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
	else
		gray = image.clone();

	std::cout << gray.rows << std::endl;
	std::cout << gray.cols << std::endl;

//обрезаю часть изображения

	double w = gray.rows * 0.25;
	double h = gray.cols * 0.2;
	cv::Rect roi(w, h, 150, 550);
	cv::Mat croppedImage = gray(roi);
	cv::imshow("Original gray Image", gray);
	cv::imshow("Cropped Image", croppedImage);
	cv::waitKey(0);


	//cv::Mat smoothed;
	//cv::blur(croppedImage, smoothed, cv::Size(150, 1));
	//cv::imshow("smoothed", smoothed);
	//cv::waitKey(0);
	 
	cv::Mat smothedBox;
	cv::boxFilter(croppedImage, smothedBox, -1, cv::Size(150, 1));
	cv::imshow("smoothedBox", smothedBox);
	cv::waitKey(0);

	//cv::Mat smothedGaussian;
	//cv::GaussianBlur(croppedImage, smothedGaussian, cv::Size(149, 1), 0, 0);
	//cv::imshow("smothedGaussian", smothedGaussian);
	//cv::waitKey(0);



	cv::Mat kernelV = (cv::Mat_<float>(3, 3) <<
		-1, -2, -1,
		0, 0, 0,
		1, 2, 1);


	cv::Mat imageGradVertNew = filterI(smothedBox, kernelV);
	cv::imshow("ImageVertGrad", imageGradVertNew);
	cv::waitKey(0);


// оконная дисперсия
	double meanImg;


	cv::Mat binary;
	cv::threshold(imageGradVertNew, binary, 110, 255, cv::THRESH_BINARY);  
	cv::imshow("binary", binary);
	cv::waitKey(0);


}