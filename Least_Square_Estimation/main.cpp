
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/calib3d/calib3d.hpp"

#include <iostream>

using namespace cv;
using namespace std;

 Mat erode_Dilate(Mat img);
void FilterImage(Mat img_original);
void labelling();
void getFourPointsOnLines(std::vector<Point2f> & v);
bool getIntersectionPoint(Point a1, Point a2, Point b1, Point b2, Point & intPnt,std::vector<Point2f> & v);
double cross(Point v1,Point v2);
bool checkingIfExist(Point p);
void DrawLine(Mat img);
Mat getMatrixA(vector<Point2f> pointsA);
Mat getMatrixB(vector<Point2f> pointsB);
Mat getMatrix_X(cv::Mat inverse_matrixA, cv::Mat matrixB);
Mat getMatrix_PseudoInverse(cv::Mat matrixA);
double getaCos(double c);
Mat getMatrix_Transform(cv::Mat transf, double theta);

Mat frameA,frameB;
vector<Vec2f> lines;
vector<Point2f> orderCordinates(vector<Point2f> points, int a,int b,int c,int d);

char currentLabel ='A';
Mat img_original, img_blur,img_edge, final_image;
std::vector<Point2f> PointsA,PointsB;

Point  intPnt;
std::vector<std::string> labels;
std::vector<Point> IntersectionPoints;


int main(int argc, char** argv)
{
 
  frameA=imread("ImageA.jpg",1);
  frameB= imread("ImageB.jpg", 1);

 
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
  getFourPointsOnLines(PointsA);
  HoughLines(grayimageB, lines, 0.882, CV_PI/180.5, 90, 0, 0 );
  DrawLine(frameB);
  getFourPointsOnLines(PointsB);
 // cout<<PointsA<<endl;
  //cout<<PointsB<<endl;
 PointsA= orderCordinates(PointsA,1,3,2,0);
 PointsB=orderCordinates(PointsB,2,0,1,3);
 cout<<"After order"<<endl;
 cout<<PointsA<<endl;
  cout<<PointsB<<endl;
   Mat h =cv::findHomography(PointsA,PointsB);
 //cout<<"Find Homography"<<endl<<h<<endl;

    Mat inverse=h.inv();
    Mat im_dst= cv::Mat::zeros(frameB.rows, frameB.cols, frameA.type());
    //getMatrixA(PointsA);
    //getMatrixB(PointsB);
    Mat matTransFormed= getMatrix_X(getMatrix_PseudoInverse(getMatrixA(PointsA)), getMatrixB(PointsB));
    double theta =getaCos(matTransFormed.at<double>(0,0));
    
    cout<<"Theta value: "<<endl<<theta<<endl;
    
//    getMatrix_Transform(matTransFormed,theta);
    
    //warpPerspective(frameB, im_dst, getMatrix_Transform(matTransFormed,theta), frameA.size());
    warpPerspective(frameB, im_dst, inverse, frameA.size());
  //  warpAffine(frameB, im_dst, getMatrix_Transform(matTransFormed,theta), im_dst.size());

cv::Mat diff_Image= cv::Mat::zeros(frameB.rows, frameB.cols, frameA.type());

 cv::absdiff(frameA,im_dst,  diff_Image);
//
    cv::Mat foregroundMask = cv::Mat::zeros(diff_Image.rows, diff_Image.cols, CV_8UC1);

    float threshold = 150.0f;
    float dist;

    for(int j=0; j<diff_Image.rows; ++j)
        for(int i=0; i<diff_Image.cols; ++i)
        {
            cv::Vec3b pix = diff_Image.at<cv::Vec3b>(j,i);

            dist = (pix[0]*pix[0] + pix[1]*pix[1] + pix[2]*pix[2]);
            dist = sqrt(dist);

            if(dist>threshold)
            {
                foregroundMask.at<unsigned char>(j,i) = 255;
            }
        }

imshow("frameA",frameA);
imshow("frameB",frameB);
imshow("Transformed",im_dst);
imshow("Difference",diff_Image);
imshow("real diff",foregroundMask);

  //getFourPointsOnLines();
 waitKey();
 return 0;
}
void FilterImage(Mat img_original)
{
  cv::GaussianBlur(img_original, img_blur, Size(3,3),1);
  cv::Canny(img_blur,img_edge, 110,255,3, true);
}
void getFourPointsOnLines(std::vector<Point2f> & v)
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
      getIntersectionPoint(pt1, pt2, pt3, pt4, intPnt,v);
    }

  }

}
cout<<"Number of lines: "<<lines.size()<<endl;
//labelling();
//imshow("source", img_original);
//imshow("detected lines", final_image);

}
bool getIntersectionPoint(Point a1, Point a2, Point b1, Point b2, Point & intPnt,std::vector<Point2f> &v){
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
      //  cout<<intPnt<<endl;
           ostringstream ss;
           ss<<currentLabel;
       std::string s= ss.str();
        circle(img_original,intPnt, 5, Scalar(0,0,255), 2, CV_AA,0 );
       // putText(frameB, s, Point(intPnt.x+8,intPnt.y+5), 1, 1.2, Scalar(0,0,255), 1, 8, false );
        IntersectionPoints.push_back(intPnt);
        v.push_back(intPnt);
        currentLabel++;
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
    for (int i = 0; i < PointsA.size(); i++)
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
      putText(frameA, s,ptToUse, 1, 1.2, Scalar(0,255,0), 2, 8, false );
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

vector<Point2f> orderCordinates(vector<Point2f> points, int a,int b,int c,int d){
    
    Point2f A,B,C,D;
    
    A=points[a];
    B=points[b];
    C=points[c];
    D=points[d];
    
    vector<Point2f> temp;
    temp.push_back(A);
    temp.push_back(B);
    temp.push_back(C);
    temp.push_back(D);
    return temp;   
    
}

Mat getMatrixA(vector<Point2f> pointsA)
{ 
    
    int rows =8;
    int cols =4;
    cv::Mat p1= cv::Mat::zeros(rows, cols, cv::DataType<double>::type);
  
   p1.at<double>(0,0) = pointsA[0].x;
   p1.at<double>(0,1) = -pointsA[0].y;
   p1.at<double>(0,2) = 1;
   p1.at<double>(0,3) = 0;
  
   p1.at<double>(1,0) = pointsA[0].y;
   p1.at<double>(1,1) = pointsA[0].x;
   p1.at<double>(1,2) = 0;
   p1.at<double>(1,3) = 1;
   
  
   p1.at<double>(2,0) = pointsA[1].x;
   p1.at<double>(2,1) = -pointsA[1].y;
   p1.at<double>(2,2) = 1;
   p1.at<double>(2,3) = 0;
  
   p1.at<double>(3,0) = pointsA[1].y;
   p1.at<double>(3,1) = pointsA[1].x;
   p1.at<double>(3,2) = 0;
   p1.at<double>(3,3) = 1;
   
   
   p1.at<double>(4,0) = pointsA[2].x;
   p1.at<double>(4,1) = -pointsA[2].y;
   p1.at<double>(4,2) = 1;
   p1.at<double>(4,3) = 0;
  
   p1.at<double>(5,0) = pointsA[2].y;
   p1.at<double>(5,1) = pointsA[2].x;
   p1.at<double>(5,2) = 0;
   p1.at<double>(5,3) = 1;
   
   
   p1.at<double>(6,0) = pointsA[3].x;
   p1.at<double>(6,1) = -pointsA[3].y;
   p1.at<double>(6,2) = 1;
   p1.at<double>(6,3) = 0;
  
   p1.at<double>(7,0) = pointsA[3].y;
   p1.at<double>(7,1) = pointsA[3].x;
   p1.at<double>(7,2) = 0;
   p1.at<double>(7,3) = 1;
   

     cout<<endl<<"Matrix A Normal : "<<endl<<p1<<endl<<endl;
     cout<<"Matrix A Transposed : "<<endl<<p1.t()<<endl<<endl;
    return p1;
}

Mat getMatrixB(vector<Point2f> pointsB)
{ 
    
    int rows =8;
    int cols =1;
   cv::Mat p1= cv::Mat::zeros(rows, cols, cv::DataType<double>::type);
   p1.at<double>(0,0) = pointsB[0].x;
   p1.at<double>(1,0) = pointsB[0].y;
   p1.at<double>(2,0) = pointsB[1].x;
   p1.at<double>(3,0) = pointsB[1].y;  
   p1.at<double>(4,0) = pointsB[2].x;
   p1.at<double>(5,0) = pointsB[2].y;  
   p1.at<double>(6,0) = pointsB[3].x;
   p1.at<double>(7,0) = pointsB[3].y;
  
    cout<<"Matrix B: "<<endl<<p1<<endl;
   
    return p1;
}

Mat getMatrix_PseudoInverse(cv::Mat matrixA)
{ 
    
    int rows =8;
    int cols =1;
    cv::Mat p1= cv::Mat::zeros(rows, cols, cv::DataType<double>::type);
    
    p1=matrixA.t();
    p1 *=matrixA;
    p1 = p1.inv();
    p1 *=matrixA.t();
    return p1;
}

Mat getMatrix_X(cv::Mat inverse_matrixA, cv::Mat matrixB)
{   
    int rows =8;
    int cols =1;
    cv::Mat p1= cv::Mat::zeros(rows, cols, cv::DataType<double>::type);
    p1 =inverse_matrixA *matrixB;
    
    cout<<"Transformed X matrix: "<<endl<<p1<<endl;
    return p1;
}

double getaCos(double c)
{
    return acos(c);
}

Mat getMatrix_Transform(cv::Mat transf, double theta)
{   
    int rows =3;
    int cols =3;
    cv::Mat p1= cv::Mat::zeros(rows, cols, cv::DataType<double>::type);
   
   p1.at<double>(0,0) = cos(theta); 
   p1.at<double>(0,1) = -sin(theta);
   p1.at<double>(0,2) =  transf.at<double>(2,0);

   p1.at<double>(1,0) = sin(theta); 
   p1.at<double>(1,1) = cos(theta);
   p1.at<double>(1,2) = transf.at<double>(3,0);
//   
    
  /*  p1.at<double>(0,0) = 0.9999999999; 
   p1.at<double>(0,1) = 0.23382937339;
   p1.at<double>(0,2) =  0;

   p1.at<double>(1,0) = -0.23382937339; 
   p1.at<double>(1,1) = 0.999999999;
   p1.at<double>(1,2) = 0;*/
   
   p1.at<double>(2,0) = 0; 
   p1.at<double>(2,1) = 0;
   p1.at<double>(2,2) = 1;
   
    cout<<"Transformed matrix: "<<endl<<p1<<endl;
    return p1;
}

