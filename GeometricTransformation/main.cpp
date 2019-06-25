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
    Mat smoothImg,smoothImg1,gau,gau1;
    //��ֵ�˲�
    /*geo.ImgSmooth(demo, smoothImg, Size(3, 3), BOX_FILTER);
    blur(demo, smoothImg1, Size(3, 3));
    imshow("������ֵ�˲�", smoothImg1);
    imshow("��ֵ", smoothImg1 - smoothImg);*/
    //��˹�˲�
    //geo.ImgSmooth(demo, gau, Size(3, 3), GAUSSIAN_FILTER,5);
    GaussianBlur(demo, gau1, Size(3, 3), 5);
    imshow("������˹�˲�", gau1);
    waitKey(0);
    return 0;
}