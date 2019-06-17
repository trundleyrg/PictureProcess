#include "ImgSegment.h"

//static void adjustThreshold(int, void* usrdata);

int main() {
    Mat srcImg = imread("lena512.bmp", 0);
    Segment seg;
    seg.SegThreshold(srcImg, srcImg, 100, 255);
    /*namedWindow("阈值分割");
    int thresholdValue = 100;
    createTrackbar("threshold", "阈值分割", &thresholdValue,
        200, Segment::adjustThreshold, &srcImg);
    Segment::adjustThreshold(thresholdValue, &srcImg);*/

    while (true) {
        //seg.thresholdSeg(srcImg);
    }
    return 0;
}