#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>
#include <iostream>
#include "include/libname.hpp"

cv::Mat filterI(cv::Mat collage, cv::Mat kernel) {
	cv::Mat filteredImage;

	cv::filter2D(collage, filteredImage, CV_32F, kernel);
	double minVal, maxVal;
	cv::minMaxLoc(filteredImage, &minVal, &maxVal);
	cv::Mat normalized;
	filteredImage.convertTo(normalized, CV_8U, 255.0 / (maxVal - minVal), -minVal * 255.0 / (maxVal - minVal));

	return normalized;
}


int find_periodical_structure(cv::Mat& plot, std::vector<float>& varience) {
	int count = 0;
	int y = 0;
	while (y < varience.size()) {

		int start_of_red = -1;
		while (y < varience.size()) {
			if (varience[y] != 0) {
				start_of_red = y;
				break;
			}
			y++;
		}
		if (start_of_red == -1) break;


		int white_count = 0;
		int tolerance = 3;
		int end_of_red = -1;
		for (int i = start_of_red; i < varience.size(); ++i) {
			if (varience[i] == 0) {
				white_count++;
				if (white_count >= tolerance) {
					end_of_red = i - white_count;
					y = i;
					break;
				}
			}
			else {
				white_count = 0;
			}
		}
		if (end_of_red == -1) break;


		int next_red = -1;
		for (int i = end_of_red + tolerance; i < varience.size(); ++i) {
			if (varience[i] != 0) {
				next_red = i;
				break;
			}
		}
		if (next_red == -1) break;


		int center_y = (end_of_red + next_red) / 2;
		int radius = std::abs(end_of_red - next_red) / 2;

		if (radius > 0) {
			count++;
			cv::ellipse(
				plot,
				cv::Point(30, center_y),
				cv::Size(radius, radius),
				0,
				270, 450,
				cv::Scalar(255, 0, 255),
				2
			);
		}
	}
	return count;
}

void window_varience(cv::Mat& image, std::vector<float>& varience) {
	for (int y = 0; y < image.rows; y++) {
		cv::Mat row = image.row(y);
		cv::Scalar mean, stddev;
		cv::meanStdDev(row, mean, stddev);
		varience[y] = stddev[0] * stddev[0];
	}
}

cv::Mat build_plot(cv::Mat& image, std::vector<float>& varience, float max_var) {
	int plot_x = image.rows;
	int plot_y = 600;
	cv::Mat plot = cv::Mat::zeros(plot_x, plot_y, CV_8UC3);
	plot.setTo(cv::Scalar(255, 255, 255));

	for (int y = 0; y < image.rows; y++) {
		int x = static_cast<int>(varience[y] * plot_y / max_var);
		cv::line(plot, cv::Point(0, y * plot_x / image.rows), cv::Point(x, y * plot_x / image.rows),
			cv::Scalar(0, 0, 255), 1);

	}
	return plot;
}

void suppression_not_max(cv::Mat& plot, std::vector<float>& varience, float& max_var, int& plot_x) {

	for (int y = 0; y < plot.rows; y++) {
		int x = static_cast<int>(varience[y]);
		if (x < max_var * 0.25) {
			varience[y] = 0;
			cv::line(plot, cv::Point(0, y * plot_x / plot.rows), cv::Point(x, y * plot_x / plot.rows),
				cv::Scalar(255, 255, 255), 1);
		}
	}
}

cv::Mat concateImages(cv::Mat& imageGradVertNew, cv::Mat& smoothed_plot) {
	cv::Mat final(imageGradVertNew.rows,
		imageGradVertNew.cols + smoothed_plot.cols,
		CV_8UC3);

	cv::Mat left_roi = final(cv::Rect(0, 0, imageGradVertNew.cols, imageGradVertNew.rows));
	if (imageGradVertNew.channels() == 1)
		cv::cvtColor(imageGradVertNew, left_roi, cv::COLOR_GRAY2BGR);
	else
		imageGradVertNew.copyTo(left_roi);


	cv::Mat right_roi = final(cv::Rect(imageGradVertNew.cols, 0, smoothed_plot.cols, smoothed_plot.rows));
	if (smoothed_plot.channels() == 1)
		cv::cvtColor(smoothed_plot, right_roi, cv::COLOR_GRAY2BGR);
	else
		smoothed_plot.copyTo(right_roi);
	return final;
}

int count_money(cv::Mat input_image) {
	double w = input_image.rows * 0.4;
	double h = input_image.cols * 0.3;
	cv::Rect roi(w, h, 150, 550);
	cv::Mat croppedImage = input_image(roi);

	cv::Mat smothedBox;
	cv::boxFilter(croppedImage, smothedBox, -1, cv::Size(150, 1));

	// нахожу градиент по изображению по вертикали


	cv::Mat kernelV = (cv::Mat_<float>(3, 3) <<
		-1, -2, -1,
		0, 0, 0,
		1, 2, 1);



	cv::Mat imageGradVertNew = filterI(smothedBox, kernelV);
	//cv::imshow("imageGradVertNew", imageGradVertNew);
	//cv::waitKey(0);
	cv::imwrite("imageGradVertNew.png", imageGradVertNew);

	// оконная дисперсия

	std::vector<float> varience(imageGradVertNew.rows);

	window_varience(imageGradVertNew, varience);

	float max_var = *std::max_element(varience.begin(), varience.end());

	// гистограмка

	cv::Mat plot = build_plot(imageGradVertNew, varience, max_var);

	//cv::imshow("Variance Plot", plot);
	//cv::waitKey(0);
	cv::imwrite("plot.png", plot);

	int plot_x = imageGradVertNew.rows;

	suppression_not_max(plot, varience, max_var, plot_x);



	cv::Mat smoothed_plot;
	cv::blur(plot, smoothed_plot, cv::Size(3, 3), cv::Point(-1, -1));
	cv::imwrite("smoothed_plot.png", smoothed_plot);

	int ans = find_periodical_structure(smoothed_plot, varience);

	std::string number = std::to_string(ans);
	cv::Mat final = concateImages(croppedImage, smoothed_plot);
	//cv::imshow("Anylize image", final);
	//cv::imwrite("final.png", final);
	cv::waitKey(0);



	int fontFace = cv::FONT_HERSHEY_SIMPLEX;
	double fontScale = 2.0;
	int thickness = 3;       
	cv::Scalar color(0, 0, 255);  
	int baseline = 0;
	cv::Size textSize = cv::getTextSize(number, fontFace, fontScale, thickness, &baseline);
	cv::Point textOrg(input_image.cols - textSize.width - 20, textSize.height + 20);
	cv::putText(input_image, number, textOrg, fontFace, fontScale, color, thickness);
	//cv::imshow("Result", input_image);
	//cv::waitKey(0);
	cv::imwrite("Result.png", input_image);
	return ans;

}



