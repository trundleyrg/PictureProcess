#pragma once
#include "open.h"

#define BILINEAR_INTERPOLATION 1 //双线性插值法
#define NEARESTNEIGHBOR_INTERPOLATION 2  //最近邻插值
enum Filter
{
    BOX_FILTER,//盒状滤波器
    //MEDIAN_FILTER,//中值滤波器,比较特殊，单独实现
    GAUSSIAN_FILTER//高斯滤波器
};
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
    //图像平滑，使用盒状滤波器
    void ImgSmooth(Mat src, Mat &dst, Size dimension,Filter f=BOX_FILTER,double sigma=1.0);
    //图像锐化
    void ImgSharpen(Mat src, Mat &dst, Size dimension, Filter f = BOX_FILTER,double sigma=1.0);
private:
    //下面两种值填充法只能用于缩放函数中使用，还没有实现通用化
    // 均值填充法
    void smallResize(Mat src, Mat &dst, Size dimension, float ratioX, float ratioY);
    // 双线性插值法
    void augmentResize(Mat src, Mat &dst, Size dimension, float ratioX, float ratioY);
    
    // 滤波器初始化

    // 盒状滤波器
    // dimension：滤波器尺寸
    Mat BoxFilter(Size dimension);
    // 高斯滤波器
    // sigma:定义高斯滤波器标准差
    Mat GaussianFilter(Size dimension, double sigma);
};