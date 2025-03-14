# ����� �� ������������ ������ �1
#### ������������ �������: 1 �������� � ���������� semcv �������
``` 

cv::Mat gen_tgtimg00(const int lev0, const int lev1, const int lev2) {
	cv::Mat tgtimg(256, 256, CV_8U,cv::Scalar(lev0));
	cv::rectangle(tgtimg, cv::Point(23, 23), cv::Point(232, 232), cv::Scalar(lev1), -1);
	cv::circle(tgtimg, cv::Point(128, 128), 83, cv::Scalar(lev2), -1);
  return tgtimg;
}
```
 ![������ ������ ������� gen_tgtimg00](https://github.com/KaterinaVat/misis2025s-3-vatagina-e-e/blob/main/assests/lab2/func_t.png)

 #### ������������ �������: 2 �������� � ���������� semcv �������
 ```
 cv::Mat add_noise_gau(const cv::Mat& img, const int std) {
	double mean = 0;
	cv::Mat mat(img.size(), CV_8U);
	cv::randn(mat, cv::Scalar(mean), cv::Scalar(std));
	cv::Mat res;
	cv::add(img, mat, res);
	return res;
}
 ```
 ![������ ������ ������� add_noise_gau](https://github.com/KaterinaVat/misis2025s-3-vatagina-e-e/blob/main/assests/lab2/add_noise.png)


 #### ������������ �������: 3 �������� �������� ���������� ���������� task02, �������
 #### 3.1 ���������� �������� ����������� ��� 4-� ������� ������� ������� ����-��������-����� ([0,127,255], [20,127,235], [55,127,200], [90,127,165]) 
  ![](https://github.com/KaterinaVat/misis2025s-3-vatagina-e-e/blob/main/assests/lab2/gen_br_1.png)
  ![](https://github.com/KaterinaVat/misis2025s-3-vatagina-e-e/blob/main/assests/lab2/gen_br2.png)
  ![](https://github.com/KaterinaVat/misis2025s-3-vatagina-e-e/blob/main/assests/lab2/gen_br3.png)
  ![](https://github.com/KaterinaVat/misis2025s-3-vatagina-e-e/blob/main/assests/lab2/gen_br4.png)

  #### 3.2 ������� ����������� � �������������� ������
  ![](https://github.com/KaterinaVat/misis2025s-3-vatagina-e-e/blob/main/assests/lab2/collage.png)

  #### 3.3 3.4 ��������� ��� � ��������, � ����� ������� � ������������ ������
   - �������������� 
	- ��� � ����������� ����������� 3
	- ��� � ����������� ����������� 7
	- ��� � ����������� ����������� 15

  ![](https://github.com/KaterinaVat/misis2025s-3-vatagina-e-e/blob/main/assests/lab2/final_collage.png)

  #### 3.5 ��������� �������� ��������� � ���� �� ���������� � �������� ��������� ��������� ������ ����
  ![](https://github.com/KaterinaVat/misis2025s-3-vatagina-e-e/blob/main/assests/lab2/final.png)

  #### ��������� ������������� ��������� ����������� � ������������
  ![](https://github.com/KaterinaVat/misis2025s-3-vatagina-e-e/blob/main/assests/lab2/table.png)

  #### ������ ���������� ������� ����������� �����������
  ![](https://github.com/KaterinaVat/misis2025s-3-vatagina-e-e/blob/main/assests/lab2/histBRCollage.png)
