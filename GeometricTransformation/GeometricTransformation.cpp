#include "GeometricTransformation.h"

// �ߴ�任����������ĳߴ磬�Ա�ԭͼ���С�����Ż���չͼ��
void GeometricTransformation::ImgResize(Mat src, Mat &dst, Size dimension) {
    // �⺯�����
    Mat res;
    resize(src, res, dimension);
    cout << "ԭʼ�ߴ�" << src.cols << " " << src.rows << endl;
    cout << "���ųߴ�" << res.cols << " " << res.rows << endl;
    imshow("��ʵ��", res);

    dst = Mat(dimension, CV_8UC3, Scalar::all(0));
    GeometricTransformation geo;
    //vector<Mat> channels;
    //split(dst, channels);
    //ԭ��ʵ��:����ӳ�䷨
    //���ȼ���ӳ�����������
    //ͨ��(v,w)=T.T()*(x,y)ȷ���¾���Ԫ����ÿ��λ�ö�Ӧ�ľ�ͼ��Ԫ��λ��
    float ratioX = dimension.width / (float)src.rows;
    float ratioY = dimension.height / (float)src.cols;
    //���ֵ��䷨���÷����γ�ͼ��Ч�����ã�������������
    geo.augmentResize(src, dst, dimension, ratioX, ratioY);
    //˫���Բ�ֵ��
    geo.smallResize(src, dst, dimension, ratioX, ratioY);
}

//���ֵ��䣬ͼ������
void GeometricTransformation::smallResize(Mat src, Mat &dst, Size dimension, float ratioX, float ratioY) {
    // ˫int������ֱ�ӵõ����ͽ������Ҫ��ת��һ����Ϊ�����������ܻ�ø�������
    //cout << src.cols << src.rows << endl;
    for (int i = 0; i < dst.rows; i++)
        for (int j = 0; j < dst.cols; j++) {
            int x = (int)(i / ratioX);
            int y = (int)(j / ratioY);
            int temp = src.at<Vec3b>(x, y)[0];
            dst.at<Vec3b>(i, j)[0] = src.at<Vec3b>(x, y)[0];
            dst.at<Vec3b>(i, j)[1] = src.at<Vec3b>(x, y)[1];
            dst.at<Vec3b>(i, j)[2] = src.at<Vec3b>(x, y)[2];
        }
    imshow("����", dst);
}

// ˫���Բ�ֵ,ͼ������
void GeometricTransformation::augmentResize(Mat src, Mat &dst, Size dimension, float ratioX, float ratioY) {
    double scaleX = (double)src.cols / dst.cols;
    double scaleY = (double)src.rows / dst.rows;
    //ʹ��ָ���������ֵ
    int stepDst = dst.step;
    int stepSrc = src.step;
    uchar* DataSrc = src.data;
    uchar* DataDst = dst.data;

    for (int j = 0; j < dst.rows; j++) {
        float fy = (float)((j + 0.5)*scaleY - 0.5);//yС������
        int sy = cvFloor(fy);//y��������
        fy -= sy;

        //Ԥ��Խ�����
        sy = min(sy, src.rows - 3);
        sy = max(1, sy);

        short cbufy[2];
        cbufy[0] = saturate_cast<short>((1.f - fy) * 2048);//��λ
        cbufy[1] = 2048 - cbufy[0];//С��λ

        for (int i = 0; i < dst.cols; ++i) {
            float fx = (float)((i + 0.5)*scaleX - 0.5);//xС������
            int sx = cvFloor(fx);//x��������
            fx -= sx;

            //Խ����ʴ���
            if (sx < 0) {
                fx = 0, sx = 0;
            }
            if (sx >= src.cols - 1) {
                fx = 0, sx = src.cols - 2;
            }

            short cbufx[2];
            cbufx[0] = saturate_cast<short>((1.f - fx) * 2048);
            cbufx[1] = 2048 - cbufx[0];

            for (int c = 0; c < src.channels(); ++c) {
                *(DataDst + j*stepDst + 3 * i + c) = (*(DataSrc + sy*stepSrc + 3 * sx + c)*cbufx[0] * cbufy[0] +
                    *(DataSrc + (sy + 1)*stepSrc + 3 * sx + c)*cbufx[0] * cbufy[1] +
                    *(DataSrc + sy*stepSrc + 3 * (sx + 1) + c)*cbufx[1] * cbufy[0] +
                    *(DataSrc + (sy + 1)*stepSrc + 3 * (sx + 1) + c)*cbufx[1] * cbufy[1]) >> 22;
            }
            imshow("cha", dst);
        }
    }
    //for (int i = 0; i < dst.rows; i++)
    //    for (int j = 0; j < dst.cols; j++)
    //        for (int c = 0; c < dst.channels(); c++) {
    //            int x = i / ratioX;//x�������֣��������ּ��ص���src�����Ͻ�����λ��(opencv����۵�����ϵ)
    //            int y = j / ratioY;//y��������
    //            float fx = i / ratioX - x;//xС������,�ò�����ΪȨ��ֵ����ӳӳ�����Χ�ĸ����ض���ͼ�е����ص�Ӱ�����
    //            float fy = j / ratioY - y;//yС������

    //            float bottom[3], top[3];
    //            if (fx == 0 && fy == 0) {//Ϊ�����Ч�ʣ���ͼ���١���������������жϣ���Ϊ������ֱ������
    //                dst.at<Vec3b>(i, j)[c] = src.at<Vec3b>(x, y)[c];
    //            }
    //            else if (x>src.rows-1 ) {//�߽���,������Χ��ֱ�Ӹ�ֵ

    //            }
    //            else {//˫���Բ�ֵ
    //                if (x + 1 >= src.rows || )
    //                    //������x������ɲ�ֵ
    //                    bottom[c] = fx*src.at<Vec3b>(x, y)[c] + (1 - fx)*src.at<Vec3b>(x + 1, y)[c];
    //                top[c] = fx*src.at<Vec3b>(x, y + 1)[c] + (1 - fx)*src.at<Vec3b>(x + 1, y + 1)[c];
    //                //Ȼ����y������ɲ�ֵ
    //                dst.at<Vec3b>(i, j)[c] = fy*bottom[c] + (1 - fy)*top[c];
    //            }
    //        }
}