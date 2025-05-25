#include <fstream>
#include <iostream>
#include ".\semcv\semcv.hpp"
#include <opencv2/core/utils/logger.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <filesystem>
#include <cstdlib>
#include <ctime>
#include <cmath>

// генерация картинки и ее сохранение по пути


cv::Mat generateImage(int tile_size,
    int min_elps_width, int max_elps_width,
    int min_elps_height, int max_elps_height,
    int bg_color, int elps_color, cv::FileStorage fsJSON) {

    const int margin = 32;
    cv::Mat image(tile_size, tile_size, CV_8U, cv::Scalar(bg_color));

    int rotAngle = rand() % 180;
    int x = rand() % (tile_size - 2 * margin + 1) + margin;
    int y = rand() % (tile_size - 2 * margin + 1) + margin;

    int max_radius = std::min({ x - margin, tile_size - x - margin, y - margin, tile_size - y - margin });
    int elps_width = rand() % (max_elps_width - min_elps_width + 1) + min_elps_width;
    int elps_height = rand() % (max_elps_height - min_elps_height + 1) + min_elps_height;

    double a = elps_width / 2.0;
    double b = elps_height / 2.0;
    double r = std::sqrt(a * a + b * b);

    double scale = 1.0;
    if (r + margin > max_radius) {
        scale = (max_radius - margin) / r;
        a *= scale;
        b *= scale;
    }
    int safe_width = std::max(15, (int)std::round(a));
    int safe_height = std::max(20, (int)std::round(b));

    cv::ellipse(image, cv::Point(x, y),
        cv::Size(safe_width, safe_height),
        rotAngle, 0, 360,
        elps_color, -1);

    fsJSON << "{"
        << "elps_angle" << rotAngle
        << "elps_width" << static_cast<int>(safe_width)
        << "elps_height" << static_cast<int>(safe_height)
        << "elps_x" << static_cast<int>(x)
        << "elps_y" << static_cast<int>(y)
        << "}";

    return image;
}

cv::Mat concateImages(int n, int min_elps_width, int max_elps_width,
    int min_elps_height, int max_elps_height,
    int bg_color, int elps_color, cv::FileStorage fsJSON) {

    const int tile_size = 256;
    const int collage_size = tile_size * n;
    cv::Mat collage(collage_size, collage_size, CV_8U, cv::Scalar(bg_color));

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cv::Mat tile = generateImage(tile_size,
                min_elps_width, max_elps_width,
                min_elps_height, max_elps_height,
                bg_color, elps_color, fsJSON);

            cv::Rect roi(i * tile_size, j * tile_size, tile_size, tile_size);
            tile.copyTo(collage(roi));
           fsJSON << "pic_coordinates" << "{"
                << "col" << i * 256
                << "row" << j * 256
                << "}";
        }
    }

    return collage;
}

int estimateBlurSize(const cv::Mat& image, int known_blur = 25) {
    if (image.empty()) return -1;
    const double BASE_COEFF = 25000.0;  
    static double calibrated_coeff = BASE_COEFF;
    static bool is_calibrated = false;
    cv::Mat gray;
    if (image.channels() > 1) {
        cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    }
    else {
        gray = image.clone();
    }
    if (gray.depth() == CV_32F || gray.depth() == CV_64F) {
        gray = gray * 255.0;
        gray.convertTo(gray, CV_8U);
    }

    cv::Mat laplacian;
    cv::Laplacian(gray, laplacian, CV_64F);

    cv::Scalar mean, stddev;
    cv::meanStdDev(laplacian, mean, stddev);
    double variance = stddev.val[0] * stddev.val[0];

    if (known_blur > 0 && !is_calibrated) {
        calibrated_coeff = known_blur * (variance + 1e-6);
        is_calibrated = true;
    }

    double blurSize = calibrated_coeff / (variance + 1e-6);
    blurSize = std::max(1.0, std::min(blurSize, 100.0));

    return static_cast<int>(std::round(blurSize));
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

void imageGeneration(std::string f) {
    std::srand(std::time(nullptr));
    cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_SILENT);


    cv::FileStorage fs(f, cv::FileStorage::READ);
    std::string output_path = (std::string) fs["output_path"];
    int n = (int)fs["n"];
    int bg_color = (int)fs["bg_color"];
    int elps_color = (int)fs["elps_color"];
    int noise_std = (int)fs["noise_std"];
    int blur_size = (int)fs["blur_size"];
    int min_elps_width = (int)fs["min_elps_width"];
    int max_elps_width = (int)fs["max_elps_width"];
    int min_elps_height = (int)fs["min_elps_height"];
    int max_elps_height = (int)fs["max_elps_height"];


    std::string imagePath = output_path;
    std::string idealJSON = output_path.substr(0, output_path.size() - 4) + "I.json";


    cv::FileStorage fsIJSON(idealJSON, cv::FileStorage::WRITE | cv::FileStorage::FORMAT_JSON);
    if (!fsIJSON.isOpened()) {
        std::cerr << "Can not open jsonFile: " << idealJSON << std::endl;
    }
    fsIJSON << "blur_size" << blur_size;
    fsIJSON << "colors" << "{";
    fsIJSON << "bg_color" << bg_color; 
    fsIJSON << "elps_color" << elps_color;
    fsIJSON << "}";
    fsIJSON << "noise_std" << noise_std;
    fsIJSON << "size_of_collage" << 255;
    fsIJSON << "objects" << "[";


    cv::Mat collage = concateImages(n, min_elps_width, max_elps_width,
    min_elps_height, max_elps_height, bg_color, elps_color, fsIJSON);

    fsIJSON << "]";
    fsIJSON.release();
    std::cout << "jsonchick esti" << std::endl;

    cv::GaussianBlur(collage, collage, cv::Size(blur_size, blur_size), 0);

    cv::Mat noisedCollage = add_noise_gau(collage, noise_std);
    cv::imshow("Generated Collage", noisedCollage);
    cv::waitKey(0);
    


    if (!cv::imwrite(imagePath, noisedCollage)) {
        std::cerr << "Error: save image " << imagePath << std::endl;
    }
    std::cout << "Image saved: " << imagePath << std::endl;
 
}

