#include <cv.h>
#include <highgui.h>
#include <stdio.h>
 
int main ( int argc, char **argv )
{
 CvCapture* capture = 0;
 IplImage* img = 0;
 
 capture = cvCaptureFromCAM( 0 );
 if ( !capture )            // Check for Camera capture
  return -1;
 
 cvNamedWindow("Camera",CV_WINDOW_AUTOSIZE);
 cvNamedWindow("Threshold",CV_WINDOW_AUTOSIZE);
 
 IplImage* gray = 0;
 IplImage* thres = 0;
  
 bool init = false;   // Flag to identify initialization of Image objects
 
     if(init == false)
 {
  img = cvQueryFrame( capture ); // Query for the frame
 
         if( !img )   // Exit if camera frame is not obtained
   return -1;
 
  // Creation of Intermediate 'Image' Objects required later
  gray = cvCreateImage( cvGetSize(img), 8, 1 );  // To hold Grayscale Image
  thres = cvCreateImage( cvGetSize(img), 8, 1 );  // To hold OTSU thresholded Image
 
  init = true;
 }
 
    
 int ihist[256];   // Array to store Histogram values
 float hist_val[256];  // Array to store Normalised Histogram values
 int pos ;   // Position or pixel value of the image
 float prbn;   // First order cumulative
 float meanitr;   // Second order cumulative
 float meanglb;   // Global mean level
 int OPT_THRESH_VAL;             // Optimum threshold value
 float param1,param2;  // Parameters required to work out OTSU threshold algorithm
 double param3;
 int h,w;   // Variables to store Image Height and Width
  
 h = img->height;  // Height and width of the Image
 w = img->width;
 
 int key = 0;
 while(key != 'q')
 {
 
  //Step : Capture Image from Camera
  //Info : Inbuit function from OpenCV
  //Note : 
 
  img = cvQueryFrame( capture ); // Query for the frame
 
  //Step : Convert Image captured from Camera to GrayScale
  //Info : Inbuit function from OpenCV
  //Note : Image from Camera and Grayscale are held using seperate "IplImage" objects
 
  cvCvtColor(img,gray,CV_RGB2GRAY); // Convert RGB image to Gray
 
 
  //Step : Threshold the image using optimum Threshold value obtained from OTSU method
  //Info : 
  //Note : 
 
  memset(ihist, 0, 256);
 
  for(int j = 0; j < gray->height; ++j) // Use Histogram values from Gray image
  {
   uchar* hist = (uchar*) (gray->imageData + j * gray->widthStep);
   for(int i = 0; i < gray->width; i++ )
   {
    pos = hist[i];  // Check the pixel value
    ihist[pos] += 1; // Use the pixel value as the position/"Weight"
   }
  }
 
  //Parameters required to calculate threshold using OTSU Method
  prbn = 0.0;                   // First order cumulative
  meanitr = 0.0;                // Second order cumulative
  meanglb = 0.0;                // Global mean level
  OPT_THRESH_VAL = 0;           // Optimum threshold value
  param1,param2;                // Parameters required to work out OTSU threshold algorithm
  param3 = 0.0;
 
  //Normalise histogram values and calculate global mean level
  for(int i = 0; i < 256; ++i)
  {
   hist_val[i] = ihist[i] / (float)(w * h);
   meanglb += ((float)i * hist_val[i]);
  }
 
  // Implementation of OTSU algorithm
  for (int i = 0; i < 255; i++)
  {
   prbn += (float)hist_val[i];
   meanitr += ((float)i * hist_val[i]);
 
   param1 = (float)((meanglb * prbn) - meanitr);
   param2 = (float)(param1 * param1) /(float) ( prbn * (1.0f - prbn) );
 
   if (param2 > param3)
   {
       param3 = param2;
       OPT_THRESH_VAL = i;     // Update the "Weight/Value" as Optimum Threshold value
   }
  }
 
  cvThreshold(gray,thres,OPT_THRESH_VAL,255,CV_THRESH_BINARY); // Threshold the Image using the value obtained from OTSU method
 
  cvShowImage( "Camera",img);
  cvShowImage( "Threshold",thres);
 
  key = cvWaitKey(1); // OPENCV: wait for 1ms before accessing next frame
 
 } // End of while loop
 
 cvDestroyWindow( "Camera" ); // Release various parameters
 cvReleaseImage(&img);
 cvReleaseImage(&gray);
 cvReleaseImage(&thres);
     return 0;
}
// End of Main Loop
