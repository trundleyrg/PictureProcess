#include "ImgSegment.h"

void Segment::AdjustThreshold(int thresholdValue, void* usrdata) {
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

// ���������㷨
// �Ҷ�ʵ�֣���֪һ����ʼ�����㣬����һ��ͬ�ߴ�ȫ��ͼ��
// ���ݳ�ʼ�������ж���Χ�����������Ƿ����Ԥ����ֵ��δ�����򽫸õ������������ջ��
// ջΪ��ʱ�˳�
Mat Segment::RegionGrowGray(Mat src, Point pt, int th) {
    Mat maskImg = Mat::zeros(src.size(), CV_8UC1);//
    vector<Point> GrowPtStack;//������ջ
    int neighborhood[8][2] = { {-1,-1},{-1,0},{-1,1},{0,-1},
        {0,1},{1,-1},{1,0},{1,1} };//������
    int curValue = 0;
    int maskValue = 0;
    int srcValue = 0;//�������Ҷ�

    GrowPtStack.push_back(pt);
    srcValue = src.at<uchar>(pt.x, pt.y);
    cout << pt.x << "   " << pt.y << endl;
    maskImg.at<uchar>(pt.x, pt.y) = 255;

    while (!GrowPtStack.empty()) {
        Point pt = GrowPtStack.back();
        GrowPtStack.pop_back();

        for (int i = 0; i < 8; i++) {
            Point ptGrowing = Point(pt.x + neighborhood[i][0], pt.y + neighborhood[i][1]);            

            //��Ե����
            if (ptGrowing.x<0 || ptGrowing.y<0 || ptGrowing.x>=src.cols || ptGrowing.y>=src.rows) {
                continue;
            }

            maskValue = maskImg.at<uchar>(ptGrowing.x, ptGrowing.y);
            //����Ԫ����ջ
            if (maskValue == 0) {
                curValue = src.at<uchar>(ptGrowing.x, ptGrowing.y);
                if (abs(srcValue - curValue) < th) {
                    GrowPtStack.push_back(ptGrowing);
                    maskImg.at<uchar>(ptGrowing.x, ptGrowing.y) = 255;
                }
            }
        }
    }
    imshow("maskImg", maskImg);
    waitKey(0);
    return maskImg.clone();
}