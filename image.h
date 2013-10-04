#ifndef IMAGE_H
#define IMAGE_Hqt

#include <opencv2/video/video.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <string>
#include <vector>
#include <opencv2/nonfree/features2d.hpp>

class Image
{
public:
    Image();
    Image(std::string);
    Image(std::string, std::string);
    cv::VideoCapture cap;
    cv::Mat raw;
    cv::Mat gray;
    cv::Mat canny;
    cv::Mat prevRaw;
    cv::Mat flow;
    cv::Mat srcLR;
    cv::Mat bw;
    cv::Mat blur;
    std::vector<cv::KeyPoint> keypoints;
    cv::Mat descriptor;

    std::vector<cv::Mat> bwList;
};



#endif // IMAGE_H
