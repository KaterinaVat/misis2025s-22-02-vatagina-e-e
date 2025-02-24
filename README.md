# Отчет по лабораторной работе №1
#### Фурмулировка задания: 1.2 добавьте в библиотеку semcv функцию

```
	std::string semcv::strid_from_mat(const cv::Mat& img, const int n) {
	std::string height = converting_numbers(img.rows, n);
	std::string width = converting_numbers(img.cols, n);
	std::string channels = std::to_string(img.channels());
	int type = img.type();
	std::string typeStr;
	uchar depth = type & CV_MAT_DEPTH_MASK;
	uchar chans = 1 + (type >> CV_CN_SHIFT);
	switch (depth) {
	case CV_8U:  typeStr = "uint08"; break;
	case CV_8S:  typeStr = "sint08"; break;
	case CV_16U: typeStr = "uint16"; break;
	case CV_16S: typeStr = "sint16"; break;
	case CV_32S: typeStr = "sint32"; break;
	case CV_32F: typeStr = "real32"; break;
	case CV_64F: typeStr = "real64"; break;
	default:     typeStr = "unknown"; break;
	}
	std::string result = width + 'x' + height + '.' + channels + '.' + typeStr;
	saveImage(img);
	createFile("output_images");
	return result;
}
```
где присутствуют фунции:
######	- converting_numbers(img.rows, n) - для задания XXXXX вида чисел параметров
######	- saveImage(img) - для сохранения изображений в форматах .png, .tiff, .jpg
######	- createFile() - записи .lst


 #### Фурмулировка задания: 2.4 добавьте в библиотеку semcv функцию get_list_of_file_paths

```
std::vector<std::filesystem::path> semcv::get_list_of_file_paths(const std::filesystem::path& path_lst) {
	std::vector<std::filesystem::path> paths;
	std::ifstream in(path_lst, std::ios_base::in);
	std::string line;
	while (getline(in, line)) {
	paths.push_back(std::filesystem::absolute(line));
	}
	in.close();
	std::cout << "###Images wrote in the file###" << std::endl;
	for (int i = 0; i < paths.size(); i++) {
		std::cout << paths[i] << std::endl;
	}
	return paths;
}
```


 ![Example of output strid_from_mat and get_list_of_file_paths function](https://github.com/KaterinaVat/misis2025s-3-vatagina-e-e/blob/main/assests/strid_image_fun.png)
#### Фурмулировка задания: 2.4 тестовое консольное приложение task01-01
#### входные параметры:
#####  - путь к lst-файлу
![Example of output](https://github.com/KaterinaVat/misis2025s-3-vatagina-e-e/blob/main/assests/fun1_in_dir.png)
![Example of output](https://github.com/KaterinaVat/misis2025s-3-vatagina-e-e/blob/main/assests/2.png)


 #### Формулировка задания: 3.1 Реализация генерации серого одноканального 8bpp
    
```
cv::Mat semcv::greyImage() {
	cv::Mat img(30, 768, CV_8U, cv::Scalar(0)); 
	int value = 0; 
	int width = 768; 
	int height = 30; 
	int stripeWidth = 3; 

	for (int col = 0; col < width; col += stripeWidth) {
		for (int i = 0; i < stripeWidth; i++) {
			for (int row = 0; row < height; row++) {
				img.at<uchar>(row, col + i) = static_cast<uchar>(value);
			}
		}
		value++; 
	}

	cv::namedWindow("Striped Image", cv::WINDOW_AUTOSIZE);
	cv::imshow("Striped Image", img);
	cv::waitKey(0);
	return img;
}
```
![Генерация серого изображения](https://github.com/KaterinaVat/misis2025s-3-vatagina-e-e/blob/main/assests/greyImage.png)

 #### Формулировка задания: 3.2 Функция гаммы коррекции серого одноканального 8bpp->8bpp изображения
 ```
 cv::Mat semcv::gammaCorr(const cv::Mat& img, double& gamma) {
	cv::Mat normalized;
	img.convertTo(normalized, CV_32F, 1.0 / 255.0);
	cv::Mat gammaCorrected;
	cv::pow(normalized, gamma, gammaCorrected);
	gammaCorrected.convertTo(gammaCorrected, CV_8U, 255.0);

	return gammaCorrected;
}
```
 #### Формулировка задания: 3.3 Создание тестового консольного приложения, которое создает вертикальный коллаж из серых изображений, к которым применили гамма коррекцию
 ![Генерация коллажа](https://github.com/KaterinaVat/misis2025s-3-vatagina-e-e/blob/main/assests/collage.png)



