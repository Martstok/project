#ifndef IMAGE_H
#define IMAGE_Hqt

#include <opencv2/video/video.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <string>


class Image
{
public:
    Image();
    Image(std::string);
    cv::VideoCapture cap;
    cv::Mat raw;
    cv::Mat gray;
    cv::Mat canny;
    cv::Mat prevRaw;
    cv::Mat flow;
};

#endif // IMAGE_H
