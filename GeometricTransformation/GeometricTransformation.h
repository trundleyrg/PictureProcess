#pragma once
#include "open.h"

#define BILINEAR_INTERPOLATION 1 //˫���Բ�ֵ��
#define NEARESTNEIGHBOR_INTERPOLATION 2  //����ڲ�ֵ

// ���α任��
class GeometricTransformation {

    // ����任�ľ����ʾ��ʽΪԭͼ����Է������
public:
    //����
    void ImgResize(Mat src, Mat &dst, Size dimension,int method=BILINEAR_INTERPOLATION);
    //��ת
    void ImgRotate(Mat src, Mat &dst, double angle);
    //ƽ��
    void ImgWave(Mat src, Mat &dst, Size wave);
    //��Ե��ȡ
    //ͼ��ƽ������
private:
    // ��ֵ��䷨
    void smallResize(Mat src, Mat &dst, Size dimension, float ratioX, float ratioY);
    // ˫���Բ�ֵ��
    void augmentResize(Mat src, Mat &dst, Size dimension, float ratioX, float ratioY);
};