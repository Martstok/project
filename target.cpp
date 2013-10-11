#include "target.h"

using namespace std;
using namespace cv;

Target::Target()
{
}

void Target::initContourVectors(){
    hullI=vector<vector<int> >(contours.size());
    hullP=vector<vector<Point> >(contours.size());
    defects=vector<vector<Vec4i> > (contours.size());
}
