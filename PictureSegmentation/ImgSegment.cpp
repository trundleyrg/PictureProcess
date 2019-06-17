#include "ImgSegment.h"

void Segment::adjustThreshold(int thresholdValue, void* usrdata) {
    Mat srcImg;
    // 分割操作直接在原图像矩阵上进行。
    // 为避免污染源数据，使用copyto函数，复制一个新矩阵对象。执行速度降低
    (*(Mat*)(usrdata)).copyTo(srcImg);
    cout << "adjust:" << thresholdValue << endl;

    // 阈值分割
    threshold(srcImg, srcImg, thresholdValue, 255, THRESH_BINARY);

    // 轮廓查找
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    findContours(srcImg, contours, hierarchy,
        RETR_LIST, CHAIN_APPROX_SIMPLE);

    // 轮廓填充
    /*drawContours(srcImg, contours, -1,
        Scalar::all(255), 1, 8, hierarchy);*/
    for (int index = 0; index >= 0; index = hierarchy[index][0]) {
        Scalar color(rand() & 255, rand() & 255, rand() & 255);
        drawContours(srcImg, contours, index,
            color, 1, 8, hierarchy);
    }

    imshow("阈值分割", srcImg);
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
    imshow("阈值切割", output);
    waitKey(0);
}