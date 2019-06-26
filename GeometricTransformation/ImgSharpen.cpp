#include "GeometricTransformation.h"

// ͼ����
// ������˹ģ��,3*3
// �ռ�������
void ImgSharpen::Sharpen(Mat src, Mat &dst) {
    dst = Mat(src.size(), CV_8UC3, Scalar::all(0));
    //����������˹ģ������
    int laplacian[3][3] = { {0,1,0},{1,-4,1},{0,1,0} };
    //��ԭͼ�������
    for (int j = 0; j < dst.rows; ++j) {
        for (int i = 0; i < dst.cols; ++i) {
            for (int c = 0; c < dst.channels(); ++c) {
                int tempSum = 0;
                for (int a = -1; a <= 1; a++) {
                    for (int b = -1; b <= 1; b++) {
                        if (i + a < 0 || i + a >= src.cols || j + b < 0 || j + b >= src.rows) {//����Խ����
                            continue;
                        }
                        tempSum += laplacian[1 + a][1 + b] * src.at<Vec3b>(i + a, j + b)[c];
                    }
                }
                dst.at<Vec3b>(i, j)[c] = abs(tempSum);
            }
        }
    }
    imshow("�񻯽��", dst);
}

// �����ڱ�/�������˲�
// weight>1Ϊ�������˲�
//1. ģ��ԭͼ��,ģ��ͼ�����ͨ����ԭͼ���������
//2. ��ԭͼ���м�ȥģ��ͼ�񣬻��ģ��ͼ��    
//3. ��ģ��ӵ�ԭͼ����
void ImgSharpen::UnsharpenedMask(Mat src, Mat &dst, float weight) {
    ImgSharpen::Sharpen(src, dst);
    Mat maskImg;
    maskImg = src - dst;
    addWeighted(src, 1, maskImg, weight, 0.0, dst);
    imshow("�������˲�", dst);
}

// sobel����
// ��ͬ��������˹���ӣ�sobel������һ��΢�����ӣ��ֱ���ˮƽ����ʹ�ֱ����������Ե���
// Ϊ�˼ӿ�����ٶȣ���ʹ��G=sqrt(pow(Gx,2),pow(Gy,2))����ʹ�� G=abs(Gx)+abs(Gy)�����ƴ���
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
    imshow("ˮƽ", dstX);
    imshow("��ֱ", dstY);
    //��ֱ��ˮƽ ��Ȩ��
    addWeighted(dstX, 0.5, dstY, 0.5, 0, dst);
    imshow("�ںϽ��", dst);
    //imwrite("sobel��.jpg", dst);
}

