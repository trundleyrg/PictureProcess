#pragma once
#include "open.h"

// ͼ��ָ���
class Segment {
public:
    // ��ֵ�ָ�,����������չʾ
    static void AdjustThreshold(int, void*);
    void SegThreshold(Mat input, Mat output, int value, int upValue);
    // ��������
    Mat RegionGrowGray(Mat src, Point2i pt, int th);
    // ����
    void ImgDilate(Mat src, Mat res, Mat kernel);
    // ��ʴ

    // ������

    // ������

};