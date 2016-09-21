#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

//main functions

void processImage(Mat kernel);
Mat Lap(Mat src , Mat kernel );
//images
Mat image;
Mat processedImage;

Mat img, imgLaplacian, imgResult;
int window_size = 3;
int main(int argc, char *argv[])
{

 img =imread("edge detection.jpg",0);

 Mat Smoothblur = (Mat_<float>(3,3) << 
  1,  1, 1,
  1, 1, 1,
  1,  -1, 1); 
 Mat kernel = (Mat_<float>(3,3) << 
  0,  -1, 0,
  -1, 4, -1,
  0,  -1, 0); 
 Mat kernelB = (Mat_<float>(3,3) << 
  -1,  -1, -1,
  -1, 8, -1,
  -1,  -1, -1); 
 Mat Geadient = (Mat_<float>(3,3) << 
  -1,  0, 1,
  -1, 0, 1,
  -1, 0, 1); 
 

 imshow("OrignalImage",img);
 Mat blured=Mat::zeros(img.size(), CV_8U);
 Lap(img,Smoothblur).copyTo(blured);
 imshow("Processed_SmoothBlur", blured);
 Lap(blured,kernel).copyTo(imgLaplacian);
 Lap(imgLaplacian,Smoothblur).copyTo(imgLaplacian);
 imshow("Processed_Image_TaskA", imgLaplacian);
 imgLaplacian = Mat::zeros(img.size(), CV_8U);
 Lap(blured,kernelB).copyTo(imgLaplacian);
 Lap(imgLaplacian,Smoothblur).copyTo(imgLaplacian);
 imshow("Processed_Image_TaskB", imgLaplacian);
 imgLaplacian = Mat::zeros(img.size(), CV_8U);
 Lap(blured,Geadient).copyTo(imgLaplacian);
 Lap(imgLaplacian,Smoothblur).copyTo(imgLaplacian);
 imshow("Processed_Gradient", imgLaplacian);
 waitKey(0);
 return 0;
}

Mat Lap(Mat src , Mat kernel ){

  Mat temp;
  temp=Mat::zeros(img.size(), CV_8U);
  for(int i=1; i<img.rows-1; i++) {
    for(int j=1; j<img.cols-1; j++) {
      temp.at<uchar>(i,j)=
      ( src.at<uchar>(i - 1, j - 1) * kernel.at<float>(0,0) +
        src.at<uchar>(i- 1, j) * kernel.at<float>(0,1) +
        src.at<uchar>(i - 1,j + 1) * kernel.at<float>(0,2) +
        src.at<uchar>(i, j - 1) * kernel.at<float>(1,0)+
        src.at<uchar>(i, j) * kernel.at<float>(1,1) +
        src.at<uchar>(i, j + 1) * kernel.at<float>(1,2)+
        src.at<uchar>(i + 1, j - 1) * kernel.at<float>(2,0) +
        src.at<uchar>(i + 1, j) * kernel.at<float>(2,1) +
        src.at<uchar>(i + 1, j + 1) * kernel.at<float>(2,2))
      / 9;}}
      return temp;

    }