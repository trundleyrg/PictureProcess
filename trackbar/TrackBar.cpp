#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv;

static void ContrastAndBright(int, void* usrdata);


int g_ContrastValue = 80;
int g_BrightValue = 80;

//窗口创建亮度、对比度两个进度条，通过回调函数响应键鼠操作改变原图像对比度、亮度
int main() {
    Mat SrcImg = imread("lena512color.tiff");
    if (SrcImg.empty()) {
        return -1;
    }
    namedWindow("DisplayWindow");
    cout << SrcImg.channels() << endl;
    createTrackbar("contrast",
        "DisplayWindow",
        &g_ContrastValue,
        256,
        ContrastAndBright, &SrcImg);
    createTrackbar("bright", "DisplayWindow",
        &g_BrightValue, 256, ContrastAndBright, &SrcImg);

    //回调函数初始化
    ContrastAndBright(g_ContrastValue, &SrcImg);
    ContrastAndBright(g_ContrastValue, &SrcImg);

    while (char(waitKey(1))!='q'){
    }
    return 0;
}

static void ContrastAndBright(int, void *usrdata) {
    namedWindow("InitialImg");
    Mat SrcImg = *(Mat*)(usrdata);
    Mat DstImg = Mat::zeros(SrcImg.size(), SrcImg.type());

    //cout << value << endl;
    cout << SrcImg.channels() << endl;
    for (int i = 0; i < SrcImg.rows; i++)
        for (int j = 0; j < SrcImg.cols; j++)
            for (int c = 0; c < 3; c++) {
                //cout << SrcImg.at<Vec3b>(i, j)[c];
                DstImg.at<Vec3b>(i, j)[c] = saturate_cast<uchar>(
                    (g_ContrastValue*0.01)*(SrcImg.at<Vec3b>(i, j)[c])
                    + g_BrightValue);
    }

    //imshow("InitialImg", SrcImg);
    imshow("DisplayWindow", DstImg);
}