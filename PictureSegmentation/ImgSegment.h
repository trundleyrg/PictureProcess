#pragma once
#include "open.h"

// ͼ��ָ���
class Segment {
public:
    // ��ֵ�ָ�,����������չʾ
    static void adjustThreshold(int, void*);
    void SegThreshold(Mat input, Mat output, int value, int upValue);
};