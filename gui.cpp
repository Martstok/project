#include "gui.h"
#include "image.h"
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
    dilate = 10;
    houghThreshold = 1;
    houghMaxGap = 10;
    houghMinLength = 50;
}

void initializeTrackbars(GuiParameters* guiParameters){
    for(int i=0;i<NSAMPLES;i++){
        guiParameters->c_lower[i][0]=0;
        guiParameters->c_upper[i][0]=255;
        guiParameters->c_lower[i][1]=0;
        guiParameters->c_upper[i][1]=50;
        guiParameters->c_lower[i][2]=0;
        guiParameters->c_upper[i][2]=255;
    }
    createTrackbar("lowerH","trackbars",&(guiParameters->c_lower)[0][0],255);
    createTrackbar("lowerS","trackbars",&(guiParameters->c_lower)[0][1],255);
    createTrackbar("lowerV","trackbars",&(guiParameters->c_lower)[0][2],255);
    createTrackbar("upperH","trackbars",&(guiParameters->c_upper)[0][0],255);
    createTrackbar("upperS","trackbars",&(guiParameters->c_upper)[0][1],255);
    createTrackbar("upperV","trackbars",&(guiParameters->c_upper)[0][2],255);
    createTrackbar("dilate+erode","trackbars",&(guiParameters->dilate), 25);
    createTrackbar("Hough", "trackbars", &(guiParameters->houghThreshold),1000);
    createTrackbar("HoughMinLength", "trackbars", &(guiParameters->houghMinLength),255);
    createTrackbar("HoughMaxGap", "trackbars", &(guiParameters->houghMaxGap),255);
}


void initializeGui(GuiParameters* guiParameters){
    namedWindow("frame", CV_WINDOW_AUTOSIZE);
    namedWindow("BW", CV_WINDOW_AUTOSIZE);
    namedWindow("trackbars",CV_WINDOW_AUTOSIZE);
//    namedWindow("hough", CV_WINDOW_AUTOSIZE);
    moveWindow("trackbars", 600,0);
    moveWindow("BW", 0,600);
    namedWindow("result", CV_WINDOW_AUTOSIZE);
    moveWindow("result",0,0);
    namedWindow("result2",CV_WINDOW_AUTOSIZE);
    initializeTrackbars(guiParameters);
    namedWindow("plot", CV_WINDOW_AUTOSIZE);
}

void updateWindows(Image* img){
    imshow("frame", img->rawLR);
//    imshow("hough", img->hough);
//    imshow("BW", img->bw);
    imshow("result", img->result);
    imshow("result2", img->result2);
    imshow("plot", img->plot);
}
