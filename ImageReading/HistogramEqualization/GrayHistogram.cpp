#include "open.h"
#include "color.h"

//display gray image histogram
void DisplayHistogram(Mat RawImg) {
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
        rectangle(dstImg, Point(i*scale, size - 1),//left bottom point
            Point((i + 1)*scale - 1, size - realValue), //right top point
            Scalar(255)); //display color
    }
    imshow("histogram", dstImg);
    //imwrite("results/lenaGrayHistogram.jpg", dstImg);
    waitKey(0);
}

void EqualizeHist(Mat RawImg) {
    //input and output images need the same size
    Mat ResImg(RawImg.rows, RawImg.cols, RawImg.type(), RawImg.channels());
    equalizeHist(RawImg, ResImg);
    imshow("qualizeHist", ResImg);
    //imwrite("results/lenaGrayEqalizeHist.jpg", ResImg);
    waitKey(0);
}

int main() {
    //gray image process
    Mat img = imread("lena512.bmp",CV_LOAD_IMAGE_GRAYSCALE);
    if (img.empty()) {
        return -1;
    }
    //gray image process
    DisplayHistogram(img);
    EqualizeHist(img);

    //color image process
    ColorHist demo;
    Mat color = imread("lena512color.tiff");
    if (color.empty()) {
        return -1;
    }
    imshow("color", color);
    waitKey(0);
    demo.ConvertColor2Gray(color);
    Mat gray = demo.ConvertColor2Gray1(color);
    imshow("gray", gray);
    waitKey(0);
    demo.GetChannel(color);
    demo.DisplayColorHistogram(color);
    demo.EqualizeColorHist1(color);
}