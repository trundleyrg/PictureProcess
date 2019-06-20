#include "ImgSegment.h"

void Segment::AdjustThreshold(int thresholdValue, void* usrdata) {
    Mat srcImg;
    // 分割操作直接在原图像矩阵上进行。
    // 为避免污染源数据，使用copyto函数，复制一个新矩阵对象。执行速度降低
    (*(Mat*)(usrdata)).copyTo(srcImg);
    cout << "adjust:" << thresholdValue << endl;

    // 阈值分割
    threshold(srcImg, srcImg, thresholdValue, 255, THRESH_BINARY);

    // 轮廓查找
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    findContours(srcImg, contours, hierarchy,
        RETR_LIST, CHAIN_APPROX_SIMPLE);

    // 轮廓填充
    /*drawContours(srcImg, contours, -1,
        Scalar::all(255), 1, 8, hierarchy);*/
    for (int index = 0; index >= 0; index = hierarchy[index][0]) {
        Scalar color(rand() & 255, rand() & 255, rand() & 255);
        drawContours(srcImg, contours, index,
            color, 1, 8, hierarchy);
    }

    imshow("阈值分割", srcImg);
    waitKey(0);
}

void Segment::SegThreshold(Mat input, Mat output, int value, int upValue) {
    output = input.clone();
    //cout << output.channels() << endl;

    Mat_<uchar>::iterator it = output.begin<uchar>();
    Mat_<uchar>::iterator itend = output.end<uchar>();
    for (; it != itend; it++) {
        (*it) = (*it) > value ? upValue : 0;
    }
    imshow("阈值切割", output);
    waitKey(0);
}

// 区域生长算法
// 灰度实现：已知一个初始生长点，生成一幅同尺寸全黑图像，
// 根据初始生长点判断周围八邻域像素是否大于预定阈值，未超过则将该点加入生产长点栈中
// 栈为空时退出
Mat Segment::RegionGrowGray(Mat src, Point pt, int th) {
    Mat maskImg = Mat::zeros(src.size(), CV_8UC1);//
    vector<Point> GrowPtStack;//生长点栈
    int neighborhood[8][2] = { {-1,-1},{-1,0},{-1,1},{0,-1},
        {0,1},{1,-1},{1,0},{1,1} };//八邻域
    int curValue = 0;
    int maskValue = 0;
    int srcValue = 0;//生长起点灰度

    GrowPtStack.push_back(pt);
    srcValue = src.at<uchar>(pt.x, pt.y);
    cout << pt.x << "   " << pt.y << endl;
    maskImg.at<uchar>(pt.x, pt.y) = 255;

    while (!GrowPtStack.empty()) {
        Point pt = GrowPtStack.back();
        GrowPtStack.pop_back();

        for (int i = 0; i < 8; i++) {
            Point ptGrowing = Point(pt.x + neighborhood[i][0], pt.y + neighborhood[i][1]);

            //边缘点检查
            if (ptGrowing.x < 0 || ptGrowing.y < 0 || ptGrowing.x >= src.cols || ptGrowing.y >= src.rows) {
                continue;
            }

            maskValue = maskImg.at<uchar>(ptGrowing.x, ptGrowing.y);
            //相似元素入栈
            if (maskValue == 0) {
                curValue = src.at<uchar>(ptGrowing.x, ptGrowing.y);
                if (abs(srcValue - curValue) < th) {
                    GrowPtStack.push_back(ptGrowing);
                    maskImg.at<uchar>(ptGrowing.x, ptGrowing.y) = 255;
                }
            }
        }
    }
    imshow("maskImg", maskImg);
    waitKey(0);
    return maskImg.clone();
}

//膨胀：获取区域最大值
void Segment::ImgDilate(Mat src, Mat res, Mat kernel) {
    // opencv 库函数调用
    /*Mat res1;
    dilate(src, res1, kernel, Point(0, 0));*/
    //imwrite("膨胀调库3乘3.jpg", res1);

    // 原生实现
    // 膨胀即求图像在kernel区域大小的块中的局部最大值
    int rowNum = src.rows;
    int colNum = src.cols;
    int kernelRow = kernel.rows;
    int kernelCol = kernel.cols;
    int neightborhoodLen = kernelCol*kernelRow;
    vector<int> neightborhood[2];
    for (int i = (int)-kernelRow / 2; i <= (int)kernelRow / 2; i++)
        for (int j = (int)-kernelCol / 2; j <= (int)kernelCol / 2; j++) {
            neightborhood[0].push_back(i);
            neightborhood[1].push_back(j);
        }//初始化区域访问数组

    for (int i = 0; i < rowNum; i++)
        for (int j = 0; j < colNum; j++) {
            uchar tempMax = src.at<uchar>(i, j);
            // 遍历区域内容，获得局部最大值
            for (int a = 0; a < kernelRow; a++)
                for (int b = 0; b < kernelCol; b++) {
                    // 访问越界判断
                    if ((i + a) < 0 || (i + a) >= rowNum ||
                        (j + b) < 0 || (j + b) >= colNum) {
                        continue;
                    }
                    uchar tempValue = src.at<uchar>(i + a, j + b);
                    tempMax = tempValue > tempMax ? tempValue : tempMax;
                }
            res.at<uchar>(i, j) = tempMax;
        }
    /*imshow("膨胀", res);
    imwrite("膨胀原生3乘3.jpg", res);
    imshow("比较", res1 - res);
    imwrite("膨胀对比图.jpg", res1 - res);
    waitKey(0);*/
}

//腐蚀：获取区域最小值
void Segment::ImgErode(Mat src, Mat res, Mat kernel) {
    ////opencv 库函数调用
    //Mat res1;
    //erode(src, res1, kernel);
    //imshow("腐蚀内容", res1);
    //waitKey(0);

    //腐蚀即求图像在kernel区域大小的块中的局部最小值
    int rowNum = src.rows;
    int colNum = src.cols;
    int kernelRow = kernel.rows;
    int kernelCol = kernel.cols;
    int neightborhoodLen = kernelCol*kernelRow;
    vector<int> neightborhood[2];
    for (int i = (int)-kernelRow / 2; i <= (int)kernelRow / 2; i++)
        for (int j = (int)-kernelCol / 2; j <= (int)kernelCol / 2; j++) {
            neightborhood[0].push_back(i);
            neightborhood[1].push_back(j);
        }//初始化区域访问数组

    for (int i = 0; i < rowNum; i++)
        for (int j = 0; j < colNum; j++) {
            uchar tempMin = src.at<uchar>(i, j);
            // 遍历区域内容，获得局部最大值
            for (int a = 0; a < kernelRow; a++)
                for (int b = 0; b < kernelCol; b++) {
                    // 访问越界判断
                    if ((i + a) < 0 || (i + a) >= rowNum ||
                        (j + b) < 0 || (j + b) >= colNum) {
                        continue;
                    }
                    uchar tempValue = src.at<uchar>(i + a, j + b);
                    tempMin = tempValue > tempMin ? tempMin : tempValue;
                }
            res.at<uchar>(i, j) = tempMin;
        }
    /*imshow("腐蚀", res);
    waitKey(0);*/
}

//开运算为先腐蚀后膨胀
void Segment::OpeningOperation(Mat src, Mat res, Mat kernel) {
    Segment::ImgErode(src, res, kernel);
    Segment::ImgDilate(src, res, kernel);
    /*imshow("开运算", res);
    waitKey(0);*/
}

//闭运算为先膨胀后腐蚀
void Segment::CloseingOperation(Mat src, Mat res, Mat kernel) {
    Segment::ImgDilate(src, res, kernel);
    Segment::ImgErode(src, res, kernel);
    /*imshow("闭运算", res);
    waitKey(0);*/
}

// 测地膨胀
// 单次测地膨胀：在标记图像上使用指定大小结构元执行膨胀，然后和模板图像取交集
// src ： 标记图像
// mask ：模板图像
// kernel ： 结构元
void Segment::GeodesicDilation(Mat src, Mat mask, Mat kernel, int iterations, Mat res) {
    Segment seg;
    Mat dil(src.size(), CV_8UC1);
    int srcCols = src.cols;
    int srcRows = src.rows;
    int c = 0;
    src.copyTo(res);
    if (iterations < 0) {
        //膨胀重建，循环结束的判断标准是结果图像再次执行膨胀，重建图像也不再变化
        do {
            //res.copyTo(dil);
            for (int i = 0; i < srcRows; ++i)
                for (int j = 0; j < srcCols; ++j) {
                    res.at<uchar>(i, j) = mask.at<uchar>(i, j) > res.at<uchar>(i, j) ?
                        res.at<uchar>(i, j) : mask.at<uchar>(i, j);
                }
            seg.ImgDilate(res, res, kernel);
            cout << c++ << endl;
            imshow("膨胀重建", res);
            //图像不再变化时退出
            cout << "图像变化对比" << countNonZero(src - res) << endl;
            if (countNonZero(src - res) == 0) {
                break;
            }
        } while (true);
    }
    else {
        while (iterations-- == 0) {
            seg.ImgDilate(res, res, kernel);
            for (int i = 0; i < srcRows; ++i)
                for (int j = 0; j < srcCols; ++j) {
                    res.at<uchar>(i, j) = mask.at<uchar>(i, j) > res.at<uchar>(i, j) ?
                        res.at<uchar>(i, j) : mask.at<uchar>(i, j);
                }
        }
    }
}

// 形态学梯度
// 获取方式：膨胀图像减去腐蚀图像
void Segment::MorphologicalGradient(Mat src, Mat res, Mat kernel) {
    Mat temp(src.size(), CV_8UC1);
    //Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
    Segment seg;
    seg.ImgDilate(src, temp, kernel);
    seg.ImgErode(temp, res, kernel);
    res = temp - res;
}

// 分水岭算法:分割图像中紧密相连的目标
// src 需要为三通道彩色图像，markers要求为单通道32位图像
// Marker-controlled 分水岭分割
// 图像灰度化，绘制灰度图像边缘（canny算子，形态学梯度）
// 在便源图像上查找轮廓，按照不同编号绘制到markers上。
// 分水岭运算，并填色
void Segment::WaterShed(Mat src, Mat& markers) {
    Mat gray;
    markers = Scalar::all(0);
    cvtColor(src, gray, CV_BGR2GRAY);
    Canny(gray, gray, 80, 150);
    //MorphologicalGradient(gray, gray, getStructuringElement(MORPH_RECT, Size(3, 3)));
    imshow("canny", gray);
    Mat bi(gray.size(), CV_8UC1);
    Segment::SegThreshold(gray, gray, 120, 255);
    imshow("二值", gray);

    // 轮廓查找
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    findContours(gray, contours,
        hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point());
    Mat imageContours = Mat::zeros(src.size(), CV_8UC1);  //轮廓
    int comp_count = 0;
    for (int index = 0; index >= 0; index = hierarchy[index][0], comp_count++) {
        drawContours(markers, contours, index, Scalar::all((comp_count + 1) & 255), 1, 8, hierarchy);
        drawContours(imageContours, contours, index, Scalar(255), 1, 8, hierarchy);
    }
    cout << "轮廓数量" << contours.size() << endl;
    imshow("轮廓", imageContours);

    watershed(src, markers);
    // 分水岭之后mark内容
    Mat afterWatershed;
    convertScaleAbs(markers, afterWatershed);
    //imshow("after watershed", afterWatershed);

    // 对每个区域进行颜色填充
    Mat perImg = Mat::zeros(src.size(), CV_8UC3);
    for (int i = 0; i < markers.rows; i++)
        for (int j = 0; j < markers.cols; j++) {
            int index = markers.at<int>(i, j);
            if (index == -1) {
                perImg.at<Vec3b>(i, j) = Vec3b(255, 255, 255);
            }
            else {
                perImg.at<Vec3b>(i, j) = RandomColor(index);
            }
        }
    imshow("weight", perImg);
    addWeighted(src, 0.4, perImg, 0.6, 0, src);
    imshow("result", src);
    imwrite("canny梯度.jpg", perImg);
    waitKey(0);
}

//生成随机颜色函数
Vec3b Segment::RandomColor(int value) {
    value = value % 255;  //生成0-255的随机数  
    RNG rng;
    int aa = rng.uniform(0, value);
    int bb = rng.uniform(0, value);
    int cc = rng.uniform(0, value);
    return Vec3b(aa, bb, cc);
}