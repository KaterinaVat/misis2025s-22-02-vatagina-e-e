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


cv::Mat generateImage(int tile_size,
    int min_elps_width, int max_elps_width,
    int min_elps_height, int max_elps_height,
    int bg_color, int elps_color) {

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



    return image;
}

cv::Mat concateImages(int n, int min_elps_width, int max_elps_width,
    int min_elps_height, int max_elps_height,
    int bg_color, int elps_color) {

    const int tile_size = 256;
    const int collage_size = tile_size * n;
    cv::Mat collage(collage_size, collage_size, CV_8U, cv::Scalar(bg_color));

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cv::Mat tile = generateImage(tile_size,
                min_elps_width, max_elps_width,
                min_elps_height, max_elps_height,
                bg_color, elps_color);

            cv::Rect roi(i * tile_size, j * tile_size, tile_size, tile_size);
            tile.copyTo(collage(roi));
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



int main() {
    std::srand(std::time(nullptr));
    cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_SILENT);

    std::string f = "C:\\c++\\misis2025s-3-improc\\prj.lab\\lab04\\runScript.json";
 
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

    cv::Mat collage = concateImages(n, min_elps_width, max_elps_width,
    min_elps_height, max_elps_height, bg_color, elps_color);

    cv::GaussianBlur(collage, collage, cv::Size(blur_size, blur_size), 0);

    cv::Mat noisedCollage = add_noise_gau(collage, noise_std);
    cv::imshow("Generated Collage", noisedCollage);
    cv::waitKey(0);
    
    std::string imagePath = output_path +"\\collage_gt.png";
    std::string jsonPath = output_path +"\\collage_gt.json";


    if (!cv::imwrite(imagePath, noisedCollage)) {
        std::cerr << "Error: save image " << imagePath << std::endl;
        return -1;
    }
    std::cout << "Image saved: " << imagePath << std::endl;

    cv::FileStorage fsJSON(jsonPath, cv::FileStorage::WRITE | cv::FileStorage::FORMAT_JSON);
    if (!fsJSON.isOpened()) {
        std::cerr << "Can not open jsonFile: " << jsonPath << std::endl;
        return -1;
    }

    fsJSON << "blur_size" << blur_size;

    fsJSON << "colors" << "{";
    fsJSON << "bg_color" << bg_color;
    fsJSON << "elps_color" << elps_color;
    fsJSON << "}";

    fsJSON << "noise_std" << noise_std;
    fsJSON << "size_of_collage" << 255;

    fsJSON << "objects" << "[";

    

    // обработка
    // 1 бинаризация

    cv::Mat binaryCollage;
    cv::threshold(noisedCollage, binaryCollage, 80, 255, cv::THRESH_BINARY);
    cv::imshow("Binary image", binaryCollage);
    cv::waitKey(0);

    std::string binaryPath = "C:\\c++\\misis2025s-3-improc\\assests\\lab4\\binaryImage.png";
    cv::imwrite(binaryPath, binaryCollage);


    int kernelSize = 5;
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(2 * kernelSize + 1, 2 * kernelSize + 1), cv::Point(kernelSize, kernelSize));
    cv::Mat openedImage;
    morphologyEx(binaryCollage, openedImage, cv::MORPH_OPEN, kernel);
    cv::imshow("Opened image", openedImage);
    cv::waitKey(0);

    std::string openedPath = "C:\\c++\\misis2025s-3-improc\\assests\\lab4\\openedImage.png";
    cv::imwrite(openedPath, openedImage);

    cv::Mat labels;
    cv::Mat stats;
    cv::Mat centroids;
    int numComponents = connectedComponentsWithStats(openedImage, labels, stats, centroids, 8, CV_32S);

    cv::Mat output;
    cvtColor(openedImage, output, cv::COLOR_GRAY2BGR);

    for (int i = 1; i < numComponents; ++i) {
        cv::Mat mask = labels == i;
        std::vector<std::vector<cv::Point>> contours;
        findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
        if (!contours.empty() && contours[0].size() >= 5) {
            cv::RotatedRect ellipse = fitEllipse(contours[0]);
            cv::ellipse(output, ellipse, cv::Scalar(0, 255, 0), 2);
            fsJSON << "{"
                << "elps_angle" << ellipse.angle
                << "elps_width" << static_cast<int>(ellipse.size.width)
                << "elps_height" << static_cast<int>(ellipse.size.height)
                << "elps_x" << static_cast<int>(ellipse.center.x)
                << "elps_y" << static_cast<int>(ellipse.center.y)
                << "}"
                << "pic_coordinates" << "{"
                << "col" << i % n * 256
                << "row" << i / n * 256
                    << "}";
        }
    }
    fsJSON << "]";
    fsJSON.release();
    imshow("Ellipses", output);
    cv::waitKey(0);
    std::string ellipsesPath = "C:\\c++\\misis2025s-3-improc\\assests\\lab4\\ellipsesImage.png";
    cv::imwrite(ellipsesPath, output);


    return 0;
}
