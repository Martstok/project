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
