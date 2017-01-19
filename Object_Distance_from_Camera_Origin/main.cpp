#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <iomanip>

using namespace cv;
using namespace std;

/** Headers **/
static vector<Point3f> buildMarkerPoints();
static vector<Point2f> findBoardCorners(const Mat& frame);
static double findDistance(const vector<Point3f>& markerPoints, const vector<Point2f>& corners);
static void drawDistance(Mat& frame, const vector<Point2f>& corners, double distance);

/** Global variables */
static String cameraDataFileName = "out_camera_data.yml";
const Size BOARD_SIZE(14, 9);
const double CHECKERBOARD_SIZE = 2.6; //Width and/or height (it's a square) in cm

int main()
{
	VideoCapture vc(0);

	if(!vc.isOpened())
		throw new runtime_error("Couldn't open handle to video camera");

	Mat frame;
	vector<Point3f> markerPoints = buildMarkerPoints();

	while(true)
	{
		vc >> frame;

		vector<Point2f> corners = findBoardCorners(frame);
		double distance = findDistance(markerPoints, corners);

		drawDistance(frame, corners, distance);

		imshow("Result", frame);

		if (waitKey(30) == 27) //Display images in 30fps and when ASCII key 27 (ESC) is pressed, quit application
			break;
	}

	return 0;
}

vector<Point3f> buildMarkerPoints()
{
	vector<Point3f> results;

	for(float y = 0; y < BOARD_SIZE.height * CHECKERBOARD_SIZE; y += CHECKERBOARD_SIZE)
	{
		for(float x = 0; x < BOARD_SIZE.width * CHECKERBOARD_SIZE; x += CHECKERBOARD_SIZE)
		{
			results.push_back(Point3f(x, y, 0.0f));
		}
	}

	return results;
}

vector<Point2f> findBoardCorners(const Mat& frame)
{
	vector<Point2f> boardCorners;

	bool found = findChessboardCorners(frame, BOARD_SIZE, boardCorners, CV_CALIB_CB_ADAPTIVE_THRESH );

	return boardCorners;
}

double findDistance(const vector<Point3f>& markerPoints, const vector<Point2f>& corners)
{
	if(corners.size() <= 0 || corners.size() != markerPoints.size())
		return -1;

	FileStorage fs(cameraDataFileName, FileStorage::READ);
	Mat intrinsics, distortion;
	fs["camera_matrix"] >> intrinsics;
	fs["distortion_coefficients"] >> distortion;


	Mat rvec, tvec;
	solvePnP(markerPoints, Mat(corners), intrinsics,
			 distortion, rvec, tvec, false);

	return tvec.at<double>(2);
}

void drawDistance(Mat& frame, const vector<Point2f>& corners, double distance)
{
	if(corners.size() <= 0)
		return;

	stringstream ss;
	ss << fixed << setprecision(3) << distance*0.393701<< "INCH";

	putText(frame, ss.str(), corners[2], 0, 1.0, Scalar(0, 255, 255), 3);
}