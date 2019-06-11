## GrayHistogram.cpp
灰度图像处理、主函数文件

DisplayHistogram1:
1.预定义直方图创建函数所需要的变量值。
2.从数据创建直方图。calcHist函数有三种使用方式，本项目使用第一种函数定义方式。
calcHist( const Mat* images, //数组数据，输入图像，允许同时输入多张图片
    int nimages, //输入图片数量
    const int* channels, //该整数表明计算直方图时，使用输入数据中的哪些通道
    InputArray mask, //选择数组中哪些像素参与直方图计算。不使用可赋值为Mat()或者noArray
    OutputArray hist, //直方图计算的结果
    int dims, //输出直方图的维度，与channels中元素个数相同
    const int* histSize, //指定各维度需要分为多少个区间，灰度图像像素范围0-255，可以使用255来给每个像素做数量统计
    const float** ranges,  //长度等于dims，定义区间长度
    bool uniform = true,  
    bool accumulate = false 
);
3.定义灰度直方图画布数组，Mat类型。行为scale*size，高为size，颜色通道为1。
4.获得直方图数组中像素最小值和最大值。
5.遍历直方图结果，对获取每个像素值的对应个数，并使用rectangle函数绘制直方图。
6.显示灰度图

## Color.cpp
彩色图像处理

ConvertColor2Gray：
调用cvtColor函数实现彩色图像转灰度图像。
ConvertColor2Gray1:
对彩色图像RGB三通道求均值，将彩色图像转换为灰度图像。
GetChannel:
使用split函数将彩色图像切分为BGR三个单通道数组。
使用merge函数将三个单通道数组合并为一张彩色图像。

