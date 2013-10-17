#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "image.h"
#include <string>
#include "stdio.h"
#include <opencv2/imgproc/imgproc.hpp>
#include "gui.h"
#include "colorThresholding.h"
#include "target.h"
#include "math.h"
#define PI 3.14159265

#include <sstream>

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
    string sourceReference = "Windmill7.jpg";
    string sourceType = "image";
    string sourceReference2 = "Windmill.jpg";
    string sourceType2 = "image";
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
        M = getStructuringElement(MORPH_RECT, Size(guiParameters->erode+1, guiParameters->erode+1));
        dilate(img->bw, img->bw, M);
        int cannyRatio = 3;
        Canny(img->bw, img->canny, guiParameters->cannyThreshold,guiParameters->cannyThreshold*cannyRatio,3);



        cvtColor(img->canny, img->canny, CV_GRAY2BGR);


        //Contours begin here
        generateContours(img, target);
        int numOfContoursToFind = 1;
        int currentIndex;
        target->indicesOfBiggestContours = findBiggestContours(target->contours, numOfContoursToFind);
        for(int i = 0; i < target->indicesOfBiggestContours.size(); i++){
             currentIndex = target->indicesOfBiggestContours[i];
//            drawContours(img->canny, target->contours, currentIndex, cv::Scalar(255), 5, 8, vector<Vec4i>(),0, Point());
            approxPolyDP(Mat(target->contours[currentIndex]), target->contours[currentIndex],11,true);
//            drawContours(img->canny, target->contours, currentIndex, cv::Scalar(0,0,255), 3, 8, vector<Vec4i>(),0, Point());

            convexHull(Mat(target->contours[currentIndex]), target->hullP[currentIndex], false, true);
            convexHull(Mat(target->contours[currentIndex]), target->hullI[currentIndex], false, false);
            if(target->hullI[currentIndex].size() > 3){
                convexityDefects(target->contours[currentIndex],target->hullI[currentIndex],target->defects[currentIndex]);

                drawContours(img->canny, target->hullP, currentIndex,Scalar(200,0,0),2, 8, vector<Vec4i>(), 0, Point());
                approxPolyDP(Mat(target->hullP[currentIndex]), target->hullP[currentIndex], 10,true);
//                drawContours(img->canny, target->defects[currentIndex], Scalar(0,0,255), 2, 8, vector<Vec4i>(), 0, Point());


                vector<Vec4i>::iterator d = target->defects[currentIndex].begin();

                for(d = target->defects[currentIndex].begin(); d!= target->defects[currentIndex].end(); d++){

                    Vec4i v = (*d);
                    int startIndex=v[0];
                    int endIndex=v[1];
                    int farIndex=v[2];
                    Point ptStart(target->contours[currentIndex][startIndex] );
                    Point ptEnd(target->contours[currentIndex][endIndex] );
                    Point ptFar(target->contours[currentIndex][farIndex] );
                    float depth = v[3] / 256;
                    line( img->canny, ptStart, ptFar, Scalar(0,0,255), 1 );
                    line( img->canny, ptEnd, ptFar, Scalar(0,255,0), 1 );
                    circle( img->canny, ptFar,   4, Scalar(0,255,0), 2 );
                    circle( img->canny, ptEnd,   4, Scalar(0,0,255), 2 );
                    circle( img->canny, ptStart,   10, Scalar(255,0,0), 2 );

//                    double angle = atan((ptStart.y - ptEnd.y)/(ptEnd.x - ptStart.x))*180/PI;
//                    double angle = atan2(ptFar.y - ptStart.y,ptStart.x - ptFar.x)*180/PI;
//                      double angle = atan2(ptFar.y - ptEnd.y,ptEnd.x - ptFar.x)*180/PI;
                      double angle1;
                      double angle2;
//                    if(angle < 0){
//                        angle += 360;
//                    }



                    int averagedAngle;

                    Vec4i vPrev = v;
                    if(d != target->defects[currentIndex].begin()){
                        vPrev = *(d-1);

                    }
                    else{
                        vPrev=(*(target->defects[currentIndex].end()-1));
                    }
                    Point ptStartPrev(target->contours[currentIndex][vPrev[0]]);
                    Point ptEndPrev(target->contours[currentIndex][vPrev[1]]);
                    Point ptFarPrev(target->contours[currentIndex][vPrev[2]]);
                    angle1 = atan2(ptFar.y - ptStart.y,ptStart.x - ptFar.x)*180/PI;


                    angle2 = atan2(ptFarPrev.y - ptEndPrev.y,ptEndPrev.x - ptFarPrev.x)*180/PI;
                    if(angle1 < 0){
                        angle1 += 360;
                    }
                    if(angle2 < 0){
                        angle2 += 360;
                    }
                    averagedAngle = (angle1+angle2)/2;
                    averagedAngle = (angle1+angle2)/2;

//                    if(d == target->defects[currentIndex].begin()+1){
                        cout << "ptStart: " << ptStart << endl;
                        cout << "ptEndPrev: " << ptEndPrev << endl;
                        cout << "angle1: " << angle1 << endl;
                        cout << "angle2: " << angle2 << endl;
                        cout << "avgAngle: " << averagedAngle << endl << endl << endl;
//                    }


                    stringstream ss;
                    ss << averagedAngle;
                    putText(img->canny, ss.str(), Point(ptStart.x, ptStart.y), FONT_HERSHEY_PLAIN, 3, Scalar(255), 1,8,false);
                }


            }


        }





        imshow("BW", img->bw);
        imshow("canny", img->canny);
        char c = cvWaitKey(33);
//        c = cvWaitKey(99999999);
        if (c == 27) break;

    }
    return 0;
}


vector<int> findBiggestContours(vector<vector<Point> > contours, int numOfContoursToFind){
    int size = min(numOfContoursToFind,(int)contours.size());
    vector<int> indicesOfBiggestContours;
    for (int j = 0; j < size ; j++){
        int sizeOfBiggestContour = 0;
        int indexOfBiggestContour = 0;
        for (int i = 0; i < contours.size(); i++){
            bool alreadyFound = false;
            for(int k = 0; k < size; k++){
                if(!indicesOfBiggestContours.empty() && indicesOfBiggestContours[k] == i)
                    alreadyFound = true;
            }
            if (alreadyFound == true){
                alreadyFound == false;
                continue;
            }
            if(contours[i].size() > sizeOfBiggestContour){
              sizeOfBiggestContour = contours[i].size();
              indexOfBiggestContour = i;
            }
        }
        indicesOfBiggestContours.push_back(indexOfBiggestContour);
    }

    return indicesOfBiggestContours;
}



void generateContours(Image* img, Target* target){
    Mat aBw;
//	Mat mask = Mat(m->src.size(), CV_8U);
//	pyrUp(m->bw,m->bw);
    img->bw.copyTo(aBw);
    findContours(aBw,target->contours,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);
    target->initContourVectors();
//    target->cIdx=findBiggestContours(target->contours, amount);
//        for(target->idxNr = 0; target->idxNr < amount; target->idxNr++){
//              if(target->cIdx[target->idxNr]!=-1){
//                  approxPolyDP( Mat(target->contours[target->cIdx[target->idxNr]]), target->contours[target->cIdx[target->idxNr]], 11, true );
//                  target->bRect=boundingRect(Mat(target->contours[target->cIdx[target->idxNr]]));
//                  convexHull(Mat(target->contours[target->cIdx[target->idxNr]]),target->hullP[target->cIdx[target->idxNr]],false,true);
//                  convexHull(Mat(target->contours[target->cIdx[target->idxNr]]),target->hullI[target->cIdx[target->idxNr]],false,false);
//                  approxPolyDP( Mat(target->hullP[target->cIdx[target->idxNr]]), target->hullP[target->cIdx[target->idxNr]], 18, true );
//                  if(target->contours[target->cIdx[target->idxNr]].size()>3 ){
//                      convexityDefects(target->contours[target->cIdx[target->idxNr]],target->hullI[target->cIdx[target->idxNr]],target->defects[target->cIdx[target->idxNr]]);
//                      target->eleminateDefects(m);
//                  }
//                  double cArea = contourArea(target->contours[target->cIdx[target->idxNr]]);
//                  double cLength = arcLength(target->contours[target->cIdx[target->idxNr]],true);
//    // 		      cout << "area: "<< cArea << endl;
//    // 		      cout << "length: "<< cLength << endl;
//    // 		      cout << "A/L: "<< cArea/cLength << endl;
//    // 		      drawContours(m->src, target->contours, target->cIdx[target->idxNr], cv::Scalar(255), CV_FILLED, 8, vector<Vec4i>(),0, Point());



//                  //Std deviation
//                  mask = Scalar(0);
//                  drawContours(mask, target->contours, target->cIdx[target->idxNr], cv::Scalar(255), CV_FILLED, 8, vector<Vec4i>(),0, Point());
//                  Mat mean, stddev;
//                  meanStdDev(m->src, mean, stddev, mask);
//    // 		      cout << stddev << endl;
//    // 		      cout << stddev.at<double>(1,1) << endl;
//                  if(stddev.at<double>(1,1) > stddevLimit){
//                continue;
//                  }
//                  if( cArea/cLength > alLimit){
//                continue;
//                  }
//                  if( cArea < 5000){
//                continue;
//                  }
//                  if (faceDetect(m, mask, target)){
//                continue;
//                  }

//                  bool isHand=target->detectIfHand();
//                  target->printGestureInfo(m->src);
//                  if(isHand){
//                      target->getFingerTips(m);
//                      target->drawFingerTips(m);
//                      myDrawContours(m,target);
//                  }

//              }
//        }
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
