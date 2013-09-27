#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "image.h"
#include <string>
#include "stdio.h"
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

RNG rng(12345);
int main(){
    cout << "hea"<<endl;
    string sourceReference = "video.mp4";
    Image* img = new Image(sourceReference);
    Image* prevImg = new Image;
    namedWindow("frame", CV_WINDOW_KEEPRATIO);
    namedWindow("flow", CV_WINDOW_KEEPRATIO);
    int framenum = 0;
    int thresh = 100;
    img->cap >> img->raw;


    while(true){
        img->prevRaw = img->raw;
        img->cap >> img->raw;
        if (img->raw.empty()){
            break;
        }

        calcOpticalFlowSF(img->prevRaw, img->raw, img->flow,3,2,4);


        cvtColor(img->raw, img->gray, CV_BGR2GRAY);

        vector<vector<Point> > contours;
        vector<Vec4i> hierarchy;
        Canny( img->gray, img->canny, thresh, thresh*2, 3 );

        imshow("frame", img->raw);
        imshow("frame", img->flow);
        char c = cvWaitKey(33);
        if (c == 27) break;

    }
    return 0;
}
