#ifndef IMAGE_H
#define IMAGE_H

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
    cv::Mat rawLR;
    cv::Mat bw;
    cv::Mat blur;
    cv::Mat grayLR;
    cv::Mat hough;
    std::vector<cv::KeyPoint> keypoints;
    cv::Mat descriptor;

    std::vector<cv::Mat> bwList;

};

cv::Mat downsample(cv::Mat inputImage, int upperLimit);


#endif // IMAGE_H
