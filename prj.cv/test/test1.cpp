#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>
#include <iostream>
#include <fstream>

void countCoins(cv::Mat src) {
    cv::Mat gray;
    //беру входное изображение и конвертирую его в серое изображение
    if (src.channels() == 3)
        cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);
    else
        gray = src.clone();

     // затем применяю размыкания к изображнию  (эрозия -> дилатация)
     // цель : удалить мелкие шумы на изображении, сохранив основную форму монеток
     cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(1, 35));
     cv::Mat opened;
     cv::morphologyEx(gray, opened, cv::MORPH_OPEN, kernel);
  

    
    cv::Mat enhanced;
    cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE();
    clahe->setClipLimit(4.0);
    clahe->apply(opened, enhanced);


    //// 3. Горизонтальный градиент (границы монет)
    cv::Mat grad_y;
    cv::Sobel(enhanced, grad_y, CV_16S, 0, 1);
    cv::convertScaleAbs(grad_y, grad_y);

    cv::Mat binary;
    cv::threshold(grad_y, binary, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);

    //// 5. Морфологическое закрытие для объединения линий
    cv::Mat morph;
    cv::Mat morph_kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(25, 3));
    cv::morphologyEx(binary, morph, cv::MORPH_CLOSE, morph_kernel);

    //// 6. Контуры
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(morph, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    //// 7. Фильтрация прямоугольников
    int count = 0;
    cv::Mat result = src.clone();
    for (const auto& contour : contours) {
        double area = cv::contourArea(contour);
        cv::Rect rect = cv::boundingRect(contour);
        float aspect = (float)rect.width / rect.height;

        if (area > 1000)
        {
            cv::rectangle(result, rect, cv::Scalar(0, 255, 0), 2);
            count++;
        }
    }

    std::cout << "Detected coins (boundaries): " << count << std::endl;


    cv::imshow("initial", gray);
    cv::imshow("opened", opened);
    cv::imshow("Enhanced", enhanced);
    cv::imshow("Gradient X", grad_y);
    cv::imshow("Binary", binary);
    cv::imshow("Morphology", morph);
    cv::imshow("Result", result);
    cv::waitKey(0);
}

int main() {
    cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_SILENT);
    cv::Mat image = cv::imread("C:\\c++\\misis2025s-3-improc\\prj.cv\\examples\\IMG_6448.png", cv::IMREAD_COLOR);
    if (image.empty()) {
        std::cerr << "Could not read the image." << std::endl;
        return -1;
    }
    countCoins(image);
    return 0;
}
