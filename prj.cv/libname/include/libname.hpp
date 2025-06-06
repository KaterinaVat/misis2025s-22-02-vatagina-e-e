#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>


cv::Mat filterI(cv::Mat collage, cv::Mat kernel);

int find_periodical_structure(cv::Mat& plot, std::vector<float>& varience);

void window_varience(cv::Mat& image, std::vector<float>& varience);

cv::Mat build_plot(cv::Mat& image, std::vector<float>& varience, float max_var);

void suppression_not_max(cv::Mat& plot, std::vector<float>& varience, float& max_var, int& plot_x);

cv::Mat concateImages(cv::Mat& imageGradVertNew, cv::Mat& smoothed_plot);

int count_money(cv::Mat input_image);
