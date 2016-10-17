<span style="font-size:18px;">#include<opencv2/opencv.hpp>
#include<iostream>
#include"ZBase64.h"
#include<vector>


using namespace std;
using namespace cv;

void main()
{
	Mat img = imread("1.bmp");

	vector<uchar> vecImg;                               //Mat 图片数据转换为vector<uchar>
	vector<int> vecCompression_params;
	vecCompression_params.push_back(CV_IMWRITE_JPEG_QUALITY);
	vecCompression_params.push_back(90);
	imencode(".jpg", img, vecImg, vecCompression_params);

	ZBase64 base64;
	string imgbase64 = base64.Encode(vecImg.data(), vecImg.size());     //实现图片的base64编码

	cout << imgbase64 << endl;
}</span>