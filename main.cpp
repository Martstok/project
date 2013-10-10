#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "image.h"
#include <string>
#include "stdio.h"
#include <opencv2/imgproc/imgproc.hpp>
#include "gui.h"
#include "colorThresholding.h"

#include <vector>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/legacy/legacy.hpp>


using namespace std;
using namespace cv;

//Define variables
GuiParameters* guiParameters = new GuiParameters;


//Declare functions
void normalizeColors(Image *img);
void produceBinaries(Image *img);
void initTrackbars();
void surf(Image* img);
vector<Vec2f> detectVerticalHoughLine(vector<Vec2f> lines);

//================================MAIN================================

int main(){
    string sourceReference = "Windmill2.jpg";
    string sourceType = "image";
    string sourceReference2 = "Windmill.jpg";
    string sourceType2 = "image";
    Image* img = new Image(sourceReference, sourceType);
    Image* prevImg = new Image;

    Image* img2 = new Image(sourceReference2, sourceType2);
    initializeGui(guiParameters);

    if (sourceType == "video"){
    img->cap >> img->raw;
    }

    vector<Vec4i> lines;
    vector<Vec2f> lines2;
    vector<Vec2f> vLines;
    while(true){
        if (sourceType == "video"){
            img->prevRaw = img->raw;
            img->cap >> img->raw;
        }
        if (img->raw.empty()){
            break;
        }
        img->raw.copyTo(img->srcLR);
//        pyrDown(img->raw, img->srcLR);
//        blur(img->srcLR,img->blur,Size(5,5));
        produceBinaries(img, guiParameters);
        cvtColor(img->raw, img->gray, CV_BGR2GRAY);
        pyrDown(img->gray, img->grayLR);
        imshow("frame", img->raw);
        blur(img->bw,img->bw,Size(guiParameters->blur+1,guiParameters->blur+1));
        imshow("BW", img->bw);




        Mat M = getStructuringElement(MORPH_RECT, Size(guiParameters->erode+1, guiParameters->erode+1));
        erode(img->bw, img->bw, M);
        int cannyRatio = 3;
        Canny(img->bw, img->canny, guiParameters->cannyThreshold,guiParameters->cannyThreshold*cannyRatio,3);

//        HoughLinesP(img->canny, lines, 1, CV_PI/180, guiParameters->houghThreshold+1, guiParameters->houghMinLength, guiParameters->houghMaxGap );

        img->hough = Mat(1000,360, CV_8UC1, Scalar(0));
        img->bw.copyTo(img->hough);
        img->hough = Scalar(0);

        HoughLines(img->canny, lines2, 1, CV_PI/180, guiParameters->houghThreshold );
        cvtColor(img->canny,img->canny, CV_GRAY2BGR);
        for( size_t i = 0; i < lines2.size(); i++ )
        {
          float rho = lines2[i][0], theta = lines2[i][1];
          Point pt1, pt2;
          double a = cos(theta), b = sin(theta);
          double x0 = a*rho, y0 = b*rho;
          pt1.x = cvRound(x0 + 1000*(-b));
          pt1.y = cvRound(y0 + 1000*(a));
          pt2.x = cvRound(x0 - 1000*(-b));
          pt2.y = cvRound(y0 - 1000*(a));
          line( img->canny, pt1, pt2, Scalar(0,0,255), 3, CV_AA);
//          cout << "theta:" << theta/CV_PI*180 << endl;
//          cout << "rho: " << rho << endl;
          img->hough.at<Scalar>(abs(int(rho)),int(theta/CV_PI*180)) = 255;
//          cout <<"here "<< img->hough.at<Scalar>(abs(int(rho)),int(theta/CV_PI*180))<<endl;
//          cout << i << endl;
        }

 cout << "theta000:  " << endl;

        vLines = detectVerticalHoughLine(lines2);
         cout << "theta001:  " << vLines.size() << endl;
        for( size_t i = 0; i < vLines.size(); i++ ){
            cout << "almsost theta" << endl;
            cout << "theta:  " << vLines.size() << endl;
            char c = cvWaitKey(330);
            float rho = vLines[i][0], theta = vLines[i][1];
            Point pt1, pt2;
            double a = cos(theta), b = sin(theta);
            double x0 = a*rho, y0 = b*rho;
            pt1.x = cvRound(x0 + 1000*(-b));
            pt1.y = cvRound(y0 + 1000*(a));
            pt2.x = cvRound(x0 - 1000*(-b));
            pt2.y = cvRound(y0 - 1000*(a));
//        line( img->hough, pt1, pt2, Scalar(0,0,255), 3, CV_AA);
        }
//        Mat temp;
//        img->hough.copyTo(temp);
//        for( size_t i = 0; i < lines.size(); i++ )
//        {
//          Vec4i l = lines[i];
//          line( img->canny, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(255,0,0), 3, CV_AA);
//          circle(temp, Point(l[0], l[1]), 30, Scalar(10,0,0), -1);
//          img->hough += temp;
//          circle(temp, Point(l[2], l[3]), 30, Scalar(10,0,0), -1);
//          img->hough += temp;

//        }

        imshow("houghLines", img->hough);
        imshow("canny", img->canny);
        char c = cvWaitKey(33);
//        c = cvWaitKey(99999999);
        if (c == 27) break;
    }
    return 0;
}



vector<Vec2f> detectVerticalHoughLine(vector<Vec2f> lines){
     cout << "theta2:  " << lines.size() << endl;
    int ratio = 15;
    vector<Vec2f> verticalLines;
    for(size_t i = 0; i < lines.size(); i++) {
//        if(abs(lines[i][1]) == 0.01 ){

//            verticalLines.push_back(lines[i]);
//            cout << "porke " << endl;
//        }
    }
     cout << "theta:  " << verticalLines.size() << endl;
   return verticalLines;
}





