#include "GeometricTransformation.h"

int main() {
    Mat demo = imread("../PictureSegmentation/lena512color.tiff", 1);
    Size minscale = Size(int(demo.cols * 2 / 3), int(demo.rows * 2 / 3));
    Size augscale = Size(int(demo.cols * 4 / 3), int(demo.rows * 4 / 3));
    GeometricTransformation geo;

    // ����ͼ��
    Mat dst;
    //geo.ImgResize(demo, dst, minscale);
    // �Ŵ�ͼ��
    Mat augment;
    /*geo.ImgResize(demo, augment, augscale, NEARESTNEIGHBOR_INTERPOLATION);*/

    //ͼ����ת
    Mat rotateImg;
    float angle = 30;
    //geo.ImgRotate(demo, rotateImg, angle);


    //ͼ��ƽ��
    Mat waveImg;
    //geo.ImgWave(demo, waveImg, Size(10, 40));

    //��Ե��ȡ
    
    //ͼ��ƽ��
    waitKey(0);
    return 0;
}