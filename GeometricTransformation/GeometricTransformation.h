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
#define SHARPEN 101//��
#define SMOOTH  102//ƽ��
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
    // flag���ж���ƽ��������
    Mat BoxFilter(Size dimension,int flag);
    // ��˹�˲���
    // sigma:�����˹�˲�����׼��
    Mat GaussianFilter(Size dimension, int flag, double sigma);
};