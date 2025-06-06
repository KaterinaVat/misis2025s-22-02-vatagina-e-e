#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>
#include <libname/include/libname.hpp>
#include <iostream>
#include <fstream>
#include <string>

int main() {

    cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_SILENT);
    std::ifstream f("C:\\c++\\misis2025s-3-improc\\prj.cv\\examples\\input_data.txt");

    if (!f.is_open()) {
        std::cerr << "Error opening file!" << std::endl;
        return 1;
    }

    std::string line;
    std::string path;
    int number;
    double sum = 0;
    std::cout << "test ¹" << "      " << "detected" << "       " << "ground_truth" << "         "<<"acs"<< std::endl;
    int n = 0;
    while (std::getline(f, line)) {
        n++;
        size_t space_pos = line.find(' ');
        path = line.substr(0, space_pos);
        int ground_truth = std::stoi(line.substr(space_pos + 1));

        cv::Mat image = cv::imread(path);

        cv::Mat gray;
        if (image.channels() == 3)
            cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
        else
            gray = image.clone();

        int count_coins = count_money(gray);

        double error = std::abs(count_coins - ground_truth) / static_cast<double>(ground_truth);
        double acs = (1.0 - error) * 100.0;
        std::cout << "test " << n << "        " << count_coins << "         " << ground_truth << "                "<<acs<<  std::endl;

    }

    f.close(); 

    return 0;
}