#ifndef TARGET_H
#define TARGET_H

#include <opencv2/core/core.hpp>
#include <vector>

class Image;

//Stores data from contour analysis and contains functions finding final results.
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

    //Uses the segmented binary image to find contours.
    void generateContours(Image* img);

    //Finds indices of biggest contours in sorted order.
    void findBiggestContours(int numOfContoursToFind);

    //Draws contours and convexity defects and calls analyseGeometry
    void getResults(Image* img);

    //Finds hub center position and angles, and draws them on image
    void analyzeGeometry(Image* img, int);

    //Temporary template for shape factor analysis, not currently in use
    double getAreaToCircumferenceRatio(int currentIndex);
};

#endif // TARGET_H
