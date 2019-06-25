#include "GeometricTransformation.h"

// 尺寸变换，根据输入的尺寸，对比原图像大小，缩放或扩展图像
void GeometricTransformation::ImgResize(Mat src, Mat &dst, Size dimension, int method) {
    // 库函数结果
    Mat res;
    resize(src, res, dimension);
    cout << "原始尺寸" << src.cols << " " << src.rows << endl;
    cout << "缩放尺寸" << res.cols << " " << res.rows << endl;
    imshow("库实现", res);

    dst = Mat(dimension, CV_8UC3, Scalar::all(0));
    GeometricTransformation geo;
    //vector<Mat> channels;
    //split(dst, channels);
    //原生实现:反向映射法
    //首先计算映射矩阵的逆矩阵
    //通过(v,w)=T.T()*(x,y)确定新矩阵元素中每个位置对应的旧图像元素位置
    float ratioX = dimension.width / (float)src.rows;
    float ratioY = dimension.height / (float)src.cols;
    if (method == BILINEAR_INTERPOLATION) {
        //双线性插值法
        geo.augmentResize(src, dst, dimension, ratioX, ratioY);
    }
    else if (method == NEARESTNEIGHBOR_INTERPOLATION) {
        //最近值填充法，该方法形成图像效果不好，有马赛克问题
        geo.smallResize(src, dst, dimension, ratioX, ratioY);
    }
}

//最近值填充，图像缩放
void GeometricTransformation::smallResize(Mat src, Mat &dst, Size dimension, float ratioX, float ratioY) {
    // 双int做除法直接得到整型结果，需要先转换一个作为浮点型数才能获得浮点结果。
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
    imshow("缩放", dst);
}

// 双线性插值,图像缩放,计算加速版本（浮点计算转化为整型计算，指针访问替代at访问）
// 整数部分用来提供坐标信息，小数部分提供原图像中4个邻近位置对新图像的像素的影响权重
// 浮点计算转化为整型计算的加速方法：将代表权重的小数部分乘以2048，在计算结束之后右移回原值，即可将权重部分的计算由小数转变为整数。
// 
void GeometricTransformation::augmentResize(Mat src, Mat &dst, Size dimension, float ratioX, float ratioY) {
    double scaleX = (double)src.cols / dst.cols;
    double scaleY = (double)src.rows / dst.rows;
    //使用指针访问像素值
    int stepDst = dst.step;
    int stepSrc = src.step;
    uchar* DataSrc = src.data;
    uchar* DataDst = dst.data;

    for (int j = 0; j < dst.rows; j++) {
        float fy = (float)((j + 0.5)*scaleY - 0.5);//y小数部分
        int sy = cvFloor(fy);//y整数部分
        fy -= sy;

        //预防越界访问
        sy = min(sy, src.rows - 2);
        sy = max(1, sy);
        //小数转为整数
        short cbufy[2];
        cbufy[0] = saturate_cast<short>((1.f - fy) * 2048);//右侧原图元素距新图元素的单位距离内比值
        cbufy[1] = 2048 - cbufy[0];//左侧原图元素距新图元素的单位距离的小数部分

        for (int i = 0; i < dst.cols; ++i) {

            float fx = (float)((i + 0.5)*scaleX - 0.5);//x小数部分
            int sx = cvFloor(fx);//x整数部分
            fx -= sx;

            //越界访问处理
            if (sx < 0) {
                fx = 0, sx = 0;
            }
            if (sx >= src.cols - 1) {//考虑到双线性插值会访问x+1的内容，因此，x的值不能超过cols-2
                fx = 0, sx = src.cols - 2;
            }
            //浮点计算转为整型计算
            short cbufx[2];
            cbufx[0] = saturate_cast<short>((1.f - fx) * 2048);
            cbufx[1] = 2048 - cbufx[0];

            // 双线性插值公式
            // 未加速的计算公式
            // dst(i,j)[c]=[src(sx,sy)[c]*fx+src(sx+1,sy)[c]*(1-fx)]*fy+[src(sx,sy+1)*fx+src(sx+1,sy+1)*(1-fx)]*(1-fy)
            // 使用加速数组替换浮点数fx,fy
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

// 图片旋转
// 1.将坐标原点由图像的左上角变换到旋转中心
// 2.以旋转中心为原点，图像旋转角度angle
// 3.旋转结束后，坐标原点变换到旋转后图像的左上角
void GeometricTransformation::ImgRotate(Mat src, Mat &dst, double angle) {
    //角度转化为弧度
    double radian = angle*M_PI / 180;
    double co = fabs(cos(radian));
    double si = fabs(sin(radian));

    //新图像长宽设置，以及新图像矩阵初始化
    int dstWidth = (int)src.cols*co + src.rows*si;
    int dstHeight = (int)src.cols*si + src.rows*co;
    dst = Mat(Size(dstWidth, dstHeight), CV_8UC3, Scalar::all(0));

    //以图像中心作为旋转中心，根据角度变换矩阵完成对新图的像素填充。
    //Point srcCenter(int(src.rows / 2), int(src.cols / 2));
    //旋转中心
    int xOff = dstWidth / 2;
    int yOff = dstHeight / 2;
    //新图像素范围
    int yMin = 0;
    int yMax = dstHeight;
    int xMin = 0;
    int xMax = dstWidth;

    double xSrc = 0, ySrc = 0;//原图坐标
    for (int y = yMin; y < yMax; y++) {
        for (int x = xMin; x < xMax; x++) {
            //以原图像左上角坐标为原点，获得新图中x，y坐标对应于原图中x,y的坐标点。
            //此时获得的该坐标为double浮点值，之后可以使用双线性插值方法，将该浮点值周围四个点的像素值加权赋值给新图对应坐标。
            ySrc = si*(x - xOff) + co*(y - yOff) + int(src.rows / 2);
            xSrc = co*(x - xOff) - si*(y - yOff) + int(src.cols / 2);
            //边界检查
            if (ySrc >= 0.&&ySrc < src.rows - 0.5&&xSrc >= 0 && xSrc < src.cols - 0.5) {//如果在原图范围内
                int xSmall = floor(xSrc);
                int xLarge = ceil(xSrc);
                int ySmall = floor(ySrc);
                int yLarge = ceil(ySrc);
                for (int channel = 0; channel < 3; channel++) {//双线性插值
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
    imshow("旋转", dst);
}

// 图像平移，直接使用反向映射+仿射矩阵即可完成计算。
void GeometricTransformation::ImgWave(Mat src, Mat &dst, Size wave) {
    dst = Mat(src.size(), CV_8UC3, Scalar::all(0));
    for (int i = 0; i < dst.rows; i++) {
        for (int j = 0; j < dst.cols; j++) {
            for (int channel = 0; channel < 3; channel++) {
                if (i - wave.width < 0 || j - wave.height < 0) {//数组越界访问
                    continue;
                }
                dst.at < Vec3b >(i, j)[channel] = src.at <Vec3b>(i - wave.width, j - wave.height)[channel];
            }
        }
    }
    imshow("图像旋转", dst);
}

// 图像平滑
// 空间域处理方法
void GeometricTransformation::ImgSmooth(Mat src, Mat &dst, Size dimension, Filter f, double sigma) {
    dst = Mat(src.size(), CV_8UC3, Scalar::all(0));

    if (dimension.height % 2 == 0 || dimension.width % 2 == 0) {//模板尺寸检查
        cout << "请使用奇数大小的滤波器尺寸" << endl;
        return;
    }
    //滤波器模板初始化
    Mat filter = Mat(dimension, CV_8U, Scalar::all(0));
    switch (f) {
    case BOX_FILTER:
        filter = BoxFilter(dimension);
        break;
    case GAUSSIAN_FILTER:
        filter = GaussianFilter(dimension, sigma);
        break;
    }
    //调试：滤波器值观察
    for (int i = 0; i < filter.cols; i++) {
        for (int j = 0; j < filter.rows; j++) {
            cout << (int)filter.at<uchar>(i, j) << " ";
        }
        cout << endl;
    }

    //原图像滑动范围定义
    int xMin = floor(dimension.width / 2);
    int yMin = int(floor(dimension.height / 2));
    int sumValue = 0;
    //滤波器模板系数计算
    for (int a = -xMin; a <= xMin; ++a) {
        for (int b = -yMin; b <= yMin; ++b) {
            sumValue += (int)filter.at<uchar>(a + xMin, b + yMin);
        }
    }
    if (sumValue == 0) {
        cout << "请确定滤波器模板是否存在" << endl;
        return;
    }

    //卷积核在原图像上滑动完成平滑处理
    for (int c = 0; c < 3; c++) {
        for (int j = 0; j < src.rows; ++j) {
            for (int i = 0; i < src.cols; ++i) {
                int meanValue = 0;
                for (int a = -xMin; a <= xMin; ++a) {
                    for (int b = -yMin; b <= yMin; ++b) {
                        if (i + a < 0 || i + a >= src.cols || j + b < 0 || j + b >= src.rows) {//访问越界检查
                            continue;
                        }
                        meanValue += filter.at<uchar>(a + xMin, b + yMin)*src.at<Vec3b>(i + a, j + b)[c];
                    }
                }
                dst.at<Vec3b>(i, j)[c] = saturate_cast<uchar>(meanValue / sumValue);
            }
        }
    }
    imshow("图像平滑", dst);
}

// 图像锐化
// 空间域处理方法
void GeometricTransformation::ImgSharpen(Mat src, Mat &dst, Size dimension, Filter f /* = BOX_FILTER */, double sigma/* =1.0 */) {
    //等待处理
}

//滤波器

//盒状滤波器
//所有系数都相等的空间均值滤波器,
Mat GeometricTransformation::BoxFilter(Size dimension) {
    return Mat::ones(dimension, CV_8U);
}

//高斯平滑滤波器
//根据高斯分布函数计算模板上各元素值
//模板尺寸(2k+1)
//高斯分布函数 h(i,j)=1/(2*pi*sigma^2)*exp((-pow(i-k-1,2)-pow(j-k-1,2))/2*sigma*sigma)
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