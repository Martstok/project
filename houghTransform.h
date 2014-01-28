#ifndef HOUGHTRANSFORM_H
#define HOUGHTRANSFORM_H

#include <opencv2/core/core.hpp>
#include <vector>

class Image;
class GuiParameters;

class HoughTransform
{
public:
    std::vector<cv::Vec2f> lines;
    std::vector<int> test;

    HoughTransform();
    void generateLongHoughLines(Image* img, GuiParameters* guiParameters);
    void generateShortHoughLines(Image* img, GuiParameters* guiParameters);
    void drawLongHoughLines(Image* img, GuiParameters* guiParameters);
    void drawShortHoughLines(Image* img, GuiParameters* guiParameters);
};

#endif // HOUGHTRANSFORM_H
