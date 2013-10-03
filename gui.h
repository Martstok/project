#ifndef GUI_H
#define GUI_H

#define NSAMPLES 7
struct GuiParameters
{
    GuiParameters();
    int c_lower[NSAMPLES][3];
    int c_upper[NSAMPLES][3];
    int avgColor[NSAMPLES][3];
    int area;
    int gap;
};

void initializeTrackbars(GuiParameters*);









#endif // GUI_H
