#include "target.h"
#include "image.h"
#include <iostream>

using namespace std;
using namespace cv;

#define PI 3.14159265

Target::Target()
{
}

void Target::initContourVectors(){
    hullI=vector<vector<int> >(contours.size());
    hullP=vector<vector<Point> >(contours.size());
    defects=vector<vector<Vec4i> > (contours.size());
}

void Target::generateContours(Image* img){
    Mat aBw;
//	Mat mask = Mat(m->src.size(), CV_8U);
//	pyrUp(m->bw,m->bw);
    img->bw.copyTo(aBw);
    findContours(aBw,this->contours,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);
    this->initContourVectors();
//    cIdx=findBiggestContours(contours, amount);
//        for(idxNr = 0; idxNr < amount; idxNr++){
//              if(cIdx[idxNr]!=-1){
//                  approxPolyDP( Mat(contours[cIdx[idxNr]]), contours[cIdx[idxNr]], 11, true );
//                  bRect=boundingRect(Mat(contours[cIdx[idxNr]]));
//                  convexHull(Mat(contours[cIdx[idxNr]]),hullP[cIdx[idxNr]],false,true);
//                  convexHull(Mat(contours[cIdx[idxNr]]),hullI[cIdx[idxNr]],false,false);
//                  approxPolyDP( Mat(hullP[cIdx[idxNr]]), hullP[cIdx[idxNr]], 18, true );
//                  if(contours[cIdx[idxNr]].size()>3 ){
//                      convexityDefects(contours[cIdx[idxNr]],hullI[cIdx[idxNr]],defects[cIdx[idxNr]]);
//                      eleminateDefects(m);
//                  }
//                  double cArea = contourArea(contours[cIdx[idxNr]]);
//                  double cLength = arcLength(contours[cIdx[idxNr]],true);
//    // 		      cout << "area: "<< cArea << endl;
//    // 		      cout << "length: "<< cLength << endl;
//    // 		      cout << "A/L: "<< cArea/cLength << endl;
//    // 		      drawContours(m->src, contours, cIdx[idxNr], cv::Scalar(255), CV_FILLED, 8, vector<Vec4i>(),0, Point());



//                  //Std deviation
//                  mask = Scalar(0);
//                  drawContours(mask, contours, cIdx[idxNr], cv::Scalar(255), CV_FILLED, 8, vector<Vec4i>(),0, Point());
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

//                  bool isHand=detectIfHand();
//                  printGestureInfo(m->src);
//                  if(isHand){
//                      getFingerTips(m);
//                      drawFingerTips(m);
//                      myDrawContours(m,target);
//                  }

//              }
//        }
}

void Target::findBiggestContours(int numOfContoursToFind){
    indicesOfBiggestContours.clear();
    int size = min(numOfContoursToFind,(int)contours.size());
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
}

void Target::getResults(Image* img){
    int currentIndex;
    for(int i = 0; i < indicesOfBiggestContours.size(); i++){
         currentIndex = indicesOfBiggestContours[i];
//            drawContours(img->canny, contours, currentIndex, cv::Scalar(255), 5, 8, vector<Vec4i>(),0, Point());
        approxPolyDP(Mat(contours[currentIndex]), contours[currentIndex],11,true);
//            drawContours(img->canny, contours, currentIndex, cv::Scalar(0,0,255), 3, 8, vector<Vec4i>(),0, Point());

        convexHull(Mat(contours[currentIndex]), hullP[currentIndex], false, true);
        convexHull(Mat(contours[currentIndex]), hullI[currentIndex], false, false);
        if(hullI[currentIndex].size() > 3){
            convexityDefects(contours[currentIndex],hullI[currentIndex],defects[currentIndex]);
            cout << "index: " << currentIndex << endl;
            drawContours(img->canny, hullP, currentIndex,Scalar(200,0,0),2, 8, vector<Vec4i>(), 0, Point());
            approxPolyDP(Mat(hullP[currentIndex]), hullP[currentIndex], 10,true);
//                drawContours(img->canny, defects[currentIndex], Scalar(0,0,255), 2, 8, vector<Vec4i>(), 0, Point());


            this->analyzeGeometry(img, currentIndex);








        }


    }
}

void Target::analyzeGeometry(Image* img, int currentIndex){
    vector<Vec4i>::iterator d = defects[currentIndex].begin();
    for(d = defects[currentIndex].begin(); d!= defects[currentIndex].end(); d++){

        Vec4i v = (*d);
        int startIndex=v[0];
        int endIndex=v[1];
        int farIndex=v[2];
        Point ptStart(contours[currentIndex][startIndex] );
        Point ptEnd(contours[currentIndex][endIndex] );
        Point ptFar(contours[currentIndex][farIndex] );
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
        if(d != defects[currentIndex].begin()){
            vPrev = *(d-1);

        }
        else{
            vPrev=(*(defects[currentIndex].end()-1));
        }
        Point ptStartPrev(contours[currentIndex][vPrev[0]]);
        Point ptEndPrev(contours[currentIndex][vPrev[1]]);
        Point ptFarPrev(contours[currentIndex][vPrev[2]]);
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

////                    if(d == defects[currentIndex].begin()+1){
//                        cout << "ptStart: " << ptStart << endl;
//                        cout << "ptEndPrev: " << ptEndPrev << endl;
//                        cout << "angle1: " << angle1 << endl;
//                        cout << "angle2: " << angle2 << endl;
//                        cout << "avgAngle: " << averagedAngle << endl << endl << endl;
////                    }


        stringstream ss;
        ss << averagedAngle;
        putText(img->canny, ss.str(), Point(ptStart.x, ptStart.y), FONT_HERSHEY_PLAIN, 3, Scalar(255), 1,8,false);
    }

    d = defects[currentIndex].begin();
    Point ptFarSum;
    for(d = defects[currentIndex].begin(); d!= defects[currentIndex].end(); d++){
         Vec4i v = (*d);
         int farIndex=v[2];
         Point ptFar(contours[currentIndex][farIndex] );
         ptFarSum += ptFar;
    }
    int num = defects[currentIndex].size();
    Point averagePtFar = Point(ptFarSum.x/num, ptFarSum.y/num);
    cout << "pt: " << averagePtFar.x << endl;
    circle(img->canny, averagePtFar, 20, Scalar(155,155,0), 2);

}
