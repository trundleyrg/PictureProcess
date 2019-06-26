#include "GeometricTransformation.h"

int main() {
    Mat demo = imread("../PictureSegmentation/lena512color.tiff", 1);
    
    //����任
    GeometricTransformation geo;
    // ����ͼ��
    Mat dst;
    Size minscale = Size(int(demo.cols * 2 / 3), int(demo.rows * 2 / 3));
    Size augscale = Size(int(demo.cols * 4 / 3), int(demo.rows * 4 / 3));
    /*geo.ImgResize(demo, dst, minscale);*/
    // �Ŵ�ͼ��
    /*Mat augment;
    geo.ImgResize(demo, augment, augscale, NEARESTNEIGHBOR_INTERPOLATION);*/
    //ͼ����ת
    /*Mat rotateImg;
    float angle = 30;
    geo.ImgRotate(demo, rotateImg, angle);*/
    //ͼ��ƽ��
    /*Mat waveImg;
    geo.ImgWave(demo, waveImg, Size(10, 40));*/

    // ͼ��ƽ��
    Mat smoothImg, smoothImg1, gau, gau1,meanImg,meanImg1;
    Smooth smo;
    //��ֵ�˲�
    /*smo.ImgSmooth(demo, smoothImg, Size(3, 3), BOX_FILTER);
    blur(demo, smoothImg1, Size(3, 3));
    imshow("������ֵ�˲�", smoothImg1);
    imshow("��ֵ", smoothImg1 - smoothImg);*/
    //��˹�˲�
    /*smo.ImgSmooth(demo, gau, Size(3, 3), GAUSSIAN_FILTER, 0.8);
    GaussianBlur(demo, gau1, Size(3, 3), 5);
    imshow("������˹�˲�", gau1);
    imshow("��˹��ֵ", gau1 - gau);*/
    //��ֵ�˲�
    /*smo.ImgMeanFilter(demo, meanImg, Size(3, 3));
    medianBlur(demo, meanImg1, 3);
    imshow("������ֵ�˲�", meanImg1);
    imshow("��ֵ��ֵ", meanImg1 - meanImg);*/

    //ͼ����
    Mat gray = imread("../PictureSegmentation/lena512.bmp", CV_LOAD_IMAGE_GRAYSCALE);
    Mat lapImg,highEmphasis,sobelImg;
    ImgSharpen sa;
    //������˹ģ��
    //sa.Sharpen(demo, lapImg);
    //�����ڱ�
    //sa.UnsharpenedMask(demo, highEmphasis, 1);
    //sobel����
    sa.SobelSharp(gray, sobelImg);
    waitKey(0);
    return 0;
}