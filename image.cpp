#include "image.h"

using namespace std;
using namespace cv;

Image::Image()
{
}


Image::Image(string sourceReference)
{
    cap = VideoCapture(sourceReference);
}


Image::Image(string sourceReference, string sourceType)
{
    if (sourceType=="video"){
        cap = VideoCapture(sourceReference);
    }
    else if (sourceType=="image"){
        raw = imread(sourceReference);
    }
}


Mat downsample(cv::Mat inputImage, int upperLimit){
    while(max(inputImage.cols, inputImage.rows) > upperLimit){
        pyrDown(inputImage,inputImage);
    }
    return inputImage;
}
