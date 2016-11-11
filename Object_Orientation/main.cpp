


#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <math.h>

using namespace cv;
using namespace std;
Point  COG;//COG
Mat drawing ;
vector<Vec4i> hierarchy;
vector<Point> origins;
void PrintDegree(vector<vector<Point> > contours );
void pointTheCenter(Point  COG);
Mat erode_Dilate(Mat img);
void DrawRect(int indexOfBiggestArea, vector<vector<Point> > contours);
void FindBiggestArea(int* indexOfBiggestArea , vector<vector<Point> > contours);
void Circle_Color(int indexOfBiggestArea , vector<vector<Point> > contours, vector<float>radius, vector<Point2f>center, vector<vector<Point> > contours_poly, Scalar color);
/** @function main */
int main(int argc, char** argv) {
  VideoCapture cap("MeasuringAngle.mp4"); // open the default camera
  if (!cap.isOpened()) // check if we succeeded
    return -1;

  for (;;) {
    vector<Point> origins;
    Mat frame;
    cap >> frame; // get a new frame from camera
    Mat imgHSV;
    cvtColor(frame, imgHSV, CV_BGR2HSV);

    Mat imgThresh, imgthresh_B;
    //orange (11, 247, 171)
    inRange(imgHSV, Scalar(8, 235, 160), Scalar(25, 260, 185), imgThresh);
    inRange(imgHSV, Scalar(110, 60, 60), Scalar(130, 255, 255), imgthresh_B);

    imgThresh = erode_Dilate(imgThresh);
    vector<vector<Point> > contours, contoursB;
    findContours(imgThresh, contours, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
    findContours(imgthresh_B, contoursB, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);


    // Approximate contours to polygons + get bounding rects and circles
    vector<vector<Point> > contours_poly(contours.size()), contours_polyB(contoursB.size());
    // vector<Rect> boundRect(contours.size());
    vector<Point2f>centerb(contoursB.size());
    vector<float>radiusb(contoursB.size());


    drawing = Mat::zeros(frame.size(), CV_8UC3);
    frame.copyTo(drawing);
    int maxi = -1;
    FindBiggestArea(&maxi , contours);
    if (maxi != -1) {
      origins.push_back(COG);

      DrawRect( maxi , contours );

    }
    maxi = -1;
    FindBiggestArea(&maxi , contoursB);
    if (maxi != -1) {
      origins.push_back(COG);
      Circle_Color(maxi , contoursB, radiusb, centerb, contours_polyB, Scalar(0, 0, 255));
    }

    cout << origins.size() << endl;
    if (origins.size() > 1) {
      cv::line(drawing, origins.at(0), origins.at(1), cv::Scalar(0, 255, 255));
    }
    imshow("Output", drawing);

    if (waitKey(30) >= 0) break;
  } imshow("Output", drawing);
}




Mat erode_Dilate(Mat img) {

  Mat erodeElement = getStructuringElement(MORPH_RECT, Size(3, 3));
  Mat dilateElement = getStructuringElement(MORPH_RECT, Size(8, 8));
  erode(img, img, erodeElement);
  dilate(img, img, dilateElement);
  return img;


}


void FindBiggestArea(int* indexOfBiggestArea , vector<vector<Point> > contours) {

  Moments moment;
  double biggestAreay = 0;
  if (contours.size() > 0) {
    for (int i = 0; i < contours.size(); ++i) {
      /* code */
      moment = moments((cv::Mat)contours[i]);
      if (moment.m00 > biggestAreay) {
        biggestAreay = moment.m00;

        *indexOfBiggestArea = i;
        COG = Point (moment.m10 / moment.m00, moment.m01 / moment.m00);//COG
      }


    }
  }

}

void Circle_Color(int indexOfBiggestArea , vector<vector<Point> > contours, vector<float>radius, vector<Point2f>center,
                  vector<vector<Point> > contours_poly, Scalar color) {
  pointTheCenter(COG);
  approxPolyDP(Mat(contours[indexOfBiggestArea]), contours_poly[indexOfBiggestArea], 3, true);
  //  boundRect[indexOfBiggestArea] = boundingRect(Mat(contours_poly[indexOfBiggestArea]));
  minEnclosingCircle((Mat) contours_poly[indexOfBiggestArea], center[indexOfBiggestArea], radius[indexOfBiggestArea]);
  //  drawContours(drawing, contours, indexOfBiggestArea, Scalar(0, 0, 255), 2, 8, hierarchy, 0, Point());
  circle(drawing, center[indexOfBiggestArea], (int) radius[indexOfBiggestArea], color, 2, 8, 0);

}

void DrawRect(int indexOfBiggestArea , vector<vector<Point> > contours ) {

  vector<Point2f> vertVect;
  RotatedRect calculatedRect;

  cv::RotatedRect boundingBox = cv::minAreaRect(contours[indexOfBiggestArea]);
  // draw the rotated rect
  cv::Point2f corners[4];
  boundingBox.points(corners);
  cv::line(drawing, corners[0], corners[1], cv::Scalar(255, 255, 255));
  cv::line(drawing, corners[1], corners[2], cv::Scalar(255, 255, 255));
  cv::line(drawing, corners[2], corners[3], cv::Scalar(255, 255, 255));
  cv::line(drawing, corners[3], corners[0], cv::Scalar(255, 255, 255));

  float blob_angle_deg = boundingBox.angle;
  if (boundingBox.size.width < boundingBox.size.height) {
    blob_angle_deg = 90 + blob_angle_deg;
  }
  Mat mapMatrix = getRotationMatrix2D(COG, blob_angle_deg, 1.0);
  for (int i = 0; i < 4; i++) {
    vertVect.push_back(corners[i] + Point2f(200, 0));
  }
  calculatedRect = minAreaRect(vertVect);
  stringstream ss (stringstream::in | stringstream::out);
  if (calculatedRect.angle < -1) ss << calculatedRect.angle*(-1) + 90;
  else
    ss << calculatedRect.angle*(-1);

  string deg = ss.str() + " degrees";

  putText(drawing, deg, COG, 1, 1.2, Scalar(0, 255, 255), 2, 8, false);

}

void pointTheCenter(Point COG) {

  putText(drawing, "X", COG, 1, 1.2, Scalar(0, 255, 255), 2, 8, false);

}




