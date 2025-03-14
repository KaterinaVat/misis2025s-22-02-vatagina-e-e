#include <opencv2/core/utils/logger.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <./semcv/semcv.hpp>
using namespace cv;
int main()
{
    cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_SILENT);
 

	std::vector<std::vector<int>> brightLevel = { {15, 80, 190}, {50, 120, 220 } };
    std::vector<int> noise_std = { 3,7,15 };

	cv::Mat img;
	cv::Mat hist_collage(256*4, 256*2, CV_8U);
	cv::Mat hist;
	cv::Mat nImg;
	for (int i = 0; i < brightLevel.size(); i++) {
		img = gen_tgtimg00(brightLevel[i][0], brightLevel[i][1], brightLevel[i][2]); 
		hist = buildHist(img);
		cv::Rect roi(256 * i, 0, img.cols, img.rows);
		hist.copyTo(hist_collage(roi));
		for (int j = 0; j < 3; j++) {
			nImg = add_noise_gau(img, noise_std[j]);
			hist = buildHist(nImg);

			cv::Rect roi(i*256, (j + 1) * 256, hist.cols, hist.rows);
			hist.copyTo(hist_collage(roi));

		}
	}
	cv::imshow("hist collage", hist_collage);
	cv::waitKey(0);
}