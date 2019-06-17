#include "ImgSegment.h"

void Segment::adjustThreshold(int thresholdValue, void* usrdata) {
    Mat srcImg;
    // �ָ����ֱ����ԭͼ������Ͻ��С�
    // Ϊ������ȾԴ���ݣ�ʹ��copyto����������һ���¾������ִ���ٶȽ���
    (*(Mat*)(usrdata)).copyTo(srcImg);
    cout << "adjust:" << thresholdValue << endl;

    // ��ֵ�ָ�
    threshold(srcImg, srcImg, thresholdValue, 255, THRESH_BINARY);

    // ��������
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    findContours(srcImg, contours, hierarchy,
        RETR_LIST, CHAIN_APPROX_SIMPLE);

    // �������
    /*drawContours(srcImg, contours, -1,
        Scalar::all(255), 1, 8, hierarchy);*/
    for (int index = 0; index >= 0; index = hierarchy[index][0]) {
        Scalar color(rand() & 255, rand() & 255, rand() & 255);
        drawContours(srcImg, contours, index,
            color, 1, 8, hierarchy);
    }

    imshow("��ֵ�ָ�", srcImg);
    waitKey(0);
}

void Segment::SegThreshold(Mat input, Mat output, int value, int upValue) {
    output = input.clone();
    cout << output.channels() << endl;

    Mat_<uchar>::iterator it = output.begin<uchar>();
    Mat_<uchar>::iterator itend = output.end<uchar>();
    for (; it != itend; it++) {
        (*it) = (*it) > value ? upValue : 0;
    }
    imshow("��ֵ�и�", output);
    waitKey(0);
}