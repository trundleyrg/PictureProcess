#pragma once
#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

Mat ConvertColor2Gray(Mat color);//api come true
Mat ConvertColor2Gray1(Mat color);//matrix process
void GetChannel(Mat color);
void DisplayColorHistogram(Mat RawImg);
void EqualizeColorHist(Mat color);