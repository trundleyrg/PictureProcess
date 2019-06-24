#pragma once
#include "open.h"

#define BILINEAR_INTERPOLATION 1 //双线性插值法
#define NEARESTNEIGHBOR_INTERPOLATION 2  //最近邻插值

// 几何变换类
class GeometricTransformation {

    // 仿射变换的矩阵表示形式为原图像乘以仿射矩阵
public:
    //缩放
    void ImgResize(Mat src, Mat &dst, Size dimension,int method=BILINEAR_INTERPOLATION);
    //旋转
    void ImgRotate(Mat src, Mat &dst, double angle);
    //平移
    void ImgWave(Mat src, Mat &dst, Size wave);
    //边缘提取
    //图像平滑与锐化
private:
    // 均值填充法
    void smallResize(Mat src, Mat &dst, Size dimension, float ratioX, float ratioY);
    // 双线性插值法
    void augmentResize(Mat src, Mat &dst, Size dimension, float ratioX, float ratioY);
};