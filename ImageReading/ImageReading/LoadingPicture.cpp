#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace cv;

//Realize the common image format of the read, display, storage
int main()
{
    Mat img = imread("luna.jpg");
    if (img.empty())
    {
        return -1;
    }
    namedWindow("test", cv::WINDOW_NORMAL);
    imshow("test", img);
    waitKey(0);
    //    destroyWindow("test");
    Mat out;
    GaussianBlur(img, out, cv::Size(5, 5), 3, 3);
    //std::cout << cv::WINDOW_AUTOSIZE << std::endl;
    std::cout << out.rows << std::endl;
    std::cout << out.cols << std::endl;
    //std::cout << out.channels << std::endl;

    imwrite("2.jpg", out);
    waitKey(0);
    return 0;
}