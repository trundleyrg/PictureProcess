#include "GeometricTransformation.h"

// 图像锐化
// 拉普拉斯模板,3*3
// 空间域处理方法
void ImgSharpen::Sharpen(Mat src, Mat &dst) {
    dst = Mat(src.size(), CV_8UC3, Scalar::all(0));
    //生成拉普拉斯模板算子
    int laplacian[3][3] = { {0,1,0},{1,-4,1},{0,1,0} };
    //和原图像做卷积
    for (int j = 0; j < dst.rows; ++j) {
        for (int i = 0; i < dst.cols; ++i) {
            for (int c = 0; c < dst.channels(); ++c) {
                int tempSum = 0;
                for (int a = -1; a <= 1; a++) {
                    for (int b = -1; b <= 1; b++) {
                        if (i + a < 0 || i + a >= src.cols || j + b < 0 || j + b >= src.rows) {//访问越界检查
                            continue;
                        }
                        tempSum += laplacian[1 + a][1 + b] * src.at<Vec3b>(i + a, j + b)[c];
                    }
                }
                dst.at<Vec3b>(i, j)[c] = abs(tempSum);
            }
        }
    }
    imshow("锐化结果", dst);
}

// 非锐化掩蔽/高提升滤波
// weight>1为高提升滤波
//1. 模糊原图像,模糊图像可以通过对原图做锐化来获得
//2. 从原图像中减去模糊图像，获得模板图像    
//3. 将模板加到原图像上
void ImgSharpen::UnsharpenedMask(Mat src, Mat &dst, float weight) {
    ImgSharpen::Sharpen(src, dst);
    Mat maskImg;
    maskImg = src - dst;
    addWeighted(src, 1, maskImg, weight, 0.0, dst);
    imshow("高提升滤波", dst);
}