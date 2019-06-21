#include "GeometricTransformation.h"

int main() {
    Mat demo = imread("../PictureSegmentation/lena512color.tiff", 1);
    Size minscale = Size(int(demo.cols * 2 / 3), int(demo.rows * 2 / 3));
    Size augscale = Size(int(demo.cols * 4/3), int(demo.rows * 4/3));
    GeometricTransformation geo;
    
    // Ëõ·ÅÍ¼Ïñ
    Mat dst;
    //geo.ImgResize(demo, dst, minscale);
    // ·Å´óÍ¼Ïñ
    Mat augment;
    geo.ImgResize(demo, augment, augscale);

    waitKey(0);
}