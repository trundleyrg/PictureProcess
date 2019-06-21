#pragma once
#include "open.h"

// ���α任��
class GeometricTransformation {

    // ����任�ľ����ʾ��ʽΪԭͼ����Է������
public:
    //����
    void ImgResize(Mat src, Mat &dst, Size dimension);
    //��ת
    //ƽ��
    //��Ե��ȡ
    //ͼ��ƽ������
private:
    // ʹ�þ�ֵ���������ͼ��
    void smallResize(Mat src, Mat &dst, Size dimension, float ratioX, float ratioY);
    // ʹ��˫���Բ�ֵ��÷Ŵ�ͼ��
    void augmentResize(Mat src, Mat &dst, Size dimension, float ratioX, float ratioY);
};