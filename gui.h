#ifndef GUI_H
#define GUI_H

#define NSAMPLES 7

class Image;

//Contains parameters which can be adjusted using the GUI.
struct GuiParameters
{
    GuiParameters();
    int c_lower[NSAMPLES][3];
    int c_upper[NSAMPLES][3];
    int avgColor[NSAMPLES][3];
    int blur;
    int dilate;
    int houghThreshold;
    int houghMinLength;
    int houghMaxGap;
};

//Initializes GUI windows.
void initializeGui(GuiParameters* guiParameters);

//Links trackbars to variables and sets initial values.
void initializeTrackbars(GuiParameters*);

//Updates GUI for each program iteration.
void updateWindows(Image* img);



#endif // GUI_H
