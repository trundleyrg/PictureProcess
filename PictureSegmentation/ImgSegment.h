#pragma once
#include "open.h"

// 图像分割类
class Segment {
public:
    // 阈值分割,轮廓查找与展示
    static void AdjustThreshold(int, void*);
    void SegThreshold(Mat input, Mat output, int value, int upValue);
    // 区域生长
    Mat RegionGrowGray(Mat src, Point2i pt, int th);
    // 膨胀
    void ImgDilate(Mat src, Mat res, Mat kernel);
    // 腐蚀
    void ImgErode(Mat src, Mat res, Mat kernel);
    // 开运算
    void OpeningOperation(Mat src, Mat res, Mat kernel);
    // 闭运算
    void CloseingOperation(Mat src, Mat res, Mat kernel);
    // 测地膨胀
    void GeodesicDilation(Mat src, Mat mask, Mat kernel, int iterations, Mat res);
    // 形态学梯度
    void MorphologicalGradient(Mat src, Mat res, Mat kernel);
    // 分水岭算法
    void WaterShed(Mat src, Mat& markers);
    // 随机生成颜色函数
    Vec3b RandomColor(int value);
};