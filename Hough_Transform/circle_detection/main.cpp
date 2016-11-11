#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

using namespace cv;
using namespace std;
Mat img_original, img_blur,img_edge, final_image;
char currentLabel ='A';
std::vector<Point> IntersectionPoints;
double xi_prime, yi_prime, xi, yi;


int circle_circle_intersection(double x0, double y0, double r0,
	double x1, double y1, double r1,
	double *xi, double *yi,
	double *xi_prime, double *yi_prime);
void labelling();

/** @function main */
int main(int argc, char** argv)
{
	Mat src, src_gray;
	const char* filename = argc >= 2 ? argv[1] : "HoughPicture.jpg";
	img_original = imread(filename, 1);
	if(img_original.empty())
	{
		cout << "can not open " << filename << endl;
		return -1;
	}

  /// Convert it to gray
	cvtColor( img_original, src_gray, CV_BGR2GRAY );

  /// Reduce the noise so we avoid false circle detection
  GaussianBlur( src_gray, src_gray, Size(9, 9), 2, 2 ); //WHEN I CHEANGED THE KERNEL SIZE FROM 9-9 TO 3-3  
  														//THEN THE BLUR IS REDUCED AND THE HOUGHCIRCLES CAN DETECT MORE CIRCLES

  vector<Vec3f> circles;

  /// Apply the Hough Transform to find the circles
  HoughCircles( src_gray, circles, CV_HOUGH_GRADIENT, 1, src_gray.rows/8, 150, 55, 0, 0);

  /// Draw the circles detected
  for( size_t i = 0; i < circles.size(); i++ )
  {
  	Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
  	int radius = cvRound(circles[i][2]);
      // circle center
  	circle( img_original, center, 3, Scalar(0,255,0), -1, 8, 0 );
      // circle outline
  	circle( img_original, center, radius, Scalar(0,0,255), 3, 8, 0 );

	for (size_t j = 0; j < circles.size(); j++)
	{ if (i==j)
	{
		/* code */
	}else
	{
		Point center2(cvRound(circles[j][0]), cvRound(circles[j][1]));
    	int radius2 = cvRound(circles[j][2]);
		  	circle_circle_intersection(center.x, center.y, radius,
				center2.x, center2.y, radius2,
				&xi, &yi,
				&xi_prime, &yi_prime);
		  }
	}

  }
  labelling();

  /// Show your results
  namedWindow( "Hough Circle Transform Demo", CV_WINDOW_AUTOSIZE );
  imshow( "Hough Circle Transform Demo", img_original );

  waitKey(0);
  return 0;
}
int circle_circle_intersection(double x0, double y0, double r0,
	double x1, double y1, double r1,
	double *xi, double *yi,
	double *xi_prime, double *yi_prime)
{
	double a, dx, dy, d, h, rx, ry;
	double x2, y2;

  /* dx and dy are the vertical and horizontal distances between
   * the circle centers.
   */
   dx = x1 - x0;
   dy = y1 - y0;

  /* Determine the straight-line distance between the centers. */
  //d = sqrt((dy*dy) + (dx*dx));
  d = hypot(dx,dy); // Suggested by Keith Briggs

  /* Check for solvability. */
  if (d > (r0 + r1))
  {
    /* no solution. circles do not intersect. */
  	return 0;
  }
  if (d < fabs(r0 - r1))
  {
    /* no solution. one circle is contained in the other */
  	return 0;
  }

  /* 'point 2' is the point where the line through the circle
   * intersection points crosses the line between the circle
   * centers.  
   */

  /* Determine the distance from point 0 to point 2. */
   a = ((r0*r0) - (r1*r1) + (d*d)) / (2.0 * d) ;

  /* Determine the coordinates of point 2. */
   x2 = x0 + (dx * a/d);
   y2 = y0 + (dy * a/d);

  /* Determine the distance from point 2 to either of the
   * intersection points.
   */
   h = sqrt((r0*r0) - (a*a));

  /* Now determine the offsets of the intersection points from
   * point 2.
   */
   rx = -dy * (h/d);
   ry = dx * (h/d);

  /* Determine the absolute intersection points. */
   *xi = x2 + rx;
   *xi_prime = x2 - rx;
   *yi = y2 + ry;
   *yi_prime = y2 - ry;
       IntersectionPoints.push_back(Point(*xi_prime,*yi_prime));

       cout<<*xi_prime<<"  "<<*yi_prime<<endl;

   return 1;
}

void labelling()
  {
    Point ptToUse;
  
    
    for (int i = 0; i < IntersectionPoints.size(); i++)
    {  
        ptToUse = Point(IntersectionPoints[i].x, IntersectionPoints[i].y);  
       
       circle(img_original,ptToUse, 5, Scalar(0,255,255), 2, CV_AA,0 ); 
    }
    for (int i = 0; i < IntersectionPoints.size(); i++)
    {  ostringstream ss;
    	ss<<currentLabel;
        std::string s= ss.str();
        ptToUse = Point(IntersectionPoints[i].x, IntersectionPoints[i].y);  
      putText(img_original, s,Point(ptToUse.x+10,ptToUse.y), 1, 1.2, Scalar(255,0,0), 2, 8, false );
      currentLabel++;

    }

  }

