#include "GeometricTransformation.h"

int main() {
    Mat demo = imread("../PictureSegmentation/lena512color.tiff", 1);
    
    //仿射变换
    GeometricTransformation geo;
    // 缩放图像
    Mat dst;
    Size minscale = Size(int(demo.cols * 2 / 3), int(demo.rows * 2 / 3));
    Size augscale = Size(int(demo.cols * 4 / 3), int(demo.rows * 4 / 3));
    /*geo.ImgResize(demo, dst, minscale);*/
    // 放大图像
    /*Mat augment;
    geo.ImgResize(demo, augment, augscale, NEARESTNEIGHBOR_INTERPOLATION);*/
    //图像旋转
    /*Mat rotateImg;
    float angle = 30;
    geo.ImgRotate(demo, rotateImg, angle);*/
    //图像平移
    /*Mat waveImg;
    geo.ImgWave(demo, waveImg, Size(10, 40));*/

    // 图像平滑
    Mat smoothImg, smoothImg1, gau, gau1,meanImg,meanImg1;
    Smooth smo;
    //均值滤波
    /*smo.ImgSmooth(demo, smoothImg, Size(3, 3), BOX_FILTER);
    blur(demo, smoothImg1, Size(3, 3));
    imshow("调包均值滤波", smoothImg1);
    imshow("差值", smoothImg1 - smoothImg);*/
    //高斯滤波
    /*smo.ImgSmooth(demo, gau, Size(3, 3), GAUSSIAN_FILTER, 0.8);
    GaussianBlur(demo, gau1, Size(3, 3), 5);
    imshow("调包高斯滤波", gau1);
    imshow("高斯差值", gau1 - gau);*/
    //中值滤波
    /*smo.ImgMeanFilter(demo, meanImg, Size(3, 3));
    medianBlur(demo, meanImg1, 3);
    imshow("调包中值滤波", meanImg1);
    imshow("中值差值", meanImg1 - meanImg);*/

    //图像锐化
    Mat gray = imread("../PictureSegmentation/lena512.bmp", CV_LOAD_IMAGE_GRAYSCALE);
    Mat lapImg,highEmphasis,sobelImg;
    ImgSharpen sa;
    //拉普拉斯模板
    //sa.Sharpen(demo, lapImg);
    //非锐化掩蔽
    //sa.UnsharpenedMask(demo, highEmphasis, 1);
    //sobel算子
    sa.SobelSharp(gray, sobelImg);
    waitKey(0);
    return 0;
}