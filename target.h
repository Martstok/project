#ifndef TARGET_H
#define TARGET_H

#include <opencv2/core/core.hpp>
#include <vector>

class Target
{
public:
    std::vector<std::vector<cv::Point> > contours;
    std::vector<std::vector<int> > hullI;
    std::vector<std::vector<cv::Point> > hullP;
    std::vector<std::vector<cv::Vec4i> > defects;
    std::vector<int> indicesOfBiggestContours;

    void initContourVectors();
    Target();
};

#endif // TARGET_H
