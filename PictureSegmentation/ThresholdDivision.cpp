#include "ImgSegment.h"

//static void adjustThreshold(int, void* usrdata);

int main() {
    Mat srcImg = imread("lena512.bmp", 0);
    Segment seg;

    ////��ֵ�ָ�ԭ��ʵ�ֵ���
    //seg.SegThreshold(srcImg, srcImg, 100, 255);

    /*namedWindow("��ֵ�ָ�");
    int thresholdValue = 100;
    createTrackbar("threshold", "��ֵ�ָ�", &thresholdValue,
        200, Segment::adjustThreshold, &srcImg);
    Segment::adjustThreshold(thresholdValue, &srcImg);*/

    //// ���������㷨
    //Point grow(180, 20);//��ʼ������
    //seg.RegionGrowGray(srcImg, grow, 20);

    //��̬ѧ�任
    Mat demoImg = imread("��̬ѧ�任.jpg", 0);
    imshow("��̬ѧ�任", demoImg);
    //waitKey(0);
    // ͼ�������븯ʴ
    Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));// ��ȡ�Զ���ˣ�ȫһ���
    Mat out = Mat::zeros(demoImg.size(), CV_8UC1);//��ʼ�����ͼ��;
    seg.ImgDilate(demoImg, out, element);
    return 0;
}