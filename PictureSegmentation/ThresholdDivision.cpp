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

    Point grow(180, 20);
    seg.RegionGrowGray(srcImg, grow, 20);
    return 0;
}