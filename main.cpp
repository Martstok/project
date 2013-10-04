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

//================================MAIN================================

int main(){
    string sourceReference = "Windmill5.jpg";
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

    while(true){
        if (sourceType == "video"){
            img->prevRaw = img->raw;
            img->cap >> img->raw;
        }
        if (img->raw.empty()){
            break;
        }

        surf(img);
        surf(img2);

        // matching descriptors

        FlannBasedMatcher matcher;
        vector< DMatch > matches;
        matcher.match( img->descriptor, img2->descriptor, matches );

//        BFMatcher matcher(NORM_L2);
//        vector<DMatch> matches;
//        matcher.match(img->descriptor, img2->descriptor, matches);

        // drawing the results
        namedWindow("matches", 1);
        Mat img_matches;
        int flags = 4;
        vector<char> empty;
        drawMatches(img->raw, img->keypoints, img2->raw, img2->keypoints, matches, img_matches, Scalar::all(-1),Scalar::all(-1),vector<char>(),DrawMatchesFlags::DEFAULT);
//        drawMatches(roi, keypoints_1, src, keypoints_2, good_matches, img_matches,Scalar::all(-1), Scalar::all(-1), vector<char>(),DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
//        cout << "length" << matches.toList(0) << endl;
//        for(int i = 0; i < size(matches), i++){
//            line(img_matches,img1.raw,Point2f(img2[i].x+w,img2[i].y),Scalar(255,255,255),5);
//            line(image2,Point2f(img1[i].x-w,img1[i].y),img2[i],Scalar(255,255,255),5);
//        }
        imshow("matches", img_matches);

        pyrDown(img->raw, img->srcLR);
        blur(img->srcLR,img->blur,Size(5,5));
        produceBinaries(img, guiParameters);
        cvtColor(img->raw, img->gray, CV_BGR2GRAY);
        imshow("frame", img->raw);
//        imshow("BW", img->keypoints);
        char c = cvWaitKey(33);
        c = cvWaitKey(99999999);
        if (c == 27) break;
    }
    return 0;
}




void surf(Image* img){
    // detecting keypoints
    SurfFeatureDetector detector(1500);
    vector<KeyPoint> keypoints1, keypoints2;
    cout << "heyheyhey" << endl;
    detector.detect(img->raw, img->keypoints);
//    detector.detect(img2, keypoints2);

    // computing descriptors
    SurfDescriptorExtractor extractor;
    Mat descriptors1, descriptors2;
    extractor.compute(img->raw, img->keypoints, img->descriptor);
//    extractor.compute(img2, keypoints2, descriptors2);

//    // matching descriptors
//    BruteForceMatcher<L2<float> > matcher;
//    vector<DMatch> matches;
//    matcher.match(descriptors1, descriptors2, matches);

//    // drawing the results
//    namedWindow("matches", 1);
//    Mat img_matches;
//    drawMatches(img1, keypoints1, img2, keypoints2, matches, img_matches);
//    imshow("matches", img_matches);
//    waitKey(0);
}














