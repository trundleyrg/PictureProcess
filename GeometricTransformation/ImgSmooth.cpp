#include "GeometricTransformation.h"

// ͼ��ƽ��
// �ռ�������
void Smooth::ImgSmooth(Mat src, Mat &dst, Size dimension, Filter f, double sigma) {
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
    /*for (int i = 0; i < filter.cols; i++) {
    for (int j = 0; j < filter.rows; j++) {
    cout << (int)filter.at<uchar>(i, j) << " ";
    }
    cout << endl;
    }*/

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

// ��ֵ�˲�
// �ú���ִ���ٶ�����Ч�ʵ�
void Smooth::ImgMeanFilter(Mat src, Mat &dst, Size dimension) {
    dst = Mat(src.size(), CV_8UC3, Scalar::all(0));
    int xMin = floor(dimension.width / 2);
    int yMin = int(floor(dimension.height / 2));
    for (int c = 0; c < 3; c++) {
        for (int j = 0; j < dst.rows; ++j) {
            for (int i = 0; i < dst.cols; ++i) {
                vector<uchar> FilterNumber;//ͼ��������ʱ�洢����,ʹ��vector�ǿ��ǵ��߽����ش�����ȡ��һ���Ÿ�����ֵ
                uchar mid = 0;
                for (int a = -xMin; a <= xMin; a++) {
                    for (int b = -yMin; b <= yMin; b++) {
                        if (i + a < 0 || i + a >= src.cols || j + b < 0 || j + b >= src.rows) {//����Խ����
                            continue;
                        }
                        FilterNumber.push_back(src.at<Vec3b>(i + a, j + b)[c]);
                    }
                }
                //��������λ��
                sort(FilterNumber.begin(), FilterNumber.end());
                mid = FilterNumber.at(int(FilterNumber.size() / 2));
                dst.at<Vec3b>(i, j)[c] = mid;
            }
        }
    }
    imshow("��ֵ�˲�", dst);
}

//�˲���

//��״�˲���
//����ϵ������ȵĿռ��ֵ�˲���,
Mat Smooth::BoxFilter(Size dimension) {
    return Mat::ones(dimension, CV_8U);
}

//��˹ƽ���˲���
//���ݸ�˹�ֲ���������ģ���ϸ�Ԫ��ֵ
//ģ��ߴ�(2k+1)
//��˹�ֲ����� h(i,j)=1/(2*pi*sigma^2)*exp((-pow(i-k-1,2)-pow(j-k-1,2))/2*sigma*sigma)
Mat Smooth::GaussianFilter(Size dimension, double sigma) {
    Mat f = Mat(dimension, CV_8U, Scalar::all(0));
    int kx = (dimension.width - 1) / 2;
    int ky = (dimension.height - 1) / 2;
    double factor = 1 / (2 * M_PI*sigma*sigma);
    for (int i = 0; i < dimension.width; i++) {
        int x = pow((i - kx - 1) *(i - kx - 1), 2);
        for (int j = 0; j < dimension.height; j++) {
            int y = pow((j - ky - 1)*(j - ky - 1), 2);
            f.at<uchar>(i, j) = saturate_cast<uchar>(factor*exp((-x - y) / (2 * sigma*sigma)) + 0.5);
        }
    }
    return f;
}