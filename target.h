#ifndef TARGET_H
#define TARGET_H

#include <opencv2/core/core.hpp>
#include <vector>

class Image;
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

    void generateContours(Image* img);

    void findBiggestContours(int numOfContoursToFind);
    void getResults(Image* img);
    void analyzeGeometry(Image* img, int);
    double getAreaToCircumferenceRatio(int currentIndex);
};

#endif // TARGET_H
