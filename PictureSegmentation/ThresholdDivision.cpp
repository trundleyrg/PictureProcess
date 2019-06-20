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

    //// ���������㷨
    //Point grow(180, 20);//��ʼ������
    //seg.RegionGrowGray(srcImg, grow, 20);

    //��̬ѧ�任
    Mat demoImg = imread("��̬ѧ�任.jpg", 0);
    //imshow("��̬ѧ�任", demoImg);
    //waitKey(0);
    // ͼ�������븯ʴ
    Mat element = getStructuringElement(MORPH_RECT, Size(5, 5));// ��ȡ�Զ���ˣ�ȫһ���
    Mat out = Mat::zeros(demoImg.size(), CV_8UC1);//��ʼ�����ͼ��;
    //seg.ImgDilate(demoImg, out, element);//����
    //seg.ImgErode(demoImg, out, element);//��ʴ
    //seg.OpeningOperation(demoImg, out, element);//������
    //seg.CloseingOperation(demoImg, out, element);//�ղ���

    //// �����ؽ�����
    //// ��ԭͼִ�и�ʴ���������ɱ��ͼ��
    //Mat label(demoImg.size(), CV_8UC1);
    //seg.ImgErode(demoImg, label, element);
    //imshow("��ʴ", label);
    //seg.GeodesicDilation(label, demoImg, element, -1, out);

    // ��ˮ���㷨
    Mat color = imread("��̬ѧ�任.jpg", 1);
    Mat marks(color.size(), CV_32S);   //Opencv��ˮ��ڶ���������� 
    seg.WaterShed(color, marks);
    //imshow("1", out);
    //waitKey(0);
    return 0;
}