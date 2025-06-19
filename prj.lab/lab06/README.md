# Лабораторная работа №6  
**Детектирование объектов методом Blob Detection с использованием пирамиды масштабов**

## Использование:
.\task6.exe [image path]

Реализация детектора пятен (blob detection) с использованием:
- Пирамиды масштабов
- Линейных фильтров (LoG/DoG)
- Анализа экстремумов в масштабно-пространстве

### 1. Реализованные алгоритмы

#### Пирамида масштабов
```cpp
std::vector<cv::Mat> ScalePyramid(const cv::Mat& image) {
    std::vector<cv::Mat> pyramid;
    cv::Mat current = image.clone();
    pyramid.push_back(current);
    
    for (int i = 1; i < 5; ++i) {
        current = picDown(current); // Уменьшение в 2 раза с гауссовым размытием
        pyramid.push_back(current);
    }
    return pyramid;
}
```

- Difference of Gaussians (DoG)
```cpp
std::vector<cv::Mat> DOG(cv::Mat& gray_image, std::vector<double>& sigmas) {
    std::vector<cv::Mat> dogs;
    for (int i = 1; i < blurred.size(); i++) {
        cv::Mat dog = blurred[i] - blurred[i - 1];
        dogs.push_back(dog);
    }
    return dogs;
}
```
- Детектор пятен
```cpp
std::vector<Blob> blob_detector(const std::vector<cv::Mat>& dogs, ...) {
    // Поиск локальных экстремумов в 3D-пространстве (x,y,scale)
    if (isMax || isMin) {
        blobs.push_back({x, y, sigma, scale});
    }
}
```
![Результат работы алгоритма](https://github.com/KaterinaVat/misis2025s-22-02-vatagina-e-e/blob/main/assests/lab6/blobs_pyramid_detected.png)
2. Ключевые параметры
Параметр	Значение	Описание
Уровней пирамиды	5	От исходного до 1/16 размера
σ₀	0.5	Базовый масштаб
Порог	0.2	Минимальная значимость пятна
