
#include <opencv2/core/core.hpp>
#include "colorThresholding.h"
#include "image.h"
#include "gui.h"
#include <iostream>
using namespace std;

using namespace cv;




// Using HSV

void produceBinariesHSV(Image* img, GuiParameters* guiParameters){
    img->bwList.clear();    //Caution: Leaving this one out causes major memory leak!

    for(int i=0;i<NSAMPLES;i++){
        Scalar lowerBound;
        Scalar upperBound;
//        normalizeColors(img);
        lowerBound=Scalar( guiParameters->c_lower[i][0] , guiParameters->c_lower[i][1], guiParameters->c_lower[i][2] );
        upperBound=Scalar( guiParameters->c_upper[i][0] , guiParameters->c_upper[i][1], guiParameters->c_upper[i][2] );
//        lowerBound=Scalar( guiParameters->range,guiParameters->range,guiParameters->range );
//        upperBound=Scalar( guiParameters->range+guiParameters->gap,guiParameters->range+guiParameters->gap,guiParameters->range+guiParameters->gap);
        img->bwList.push_back(Mat(img->rawLR.rows,img->rawLR.cols,CV_8U));
        inRange(img->hsv,lowerBound,upperBound,img->bwList[i]);
    }
    img->bwList[0].copyTo(img->bw);
//    for(int i=1;i<NSAMPLES;i++){
//        img->bw+=img->bwList[i];
//    }
}




//void normalizeColors(Image* img){
//    // copy all boundries read from trackbar
//    // to all of the different boundaries
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
