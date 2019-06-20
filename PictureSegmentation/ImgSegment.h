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
    void ImgErode(Mat src, Mat res, Mat kernel);
    // ������
    void OpeningOperation(Mat src, Mat res, Mat kernel);
    // ������
    void CloseingOperation(Mat src, Mat res, Mat kernel);
    // �������
    void GeodesicDilation(Mat src, Mat mask, Mat kernel, int iterations, Mat res);
    // ��̬ѧ�ݶ�
    void MorphologicalGradient(Mat src, Mat res, Mat kernel);
    // ��ˮ���㷨
    void WaterShed(Mat src, Mat& markers);
    // ���������ɫ����
    Vec3b RandomColor(int value);
};