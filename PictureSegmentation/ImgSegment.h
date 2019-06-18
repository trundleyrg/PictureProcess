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
};