
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>

using namespace cv;
using namespace std;

 Mat erode_Dilate(Mat img);
void FilterImage(Mat img_original);
void labelling();
void getFourPointsOnLines();
bool getIntersectionPoint(Point a1, Point a2, Point b1, Point b2, Point & intPnt);
double cross(Point v1,Point v2);
bool checkingIfExist(Point p);
void DrawLine(Mat img);
vector<Vec2f> lines;

char currentLabel ='A';
Mat img_original, img_blur,img_edge, final_image;

Point  intPnt;
std::vector<std::string> labels;
std::vector<Point> IntersectionPoints;


int main(int argc, char** argv)
{
 
 Mat frameA=imread("ImageA.jpg",1);
 Mat frameB= imread("ImageB.jpg", 1);

 
 Mat grayimageA,grayimageB;
 cvtColor(frameA, grayimageA, CV_BGR2GRAY);
     GaussianBlur( frameA, grayimageA, Size( 5 ,5), 0, 0 );
    grayimageA= erode_Dilate( grayimageA);
    Canny( grayimageA, grayimageA, 100, 255 );

  cvtColor(frameB, grayimageB, CV_BGR2GRAY);
     GaussianBlur( frameB, grayimageB, Size( 5 ,5), 0, 0 );
    grayimageB= erode_Dilate( grayimageB);
    Canny( grayimageB, grayimageB, 100, 255 );

   
  HoughLines(grayimageA, lines, 0.855, CV_PI/179.5, 95, 0, 0 );
  DrawLine(frameA);

  HoughLines(grayimageB, lines, 0.882, CV_PI/180.5, 90, 0, 0 );
  DrawLine(frameB);

imshow("frameA",frameA);
imshow("frameB",frameB);
  //getFourPointsOnLines();
 // labelling();
 
 waitKey();
 return 0;
}
void FilterImage(Mat img_original)
{
  cv::GaussianBlur(img_original, img_blur, Size(3,3),1);
  cv::Canny(img_blur,img_edge, 110,255,3, true);
}
void getFourPointsOnLines()
{
  for( size_t i = 0; i < lines.size(); i++ )
  {
   float rho = lines[i][0], theta = lines[i][1];
   Point pt1,pt2;
   double a = cos(theta), b = sin(theta);
   double x0 = a*rho, y0 = b*rho;
   pt1.x = cvRound(x0 +1000* (-b));
   pt1.y = cvRound(y0 + 1000*(a));
   pt2.x = cvRound(x0 - 1000*(-b));
   pt2.y = cvRound(y0 - 1000*(a));
   for (int j = 0; j < lines.size(); ++j)
   { 
    if (i==j){
    }
    else
    {
      float rho2 = lines[j][0], theta2 = lines[j][1];
      Point  pt3,pt4;
      double a2 = cos(theta2), b2 = sin(theta2);
      double x02 = a2*rho2, y02 = b2*rho2;
      pt3.x = cvRound(x02 +1000 *(-b2));
      pt3.y = cvRound(y02 +1000 *(a2));
      pt4.x = cvRound(x02 -1000 *(-b2));
      pt4.y = cvRound(y02 - 1000 *(a2));
      getIntersectionPoint(pt1, pt2, pt3, pt4, intPnt);
    }

  }

}
cout<<"Number of lines: "<<lines.size()<<endl;
//labelling();
//imshow("source", img_original);
//imshow("detected lines", final_image);

}
bool getIntersectionPoint(Point a1, Point a2, Point b1, Point b2, Point & intPnt){
  Point p = a1;
  Point q = b1;
  Point r(a2-a1);
  Point s(b2-b1);

  if(cross(r,s) == 0) {return false;}

  double t = cross(q-p,s)/cross(r,s);

  intPnt = p + t*r;

    if (intPnt.x >0 &&intPnt.x<450&&intPnt.y>0 &&intPnt.y<400) //Storing only the points that are in our window frame
    {
      if (!checkingIfExist(intPnt))
      {      
        cout<<intPnt<<endl;
        circle(img_original,intPnt, 5, Scalar(0,0,255), 2, CV_AA,0 );
        // putText(final_image, "A", Point(intPnt.x+8,intPnt.y+5), 1, 1.2, Scalar(0,0,255), 1, 8, false );
        IntersectionPoints.push_back(intPnt);
      }
    }

    return true;
  }
  double cross(Point v1,Point v2){
    return v1.x*v2.y - v1.y*v2.x;
  }
  bool checkingIfExist(Point p)
  {
    for (int i = 0; i < IntersectionPoints.size(); ++i)
    {
      if (IntersectionPoints[i].x==p.x && IntersectionPoints[i].y==p.y)
      {
        return true;
      }
    }
    return false;

  }

  
  void labelling()
  {
    Point ptToUse;
    for (int i = 0; i < IntersectionPoints.size(); i++)
    {
      if (IntersectionPoints[i].x<300 &&IntersectionPoints[i].x>=20 &&IntersectionPoints[i].y>=20&&IntersectionPoints[i].y<300)
      {
        ptToUse = Point(IntersectionPoints[i].x, IntersectionPoints[i].y);
      }
      if (IntersectionPoints[i].x>300)
      {
        ptToUse = Point(IntersectionPoints[i].x-20, IntersectionPoints[i].y);
      }
      if (IntersectionPoints[i].x<20)
      {
        ptToUse = Point(IntersectionPoints[i].x+20, IntersectionPoints[i].y);
      }
      if (IntersectionPoints[i].y<20)
      {
        ptToUse = Point(IntersectionPoints[i].x, IntersectionPoints[i].y+20);
      }
      if (IntersectionPoints[i].y>300)
      {
        ptToUse = Point(IntersectionPoints[i].x, IntersectionPoints[i].y-20);
      }
      ostringstream ss;
       ss<<currentLabel;
       std::string s= ss.str();
      putText(img_original, s,ptToUse, 1, 1.2, Scalar(0,255,0), 2, 8, false );
      currentLabel++;

    }

  }

  Mat erode_Dilate(Mat img) {
//i//mshow("gaps",img);
    Mat erodeElement = getStructuringElement(MORPH_RECT, Size(5, 5));
    Mat dilateElement = getStructuringElement(MORPH_RECT, Size(7, 7));
  // erode(img, img, erodeElement);
   dilate(img, img, dilateElement);
   // imshow("gaps",img);
    return img;


}

void DrawLine(Mat img){


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
     line( img, pt1, pt2, Scalar(0,0,255), 1, CV_AA);
  }

}