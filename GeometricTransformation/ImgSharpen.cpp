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