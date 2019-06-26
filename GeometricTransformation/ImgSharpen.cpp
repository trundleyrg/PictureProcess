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

// sobel算子
// 不同于拉普拉斯算子，sobel算子是一阶微分算子，分别在水平方向和垂直方向上做边缘检测
// 为了加快计算速度，不使用G=sqrt(pow(Gx,2),pow(Gy,2))，而使用 G=abs(Gx)+abs(Gy)来近似代替
void ImgSharpen::SobelSharp(Mat src, Mat &dst) {
    Mat dstX = Mat::zeros(src.size(), src.type());
    Mat dstY = Mat::zeros(src.size(), src.type());
    /*uchar* p = src.data;
    uchar* pX = dstX.data;
    uchar* pY = dstY.data;
    int stepS = src.step;
    int stepX = dstX.step;
    int stepY = dstY.step;*/
    int ImgChannels = src.channels();
    for (int i = 1; i < src.cols - 1; i++) {
        for (int j = 1; j < src.rows - 1; j++) {
            dstX.at<uchar>(i, j) = abs(saturate_cast<uchar>(src.at<uchar>(i + 1, j + 1)
                + src.at<uchar>(i - 1, j + 1) + 2 * src.at<uchar>(i, j + 1)
                - 2 * src.at<uchar>(i, j - 1) - src.at<uchar>(i - 1, j - 1) - src.at<uchar>(i + 1, j - 1)
                ));
            dstY.at<uchar>(i, j) = abs(saturate_cast<uchar>(src.at<uchar>(i + 1, j + 1)
                + src.at<uchar>(i + 1, j - 1) + 2 * src.at<uchar>(i+1, j)
                - 2 * src.at<uchar>(i-1, j) - src.at<uchar>(i - 1, j - 1) - src.at<uchar>(i - 1, j + 1)
                ));
            /**(pX + i*ImgChannels + j*stepX + c) =
                *(p + (i - 1) * ImgChannels + (j + 1)*stepS + c) - *(p + (i - 1) * ImgChannels + (j - 1)*stepS + c) +
                2 * (*(p + i * ImgChannels + (j + 1)*stepS + c) - *(p + i * ImgChannels + (j - 1)*stepS + c)) +
                *(p + (i + 1) * ImgChannels + (j + 1)*stepS + c) - *(p + (i + 1) * ImgChannels + (j - 1)*stepS + c);
            *(pY + i * 3 + j*stepX + c) =
                *(p + (i + 1) * ImgChannels + (j - 1)*stepS + c) - *(p + (i - 1) * ImgChannels + (j - 1)*stepS + c) +
                2 * (*(p + (i + 1) * ImgChannels + j *stepS + c) - *(p + (i - 1) * ImgChannels + j *stepS + c)) +
                *(p + (i + 1) * ImgChannels + (j + 1)*stepS + c) - *(p + (i - 1) * ImgChannels + (j + 1)*stepS + c);*/
        }
    }
    imshow("水平", dstX);
    imshow("竖直", dstY);
    //竖直和水平 加权和
    addWeighted(dstX, 0.5, dstY, 0.5, 0, dst);
    imshow("融合结果", dst);
    //imwrite("sobel锐化.jpg", dst);
}

