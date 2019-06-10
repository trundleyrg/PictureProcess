#include <stdio.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
//#include "Color.cpp"

using namespace cv;
using namespace std;

void DispalyHistogram1(Mat RawImg) {
    Mat dstHist;
    int dims = 1;
    float hranges[] = { 0,255 };
    const float *ranges[] = { hranges };
    int size = 256;
    int channels = 0;

    calcHist(&RawImg, 1, &channels, Mat(), dstHist, dims, &size, ranges);
    int scale = 1;

    Mat dstImg(size*scale, size, CV_8U, Scalar(0));
    //get min and max value
    double minValue = 0, maxValue = 0;
    minMaxLoc(dstHist, &minValue, &maxValue, 0, 0);

    //draw Histogram
    int hpt = saturate_cast<int>(0.9*size);
    for (int i = 0; i < 256; i++) {
        float binValue = dstHist.at<float>(i);
        int realValue = saturate_cast<int>(binValue*hpt / maxValue);
        rectangle(dstImg, Point(i*scale, size - 1), Point((i + 1)*scale - 1, size - realValue), Scalar(255));
    }
    imshow("histogram", dstImg);
    waitKey(0);
}

void EqualizeHist(Mat RawImg) {
    Mat ResImg;
    equalizeHist(RawImg, ResImg);
    imshow("qualizeHist", ResImg);
    waitKey(0);
}
int main() {
    Mat img = imread("lena512.bmp");
    if (img.empty()) {
        printf("image not exist!");
        return -1;
    }
    //EqualizeHist(img);
    DispalyHistogram1(img);
}