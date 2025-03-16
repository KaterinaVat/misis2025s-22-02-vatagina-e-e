
#pragma once
#ifndef MISIS2025S_3_SEMCV
#define MISIS2025S_3_SEMCV



#include <opencv2/core.hpp>
#include <string>
#include <opencv2/core.hpp>
#include <vector>
#include <filesystem>

std::string converting_numbers(int number, const int n = 4);

void saveImage(const cv::Mat& img);

void createFile(const std::string& folderPath);

std::string strid_from_mat(const cv::Mat& img, const int n = 4);

std::vector<std::filesystem::path> get_list_of_file_paths(const std::filesystem::path& path_lst);

cv::Mat gen_tgtimg00(const int lev0, const int lev1, const int lev2);

cv::Mat greyImage();

cv::Mat gammaCorr(const cv::Mat& img, double& gamma);

cv::Mat add_noise_gau(const cv::Mat& img, const int std);
 
void imageStatistics(cv::Mat& img, cv::Mat& noiseImg);
void imageStatistics_rgb(cv::Mat& img);

cv::Mat buildHist(cv::Mat& src);

cv::Mat buildHistColor(cv::Mat& src); 


cv::Mat autocontrast(const cv::Mat& img, const double q_black, const double q_white, bool flag = false);

cv::Mat autocontrastColor(const cv::Mat& img, const double q_blue, const double q_red, const double q_green, bool flag = false);

cv::Mat autocontrast_rgb(const cv::Mat& img, const double q_black, const double q_white, bool flag = false);


#endif