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
#include "fileIO.h"
#include "houghTransform.h"

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
void autonomous(GuiParameters* guiParameters, Target* target, Image* img);  //Autonomous thresholding bound segmentation and validation

//================================MAIN================================

int main(){
    //Initialize clock
    double t = (double)getTickCount();


    string sourceReference = "Windmill14.jpg";
    string sourceType = "image";
//    sourceType = "video";
//    sourceReference = "video2.mp4";
    Image* img = new Image(sourceReference, sourceType);
    Target* target = new Target;

    initializeGui(guiParameters);

    if (sourceType == "video"){
    img->cap >> img->raw;
    }
            img->plot = Mat(181,400, CV_8UC1);

    //Main loop of the program
    while(true){
        // Output time measurement
//        cout << endl << "-------Entering new iteration---------" << endl;
//        double elapsed = (double)getTickCount()-t;
//        cout << "Last iteration time: " << elapsed/getTickFrequency() << endl;
//        t = (double)getTickCount();

        //Updates image if input is a video. If not, the same image is kept for every iteration.
        if (sourceType == "video"){
            img->prevRaw = img->raw;
            img->cap >> img->raw;
        }

        if (img->raw.empty()){
            break;
        }
        //Reduce image resolution
        img->raw.copyTo(img->rawLR);
        img->rawLR = downsample(img->raw, 500);

        //Transform to HSV color space
        cvtColor(img->rawLR, img->hsv, CV_BGR2HSV);

        //Autonomous search
//        autonomous(guiParameters, target, img);


//        ==================   Generate Histogram
//        Mat v_hist;
//        vector<Mat> hsv_planes;
//        split( img->rawLR, hsv_planes );
//        int histSize = 256;
//        float range[] = { 0, 256 } ; //the upper boundary is exclusive
//        const float* histRange = { range };
//        calcHist( &hsv_planes[2], 1, 0, Mat(), v_hist, 1, &histSize, &histRange, true, false );
//        int hist_w = 512; int hist_h = 400;
//        int bin_w = cvRound( (double) hist_w/histSize );
//        Mat histImage( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );
//        normalize(v_hist, v_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
//        for( int i = 1; i < histSize; i++ )
//        {
//            line( histImage, Point( bin_w*(i-1), hist_h - cvRound(v_hist.at<float>(i-1)) ) ,
//                             Point( bin_w*(i), hist_h - cvRound(v_hist.at<float>(i)) ),
//                             Scalar( 0, 0, 255), 2, 8, 0  );
//        }
//        namedWindow("calcHist Demo", CV_WINDOW_AUTOSIZE );
//        imshow("calcHist Demo", histImage );
        vector<double> X;
        vector<double> Y;
        img->plot = Scalar(0);
//        -----------------------In Main: Draw long hough lines


        HoughTransform* hough = new HoughTransform;
        hough->generateLongHoughLines(img, guiParameters);
        hough->drawLongHoughLines(img, guiParameters);


        //------------------------In Main: Draw short hough lines

//        drawShortHoughLines(img, guiParameters);




//        //Perform color thresholding segmentation
//        produceBinariesHSV(img, guiParameters);


//        cvtColor(img->rawLR, img->gray, CV_BGR2GRAY);
////        pyrDown(img->gray, img->grayLR);

//        //Dilate + Erode
//        M = getStructuringElement(MORPH_RECT, Size(guiParameters->dilate+1, guiParameters->dilate+1));
//        dilate(img->bw, img->bw, M);
//        erode(img->bw, img->bw, M);

//        //Create image on which to draw results
//        img->bw.copyTo(img->result);
//        cvtColor(img->result,img->result, CV_GRAY2BGR);

//        //Contours analysis begins here
//        int numOfContoursToFind = 1;
//        target->generateContours(img);
//        target->findBiggestContours(numOfContoursToFind);


//        // Generate results from found contours
//        target->getResults(img);

        updateWindows(img);
        char c = cvWaitKey(30);    //Allow some time for program to display GUI
//        c = cvWaitKey(99999999);
//        if (c == 27) break;
    }
}






void autonomous(GuiParameters* guiParameters, Target* target, Image* img){
    int numOfContoursToFind = 1;
    int currentIndex;
    int bestLimit = 0;
    int numOfConvexityDefects = 0;
    double bestRatio = 0;
    int bestArea = 0;
    int bestRange = 0;
    bool exit = false;
    for(guiParameters->c_upper[0][2] = 255; guiParameters->c_upper[0][2] >= 0; guiParameters->c_upper[0][2] -= 5){
//    for(guiParameters->c_lower[0][2] = 0; guiParameters->c_lower[0][2] <= 255; guiParameters->c_lower[0][2] += 5){
        produceBinariesHSV(img, guiParameters);
        cvtColor(img->rawLR, img->gray, CV_BGR2GRAY);
//        pyrDown(img->gray, img->grayLR);
        M = getStructuringElement(MORPH_RECT, Size(guiParameters->dilate+1, guiParameters->dilate+1));
        dilate(img->bw, img->bw, M);
        erode(img->bw, img->bw, M);
        target->generateContours(img);
        target->findBiggestContours(numOfContoursToFind);
        int currentIndex;

//        double ratio = 0;
//        int area = 0;
//        ratio = target->getAreaToCircumferenceRatio(currentIndex);
//        area = contourArea(target->contours[currentIndex]);

        //Checks if 4 convexity defects are present. (In all cases so far only one contour has been found, so probably the loop structure turns out unnecessary)
        for(int i = 0; i < target->indicesOfBiggestContours.size(); i++){
             currentIndex = target->indicesOfBiggestContours[i];
             approxPolyDP(Mat(target->contours[currentIndex]), target->contours[currentIndex],11,true);
             convexHull(Mat(target->contours[currentIndex]), target->hullI[currentIndex], false, false);
             if(target->hullI[currentIndex].size() > 3){
                 convexityDefects(target->contours[currentIndex],target->hullI[currentIndex],target->defects[currentIndex]);
                 numOfConvexityDefects = target->defects[currentIndex].size();
                 if (numOfConvexityDefects == 4){
//                     cout << "=================================" << "limit " << guiParameters->c_upper[0][2]<< endl;
                     bestLimit = guiParameters->c_upper[0][2];
                     exit = true;
                 }
             }

//        cout << "range, ratio: " << guiParameters->range << ",  " << ratio << endl;

        }


//        if (area>bestArea && ratio>0.075 && ratio<0.15){
//            bestRatio = ratio;
//            bestRange = guiParameters->range;
//            bestArea = area;
//        }
        if (exit){
            break;
        }
    }
//    cout << "best area " << bestRange << endl;
//    cout << "bestLimit " << bestLimit << endl;
//    guiParameters->range = bestRange;
    guiParameters->c_upper[0][2] = bestLimit;

}






































//vector<Vec2f> detectVerticalHoughLine(vector<Vec2f> lines){
//    int ratio = 15;
//    vector<Vec2f> verticalLines;
//    for(size_t i = 0; i < lines.size(); i++) {
//        if(abs(lines[i][1]/CV_PI*180)< 1){
//            verticalLines.push_back(lines[i]);
//        }
//    }
//    cout << "size: " << verticalLines.size()<< endl;
//   return verticalLines;
//}




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
