#include "ImgSegment.h"

void Segment::AdjustThreshold(int thresholdValue, void* usrdata) {
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

// 区域生长算法
// 灰度实现：已知一个初始生长点，生成一幅同尺寸全黑图像，
// 根据初始生长点判断周围八邻域像素是否大于预定阈值，未超过则将该点加入生产长点栈中
// 栈为空时退出
Mat Segment::RegionGrowGray(Mat src, Point pt, int th) {
    Mat maskImg = Mat::zeros(src.size(), CV_8UC1);//
    vector<Point> GrowPtStack;//生长点栈
    int neighborhood[8][2] = { {-1,-1},{-1,0},{-1,1},{0,-1},
        {0,1},{1,-1},{1,0},{1,1} };//八邻域
    int curValue = 0;
    int maskValue = 0;
    int srcValue = 0;//生长起点灰度

    GrowPtStack.push_back(pt);
    srcValue = src.at<uchar>(pt.x, pt.y);
    cout << pt.x << "   " << pt.y << endl;
    maskImg.at<uchar>(pt.x, pt.y) = 255;

    while (!GrowPtStack.empty()) {
        Point pt = GrowPtStack.back();
        GrowPtStack.pop_back();

        for (int i = 0; i < 8; i++) {
            Point ptGrowing = Point(pt.x + neighborhood[i][0], pt.y + neighborhood[i][1]);            

            //边缘点检查
            if (ptGrowing.x<0 || ptGrowing.y<0 || ptGrowing.x>=src.cols || ptGrowing.y>=src.rows) {
                continue;
            }

            maskValue = maskImg.at<uchar>(ptGrowing.x, ptGrowing.y);
            //相似元素入栈
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