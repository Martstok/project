#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;


int main(){
    string sourceReference = "video.mp4";
    VideoCapture captRefrnc(sourceReference);
    Mat frameReference;
    namedWindow("frame", CV_WINDOW_KEEPRATIO);
    int framenum = 0;

    while(true){
        captRefrnc >> frameReference;
        if (frameReference.empty()){
            break;
        }
        cout << "Frame number: " << framenum << endl;
        framenum++;
        imshow("frame", frameReference);
        char c = cvWaitKey(33);
        if (c == 27) break;

    }
    return 0;
}
