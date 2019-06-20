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
    //imshow("形态学变换", demoImg);
    //waitKey(0);
    // 图像膨胀与腐蚀
    Mat element = getStructuringElement(MORPH_RECT, Size(5, 5));// 获取自定义核，全一填充
    Mat out = Mat::zeros(demoImg.size(), CV_8UC1);//初始化结果图像;
    //seg.ImgDilate(demoImg, out, element);//膨胀
    //seg.ImgErode(demoImg, out, element);//腐蚀
    //seg.OpeningOperation(demoImg, out, element);//开操作
    //seg.CloseingOperation(demoImg, out, element);//闭操作

    //// 膨胀重建测试
    //// 对原图执行腐蚀操作，生成标记图像
    //Mat label(demoImg.size(), CV_8UC1);
    //seg.ImgErode(demoImg, label, element);
    //imshow("腐蚀", label);
    //seg.GeodesicDilation(label, demoImg, element, -1, out);

    // 分水岭算法
    Mat color = imread("形态学变换.jpg", 1);
    Mat marks(color.size(), CV_32S);   //Opencv分水岭第二个矩阵参数 
    seg.WaterShed(color, marks);
    //imshow("1", out);
    //waitKey(0);
    return 0;
}