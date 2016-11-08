//#include "opencv2/highgui/highgui.hpp"
//#include "opencv2/imgproc/imgproc.hpp"
//#include <iostream>
//#include <stdio.h>
//
//using namespace cv;

///** @function main */
//int main(int argc, char** argv)
//{
//  Mat src, src1,src_gray,src_gray1;
//
//  /// Read the image
//  src = imread( "EuroCoins.JPG", 1 );
// src1 = imread( "EuroCoins2.JPG", 1 );
//  
//
//  /// Convert it to gray
//  cvtColor( src, src_gray, CV_BGR2GRAY );
//
//  /// Reduce the noise so we avoid false circle detection
//  GaussianBlur( src_gray, src_gray, Size(9, 9), 2, 2 );
//
//  vector<Vec3f> circles;
//
//  /// Apply the Hough Transform to find the circles
//  HoughCircles( src_gray, circles, CV_HOUGH_GRADIENT, 1, src_gray.rows/8, 150, 55, 0, 0 );
//
//  /// Draw the circles detected
//  for( size_t i = 0; i < circles.size(); i++ )
//  {
//      Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
//      int radius = cvRound(circles[i][2]);
//      // circle center
//      circle( src, center, 3, Scalar(0,255,0), -1, 8, 0 );
//      // circle outline
//      circle( src, center, radius, Scalar(0,0,255), 3, 8, 0 );
//   }
//
//
//
//
//  /// Convert it to gray
//  cvtColor( src1, src_gray1, CV_BGR2GRAY );
//
//  /// Reduce the noise so we avoid false circle detection
//  GaussianBlur( src_gray1, src_gray1, Size(9, 9), 2, 2 );
//
//  vector<Vec3f> circle1;
//
//  /// Apply the Hough Transform to find the circles
//  HoughCircles( src_gray1, circle1, CV_HOUGH_GRADIENT, 1, src_gray1.rows/8, 150, 55, 0, 0 );
//
//  /// Draw the circles detected
//  for( size_t i = 0; i < circle1.size(); i++ )
//  {
//      Point center(cvRound(circle1[i][0]), cvRound(circle1[i][1]));
//      int radius = cvRound(circle1[i][2]);
//      // circle center
//      circle( src1, center, 3, Scalar(0,255,0), -1, 8, 0 );
//      // circle outline
//      circle( src1, center, radius, Scalar(0,0,255), 3, 8, 0 );
//   }
//
//
//
//
//  /// Show your results
//  namedWindow( "Hough Circle Transform Demo", CV_WINDOW_AUTOSIZE );
//  imshow( "Hough Circle Transform Demo", src );
//   imshow( "Hough Circle Transform Demo2", src1 );
//
//  waitKey(0);
//  return 0;
//}
//
//
//
























































 #include "opencv2/highgui/highgui.hpp"
 #include "opencv2/imgproc/imgproc.hpp"
 #include <iostream>
#include <list>
 
 using namespace cv;
 using namespace std;
  std::list<Point2f> mylist;
  bool intersection(Point2f o1, Point2f p1, Point2f o2, Point2f p2,
                       Point2f &r);
 int main(int argc, char** argv)
 {  Mat src = imread("HoughPicture.jpg", 0);
     Mat s;
     Mat dst, cdst;
    
  Canny(src, dst, 50, 200, 3);
  cvtColor(dst, cdst, CV_GRAY2BGR);
      vector<Vec2f> lines;
     HoughLines( dst, lines, 0.7, 2.61*M_PI/180, 50 ,0,0);
     for( size_t i = 0; i < lines.size(); i++ )
     {
      float rho = lines[i][0], theta = lines[i][1];
      Point pt1, pt2;
      double a = cos(theta), b = sin(theta);
      double x0 = a*rho, y0 = b*rho;
      pt1.x = cvRound(x0 + 1000*(-b));
      pt1.y = cvRound(y0 + 1000*(a));
      pt2.x = cvRound(x0 - 1000*(-b));
      pt2.y = cvRound(y0 - 1000*(a));
      for(int j=0;j<lines.size();j++){
          if(i!=j){
              
      float rho2 = lines[j][0], theta2 = lines[i][1];
      Point pt3, pt4;
      double a2 = cos(theta2), b2 = sin(theta);
      double x02 = a2*rho2, y02 = b2*rho2;
      pt3.x = cvRound(x02 + 1000*(-b2));
      pt3.y = cvRound(y02 + 1000*(a2));
      pt4.x = cvRound(x02 - 1000*(-b2));
      pt4.y = cvRound(y02 - 1000*(a2));
          Point2f intPoint;
        if(intersection(pt1,pt2,pt3,pt4,intPoint)){
            mylist.push_back(intPoint);
              circle(cdst,intPoint,5,Scalar(0,0,255),2,CV_AA,0);
               
        }
          }
          
          
      }
    
      }
     
        cout<< mylist.size()<<endl;
     cout<< lines.size()<<endl;
      

  //imshow("source", s);
 imshow("detected lines", cdst);

  waitKey();

  return 0;
 }

 bool intersection(Point2f o1, Point2f p1, Point2f o2, Point2f p2,
                       Point2f &r)
 {
     Point2f x = o2 - o1;
     Point2f d1 = p1 - o1;
     Point2f d2 = p2 - o2;

     float cross = d1.x*d2.y - d1.y*d2.x;
    if (abs(cross) < /*EPS*/1e-8)
         return false;

     double t1 = (x.x * d2.y - x.y * d2.x)/cross;
     r = o1 + d1 * t1;
     return true;
 }
