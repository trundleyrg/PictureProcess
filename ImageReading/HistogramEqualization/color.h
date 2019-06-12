#pragma once
#include "open.h"

class ColorHist{
public:
    // 彩色图像转换为灰度图像，原生实现和逐像素操作实现
    Mat ConvertColor2Gray(Mat color);
    Mat ConvertColor2Gray1(Mat color);
    // 通道分离和合并
    void GetChannel(Mat color);
    // 彩色图片像素分布直方图
    void DisplayColorHistogram(Mat color);
    // 彩色图直方图均衡
    void EqualizeColorHist(Mat color);
    // 彩色图直方图均衡1
    void EqualizeColorHist1(Mat color);
private:
    // 直方图均衡辅助函数，计算像素变换对照表
    Mat CreateComparisonTable(Mat single);
};