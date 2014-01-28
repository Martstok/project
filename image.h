#ifndef IMAGE_H
#define IMAGE_H

#include <opencv2/video/video.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <string>
#include <vector>
#include <opencv2/nonfree/features2d.hpp>



// Stores all images derived from the original. Should include filename upon contruction.
class Image
{
public:
    Image();
    Image(std::string); //String defines filename. Input is assumed to be video.
    Image(std::string, std::string);    // 1st string defines filename. Second is either "image" or "video".
    cv::VideoCapture cap;
    cv::Mat result;
    cv::Mat result2;
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
    cv::Mat hsv;
    std::vector<cv::KeyPoint> keypoints;
    cv::Mat descriptor;
    cv::Mat temp;
    cv::Mat temp2;
    std::vector<cv::Mat> bwList;
    cv::Mat plot;

};


// Downscales the image until amount of both rows and columns are less than upperLimit. Uses Gaussian pyramid (blur) for each iteration.
cv::Mat downsample(cv::Mat inputImage, int upperLimit);


#endif // IMAGE_H
