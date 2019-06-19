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

    //// 区域生长算法
    //Point grow(180, 20);//起始点坐标
    //seg.RegionGrowGray(srcImg, grow, 20);

    //形态学变换
    Mat demoImg = imread("形态学变换.jpg", 0);
    imshow("形态学变换", demoImg);
    //waitKey(0);
    // 图像膨胀与腐蚀
    Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));// 获取自定义核，全一填充
    Mat out = Mat::zeros(demoImg.size(), CV_8UC1);//初始化结果图像;
    seg.ImgDilate(demoImg, out, element);
    return 0;
}