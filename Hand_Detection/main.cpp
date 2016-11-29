#include <iostream>
#include <vector>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>



using namespace cv;
using namespace std;


Mat erode_Dilate(Mat img);
Point COG; //COG
string label[] = {"Pinkie", "Ring", "Middle", "Index", "Thumb"};
//char label='A';
int findBiggestContour(vector<vector<Point> > contours);
int IndexOfBiggestContour;
int counter = 0;

int main(int argc, char* argv[]) {
    // VideoCapture cap("hand.jpg");  
    for (;;) {
        Mat frame(Size(640, 420), CV_8UC3);
        Mat original(Size(640, 420), CV_8UC3);
        frame = imread("hands.jpg", 0);
        original = imread("hands.jpg", 1);


        Mat thresh;
        cv::threshold(frame, thresh, 165, 200, THRESH_BINARY);
        int biggestAreay = 0;

        Mat hsv;
        cvtColor(thresh, thresh, CV_GRAY2RGB);
        cvtColor(thresh, thresh, CV_RGB2HSV);
        //cvtColor(thresh,thresh,CV_RGB2HSV);
        Mat bw(Size(640, 420), CV_8UC1);
        inRange(thresh, Scalar(0, 0, 0), Scalar(180, 255, 0), bw);
        bw = erode_Dilate(bw);
        imshow("erode", bw);
        vector<Vec4i> hierarchy;
        vector<vector<Point> > contours_hull;

        findContours(bw.clone(), contours_hull, hierarchy, CV_RETR_TREE, true, Point(0, 0));
        cout << contours_hull.size() << endl;
        /// Find the convex hull object for each contour
        vector<vector<Point> >hull(contours_hull.size());
        //find the defects points for each contour
        vector<vector<Vec4i> > defects(contours_hull.size());

        vector<vector<int> > hullsI(contours_hull.size());

        //find the biggest contour
        IndexOfBiggestContour = findBiggestContour(contours_hull);

        Point2f rect_points[4];
        vector<RotatedRect> minRect(contours_hull.size());
        Moments moment;

        vector<vector<Point> > contours_poly(contours_hull.size());
        vector<Rect> boundRect(contours_hull.size());

        for (int i = 0; i < contours_hull.size(); i++) {
            convexHull(Mat(contours_hull[i]), hull[i], true);
            convexHull(Mat(contours_hull[i]), hullsI[i], true);
            convexityDefects(Mat(contours_hull[i]), hullsI[i], defects[i]);
            cout << defects.size() << endl;
            moment = moments((cv::Mat)contours_hull[i]);
            if (moment.m00 > biggestAreay) {
                biggestAreay = moment.m00;
                COG = Point(moment.m10 / moment.m00, moment.m01 / moment.m00); //COG
            }
            cout << defects[i].size() << endl;
            for (int j = 0; j < defects[i].size(); ++j) {
                const Vec4i& v = defects[i][j];
                float depth = v[3] / 256;
                if (depth > 30) //  filter defects by depth
                {
                    int startidx = v[0];
                    Point ptStart(contours_hull[i][startidx]);
                    int endidx = v[1];
                    Point ptEnd(contours_hull[i][endidx]);
                    int faridx = v[2];
                    Point ptFar(contours_hull[i][faridx]);
                    // line(frame, ptStart, ptEnd, Scalar(0, 255, 0), 1);
                    //line(frame, ptStart, ptFar, Scalar(0, 255, 0), 1);
                    line(original, ptEnd, ptFar, Scalar(0, 255, 0), 2);
                    circle(original, COG, 4, Scalar(0, 255, 0), 2);
                      putText(original,"COG", COG, 1, 1.2, Scalar(0, 0, 255), 2, 8, false);
                    circle(original, ptFar, 4, Scalar(0, 255, 0), 2);
                    circle(original, ptEnd, 4, Scalar(0, 255, 0), 2);
                    putText(original, label[counter], ptEnd, 1, 1.2, Scalar(0, 0, 255), 2, 8, false);

                    counter++;


                }
            }

            if (IndexOfBiggestContour == i) {
                minRect[i] = minAreaRect(Mat(contours_hull[i]));

                //draw contour of biggest object
                //   drawContours( original, contours_hull,IndexOfBiggestContour, CV_RGB(0,255,255), 2, 8, hierarchy,0, Point() );
                //draw hull of biggesr object
                drawContours(original, hull, IndexOfBiggestContour, CV_RGB(255, 215, 0), 2, 8, hierarchy, 0, Point());


                approxPolyDP(Mat(contours_hull[i]), contours_poly[i], 3, true);
                //  boundRect[i] = boundingRect( Mat(contours_poly[i]) );



            }
        }

        //imshow("original", frame);
        imshow("frame", original);


        cv::waitKey(0);
    }
    return 0;
}

Mat erode_Dilate(Mat img) {

    Mat erodeElement = getStructuringElement(MORPH_RECT, Size(3, 3));
    Mat dilateElement = getStructuringElement(MORPH_RECT, Size(8, 8));
    erode(img, img, erodeElement);
    dilate(img, img, dilateElement);
    return img;


}

int findBiggestContour(vector<vector<Point> > contours) {
    int indexOfBiggestContour = -1;
    int sizeOfBiggestContour = 0;

    for (int i = 0; i < contours.size(); i++) {
        if (contours[i].size() > sizeOfBiggestContour) {
            sizeOfBiggestContour = contours[i].size();
            indexOfBiggestContour = i;
        }
    }
    return indexOfBiggestContour;
}
