#include "ImgSegment.h"

//static void adjustThreshold(int, void* usrdata);

int main() {
    Mat srcImg = imread("lena512.bmp", 0);
    Segment seg;

    ////阈值分割原生实现调用
    //seg.SegThreshold(srcImg, srcImg, 100, 255);

    /*namedWindow("阈值分割");
    int thresholdValue = 100;
    createTrackbar("threshold", "阈值分割", &thresholdValue,
        200, Segment::adjustThreshold, &srcImg);
    Segment::adjustThreshold(thresholdValue, &srcImg);*/

    Point grow(180, 20);
    seg.RegionGrowGray(srcImg, grow, 20);
    return 0;
}