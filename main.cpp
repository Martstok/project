#include <iostream>
#include <string>
#include <ctime>
#include <sstream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "image.h"
#include "stdio.h"
#include <opencv2/imgproc/imgproc.hpp>
#include "gui.h"
#include "colorThresholding.h"
#include "target.h"

#define PI 3.14159265



#include <vector>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/legacy/legacy.hpp>


using namespace std;
using namespace cv;

//Define variables
GuiParameters* guiParameters = new GuiParameters;
Mat M;


//Declare functions
void normalizeColors(Image *img);
void produceBinaries(Image *img);
void initTrackbars();
void surf(Image* img);
vector<Vec2f> detectVerticalHoughLine(vector<Vec2f> lines);
void generateContours(Image* img, Target* target);
vector<int> findBiggestContours(vector<vector<Point> > contours, int contoursToFind);

//================================MAIN================================

int main(){
    clock_t start_time = clock();
    string sourceReference = "Windmill6.jpg";
    string sourceType = "image";
    string sourceReference2 = "Windmill.jpg";
    string sourceType2 = "image";
//    sourceType = "video";
//    sourceReference = "video.mp4";
    Image* img = new Image(sourceReference, sourceType);
    Image* prevImg = new Image;
    Target* target = new Target;


    Image* img2 = new Image(sourceReference2, sourceType2);
    initializeGui(guiParameters);

    if (sourceType == "video"){
    img->cap >> img->raw;
    }

//    vector<Vec4i> lines;
//    vector<Vec2f> lines2;
//    vector<Vec2f> vLines;
    while(true){
        cout << "Last loop time:" << float(clock() - start_time) << endl;
        start_time = clock();
        if (sourceType == "video"){
            img->prevRaw = img->raw;
            img->cap >> img->raw;
        }

        if (img->raw.empty()){
            break;
        }
        img->raw.copyTo(img->rawLR);
        img->rawLR = downsample(img->raw, 600);

//        pyrDown(img->raw, img->rawLR);
//        blur(img->rawLR,img->blur,Size(5,5));
        produceBinaries(img, guiParameters);
        cvtColor(img->rawLR, img->gray, CV_BGR2GRAY);
//        pyrDown(img->gray, img->grayLR);
        blur(img->bw,img->bw,Size(guiParameters->blur+1,guiParameters->blur+1));
        M = getStructuringElement(MORPH_RECT, Size(guiParameters->dilate+1, guiParameters->dilate+1));
        dilate(img->bw, img->bw, M);
        int cannyRatio = 3;
        Canny(img->bw, img->canny, guiParameters->cannyThreshold,guiParameters->cannyThreshold*cannyRatio,3);



        cvtColor(img->canny, img->canny, CV_GRAY2BGR);


        //Contours begin here
        target->generateContours(img);

        int numOfContoursToFind = 1;
        int currentIndex;
        target->findBiggestContours(numOfContoursToFind);
//        target->indicesOfBiggestContours = findBiggestContours(target->contours, numOfContoursToFind);
        target->getResults(img);


        updateWindows(img);
        char c = cvWaitKey(10);
//        c = cvWaitKey(99999999);
//        if (c == 27) break;

    }
    return 0;
}










































vector<Vec2f> detectVerticalHoughLine(vector<Vec2f> lines){
    int ratio = 15;
    vector<Vec2f> verticalLines;
    for(size_t i = 0; i < lines.size(); i++) {
        if(abs(lines[i][1]/CV_PI*180)< 1){
            verticalLines.push_back(lines[i]);
        }
    }
    cout << "size: " << verticalLines.size()<< endl;
   return verticalLines;
}




//-----------------------In Main: Draw long hough lines
//HoughLines(img->canny, lines2, 1, CV_PI/180, guiParameters->houghThreshold );
//cvtColor(img->canny,img->canny, CV_GRAY2BGR);
//for( size_t i = 0; i < lines2.size(); i++ )
//{
//  float rho = lines2[i][0], theta = lines2[i][1];
//  Point pt1, pt2;
//  double a = cos(theta), b = sin(theta);
//  double x0 = a*rho, y0 = b*rho;
//  pt1.x = cvRound(x0 + 1000*(-b));
//  pt1.y = cvRound(y0 + 1000*(a));
//  pt2.x = cvRound(x0 - 1000*(-b));
//  pt2.y = cvRound(y0 - 1000*(a));
//  line( img->canny, pt1, pt2, Scalar(0,0,255), 3, CV_AA);
////          img->hough.at<Scalar>(abs(int(rho)),int(theta/CV_PI*180)) = 255;           BUGGY
//}



//------------------------In Main: filter out vertical long hough lines
//vLines = detectVerticalHoughLine(lines2);
//for( size_t i = 0; i < vLines.size(); i++ ){
//    cout << "theta:  " << vLines.size() << endl;
//    float rho = vLines[i][0], theta = vLines[i][1];
//    Point pt1, pt2;
//    double a = cos(theta), b = sin(theta);
//    double x0 = a*rho, y0 = b*rho;
//    pt1.x = cvRound(x0 + 1000*(-b));
//    pt1.y = cvRound(y0 + 1000*(a));
//    pt2.x = cvRound(x0 - 1000*(-b));
//    pt2.y = cvRound(y0 - 1000*(a));
//    line( img->hough, pt1, pt2, Scalar(255,0,0), 3, CV_AA);
//}


//------------------------In Main: Draw short hough lines
//        HoughLinesP(img->canny, lines, 1, CV_PI/180, guiParameters->houghThreshold+1, guiParameters->houghMinLength, guiParameters->houghMaxGap );
//        img->hough.copyTo(temp);
//        for( size_t i = 0; i < lines.size(); i++ )
//        {
//          Vec4i l = lines[i];
//          line( img->canny, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(255,0,0), 3, CV_AA);
//        }
