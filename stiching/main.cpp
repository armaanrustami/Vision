//
//
//#include "opencv2/core/core.hpp"
//#include "opencv2/imgproc/imgproc.hpp"
//#include "opencv2/calib3d/calib3d.hpp"
//#include "opencv2/highgui/highgui.hpp"
//
//#include <stdio.h>
//#include <iostream>
//#include "opencv2/core/core.hpp"
//#include "opencv2/features2d/features2d.hpp"
//#include "opencv2/nonfree/features2d.hpp"
//#include "opencv2/nonfree/nonfree.hpp"
// 
//#include <cctype>
//#include <stdio.h>
//#include <string.h>
//#include <time.h>
//#include <opencv2/stitching/stitcher.hpp>
//
//using namespace cv;
//using namespace std;
//
//
//int main(int argc, char** argv) {
//cv::Mat b1 = imread("building1.JPG",CV_LOAD_IMAGE_UNCHANGED);
//cv::Mat b2 = imread("building2.JPG",CV_LOAD_IMAGE_UNCHANGED);
//cv::Mat b3 = imread("building3.JPG",CV_LOAD_IMAGE_UNCHANGED);
//cv::Mat b4 = imread("building4.JPG",CV_LOAD_IMAGE_UNCHANGED);
//cv::Mat b5 = imread("building5.JPG",CV_LOAD_IMAGE_UNCHANGED);
//
//vector< Mat > imageList;
//	
//	vector< vector< Rect > > vvRect;
//	Mat result;
//
//	imageList.push_back( b1 );	
//	imageList.push_back( b2);
//	imageList.push_back( b3 );	
//        imageList.push_back( b4);
//	imageList.push_back( b5 );	
//	Stitcher stitcher = Stitcher::createDefault(1);
//	stitcher.stitch(imageList, result);
//	imshow("Stitching Result", result);
//waitKey(0);
//
//}

#include <stdio.h>
#include <iostream>

#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/stitching/stitcher.hpp>

using namespace cv;
using namespace std;
Mat stict(Mat image1, Mat image2);
Mat stict1(Mat image1, Mat image2);

/** @function main */
int main(int argc, char** argv) {


    // Load the images
    Mat b2 = imread("building2.JPG");
    Mat b1 = imread("building1.JPG");
    Mat block1 = stict(b2, b1);
    cv::Mat half(block1, cv::Rect(0, 0, b1.cols, b1.rows));
    b1.copyTo(half);

    imshow("Block1", block1);

    //    
    Mat b4 = imread("building4.JPG");
    Mat b3 = imread("building3.JPG");
    Mat block2=stict( b4, b3);
     cv::Mat half1(block2,cv::Rect(0,0,b3.cols,b3.rows));
     b3.copyTo(half1);
     imshow( "Block2", block2 );
    // 

    //Mat block3=stict( block2, block1);
    // cv::Mat half3(block3,cv::Rect(0,0,block1.cols,block1.rows));
    // block1.copyTo(half3);
    // imshow( "Block3", block3 );
    // 
    // 
    Mat b5 = imread("building5.JPG");
    Mat block4 = stict(b5, b4);
    cv::Mat half4(block4, cv::Rect(0, 0, b4.cols, b4.rows));
    b4.copyTo(half4);
    imshow("Block4", block4);
 
    Mat block7 = stict1(block4,b3);
    cv::Mat half7(block7, cv::Rect(0, 0, b3.cols, b3.rows));
    b3.copyTo(half7);
    imshow("Block7", block7);

    
       Mat block8 = stict(block7,b2);
    cv::Mat half8(block8, cv::Rect(0, 0, b2.cols, b2.rows));
    b2.copyTo(half8);
    imshow("Block8", block8);

       Mat block9 = stict1(block8,b1);
    cv::Mat half9(block9, cv::Rect(0, 0, b1.cols, b1.rows));
    b1.copyTo(half9);   
//    imshow("Final", block9);
    
    Stitcher st=Stitcher::createDefault(true);
    Mat pano,pano1;
    vector< Mat> blocks;
    blocks.push_back(block1);
    blocks.push_back(block2);
    blocks.push_back(block4);
    st.estimateTransform(blocks);
    st.composePanorama(blocks,pano);
    imshow("Panorama",pano);
    waitKey(0);
    return 0;
}

Mat stict(Mat image1, Mat image2) {


    Mat gray_image1;
    Mat gray_image2;
    // Convert to Grayscale
    cvtColor(image1, gray_image1, CV_RGB2GRAY);
    cvtColor(image2, gray_image2, CV_RGB2GRAY);

    imshow("first image", image2);
    imshow("second image", image1);


    //-- Step 1: Detect the keypoints using SURF Detector
    int minHessian = 100;

    SurfFeatureDetector detector(minHessian);

    std::vector< KeyPoint > keypoints_object, keypoints_scene;

    detector.detect(gray_image1, keypoints_object);
    detector.detect(gray_image2, keypoints_scene);

    //-- Step 2: Calculate descriptors (feature vectors)
    SurfDescriptorExtractor extractor;

    Mat descriptors_object, descriptors_scene;

    extractor.compute(gray_image1, keypoints_object, descriptors_object);
    extractor.compute(gray_image2, keypoints_scene, descriptors_scene);

    //-- Step 3: Matching descriptor vectors using FLANN matcher
    FlannBasedMatcher matcher;
    std::vector< DMatch > matches;
    matcher.match(descriptors_object, descriptors_scene, matches);

    double max_dist = 0;
    double min_dist = 100;

    //-- Quick calculation of max and min distances between keypoints
    for (int i = 0; i < descriptors_object.rows; i++) {
        double dist = matches[i].distance;
        if (dist < min_dist) min_dist = dist;
        if (dist > max_dist) max_dist = dist;
    }

    printf("-- Max dist : %f \n", max_dist);
    printf("-- Min dist : %f \n", min_dist);

    //-- Use only "good" matches (i.e. whose distance is less than 3*min_dist )
    std::vector< DMatch > good_matches;

    for (int i = 0; i < descriptors_object.rows; i++) {
        if (matches[i].distance < 3 * min_dist) {
            good_matches.push_back(matches[i]);
        }
    }
    std::vector< Point2f > obj;
    std::vector< Point2f > scene;

    for (int i = 0; i < good_matches.size(); i++) {
        //-- Get the keypoints from the good matches
        obj.push_back(keypoints_object[ good_matches[i].queryIdx ].pt);
        scene.push_back(keypoints_scene[ good_matches[i].trainIdx ].pt);
    }

    // Find the Homography Matrix
    Mat H = findHomography(obj, scene, CV_RANSAC);
    // Use the Homography Matrix to warp the images
    cout << H << endl;
    cv::Mat result;
    warpPerspective(image1, result, H, cv::Size(image1.cols + image2.cols - 320, image1.rows));





    return result;




}

Mat stict1(Mat image1, Mat image2) {


    Mat gray_image1;
    Mat gray_image2;
    // Convert to Grayscale
    cvtColor(image1, gray_image1, CV_RGB2GRAY);
    cvtColor(image2, gray_image2, CV_RGB2GRAY);

    imshow("first image", image2);
    imshow("second image", image1);


    //-- Step 1: Detect the keypoints using SURF Detector
    int minHessian = 400;

    SurfFeatureDetector detector(minHessian);

    std::vector< KeyPoint > keypoints_object, keypoints_scene;

    detector.detect(gray_image1, keypoints_object);
    detector.detect(gray_image2, keypoints_scene);

    //-- Step 2: Calculate descriptors (feature vectors)
    SurfDescriptorExtractor extractor;

    Mat descriptors_object, descriptors_scene;

    extractor.compute(gray_image1, keypoints_object, descriptors_object);
    extractor.compute(gray_image2, keypoints_scene, descriptors_scene);

    //-- Step 3: Matching descriptor vectors using FLANN matcher
    FlannBasedMatcher matcher;
    std::vector< DMatch > matches;
    matcher.match(descriptors_object, descriptors_scene, matches);

    double max_dist = 0;
    double min_dist = 100;

    //-- Quick calculation of max and min distances between keypoints
    for (int i = 0; i < descriptors_object.rows; i++) {
        double dist = matches[i].distance;
        if (dist < min_dist) min_dist = dist;
        if (dist > max_dist) max_dist = dist;
    }

    printf("-- Max dist : %f \n", max_dist);
    printf("-- Min dist : %f \n", min_dist);

    //-- Use only "good" matches (i.e. whose distance is less than 3*min_dist )
    std::vector< DMatch > good_matches;

    for (int i = 0; i < descriptors_object.rows; i++) {
        if (matches[i].distance < 3 * min_dist) {
            good_matches.push_back(matches[i]);
        }
    }
    std::vector< Point2f > obj;
    std::vector< Point2f > scene;

    for (int i = 0; i < good_matches.size(); i++) {
        //-- Get the keypoints from the good matches
        obj.push_back(keypoints_object[ good_matches[i].queryIdx ].pt);
        scene.push_back(keypoints_scene[ good_matches[i].trainIdx ].pt);
    }

    // Find the Homography Matrix
    Mat H = findHomography(obj, scene, CV_RANSAC);
    // Use the Homography Matrix to warp the images
    cout << H << endl;
    cv::Mat result;
    warpPerspective(image1, result, H, cv::Size(image1.cols + image2.cols, image1.rows + 100));





    return result;




}

