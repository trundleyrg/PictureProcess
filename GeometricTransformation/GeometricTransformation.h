#pragma once
#include "open.h"

#define BILINEAR_INTERPOLATION 1 //˫���Բ�ֵ��
#define NEARESTNEIGHBOR_INTERPOLATION 2  //����ڲ�ֵ
enum Filter
{
    BOX_FILTER,//��״�˲���
    //MEDIAN_FILTER,//��ֵ�˲���,�Ƚ����⣬����ʵ��
    GAUSSIAN_FILTER//��˹�˲���
};
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
    //ͼ��ƽ����ʹ�ú�״�˲���
    void ImgSmooth(Mat src, Mat &dst, Size dimension,Filter f=BOX_FILTER,double sigma=1.0);
    //ͼ����
    void ImgSharpen(Mat src, Mat &dst, Size dimension, Filter f = BOX_FILTER,double sigma=1.0);
private:
    //��������ֵ��䷨ֻ���������ź�����ʹ�ã���û��ʵ��ͨ�û�
    // ��ֵ��䷨
    void smallResize(Mat src, Mat &dst, Size dimension, float ratioX, float ratioY);
    // ˫���Բ�ֵ��
    void augmentResize(Mat src, Mat &dst, Size dimension, float ratioX, float ratioY);
    
    // �˲�����ʼ��

    // ��״�˲���
    // dimension���˲����ߴ�
    Mat BoxFilter(Size dimension);
    // ��˹�˲���
    // sigma:�����˹�˲�����׼��
    Mat GaussianFilter(Size dimension, double sigma);
};