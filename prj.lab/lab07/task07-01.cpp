#include <iostream> 
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>


cv::Mat dilatation(cv::Mat image) {
	int row = image.rows;
	int col = image.cols;


	cv::Mat dil_image = cv::Mat::zeros(row, col, CV_8UC1);

	for (int i = 1; i < image.rows - 1; i++) {
		for (int j = 1; j < image.cols - 1; j++) {

			uchar max = 0;

			for (int i_m = -1; i_m < 2; i_m++) {
				for (int j_m = -1; j_m < 2; j_m++) {
					uchar image_value = image.at<uchar>(i_m + i, j_m + j);
					if (image_value > max) {
						max = image_value;
					}
				}
			}

			dil_image.at<uchar>(i, j) = max;
		}
	}
	if (dil_image.empty()) {
		std::cerr << "dil_image ������!" << std::endl;
	}
	else {
		return dil_image;
	}
}



cv::Mat erosion(cv::Mat image) {
	int row = image.rows;
	int col = image.cols;
	cv::Mat eroz_image = cv::Mat::zeros(row, col, CV_8UC1);

	for (int i = 1; i < image.rows - 1; i++) {
		for (int j = 1; j < image.cols - 1; j++) {

			uchar min = 1000;

			for (int i_m = -1; i_m < 2; i_m++) {
				for (int j_m = -1; j_m < 2; j_m++) {
					uchar image_value = image.at<uchar>(i_m + i, j_m + j);
					if (image_value < min) {
						min = image_value;
					}
				}
			}

			eroz_image.at<uchar>(i, j) = min;
		}
	}

	if (eroz_image.empty()) {
		std::cerr << "eroz_image ������!" << std::endl;
	}
	else {
		return eroz_image;
	}
}

cv::Mat getBoundaries(cv::Mat image) {

	cv::Mat dil_image = dilatation(image);

	// ������ ������
	cv::Mat closed_image = erosion(dil_image);

	cv::Mat eroz_on_closed = erosion(closed_image);
	cv::Mat boundaries = closed_image - eroz_on_closed;
	return boundaries;

}

int main(char argc, char** argv) {

	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_SILENT);
	if (argc < 2) {
		std::cout << "Usage: task07-01 <path>" << std::endl;
		return -1;
	}

	std::string path_blob_image = argv[1];
	// ������� ���������
	cv::Mat image = cv::imread(path_blob_image, cv::IMREAD_GRAYSCALE);
	if (image.empty()) {
		std::cout << "Can`t load image." << std::endl;
		return -1;
	}

	cv::Mat dil_image= dilatation(image);

	// ������ ������
	cv::Mat closed_image = erosion(dil_image);

	cv::Mat eroz_on_closed = erosion(closed_image);
	cv::Mat boundaries = closed_image - eroz_on_closed;

	cv::imshow("boundaries", boundaries);
	cv::waitKey(0);


}