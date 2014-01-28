#include "houghTransform.h"
#include <opencv2/core/core.hpp>
#include "image.h"
#include "gui.h"
#include <iostream>

using namespace std;
using namespace cv;

HoughTransform::HoughTransform()
{
}

//Canny transform, then hough transform
void HoughTransform::generateLongHoughLines(Image* img, GuiParameters* guiParameters){
    img->rawLR.copyTo(img->canny);
    Canny(img->rawLR, img->canny, 50, 200, 3);
    HoughLines(img->canny, this->lines, 1, CV_PI/180, guiParameters->houghThreshold+1);
    cout << "done" << endl;

}


void HoughTransform::generateShortHoughLines(Image* img, GuiParameters* guiParameters){


}

void HoughTransform::drawLongHoughLines(Image* img, GuiParameters* guiParameters){
    img->rawLR.copyTo(img->hough);
    cvtColor(img->hough,img->hough, CV_BGR2GRAY);
    cvtColor(img->hough,img->hough, CV_GRAY2BGR);
    img->rawLR.copyTo(img->result);
    for( size_t i = 0; i < this->lines.size(); i++ )
    {
      float rho = this->lines[i][0], theta = this->lines[i][1];
      Point pt1, pt2;
      double a = cos(theta), b = sin(theta);
      double x0 = a*rho, y0 = b*rho;
      pt1.x = cvRound(x0 + 1000*(-b));
      pt1.y = cvRound(y0 + 1000*(a));
      pt2.x = cvRound(x0 - 1000*(-b));
      pt2.y = cvRound(y0 - 1000*(a));
      line( img->result, pt1, pt2, Scalar(0,0,255), 3, CV_AA);
      cout << abs(int(rho)) << ", " << this->lines[i][1]/CV_PI*180;
      int sgn_rho = (rho>0)-(rho<0);
      double deg = (-this->lines[i][1]+CV_PI/2);
      double angle = deg/CV_PI*180+90;
      cout << ", " << angle << endl;

    //          img->plot.at<Scalar>(abs(int(rho)), angle) = 255;
            circle( img->plot, Point(abs(rho),angle),   4, Scalar(255), 2 );
          //          img->hough.at<Scalar>(abs(int(rho)),int(theta/CV_PI*180)) = 255;           BUGGY
    }

}


void HoughTransform::drawShortHoughLines(Image* img, GuiParameters* guiParameters){
    vector<Vec4i> lines2;
    HoughLinesP(img->canny, lines2, 1, CV_PI/180, guiParameters->houghThreshold+1, guiParameters->houghMinLength, guiParameters->houghMaxGap );
    img->rawLR.copyTo(img->result2);
    for( size_t i = 0; i < lines2.size(); i++ )
    {
      Vec4i l = lines2[i];
      line( img->result2, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(255,0,0), 3, CV_AA);

    }
    cout << endl << "-------------------" << endl;

}
