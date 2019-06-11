#include "open.h"

Mat ConvertColor2Gray(Mat color) {
    Mat gray;
    cvtColor(color, gray, COLOR_RGB2GRAY);
    //imwrite("CvtColor2Gray.jpg", gray);
    return gray;
}

Mat ConvertColor2Gray1(Mat color) {
    Mat gray(color.rows, color.cols, 0, 1);
    int rowNum = color.rows;
    int colNum = color.cols;
    for (int i = 0; i < rowNum; i++)
        for (int j = 0; j < colNum; j++) {
            gray.at<uchar>(i, j) = (color.at<Vec3b>(i, j)[0] + 
                color.at<Vec3b>(i, j)[1] + color.at<Vec3b>(i, j)[2])/3;
        }
    /*imshow("gray", gray);
    waitKey(0);*/
    //imwrite("CvtColor2Gray1.jpg", gray);
    return gray;
}

void GetChannel(Mat color) {
    vector<Mat> channels;
    Mat blue;
    Mat green;
    Mat red;
    split(color, channels);
    blue = channels.at(0);
    green = channels[1];
    red = channels[2];

    Mat mergeImage;
    merge(vector<Mat>{blue, green, red}, mergeImage);
    imshow("merge", mergeImage);
    waitKey(0);
}

void DisplayColorHistogram(Mat RawImg) {

}