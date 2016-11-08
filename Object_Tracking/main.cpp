#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace cv;
using namespace std;
Point2f point;//COG
Mat drawing ;
 vector<Vec4i> hierarchy;
Mat erode_Dilate(Mat img);
void FindBiggestArea(int* indexOfBiggestArea ,vector<vector<Point> > contours);
void Circle_Color(int indexOfBiggestArea ,vector<vector<Point> > contours,vector<float>radius,vector<Point2f>center, vector<vector<Point> > contours_poly,Scalar color);
/** @function main */
int main(int argc, char** argv) {
    VideoCapture cap("MultipleColors.mp4"); // open the default camera
    if (!cap.isOpened()) // check if we succeeded
        return -1;

    for (;;) {
        Mat frame;
        cap >> frame; // get a new frame from camera
        Mat imgHSV;
        cvtColor(frame, imgHSV, CV_BGR2HSV);

        Mat imgThresh, imgthresh_B,imgthresh_Y;
        inRange(imgHSV, Scalar(170, 150, 60), Scalar(179, 255, 255), imgThresh);
        inRange(imgHSV, Scalar(110, 60, 60), Scalar(130, 255, 255), imgthresh_B);
        inRange(imgHSV, Scalar(23, 41, 100), Scalar(60, 255, 255), imgthresh_Y);
       
       imgThresh= erode_Dilate(imgThresh);
        imgthresh_B= erode_Dilate(imgthresh_B);
         imgthresh_Y= erode_Dilate(imgthresh_Y);
      
        vector<vector<Point> > contours,contoursB,contoursY;
     
        findContours(imgThresh, contours, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
         findContours(imgthresh_Y, contoursY, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
          findContours(imgthresh_B, contoursB, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
        /// Approximate contours to polygons + get bounding rects and circles
        vector<vector<Point> > contours_poly(contours.size()), contours_polyB(contoursB.size()),contours_polyY(contoursY.size());
       // vector<Rect> boundRect(contours.size());
        vector<Point2f>center(contours.size());
        vector<float>radius(contours.size());
        vector<Point2f>centerb(contoursB.size());
        vector<float>radiusb(contoursB.size());
        vector<Point2f>centery(contoursY.size());
        vector<float>radiusy(contoursY.size());
        
         drawing = Mat::zeros(frame.size(), CV_8UC3);
        frame.copyTo(drawing);
        int maxi=-1;
         FindBiggestArea(&maxi , contours);
        if (maxi != -1) {

           Circle_Color(maxi , contours,radius,center,contours_poly,Scalar(0, 0, 255));
        }
         //center(contoursB.size());
         FindBiggestArea(&maxi , contoursB);
        if (maxi != -1) {

           Circle_Color(maxi , contoursB,radiusb,centerb,contours_polyB,Scalar(255, 255, 0));
        }
        
        FindBiggestArea(&maxi , contoursY);
         
         if (maxi != -1) {

          Circle_Color(maxi , contoursY,radiusy,centery,contours_poly,Scalar(0, 255, 255));
        }
        
        imshow("edges", drawing);
        if (waitKey(30) >= 0) break;
    }
}




Mat erode_Dilate(Mat img){

 Mat erodeElement = getStructuringElement(MORPH_RECT, Size(3, 3));
 Mat dilateElement = getStructuringElement(MORPH_RECT, Size(8, 8));
 erode(img, img, erodeElement);
 dilate(img, img, dilateElement);
 return img;


}


void FindBiggestArea(int* indexOfBiggestArea ,vector<vector<Point> > contours){

   Moments moment;    
        double biggestAreay = 0;
        for (int i = 0; i < contours.size(); ++i) {
            /* code */
            moment = moments((cv::Mat)contours[i]);
            if (moment.m00 > biggestAreay) {
                biggestAreay = moment.m00;
                point = Point2f(moment.m10 / moment.m01, moment.m01 / moment.m00);//COG
                *indexOfBiggestArea = i;
            }
        }
}

void Circle_Color(int indexOfBiggestArea ,vector<vector<Point> > contours,vector<float>radius,vector<Point2f>center, vector<vector<Point> > contours_poly, Scalar color){


            approxPolyDP(Mat(contours[indexOfBiggestArea]), contours_poly[indexOfBiggestArea], 3, true);
         //  boundRect[maxi] = boundingRect(Mat(contours_poly[maxi]));
           minEnclosingCircle((Mat) contours_poly[indexOfBiggestArea], center[indexOfBiggestArea], radius[indexOfBiggestArea]);
         //  drawContours(drawing, contours, indexOfBiggestArea, Scalar(0, 0, 255), 2, 8, hierarchy, 0, Point());
            circle(drawing, center[indexOfBiggestArea], (int) radius[indexOfBiggestArea], color, 2, 8, 0);
            //rectangle(drawing , boundRect[maxi].tl(), boundRect[maxi].br(), Scalar(0,0,255), 2, 8, 0 );

}


