#pragma once
#include "open.h"

// 几何变换类
class GeometricTransformation {

    // 仿射变换的矩阵表示形式为原图像乘以仿射矩阵
public:
    //缩放
    void ImgResize(Mat src, Mat &dst, Size dimension);
    //旋转
    //平移
    //边缘提取
    //图像平滑与锐化
private:
    // 使用均值填充获得缩放图像
    void smallResize(Mat src, Mat &dst, Size dimension, float ratioX, float ratioY);
    // 使用双线性插值获得放大图像
    void augmentResize(Mat src, Mat &dst, Size dimension, float ratioX, float ratioY);
};