#include "open.h"
#include "color.h"

Mat ColorHist::ConvertColor2Gray(Mat color) {
    Mat gray;
    cvtColor(color, gray, COLOR_RGB2GRAY);
    //imwrite("results/CvtColor2Gray.jpg", gray);
    return gray;
}

Mat ColorHist::ConvertColor2Gray1(Mat color) {
    Mat gray(color.rows, color.cols, 0, 1);
    int rowNum = color.rows;
    int colNum = color.cols;
    for (int i = 0; i < rowNum; i++)
        for (int j = 0; j < colNum; j++) {
            gray.at<uchar>(i, j) = saturate_cast<uchar>((color.at<Vec3b>(i, j)[0] + 
                color.at<Vec3b>(i, j)[1] + color.at<Vec3b>(i, j)[2])/3);
        }
    /*imshow("gray", gray);
    waitKey(0);*/
    //imwrite("results/CvtColor2Gray1.jpg", gray);
    return gray;
}

void ColorHist::GetChannel(Mat color) {
    vector<Mat> channels;
    Mat blue;
    Mat green;
    Mat red;
    split(color, channels);
    blue = channels.at(0);
    green = channels[1];
    red = channels[2];

    Mat mergeImage;
    merge(vector<Mat>{blue, green, red}, mergeImage);
    imshow("merge", mergeImage);
    waitKey(0);
}

void ColorHist::DisplayColorHistogram(Mat color) {    
    Mat dstHist;
    int channels = 0;
    int size[] = { 256 };
    float range[] = { 0,255 };
    const float *ranges[] = { range };

    //blue Histogram
    calcHist(&color, 1, &channels, Mat(), dstHist,
        1, size, ranges, true, false);
    Mat BdrawImg = Mat::zeros(Size(256, 256), CV_8UC3);
    double BhistMaxValue;
    minMaxLoc(dstHist, 0, &BhistMaxValue, 0, 0);
    for (int i = 0; i < 256; i++) {
        int value = cvRound(256 * 0.9 * (dstHist.at<float>(i) / BhistMaxValue));
        line(BdrawImg, Point(i, BdrawImg.cols - 1), 
            Point(i, BdrawImg.cols - 1 - value), Scalar(255, 0, 0));
    }

    //green Histogram
    channels = 1;
    calcHist(&color, 1, &channels, Mat(), dstHist,
        1, size, ranges);
    Mat GdrawImg = Mat::zeros(Size(256, 256), CV_8UC3);
    double GhistMaxValue;
    minMaxLoc(dstHist, 0, &GhistMaxValue, 0, 0);
    for (int i = 0; i < 256; i++) {
        int value = cvRound(256 * 0.9 * (dstHist.at<float>(i) / GhistMaxValue));        
        line(GdrawImg, Point(i, GdrawImg.cols - 1),
            Point(i, GdrawImg.cols - 1 - value), Scalar(0, 255, 0));
    }

    channels = 2;
    calcHist(&color, 1, &channels, Mat(), dstHist,
        1, size, ranges);
    Mat RdrawImg = Mat::zeros(Size(256, 256), CV_8UC3);
    double RhistMaxValue;
    minMaxLoc(dstHist, 0, &RhistMaxValue, 0, 0);
    for (int i = 0; i < 256; i++) {
        int value = cvRound(256 * 0.9*(dstHist.at<float>(i) / RhistMaxValue));
        rectangle(RdrawImg, Point(i, RdrawImg.cols - 1),
            Point(i + 1, RdrawImg.cols - 1 - value), Scalar(0, 0, 255));
    }

    add(BdrawImg, GdrawImg, RdrawImg);
    cout << RhistMaxValue << endl;
    imshow("ColorHistogram", RdrawImg);
    waitKey(0);
}

Mat ColorHist::CreateComparisonTable(Mat single) {
    Mat dsthist;
    int channels = 0;
    int size[] = { 256 };
    float range[] = { 0,256 };
    const float *ranges[] = { range };

    //getHistogram
    calcHist(&single, 1, &channels, noArray(), dsthist, 1, size, ranges);
    Mat ComparasionTable = Mat::zeros(1, 256, CV_8U);
    for (int i = 0; i < 256; i++) {
        // (L-1)*sum(i)/MN
        ComparasionTable.at<uchar>(i) = (uchar)(255 * dsthist.at<float>(i) / (single.cols*single.rows));
    }
    for (int i = 1; i < 256; i++) {
        ComparasionTable.at<uchar>(i) = saturate_cast<uchar>(
            ComparasionTable.at<uchar>(i) + ComparasionTable.at<uchar>(i - 1));
    }
    return ComparasionTable;
}

void ColorHist::EqualizeColorHist(Mat color) {
    //get Max and min
    vector<Mat> channels;
    split(color, channels);
    Mat BComposisonTable = CreateComparisonTable(channels[0]);
    Mat GComposisonTable = CreateComparisonTable(channels[1]);
    Mat RComposisonTable = CreateComparisonTable(channels[2]);
    for (int z = 0; z < 3; z++)
        for (int i = 0; i < channels[z].rows; i++)
            for (int j = 0; j < channels[z].cols; j++) {
                channels[z].at<uchar>(i, j) = BComposisonTable.at<uchar>(channels[z].at<uchar>(i, j));
        }
    Mat mergeImg;
    merge(channels, mergeImg);
    imshow("equal", mergeImg);
    waitKey(0);
    //imwrite("./results/ColorEqualHist.jpg",mergeImg);
    DisplayColorHistogram(mergeImg);
}

//将RGB格式转换为YCrCb格式，对Y通道做直方图均衡
void ColorHist::EqualizeColorHist1(Mat color) {
    Mat hsv, tempImg, desImg;
    cvtColor(color, hsv, CV_BGR2YCrCb);
    vector<Mat> channels;
    split(hsv, channels);
    equalizeHist(channels[0], channels[0]);
    
    //YUV2RGB    
    merge(channels, tempImg);
    cvtColor(tempImg, desImg, CV_YCrCb2BGR);
    imshow("equalHist", desImg);
    waitKey(0);
    DisplayColorHistogram(desImg);
}