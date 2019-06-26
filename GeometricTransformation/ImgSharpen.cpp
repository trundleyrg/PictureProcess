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