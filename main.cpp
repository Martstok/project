#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "image.h"
#include <string>
#include "stdio.h"
#include <opencv2/imgproc/imgproc.hpp>
#include "gui.h"

using namespace std;
using namespace cv;


//Define variables

GuiParameters* guiParameters = new GuiParameters;
#define NSAMPLES 7
int avgColor[NSAMPLES][3] ;
int c_lower[NSAMPLES][3];
int c_upper[NSAMPLES][3];
int area;
int gap = 50;

int threshold_value = 0;
int threshold_type = 3;
int const max_value = 255;
int const max_type = 4;
int const max_BINARY_value = 255;
char* window_name = "Threshold Demo";

char* trackbar_type = "Type: \n 0: Binary \n 1: Binary Inverted \n 2: Truncate \n 3: To Zero \n 4: To Zero Inverted";
char* trackbar_value = "Value";

//Declare functions

void normalizeColors(Image *img);
void produceBinaries(Image *img);
void initTrackbars();

//================================MAIN================================

int main(){
    string sourceReference = "Windmill6.jpg";
    string sourceType = "image";
    Image* img = new Image(sourceReference, sourceType);
    Image* prevImg = new Image;

    namedWindow("frame", CV_WINDOW_KEEPRATIO);
    namedWindow("BW", CV_WINDOW_KEEPRATIO);
    namedWindow("trackbars",CV_WINDOW_KEEPRATIO);
    moveWindow("trackbars", 355,0);
    moveWindow("BW", 0,350);
    initializeTrackbars(guiParameters);
    int framenum = 0;
    int thresh = 100;
    if (sourceType == "video"){
    img->cap >> img->raw;
    }



    while(true){
        if (sourceType == "video"){
            img->prevRaw = img->raw;
            img->cap >> img->raw;
        }
        if (img->raw.empty()){
            break;
        }
        pyrDown(img->raw, img->srcLR);
        blur(img->srcLR,img->blur,Size(5,5));

//        cvtColor(img->srcLR,img->srcLR,CV_BGR2HLS);
        produceBinaries(img);
//        cvtColor(img->srcLR,img->srcLR,CV_HLS2BGR);
        cvtColor(img->raw, img->gray, CV_BGR2GRAY);

        imshow("frame", img->raw);
        imshow("BW", img->bw);
        char c = cvWaitKey(33);
        if (c == 27) break;

    }
    return 0;
}

void produceBinaries(Image* img){

    Scalar lowerBound;
    Scalar upperBound;
    for(int i=0;i<NSAMPLES;i++){

//        normalizeColors(img);
//        lowerBound=Scalar( c_lower[i][0] , c_lower[i][1], c_lower[i][2] );
//        upperBound=Scalar( c_upper[i][0] , c_upper[i][1], c_upper[i][2] );
        lowerBound=Scalar( guiParameters->area,guiParameters->area,guiParameters->area );
        upperBound=Scalar( guiParameters->area+guiParameters->gap,guiParameters->area+guiParameters->gap,guiParameters->area+guiParameters->gap);
        img->bwList.push_back(Mat(img->srcLR.rows,img->srcLR.cols,CV_8U));
        inRange(img->blur,lowerBound,upperBound,img->bwList[i]);
    }

    img->bwList[0].copyTo(img->bw);
//    for(int i=1;i<NSAMPLES;i++){
//        img->bw+=img->bwList[i];
//    }
}

//void normalizeColors(Image* img){
//    // copy all boundries read from trackbar
//    // to all of the different boundries
//    for(int i=1;i<NSAMPLES;i++){
//        for(int j=0;j<3;j++){
//            c_lower[i][j]=c_lower[0][j];
//            c_upper[i][j]=c_upper[0][j];
//        }
//    }
//    // normalize all boundries so that
//    // threshold is whithin 0-255
//    for(int i=0;i<NSAMPLES;i++){
//        if((avgColor[i][0]-c_lower[i][0]) <0){
//            c_lower[i][0] = avgColor[i][0] ;
//        }if((avgColor[i][1]-c_lower[i][1]) <0){
//            c_lower[i][1] = avgColor[i][1] ;
//        }if((avgColor[i][2]-c_lower[i][2]) <0){
//            c_lower[i][2] = avgColor[i][2] ;
//        }if((avgColor[i][0]+c_upper[i][0]) >255){
//            c_upper[i][0] = 255-avgColor[i][0] ;
//        }if((avgColor[i][1]+c_upper[i][1]) >255){
//            c_upper[i][1] = 255-avgColor[i][1] ;
//        }if((avgColor[i][2]+c_upper[i][2]) >255){
//            c_upper[i][2] = 255-avgColor[i][2] ;
//        }
//    }
//}
