#include <iostream>
using namespace std;
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
using namespace cv;

int main()
{
    IplImage *im = cvLoadImage("coin.png",CV_LOAD_IMAGE_ANYDEPTH);
    IplImage *hole = cvCreateImage(cvSize(im->width,im->height),8,1);
    cvShowImage("Original",im);

    cvCopyImage(im,hole);
    cvFloodFill(hole,cvPoint(0,0),cvScalar(255));
    cvShowImage("Hole",hole);
    cvSaveImage("hole.png",hole);

    cvNot(hole,hole);
    cvAdd(im,hole,im);
    cvShowImage("FillHole",im);
    cvSaveImage("fillHole.png",im);

    cvWaitKey(0);
    system("pause");
    return 0;
} 

