#pragma once
#include "open.h"

class ColorHist{
public:
    // ��ɫͼ��ת��Ϊ�Ҷ�ͼ��ԭ��ʵ�ֺ������ز���ʵ��
    Mat ConvertColor2Gray(Mat color);
    Mat ConvertColor2Gray1(Mat color);
    // ͨ������ͺϲ�
    void GetChannel(Mat color);
    // ��ɫͼƬ���طֲ�ֱ��ͼ
    void DisplayColorHistogram(Mat color);
    // ��ɫͼֱ��ͼ����
    void EqualizeColorHist(Mat color);
    // ��ɫͼֱ��ͼ����1
    void EqualizeColorHist1(Mat color);
private:
    // ֱ��ͼ���⸨���������������ر任���ձ�
    Mat CreateComparisonTable(Mat single);
};