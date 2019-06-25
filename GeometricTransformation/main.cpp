#include "GeometricTransformation.h"

int main() {
    Mat demo = imread("../PictureSegmentation/lena512color.tiff", 1);
    Size minscale = Size(int(demo.cols * 2 / 3), int(demo.rows * 2 / 3));
    Size augscale = Size(int(demo.cols * 4 / 3), int(demo.rows * 4 / 3));
    GeometricTransformation geo;

    // 缩放图像
    Mat dst;
    //geo.ImgResize(demo, dst, minscale);
    // 放大图像
    Mat augment;
    /*geo.ImgResize(demo, augment, augscale, NEARESTNEIGHBOR_INTERPOLATION);*/

    //图像旋转
    Mat rotateImg;
    float angle = 30;
    //geo.ImgRotate(demo, rotateImg, angle);


    //图像平移
    Mat waveImg;
    //geo.ImgWave(demo, waveImg, Size(10, 40));

    //边缘提取
    
    //图像平滑
    Mat smoothImg,smoothImg1,gau,gau1;
    //均值滤波
    /*geo.ImgSmooth(demo, smoothImg, Size(3, 3), BOX_FILTER);
    blur(demo, smoothImg1, Size(3, 3));
    imshow("调包均值滤波", smoothImg1);
    imshow("差值", smoothImg1 - smoothImg);*/
    //高斯滤波
    //geo.ImgSmooth(demo, gau, Size(3, 3), GAUSSIAN_FILTER,5);
    GaussianBlur(demo, gau1, Size(3, 3), 5);
    imshow("调包高斯滤波", gau1);
    waitKey(0);
    return 0;
}