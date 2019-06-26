#pragma once
#include "open.h"

#define BILINEAR_INTERPOLATION 1 //˫���Բ�ֵ��
#define NEARESTNEIGHBOR_INTERPOLATION 2  //����ڲ�ֵ

enum Filter
{
    BOX_FILTER,//��״�˲���
    GAUSSIAN_FILTER//��˹�˲���
};

// ���α任��ͼ��ƽ������
// ����任�ľ����ʾ��ʽΪԭͼ����Է������
class GeometricTransformation {
public:    
    //����
    void ImgResize(Mat src, Mat &dst, Size dimension,int method=BILINEAR_INTERPOLATION);
    //��ת
    void ImgRotate(Mat src, Mat &dst, double angle);
    //ƽ��
    void ImgWave(Mat src, Mat &dst, Size wave);    
private:
    //��������ֵ��䷨ֻ���������ź�����ʹ�ã���û��ʵ��ͨ�û�

    // ��ֵ��䷨
    void smallResize(Mat src, Mat &dst, Size dimension, float ratioX, float ratioY);
    // ˫���Բ�ֵ��
    void augmentResize(Mat src, Mat &dst, Size dimension, float ratioX, float ratioY);
};

// ͼ��ƽ��
class Smooth {
public:
    //ͼ��ƽ��
    void ImgSmooth(Mat src, Mat &dst, Size dimension, Filter f = BOX_FILTER, double sigma = 1.0);
    //��ֵ�˲�
    void ImgMeanFilter(Mat src, Mat &dst, Size dimension);
private:
    // ��״�˲���
    // dimension���˲����ߴ�
    Mat BoxFilter(Size dimension);
    // ��˹�˲���
    // sigma:�����˹�˲�����׼��
    Mat GaussianFilter(Size dimension, double sigma);
};

// ͼ���񻯣���Ե���
class ImgSharpen {
public:
    //ͼ���񻯣�������˹ģ��
    void Sharpen(Mat src, Mat &dst);
    //�����ڱ�    
    void UnsharpenedMask(Mat src, Mat &dst,float weight);
    //canny����
    //sobel����
    //hough�任
};