#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>  
#include <opencv2/highgui.hpp> 
#include <opencv2/imgcodecs.hpp>
#include <opencv2/core/utils/logger.hpp>

cv::Mat makeSquare(int x, int y) {
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_SILENT);
	cv::Mat I(127, 127, CV_8U, cv::Scalar(x));
	cv::circle(I, cv::Point(64, 64), 40, cv::Scalar(y), -1);
	return I;
}

cv::Mat filter3(const cv::Mat& x_component, const cv::Mat& y_component) {
	cv::Mat magnitude;
	cv::Mat x_float, y_float;
	x_component.convertTo(x_float, CV_32F);
	y_component.convertTo(y_float, CV_32F);
	cv::magnitude(x_float, y_float, magnitude);
	cv::normalize(magnitude, magnitude, 0, 255, cv::NORM_MINMAX, CV_8U);
	return magnitude;
}

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
	std::vector<int> colors = { 0, 127, 255 };
	cv::Mat collage(127 * 2, 127 * 3, CV_8U, cv::Scalar(0));
	cv::Mat I;
	int imageCount = 0;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (i != j) {
				I = makeSquare(colors[i], colors[j]);
				if (imageCount == 1) {
					int row = 1;
					int col = 1;
					cv::Rect roi(127 * col, 127 * row, I.cols, I.rows);
					I.copyTo(collage(roi));
					imageCount++;
				}
				else {
					if (imageCount == 4) {
						int row = 0;
						int col = 1;
						cv::Rect roi(127 * col, 127 * row, I.cols, I.rows);
						I.copyTo(collage(roi));
						imageCount++;
					}
					else {
						int row = imageCount / 3;
						int col = imageCount % 3;
						cv::Rect roi(127 * col, 127 * row, I.cols, I.rows);
						I.copyTo(collage(roi));
						imageCount++;
					}
				}


			}
		}
	}
	cv::imshow("collage image", collage);
	cv::waitKey(0);

	cv::Mat kernel1 = (cv::Mat_<float>(3, 3) <<
		0, 1, 2,
		-1, 0, 1,
		-2, -1, 0);


	cv::Mat kernel2 = (cv::Mat_<float>(3, 3) <<
		2, 1, 0,
		1, 0, -1,
		0, -1, -2);

	cv::Mat filteredImage1 = filterI(collage, kernel1);
	cv::Mat filteredImage2 = filterI(collage, kernel2);
	cv::Mat filteredImage3 = filter3(filteredImage1, filteredImage2);

	cv::imshow("filter image 1", filteredImage1);
	cv::waitKey(0);
	cv::imshow("filter image 2", filteredImage2);
	cv::waitKey(0);
	cv::imshow("filter image 3", filteredImage3);
	cv::waitKey(0);



	std::vector<cv::Mat> bgr_planes;
	bgr_planes.push_back(filteredImage1);
	bgr_planes.push_back(filteredImage2);
	bgr_planes.push_back(filteredImage3);
	cv::Mat rgbImage;
	cv::merge(bgr_planes, rgbImage);
	cv::imshow("rgb", rgbImage);
	cv::waitKey(0);

	cv::Mat result(256 * 2, 381* 2, CV_8UC3, cv::Scalar(0, 0, 0));
	cv::Mat colorFilter1, colorFilter2, colorFilter3;
	cv::cvtColor(filteredImage1, colorFilter1, cv::COLOR_GRAY2BGR);
	cv::cvtColor(filteredImage2, colorFilter2, cv::COLOR_GRAY2BGR);
	cv::cvtColor(filteredImage3, colorFilter3, cv::COLOR_GRAY2BGR);


	colorFilter1.copyTo(result(cv::Rect(0, 0, colorFilter1.cols, colorFilter1.rows)));
	colorFilter2.copyTo(result(cv::Rect(381, 0, colorFilter2.cols, colorFilter2.rows)));
	colorFilter3.copyTo(result(cv::Rect(0, 256, colorFilter3.cols, colorFilter3.rows)));
	rgbImage.copyTo(result(cv::Rect(381, 256, rgbImage.cols, rgbImage.rows)));

	cv::imshow("rgb", result);
	cv::waitKey(0);
	
}