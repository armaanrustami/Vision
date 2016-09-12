#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

//main functions

void processImage(Mat kernel);

//images
Mat image;
Mat processedImage;

Mat img, imgLaplacian, imgResult;
int window_size = 3;
int main(int argc, char *argv[])
{
 
 namedWindow("OrignalImage");

 namedWindow("Processed_Image_TaskA");
 namedWindow("Processed_Image_TaskB");
    //------------------------------------------------------------------------------------------- test, first of all
    // now do it by hand
 img =imread("lena.jpg",0);
 
    // first, the good result
  //  Laplacian(img, imgLaplacian, CV_8UC1);
 cout << "let opencv do it" << endl;
   // cout << imgLaplacian << endl;

 Mat kernel = (Mat_<float>(3,3) << 
  0,  -1, 0,
  -1, 4, -1,
  0,  -1, 0); 
 Mat kernelB = (Mat_<float>(3,3) << 
  -1,  -1, -1,
  -1, 8, -1,
  -1,  -1, -1); 

 
 processImage( kernel);

 imshow("OrignalImage",img);
 imshow("Processed_Image_TaskA", imgLaplacian);

 processImage( kernelB);
 imshow("Processed_Image_TaskB", imgLaplacian);
 waitKey(0);


//no need to release memory
 return 0;
}








void processImage(Mat kernel)
{
 float matrix[3][3];
 int counter=0;
 cout<<kernel.at<float>(0,1);
    // now, reaaallly by hand
    // note that, for avoiding padding, the result image will be smaller than the original one.
 Mat frame, frame32;
 Rect roi;
 imgLaplacian = Mat::zeros(img.size(), CV_32F);
 for(int y=0; y<img.rows-window_size/2-1; y++) {
  for(int x=0; x<img.cols-window_size/2-1; x++) {

    if (x!=0 && y!=0)
    {
              /* code */
     
      matrix[0][0]=img.at<uchar>(y-1,x-1);
      matrix[0][1]=img.at<uchar>(y-1,x);
      matrix[0][2]=img.at<uchar>(y-1,x+1);                          
      matrix[1][0]=img.at<uchar>(y,x-1);
      matrix[1][1]=img.at<uchar>(y,x);
      matrix[1][2]=img.at<uchar>(y,x+1);
      matrix[2][0]=img.at<uchar>(y+1,x-1);
      matrix[2][1]=img.at<uchar>(y+1,x);
      matrix[2][2]=img.at<uchar>(y+1,x+1);
                          //cout<<"I am where x and y are different from 0"<<endl;
    }
    else
    {
      if (x  == 0 && y == 0)
      {
       
       matrix[0][0]=0;
       matrix[0][1]=0;
       matrix[0][2]=0;
       matrix[1][0]=0;
       matrix[1][1]=img.at<uchar>(y,x);
       matrix[1][2]=img.at<uchar>(y,x+1);
       matrix[2][0]=0;
       matrix[2][1]=img.at<uchar>(y+1,x);
       matrix[2][2]=img.at<uchar>(y+1,x+1);
                          //cout<<"I am where x and y are  0"<<endl;
       
     }

     if (x  != 0 && y == 0){


       matrix[0][0]=0;;
       matrix[0][1]=0;
       matrix[0][2]=0;
       matrix[1][0]=img.at<uchar>(y,x-1);
       matrix[1][1]=img.at<uchar>(y,x);
       matrix[1][2]=img.at<uchar>(y,x+1);
       matrix[2][0]=img.at<uchar>(y+1,x-1);
       matrix[2][1]=img.at<uchar>(y+1,x);
       matrix[2][2]=img.at<uchar>(y+1,x+1);
                          //cout<<"Y is  0"<<endl;
       
     }
     if (x  == 0 && y != 0){


      matrix[0][0]=0;;
      matrix[0][1]=img.at<uchar>(y-1,x);
      matrix[0][2]=img.at<uchar>(y-1,x+1);
      matrix[1][0]=0;
      matrix[1][1]=img.at<uchar>(y,x);
      matrix[1][2]=img.at<uchar>(y,x+1);
      matrix[2][0]=0;
      matrix[2][1]=img.at<uchar>(y+1,x);
      matrix[2][2]=img.at<uchar>(y+1,x+1);
                         // cout<<"X is  0"<<endl;
      
    }
  }
  
  float temp=0;
  for (int i = 0; i < 3; ++i)
  {
    for (int j = 0; j < 3; j++)
    {
                        /* code */
      temp+=matrix[i][j]*kernel.at<float>(i,j);
    }
                      /* code */
  }
  roi = Rect(x,y, window_size, window_size);
  frame = img(roi);
  imgLaplacian.at<float>(y,x) = temp;
}
}
imgLaplacian.convertTo(imgLaplacian, CV_8U);
}




