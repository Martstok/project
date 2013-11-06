#ifndef GUI_H
#define GUI_H

#define NSAMPLES 7

class Image;
struct GuiParameters
{
    GuiParameters();
    int c_lower[NSAMPLES][3];
    int c_upper[NSAMPLES][3];
    int avgColor[NSAMPLES][3];
    int range;
    int gap;
    int cannyThreshold;
    int houghThreshold;
    int houghMinLength;
    int houghMaxGap;
    int blur;
    int dilate;
};


void initializeGui(GuiParameters* guiParameters);
void initializeTrackbars(GuiParameters*);
void updateWindows(Image* img);








#endif // GUI_H
