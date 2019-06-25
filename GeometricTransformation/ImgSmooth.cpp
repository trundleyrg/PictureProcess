#include "GeometricTransformation.h"

// 图像平滑
// 空间域处理方法
void Smooth::ImgSmooth(Mat src, Mat &dst, Size dimension, Filter f, double sigma) {
    dst = Mat(src.size(), CV_8UC3, Scalar::all(0));

    if (dimension.height % 2 == 0 || dimension.width % 2 == 0) {//模板尺寸检查
        cout << "请使用奇数大小的滤波器尺寸" << endl;
        return;
    }
    //滤波器模板初始化
    Mat filter = Mat(dimension, CV_8U, Scalar::all(0));
    switch (f) {
    case BOX_FILTER:
        filter = BoxFilter(dimension);
        break;
    case GAUSSIAN_FILTER:
        filter = GaussianFilter(dimension, sigma);
        break;
    }
    //调试：滤波器值观察
    /*for (int i = 0; i < filter.cols; i++) {
    for (int j = 0; j < filter.rows; j++) {
    cout << (int)filter.at<uchar>(i, j) << " ";
    }
    cout << endl;
    }*/

    //原图像滑动范围定义
    int xMin = floor(dimension.width / 2);
    int yMin = int(floor(dimension.height / 2));
    int sumValue = 0;
    //滤波器模板系数计算
    for (int a = -xMin; a <= xMin; ++a) {
        for (int b = -yMin; b <= yMin; ++b) {
            sumValue += (int)filter.at<uchar>(a + xMin, b + yMin);
        }
    }
    if (sumValue == 0) {
        cout << "请确定滤波器模板是否存在" << endl;
        return;
    }

    //卷积核在原图像上滑动完成平滑处理
    for (int c = 0; c < 3; c++) {
        for (int j = 0; j < src.rows; ++j) {
            for (int i = 0; i < src.cols; ++i) {
                int meanValue = 0;
                for (int a = -xMin; a <= xMin; ++a) {
                    for (int b = -yMin; b <= yMin; ++b) {
                        if (i + a < 0 || i + a >= src.cols || j + b < 0 || j + b >= src.rows) {//访问越界检查
                            continue;
                        }
                        meanValue += filter.at<uchar>(a + xMin, b + yMin)*src.at<Vec3b>(i + a, j + b)[c];
                    }
                }
                dst.at<Vec3b>(i, j)[c] = saturate_cast<uchar>(meanValue / sumValue);
            }
        }
    }
    imshow("图像平滑", dst);
}

// 中值滤波
// 该函数执行速度慢，效率低
void Smooth::ImgMeanFilter(Mat src, Mat &dst, Size dimension) {
    dst = Mat(src.size(), CV_8UC3, Scalar::all(0));
    int xMin = floor(dimension.width / 2);
    int yMin = int(floor(dimension.height / 2));
    for (int c = 0; c < 3; c++) {
        for (int j = 0; j < dst.rows; ++j) {
            for (int i = 0; i < dst.cols; ++i) {
                vector<uchar> FilterNumber;//图像像素临时存储数组,使用vector是考虑到边界像素处不能取到一共九个像素值
                uchar mid = 0;
                for (int a = -xMin; a <= xMin; a++) {
                    for (int b = -yMin; b <= yMin; b++) {
                        if (i + a < 0 || i + a >= src.cols || j + b < 0 || j + b >= src.rows) {//访问越界检查
                            continue;
                        }
                        FilterNumber.push_back(src.at<Vec3b>(i + a, j + b)[c]);
                    }
                }
                //排序获得中位数
                sort(FilterNumber.begin(), FilterNumber.end());
                mid = FilterNumber.at(int(FilterNumber.size() / 2));
                dst.at<Vec3b>(i, j)[c] = mid;
            }
        }
    }
    imshow("中值滤波", dst);
}

//滤波器

//盒状滤波器
//所有系数都相等的空间均值滤波器,
Mat Smooth::BoxFilter(Size dimension) {
    return Mat::ones(dimension, CV_8U);
}

//高斯平滑滤波器
//根据高斯分布函数计算模板上各元素值
//模板尺寸(2k+1)
//高斯分布函数 h(i,j)=1/(2*pi*sigma^2)*exp((-pow(i-k-1,2)-pow(j-k-1,2))/2*sigma*sigma)
Mat Smooth::GaussianFilter(Size dimension, double sigma) {
    Mat f = Mat(dimension, CV_8U, Scalar::all(0));
    int kx = (dimension.width - 1) / 2;
    int ky = (dimension.height - 1) / 2;
    double factor = 1 / (2 * M_PI*sigma*sigma);
    for (int i = 0; i < dimension.width; i++) {
        int x = pow((i - kx - 1) *(i - kx - 1), 2);
        for (int j = 0; j < dimension.height; j++) {
            int y = pow((j - ky - 1)*(j - ky - 1), 2);
            f.at<uchar>(i, j) = saturate_cast<uchar>(factor*exp((-x - y) / (2 * sigma*sigma)) + 0.5);
        }
    }
    return f;
}