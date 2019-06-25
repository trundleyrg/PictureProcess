#include "GeometricTransformation.h"

// �ߴ�任����������ĳߴ磬�Ա�ԭͼ���С�����Ż���չͼ��
void GeometricTransformation::ImgResize(Mat src, Mat &dst, Size dimension, int method) {
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
    if (method == BILINEAR_INTERPOLATION) {
        //˫���Բ�ֵ��
        geo.augmentResize(src, dst, dimension, ratioX, ratioY);
    }
    else if (method == NEARESTNEIGHBOR_INTERPOLATION) {
        //���ֵ��䷨���÷����γ�ͼ��Ч�����ã�������������
        geo.smallResize(src, dst, dimension, ratioX, ratioY);
    }
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

// ˫���Բ�ֵ,ͼ������,������ٰ汾���������ת��Ϊ���ͼ��㣬ָ��������at���ʣ�
// �������������ṩ������Ϣ��С�������ṩԭͼ����4���ڽ�λ�ö���ͼ������ص�Ӱ��Ȩ��
// �������ת��Ϊ���ͼ���ļ��ٷ�����������Ȩ�ص�С�����ֳ���2048���ڼ������֮�����ƻ�ԭֵ�����ɽ�Ȩ�ز��ֵļ�����С��ת��Ϊ������
// 
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
        sy = min(sy, src.rows - 2);
        sy = max(1, sy);
        //С��תΪ����
        short cbufy[2];
        cbufy[0] = saturate_cast<short>((1.f - fy) * 2048);//�Ҳ�ԭͼԪ�ؾ���ͼԪ�صĵ�λ�����ڱ�ֵ
        cbufy[1] = 2048 - cbufy[0];//���ԭͼԪ�ؾ���ͼԪ�صĵ�λ�����С������

        for (int i = 0; i < dst.cols; ++i) {

            float fx = (float)((i + 0.5)*scaleX - 0.5);//xС������
            int sx = cvFloor(fx);//x��������
            fx -= sx;

            //Խ����ʴ���
            if (sx < 0) {
                fx = 0, sx = 0;
            }
            if (sx >= src.cols - 1) {//���ǵ�˫���Բ�ֵ�����x+1�����ݣ���ˣ�x��ֵ���ܳ���cols-2
                fx = 0, sx = src.cols - 2;
            }
            //�������תΪ���ͼ���
            short cbufx[2];
            cbufx[0] = saturate_cast<short>((1.f - fx) * 2048);
            cbufx[1] = 2048 - cbufx[0];

            // ˫���Բ�ֵ��ʽ
            // δ���ٵļ��㹫ʽ
            // dst(i,j)[c]=[src(sx,sy)[c]*fx+src(sx+1,sy)[c]*(1-fx)]*fy+[src(sx,sy+1)*fx+src(sx+1,sy+1)*(1-fx)]*(1-fy)
            // ʹ�ü��������滻������fx,fy
            // dst(i,j)[c]=[src(sx,sy)[c]*cbufx[0]+src(sx+1,sy)[c]*cbufx[1]]*cbufy[0]+[src(sx,sy+1)*cbufx[0]+src(sx+1,sy+1)*cbufx[1]]*cbufy[1]
            for (int c = 0; c < src.channels(); ++c) {
                *(DataDst + j*stepDst + 3 * i + c) = (*(DataSrc + sy*stepSrc + 3 * sx + c)*cbufx[0] * cbufy[0] +
                    *(DataSrc + (sy + 1)*stepSrc + 3 * sx + c)*cbufx[0] * cbufy[1] +
                    *(DataSrc + sy*stepSrc + 3 * (sx + 1) + c)*cbufx[1] * cbufy[0] +
                    *(DataSrc + (sy + 1)*stepSrc + 3 * (sx + 1) + c)*cbufx[1] * cbufy[1]) >> 22;
            }
            imshow("cha", dst);
        }
    }
}

// ͼƬ��ת
// 1.������ԭ����ͼ������ϽǱ任����ת����
// 2.����ת����Ϊԭ�㣬ͼ����ת�Ƕ�angle
// 3.��ת����������ԭ��任����ת��ͼ������Ͻ�
void GeometricTransformation::ImgRotate(Mat src, Mat &dst, double angle) {
    //�Ƕ�ת��Ϊ����
    double radian = angle*M_PI / 180;
    double co = fabs(cos(radian));
    double si = fabs(sin(radian));

    //��ͼ�񳤿����ã��Լ���ͼ������ʼ��
    int dstWidth = (int)src.cols*co + src.rows*si;
    int dstHeight = (int)src.cols*si + src.rows*co;
    dst = Mat(Size(dstWidth, dstHeight), CV_8UC3, Scalar::all(0));

    //��ͼ��������Ϊ��ת���ģ����ݽǶȱ任������ɶ���ͼ��������䡣
    //Point srcCenter(int(src.rows / 2), int(src.cols / 2));
    //��ת����
    int xOff = dstWidth / 2;
    int yOff = dstHeight / 2;
    //��ͼ���ط�Χ
    int yMin = 0;
    int yMax = dstHeight;
    int xMin = 0;
    int xMax = dstWidth;

    double xSrc = 0, ySrc = 0;//ԭͼ����
    for (int y = yMin; y < yMax; y++) {
        for (int x = xMin; x < xMax; x++) {
            //��ԭͼ�����Ͻ�����Ϊԭ�㣬�����ͼ��x��y�����Ӧ��ԭͼ��x,y������㡣
            //��ʱ��õĸ�����Ϊdouble����ֵ��֮�����ʹ��˫���Բ�ֵ���������ø���ֵ��Χ�ĸ��������ֵ��Ȩ��ֵ����ͼ��Ӧ���ꡣ
            ySrc = si*(x - xOff) + co*(y - yOff) + int(src.rows / 2);
            xSrc = co*(x - xOff) - si*(y - yOff) + int(src.cols / 2);
            //�߽���
            if (ySrc >= 0.&&ySrc < src.rows - 0.5&&xSrc >= 0 && xSrc < src.cols - 0.5) {//�����ԭͼ��Χ��
                int xSmall = floor(xSrc);
                int xLarge = ceil(xSrc);
                int ySmall = floor(ySrc);
                int yLarge = ceil(ySrc);
                for (int channel = 0; channel < 3; channel++) {//˫���Բ�ֵ
                    float a1 = xSmall >= 0 && ySmall >= 0 ?
                        src.at<Vec3b>(xSmall, ySmall)[channel] : 0;
                    float a2 = xLarge < src.cols && ySmall >= 0 ?
                        src.at<Vec3b>(xLarge, ySmall)[channel] : 0;
                    float a3 = xSmall >= 0 && yLarge < src.rows ?
                        src.at<Vec3b>(xSmall, yLarge)[channel] : 0;
                    float a4 = xLarge < src.cols && yLarge < src.rows ?
                        src.at<Vec3b>(xLarge, yLarge)[channel] : 0;
                    double ux = xSrc - xSmall;
                    double uy = ySrc - ySmall;
                    dst.at<Vec3b>(x, y)[channel] = (1 - ux)*(1 - uy)*a1 +
                        (1 - ux)*uy*a3 + (1 - uy)*ux*a2 + ux*uy*a4;
                }
            }
        }
    }
    imshow("��ת", dst);
}

// ͼ��ƽ�ƣ�ֱ��ʹ�÷���ӳ��+������󼴿���ɼ��㡣
void GeometricTransformation::ImgWave(Mat src, Mat &dst, Size wave) {
    dst = Mat(src.size(), CV_8UC3, Scalar::all(0));
    for (int i = 0; i < dst.rows; i++) {
        for (int j = 0; j < dst.cols; j++) {
            for (int channel = 0; channel < 3; channel++) {
                if (i - wave.width < 0 || j - wave.height < 0) {//����Խ�����
                    continue;
                }
                dst.at < Vec3b >(i, j)[channel] = src.at <Vec3b>(i - wave.width, j - wave.height)[channel];
            }
        }
    }
    imshow("ͼ����ת", dst);
}

// ͼ��ƽ��
// �ռ�������
void GeometricTransformation::ImgSmooth(Mat src, Mat &dst, Size dimension, Filter f, double sigma) {
    dst = Mat(src.size(), CV_8UC3, Scalar::all(0));

    if (dimension.height % 2 == 0 || dimension.width % 2 == 0) {//ģ��ߴ���
        cout << "��ʹ��������С���˲����ߴ�" << endl;
        return;
    }
    //�˲���ģ���ʼ��
    Mat filter = Mat(dimension, CV_8U, Scalar::all(0));
    switch (f) {
    case BOX_FILTER:
        filter = BoxFilter(dimension);
        break;
    case GAUSSIAN_FILTER:
        filter = GaussianFilter(dimension, sigma);
        break;
    }
    //���ԣ��˲���ֵ�۲�
    for (int i = 0; i < filter.cols; i++) {
        for (int j = 0; j < filter.rows; j++) {
            cout << (int)filter.at<uchar>(i, j) << " ";
        }
        cout << endl;
    }

    //ԭͼ�񻬶���Χ����
    int xMin = floor(dimension.width / 2);
    int yMin = int(floor(dimension.height / 2));
    int sumValue = 0;
    //�˲���ģ��ϵ������
    for (int a = -xMin; a <= xMin; ++a) {
        for (int b = -yMin; b <= yMin; ++b) {
            sumValue += (int)filter.at<uchar>(a + xMin, b + yMin);
        }
    }
    if (sumValue == 0) {
        cout << "��ȷ���˲���ģ���Ƿ����" << endl;
        return;
    }

    //�������ԭͼ���ϻ������ƽ������
    for (int c = 0; c < 3; c++) {
        for (int j = 0; j < src.rows; ++j) {
            for (int i = 0; i < src.cols; ++i) {
                int meanValue = 0;
                for (int a = -xMin; a <= xMin; ++a) {
                    for (int b = -yMin; b <= yMin; ++b) {
                        if (i + a < 0 || i + a >= src.cols || j + b < 0 || j + b >= src.rows) {//����Խ����
                            continue;
                        }
                        meanValue += filter.at<uchar>(a + xMin, b + yMin)*src.at<Vec3b>(i + a, j + b)[c];
                    }
                }
                dst.at<Vec3b>(i, j)[c] = saturate_cast<uchar>(meanValue / sumValue);
            }
        }
    }
    imshow("ͼ��ƽ��", dst);
}

// ͼ����
// �ռ�������
void GeometricTransformation::ImgSharpen(Mat src, Mat &dst, Size dimension, Filter f /* = BOX_FILTER */, double sigma/* =1.0 */) {
    //�ȴ�����
}

//�˲���

//��״�˲���
//����ϵ������ȵĿռ��ֵ�˲���,
Mat GeometricTransformation::BoxFilter(Size dimension) {
    return Mat::ones(dimension, CV_8U);
}

//��˹ƽ���˲���
//���ݸ�˹�ֲ���������ģ���ϸ�Ԫ��ֵ
//ģ��ߴ�(2k+1)
//��˹�ֲ����� h(i,j)=1/(2*pi*sigma^2)*exp((-pow(i-k-1,2)-pow(j-k-1,2))/2*sigma*sigma)
Mat GeometricTransformation::GaussianFilter(Size dimension, double sigma) {
    Mat f = Mat(dimension, CV_8U, Scalar::all(0));
    int kx = (dimension.width - 1) / 2;
    int ky = (dimension.height - 1) / 2;
    double factor = 1 / (2 * M_PI*sigma*sigma);
    for (int i = 0; i < dimension.width; i++) {
        int x = pow((i - kx - 1) *(i - kx - 1),2);
        for (int j = 0; j < dimension.height; j++) {
            int y = pow((j - ky - 1)*(j - ky - 1),2);
            f.at<uchar>(i, j) = saturate_cast<uchar>(factor*exp((-x - y) / (2 * sigma*sigma))+0.5);
        }
    }
    return f;
}