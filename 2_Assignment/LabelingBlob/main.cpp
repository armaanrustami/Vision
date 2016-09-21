    #include <iostream>
    #include <vector>
    #include <opencv2/highgui/highgui.hpp>
    #include <opencv2/imgproc/imgproc.hpp>
using namespace std;

cv::Mat processThreshold();
void depth_First_Search(int x, int y, int current_label);
void find_components();
void coloring( int lab);

cv::Mat binary;
     // direction vectors
const int dx[] = {+1, 0, -1, 0};
const int dy[] = {0, +1, 0, -1};
    cv::Mat img = cv::imread("blobs labelling.jpg", 0); // force greyscale  
    cv::Mat Timg;
    // the labels, 0 means unlabeled
    cv::Mat labels = cv::Mat::zeros(img.size(), CV_8UC3);
    cv::Mat output = cv::Mat::zeros(img.size(), CV_8UC3);
    int main(int argc, char **argv)
    {


        if(!img.data) {
            std::cout << "File not found" << std::endl;
            return -1;
        }
        cv::namedWindow("binary");
        binary=processThreshold();
        find_components();
        imshow("Real",img);
        cv::imshow("binary", binary);
         imshow("output",output);
        cv::waitKey(0);

        return 0;
    }


    cv::Mat processThreshold(){

       img.copyTo(Timg);


       for (int y = 0; y < img.rows; ++y)
       {
        for (int x = 0; x < img.cols; ++x)
        {
            if (img.at<uchar>(y, x)> 180) Timg.at<uchar>(y,x) =255;             

            else Timg.at<uchar>(y,x) = 0;         

        }
    }
    return Timg;
}



void depth_First_Search(int x, int y, int current_label) {
      if (x < 0 || x == binary.rows) return; // out of bounds
      if (y < 0 || y == binary.cols) return; // out of bounds


      if ( labels.at<uchar>(x,y)>0 || binary.at<uchar>(x,y)!=255){ return;} // already labeled or not marked with 1 in lablels

      // mark the current cell 
      //binary.at<uchar>(x,y)=current_label;
      labels.at<uchar>(x,y) = current_label;

      // recursively mark the neighbors
      for (int direction = 0; direction < 4; ++direction){
        depth_First_Search(x + dx[direction], y + dy[direction], current_label);

    }
}

void find_components() {
    int lab_Counter = 20;
  for (int i = 0; i < binary.rows; ++i) 
    for (int j = 0; j < binary.cols; ++j) 
      if (labels.at<uchar>(i,j)==0 && binary.at<uchar>(i,j)==255){ 

        lab_Counter=lab_Counter+1;
        depth_First_Search(i, j, lab_Counter);}
        coloring(lab_Counter);
     
    }


    void coloring(int lab){


        for (int i = 20; i <=lab; ++i)
        {
            unsigned char r = 255 * (rand()/(1.0 + RAND_MAX));
            unsigned char g = 255 * (rand()/(1.0 + RAND_MAX));
            unsigned char b = 255 * (rand()/(1.0 + RAND_MAX));
        /* code */
            for (int y = 0; y < labels.rows; ++y)
            {
            /* code */
                for (int x = 0;x  < labels.cols; ++x)
                {
                /* code */
                    if(labels.at<uchar>(y,x)==i){

                     output.at<cv::Vec3b>(y,x)[0] = b;
                     output.at<cv::Vec3b>(y,x)[1] = g;
                     output.at<cv::Vec3b>(y,x)[2] = r;


                 }
             }
         }


     }



 }

