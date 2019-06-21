#include "GeometricTransformation.h"

// 尺寸变换，根据输入的尺寸，对比原图像大小，缩放或扩展图像
void GeometricTransformation::ImgResize(Mat src, Mat &dst, Size dimension) {
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
    //最近值填充法，该方法形成图像效果不好，有马赛克问题
    geo.augmentResize(src, dst, dimension, ratioX, ratioY);
    //双线性插值法
    geo.smallResize(src, dst, dimension, ratioX, ratioY);
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

// 双线性插值,图像缩放
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
        sy = min(sy, src.rows - 3);
        sy = max(1, sy);

        short cbufy[2];
        cbufy[0] = saturate_cast<short>((1.f - fy) * 2048);//高位
        cbufy[1] = 2048 - cbufy[0];//小数位

        for (int i = 0; i < dst.cols; ++i) {
            float fx = (float)((i + 0.5)*scaleX - 0.5);//x小数部分
            int sx = cvFloor(fx);//x整数部分
            fx -= sx;

            //越界访问处理
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
    //            int x = i / ratioX;//x整数部分，整数部分记载的是src中左上角坐标位置(opencv世界观的坐标系)
    //            int y = j / ratioY;//y整数部分
    //            float fx = i / ratioX - x;//x小数部分,该部分作为权重值，反映映射点周围四个像素对新图中的像素的影响比例
    //            float fy = j / ratioY - y;//y小数部分

    //            float bottom[3], top[3];
    //            if (fx == 0 && fy == 0) {//为了提高效率，试图加速。因此增加了条件判断，都为整数则直接填入
    //                dst.at<Vec3b>(i, j)[c] = src.at<Vec3b>(x, y)[c];
    //            }
    //            else if (x>src.rows-1 ) {//边界检查,超过范围则直接赋值

    //            }
    //            else {//双线性插值
    //                if (x + 1 >= src.rows || )
    //                    //首先在x轴上完成插值
    //                    bottom[c] = fx*src.at<Vec3b>(x, y)[c] + (1 - fx)*src.at<Vec3b>(x + 1, y)[c];
    //                top[c] = fx*src.at<Vec3b>(x, y + 1)[c] + (1 - fx)*src.at<Vec3b>(x + 1, y + 1)[c];
    //                //然后在y轴上完成插值
    //                dst.at<Vec3b>(i, j)[c] = fy*bottom[c] + (1 - fy)*top[c];
    //            }
    //        }
}