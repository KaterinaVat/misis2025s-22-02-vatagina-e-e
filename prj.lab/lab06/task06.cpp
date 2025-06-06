#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>  
#include <opencv2/highgui.hpp> 
#include <opencv2/imgcodecs.hpp>
#include <opencv2/core/utils/logger.hpp>


struct Blob {
	int x;
	int y;
	double sigma;
	double scale;
};

cv::Mat picDown(cv::Mat& image) {
	cv::Mat small_image(image.rows / 2, image.cols / 2, CV_8UC1);
	cv::Mat blured;
	cv::GaussianBlur(image, blured, cv::Size(5, 5), 2);
	for (int i=0; i < small_image.rows; i++) {
		for (int j = 0; j < small_image.cols; j++) {
			small_image.at<uchar>(i, j) = blured.at<uchar>(i * 2, j * 2);
		}
	}

	return small_image;
}


std::vector<cv::Mat> ScalePyramid(const cv::Mat& image) {
	std::vector<cv::Mat> pyramid;
	cv::Mat current = image.clone();
	pyramid.push_back(current); 

	for (int i = 1; i < 5; ++i) {
		current = picDown(current);  
		pyramid.push_back(current);
	}

	return pyramid;
}



std::vector<double> sigma_generator(double sigma0, int num_levels) {
	std::vector<double> sigmas;
	for (int i = 0; i < num_levels; i++) {
		sigmas.push_back(sigma0 * pow(2, i));
	}
	return sigmas;
}

std::vector<cv::Mat> DOG(cv::Mat& gray_image, std::vector<double>& sigmas) {
	std::vector<cv::Mat> blurred, dogs;
	for (double sigma : sigmas) {
		cv::Mat blur;
		cv::GaussianBlur(gray_image, blur, cv::Size(), sigma, sigma);
		blur.convertTo(blur, CV_32F, 1.0 / 255);
		blurred.push_back(blur);
	}
	for (int i = 1; i < blurred.size(); i++) {
		cv::Mat dog = blurred[i] - blurred[i - 1];
		dogs.push_back(dog);
	}
	return dogs;
}


std::vector<Blob> blob_detector(
	const std::vector<cv::Mat>& dogs,
	const std::vector<double>& sigmas,
	double scale = 1.0,
	float threshold = 0.03f
) {
	std::vector<Blob> blobs;

	for (size_t s = 1; s < dogs.size() - 1; ++s) {
		const cv::Mat& prev = dogs[s - 1];
		const cv::Mat& curr = dogs[s];
		const cv::Mat& next = dogs[s + 1];

		for (int y = 1; y < curr.rows - 1; ++y) {
			for (int x = 1; x < curr.cols - 1; ++x) {
				float val = curr.at<float>(y, x);
				if (std::abs(val) < threshold) continue;

				bool isMax = true, isMin = true;
				for (int ds = -1; ds <= 1; ++ds) {
					const cv::Mat& layer = dogs[s + ds];
					for (int dy = -1; dy <= 1; ++dy) {
						for (int dx = -1; dx <= 1; ++dx) {
							if (ds == 0 && dy == 0 && dx == 0) continue;

							int ny = y + dy;
							int nx = x + dx;
							if (ny < 0 || ny >= curr.rows || nx < 0 || nx >= curr.cols) continue;

							float neighbor = layer.at<float>(ny, nx);
							if (val <= neighbor) isMax = false;
							if (val >= neighbor) isMin = false;
						}
					}
				}

				if (isMax || isMin) {
					double sigma = 0.5 * (sigmas[s] + sigmas[s + 1]);
					int cx = static_cast<int>(std::round(x * scale));
					int cy = static_cast<int>(std::round(y * scale));
					blobs.push_back({ cx, cy, sigma * scale, scale });
				}
			}
		}
	}

	return blobs;
}


void blobs(cv::Mat& image, const std::vector<Blob>& blobs) {
	for (const Blob& b : blobs) {
		int radius = static_cast<int>(std::sqrt(2.0) * b.sigma);
		cv::circle(image, cv::Point(b.x, b.y), radius, cv::Scalar(0, 255, 0), 2);
	}
}


int main(int argc, char** argv) {
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_SILENT);
	if (argc < 2) {
		std::cout << "Usage: task06 <path>" << std::endl;
		return -1;
	}

	cv::Mat img = cv::imread(argv[1], cv::IMREAD_GRAYSCALE);
	if (img.empty()) {
		std::cout << "Can`t load image." << std::endl;
		return -1;
	}

	std::vector<cv::Mat> pyramid = ScalePyramid(img);
	double sigma0 = 0.5;
	int levels = 4;
	std::vector<double> sigmas = sigma_generator(sigma0, levels);

	std::vector<Blob> all_blobs;
	for (size_t i = 0; i < pyramid.size(); ++i) {
		double scale = std::pow(2.0, static_cast<double>(i));
		std::vector<cv::Mat> dogs = DOG(pyramid[i], sigmas);
		std::vector<Blob> blobs = blob_detector(dogs, sigmas, scale);
		all_blobs.insert(all_blobs.end(), blobs.begin(), blobs.end());
	}

	cv::Mat display;
	img.copyTo(display);
	cv::cvtColor(display, display, cv::COLOR_GRAY2BGR);
	blobs(display, all_blobs);

	cv::imwrite("blobs_pyramid_detected.png", display);
	std::cout << "Detected: " << all_blobs.size() << std::endl;
	std::cout << "Result saved: blobs_pyramid_detected.png" << std::endl;

	return 0;
}
