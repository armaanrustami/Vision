#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

//main functions
void processImage();
void displayGraphics();

//images
Mat image;
Mat processedImage;

int main(int argc, char *argv[])
{
  Mat img, imgLaplacian, imgResult;
 namedWindow("Image");
  namedWindow("Processed Image");
    //------------------------------------------------------------------------------------------- test, first of all
    // now do it by hand
    img =imread("lena.jpg",0);
   
    // first, the good result
    Laplacian(img, imgLaplacian, CV_8UC1);
    cout << "let opencv do it" << endl;
   // cout << imgLaplacian << endl;

    Mat kernel = (Mat_<float>(3,3) << 
        0,  -1, 0,
        -1, 4, -1,
        0,  -1, 0); 
    int window_size = 3;
    float matrix[3][3];
    int counter=0;

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
                 /* else if (y - 1 >= 0)
                    {
                       
                    }
                    if (x + 1 > img.rows)
                    {
                      imgLaplacian.at<float>(y+1,x) = 0;
                        
                    }
                    if (y + 1 > img.cols)
                    {
                          imgLaplacian.at<float>(y,x+1) = 0;
                    }
*/
            roi = Rect(x,y, window_size, window_size);
            frame = img(roi);
           frame.convertTo(frame, CV_32F);
           // frame = frame.mul(kernel);
            float v = sum(frame.mul(kernel))[0];
            imgLaplacian.at<float>(y,x) = v;
        }
    }
    imgLaplacian.convertTo(imgLaplacian, CV_8U);
   // cout << "dudee" << imgLaplacian << endl;

    // a little bit less "by hand"..
    // using cv::filter2D
    filter2D(img, imgLaplacian, -1, kernel);
    //cout << imgLaplacian << endl;
for(int i=0;i<3;i++){{
  for (int j = 0; j < 3; ++j)
  {
    /* code */
    cout << matrix[i][j];
    cout << " ";

  }
   cout << endl;
}}
    //------------------------------------------------------------------------------------------- real stuffs now
   // img = imread("moon.jpg", 0); // load grayscale image
  imshow("Image", imgLaplacian);
  waitKey(0);


//no need to release memory
return 0;
}




void displayGraphics()
{
//display both images
imshow("Image", image);
imshow("Processed Image", processedImage);
}




void processImage()
{
int x,y;
Vec3b pixel;
unsigned char R, G, B;
processedImage = image.clone();

for(y = 0; y < processedImage.rows; y++)
 for(x = 0; x < processedImage.cols; x++)
 {
  //Get the pixel at (x,y)
  pixel = processedImage.at<Vec3b>(y, x);
  //get the separate colors
  B = pixel[0];
  G = pixel[1];
  R = pixel[2];
  //assign the complement to each color
  pixel[0] = 255 - B;
  pixel[1] = 255 - G;
  pixel[2] = 255 - R;
  //write the pixel back to the image
  processedImage.at<Vec3b>(y, x) = pixel;
 }
}




