#include <opencv2/opencv.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <cmath>
using namespace std;
using namespace cv;


#define PI    3.14159
Mat HighLightedImageT;
Mat src, src_gray;
Mat  img;
Mat dst,canny,edges;
Mat HighLight(Mat src, Mat dst);




int main(int argc, char const *argv[])
{
	
// 0 means Load image in grayscale
  img=imread("Church Canny Edge.png",0);

  cv::GaussianBlur(img,dst,Size(3,3),1);
  imshow("GaussianBlur 3 ",dst);
  cv::Canny(dst,canny,10,100,3,true);
  imshow("canny1",canny);
  cv::GaussianBlur(img,dst,Size(5,5),1);
  imshow("GaussianBlur5",dst);
  cv::Canny(dst,canny,10,100,5,true);
  imshow("canny2",canny);
  cv::GaussianBlur(img,dst,Size(7,7),1);
  imshow("GaussianBlur7",dst);
  cv::Canny(dst,canny,10,100,7,true);
  imshow("canny3",canny);

  Canny(img, edges, 100, 200, 3, true);
  cvtColor(edges,HighLightedImageT, CV_GRAY2BGR);
  HighLightedImageT=HighLight(edges,HighLightedImageT);
  imshow("HighLightT",HighLightedImageT);



  imshow("Original grayscaled",img);
  waitKey(0);
  return 0;
}



Mat HighLight(Mat src, Mat dst)
{
  //calculate gradients
  Vec3b pixel;
  pixel[1] = 255;
  Mat magX = Mat(src.rows, src.cols, CV_32F);
  Mat magY = Mat(src.rows, src.cols, CV_32F);
  Sobel(src, magX, CV_32F, 1, 0, 3);
  Sobel(src, magY, CV_32F, 0, 1, 3);
  
  for(int y = 0; y < src.rows-1; y++)
  {
    for(int x = 0; x < src.cols-1; x++)
    {
      //to find the the degree 
      double degrees = atan2(magY.at<int>(y,x), magX.at<int>(y,x)) * 180 / PI;
      
      if (degrees >= 22.5 && degrees <= 45.0)
      {
        dst.at<Vec3b>(y, x) = pixel;

      }
    }
  }

  return dst;
  
  
}


