#include "GeometricTransformation.h"

int main() {
    Mat demo = imread("../PictureSegmentation/lena512color.tiff", 1);
    Size minscale = Size(int(demo.cols * 2 / 3), int(demo.rows * 2 / 3));
    Size augscale = Size(int(demo.cols * 4 / 3), int(demo.rows * 4 / 3));
    GeometricTransformation geo;

    // Ëõ·ÅÍ¼Ïñ
    Mat dst;
    //geo.ImgResize(demo, dst, minscale);
    // ·Å´óÍ¼Ïñ
    Mat augment;
    /*geo.ImgResize(demo, augment, augscale, NEARESTNEIGHBOR_INTERPOLATION);*/

    //Í¼ÏñÐý×ª
    Mat rotateImg;
    float angle = 30;
    //geo.ImgRotate(demo, rotateImg, angle);


    //Í¼ÏñÆ½ÒÆ
    Mat waveImg;
    //geo.ImgWave(demo, waveImg, Size(10, 40));

    //±ßÔµÌáÈ¡
    
    //Í¼ÏñÆ½»¬
    waitKey(0);
    return 0;
}