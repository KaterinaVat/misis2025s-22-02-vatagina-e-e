#include "../semcv/semcv.hpp"
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <filesystem> 
#include <opencv2/core/utils/logger.hpp>
#include <vector>
namespace fs = std::filesystem;


int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Give pass to safe grey image collage" << std::endl;

    }

    cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_SILENT);

    cv::Mat img = greyImage();
    std::vector<double> gamma = { 1, 1.8, 2.0, 2.2, 2.4, 2.6 };

    cv::Mat gammaCollage(img.rows * gamma.size(), img.cols, CV_8U, cv::Scalar(0));

    for (int i = 0; i < gamma.size(); i++) {
        cv::Mat currentImg = gammaCorr(img, gamma[i]);

        int y = i * img.rows;
        cv::Rect roi(0, y, currentImg.cols, currentImg.rows);
        currentImg.copyTo(gammaCollage(roi));
    }


    cv::imshow("Collage", gammaCollage);
    cv::waitKey(0);

    std::string filename = std::string(argv[1]) + '/' + "greyimage.png";
    std::cout << "was saved by pass " << filename;
    cv::imwrite(filename, gammaCollage);

    return 0;

}
