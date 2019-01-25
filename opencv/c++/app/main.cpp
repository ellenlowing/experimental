/*
    CS585 Project: Gesture-based text entry interface

    Implementation:
      1. Extract bright regions in image with threshold
      2. Detect larger blob in thresholded binary image
      3. Use center of blob to draw
      3.5 Filter point if it is too far from previous point (not yet)
      4. Connect previous and current track point with line to get continuous stroke
*/

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include <cmath>
#include <vector>
#include <time.h>
#include <cstring>
#include <string>
using namespace std;
using namespace cv;

int w, h;
/**
    Extracts bright regions in image.

    @param gray Input image in grayscale
    @param thresh Output thresholded image
*/
void extractBright(Mat& gray, Mat& thresh) {
  thresh = gray > 253;
  int erosion_size = 3;
  int dilation_size = 3;
  Mat element = getStructuringElement(MORPH_ELLIPSE,
    Size(2 * erosion_size + 1, 2 * erosion_size + 1),
    Point(erosion_size, erosion_size));
  erode(thresh, thresh, element);
  dilate(thresh, thresh, element);
}

/**
    Gets largest blob in image if area takes up more than __% of image.

    @param thresh Input image in binary
    @param blob Output image in binary
*/
void getLargestBlob(Mat& thresh, Mat& blob) {
  Mat binary;
  threshold(thresh, binary, 0.0, 1.0, THRESH_BINARY);
  vector<vector<Point> > contours;
  vector<Vec4i> hierarchy;
  findContours(binary, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
  int maxsize = 0;
  int maxind = 0;
  for (int i = 0; i < contours.size(); i++)
  {
    double area = contourArea(contours[i]);
    if (area > maxsize) {
      maxsize = area;
      maxind = i;
    }
  }
  // cout << "The area of the largest contour detected is: " << contourArea(contours[maxind]) << endl;
  // cout << "The percentage of the largest contour in image: " << maxsize / double(w*h) << endl;
  // cout << "-----------------------------" << endl << endl;

  double percent = maxsize / double(w * h);
  if(percent > 0.001) drawContours(blob, contours, maxind, Scalar(255, 255, 255), CV_FILLED, 8, hierarchy);
}

/**
    Gets centroid of blob and flips point with respect to y-axis.

    @param blob Input image in binary
    @param centroid Output point
*/
void getCentroidofBlob(Mat& blob, Point& centroid) {
  Moments m = moments(blob, true);
  centroid = Point(m.m10/m.m00, m.m01/m.m00);
  int distToCenter = centroid.x - blob.cols/2;
  centroid.x = centroid.x - distToCenter * 2;
}

/**
    Draws line between previous and current tracked point.

    @param canvas Input image to draw on
    @param dot Input current tracked point
    @param prev Input previous tracked point
*/
void connectTheDots(Mat& canvas, Point& dot, Point& prev) {
  line(canvas, prev, dot, Scalar(255, 255, 255), 4, 8, 0);
}

/**
    Saves input image.

    @param canvas Input image to clear
*/
void saveCanvas(Mat& canvas) {
  // filename based on hour, minute, second
  time_t rawtime;
  struct tm * timeinfo;
  char filename_buf [7];
  time (&rawtime);
  timeinfo = localtime (&rawtime);
  strftime (filename_buf,7,"%H%M%S",timeinfo);
  string filename = filename_buf;

  // crop out character only
  Moments m = moments(canvas, true);
  Point centroid = Point(m.m10/m.m00, m.m01/m.m00);
  float ratio = m.m00 / (w * h);
  int width = ((h * ratio * 64) > h) ? h : h * ratio * 64;
  Rect rect(centroid.x - width/2, centroid.y - width/2, width, width);
  rectangle(canvas, rect, Scalar(255, 255, 255), 3, 8, 0);
  Mat crop = canvas(rect);
  imwrite("output/" + filename + ".jpg", crop);
}

/**
    Clears input image.

    @param canvas Input image to clear
*/
void clearCanvas(Mat& canvas) {
  canvas = Mat::zeros(canvas.size(), CV_8UC1);
}

/**
    Gets distance between two points

    @param a First point
    @param b Second point
*/
int getDistanceBetweenPoints(Point a, Point b) {
  return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

int main(int, char**)
{
    VideoCapture cap(0); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
        return -1;

    Mat frame;
    cap >> frame;
    resize(frame, frame, Size(640, 360), 0, 0, INTER_CUBIC);
    w = frame.cols;
    h = frame.rows;
    Mat canvas = Mat::zeros(frame.size(), CV_8UC1);
    Point prevCentroid;
    bool init = false;

    while(1) {
        // Get frame from video capture and extract bright regions from image
        Mat gray;
        cap >> frame;
        resize(frame, frame, Size(640, 360), 0, 0, INTER_CUBIC);
        cvtColor(frame, gray, COLOR_BGR2GRAY);
        Mat thresh;
        extractBright(gray, thresh);
        // imshow("threshold", thresh);

        // Get largest blob in thresholded image
        Mat blob = Mat::zeros(frame.size(), CV_8UC1);
        getLargestBlob(thresh, blob);
        // imshow("largest blob", blob);

        // Get center of blob, flip it in y-axis, and draw on output
        Point centroid;
        getCentroidofBlob(blob, centroid);
        if(init && getDistanceBetweenPoints(prevCentroid, centroid) < 100 && prevCentroid.x > 0 && prevCentroid.y > 0 && centroid.x > 0 && centroid.y > 0) connectTheDots(canvas, centroid, prevCentroid);
        prevCentroid = centroid;
        // imshow("output", canvas);

        // Combine windows for graphical output
        Mat output(360, 640*2, CV_8UC1);
        blob.copyTo(output(Rect(0, 0, 640, 360)));
        canvas.copyTo(output(Rect(640, 0, 640, 360)));
        imshow("output", output);

        if(!init) init = true;
        int c = waitKey(5);
        if(c == 32) clearCanvas(canvas);
        else if(c == 115) saveCanvas(canvas);
        else if(c == 113) break;
    }

    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}
