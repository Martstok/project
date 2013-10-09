#include "gui.h"
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv;


GuiParameters::GuiParameters()
{
    for(int i=0;i<NSAMPLES;i++){
        c_lower[i][0]=200;
        c_upper[i][0]=255;
        c_lower[i][1]=200;
        c_upper[i][1]=255;
        c_lower[i][2]=200;
        c_upper[i][2]=255;
    }
    area = 0;
    gap = 35;
    cannyThreshold = 50;
    houghThreshold = 50;
    houghMinLength = 50;
    houghMaxGap = 10;
}

void initializeTrackbars(GuiParameters* guiParameters){
    for(int i=0;i<NSAMPLES;i++){
        guiParameters->c_lower[i][0]=200;
        guiParameters->c_upper[i][0]=255;
        guiParameters->c_lower[i][1]=200;
        guiParameters->c_upper[i][1]=255;
        guiParameters->c_lower[i][2]=200;
        guiParameters->c_upper[i][2]=255;
    }
    createTrackbar("lower1","trackbars",&(guiParameters->c_lower)[0][0],255);
    createTrackbar("lower2","trackbars",&(guiParameters->c_lower)[0][1],255);
    createTrackbar("lower3","trackbars",&(guiParameters->c_lower)[0][2],255);
    createTrackbar("upper1","trackbars",&(guiParameters->c_upper)[0][0],255);
    createTrackbar("upper2","trackbars",&(guiParameters->c_upper)[0][1],255);
    createTrackbar("upper3","trackbars",&(guiParameters->c_upper)[0][2],255);
    createTrackbar("area","trackbars",&(guiParameters->area),235);
    createTrackbar("canny","trackbars",&(guiParameters->cannyThreshold), 255);
    createTrackbar("houghThreshold","trackbars",&(guiParameters->houghThreshold), 255);
    createTrackbar("houghMinLength","trackbars",&(guiParameters->houghMinLength), 255);
    createTrackbar("houghMaxGap","trackbars",&(guiParameters->houghMaxGap), 255);
    createTrackbar("blur","trackbars",&(guiParameters->blur), 25);
    createTrackbar("erode","trackbars",&(guiParameters->erode), 25);
}


void initializeGui(GuiParameters* guiParameters){
    namedWindow("frame", CV_WINDOW_AUTOSIZE);
    namedWindow("BW", CV_WINDOW_KEEPRATIO);
    namedWindow("trackbars",CV_WINDOW_KEEPRATIO);
    moveWindow("trackbars", 355,0);
    moveWindow("BW", 0,350);
    namedWindow("canny", CV_WINDOW_KEEPRATIO);
    moveWindow("canny",0,0);
    namedWindow("houghLines", CV_WINDOW_KEEPRATIO);
    initializeTrackbars(guiParameters);
}
