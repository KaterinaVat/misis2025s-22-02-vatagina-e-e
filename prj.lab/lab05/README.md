# Лабораторная работа №5  
**Исследование линейной фильтрации изображений**  

## Цель работы  
Изучение эффектов линейной фильтрации с различными ядрами и визуализация результатов комбинированной обработки.

## Методика эксперимента

### 1. Генерация тестового изображения
Создано изображение размером `381×254` пикселя (3×2 квадрата) с параметрами:
- Размер квадрата: `127×127` пикселей
- Радиус круга: `40` пикселей
- Яркости: `{0, 127, 255}` (все 9 возможных комбинаций)

![Тестовое изображение](https://github.com/KaterinaVat/misis2025s-22-02-vatagina-e-e/raw/main/assests/lab5/1.png)

### 2. Применённые фильтры

#### Фильтрация с ядром I₁
![Результат фильтрации I₁](https://github.com/KaterinaVat/misis2025s-3-vatagina-e-e/raw/main/assests/lab5/2.png)

```cpp
cv::Mat kernel1 = (cv::Mat_<float>(3, 3) <<
    0, 1, 2,
    -1, 0, 1,
    -2, -1, 0);
```

  - фильтрация с ядром I2
  ![](https://github.com/KaterinaVat/misis2025s-3-vatagina-e-e/blob/main/assests/lab5/3.png)
```cpp
	cv::Mat kernel2 = (cv::Mat_<float>(3, 3) <<
		2, 1, 0,
		1, 0, -1,
		0, -1, -2);
```

  - фильтрация с ядром I3
  ![](https://github.com/KaterinaVat/misis2025s-3-vatagina-e-e/blob/main/assests/lab5/4.png)
```cpp
I1 = sqrt(I1+I2)
```

- RGB-изображение
 ![](https://github.com/KaterinaVat/misis2025s-3-vatagina-e-e/blob/main/assests/lab5/5.png)

- визуализация результатов обработки
   ![](https://github.com/KaterinaVat/misis2025s-3-vatagina-e-e/blob/main/assests/lab5/6.png)

