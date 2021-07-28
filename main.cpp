#include <opencv2\opencv.hpp>
#include "colorTrackbars.h"
using namespace cv;
using namespace std;

Mat img;
VideoCapture vCap(0);
vector<vector<int>> allPoints;

vector<vector<int>> colorsToDetect{ {0,0,0,45,213,167}, {0,0,3,55,5,55} };
vector<Scalar> colorsToDisplay{ {255,0,0}, {0,255,0} };

Point detectContours(Mat inputImage) {
	vector<vector<Point>> contours;
	int contoursSize = (int)contours.size();
	vector<Vec4i> order;

	findContours(inputImage, contours, order, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	drawContours(img, contours, -1, Scalar(255, 0, 255), 2);
	vector<Rect> boundingBox(contoursSize);
	vector<vector<Point>> contoursPolyline(contoursSize);
	Point outputPoint;

	for (int i = 0; i < contoursSize; i++) {
		int area = contourArea(contours[i]);
		cout << area << '\n';
		string objType;

		if (area > 1000) {
			float peripherals = arcLength(contours[i], true);
			approxPolyDP(contours[i], contoursPolyline[i], 0.03 * peripherals, true);
			cout << contoursPolyline[i].size() << '\n';
			boundingBox[i] = boundingRect(contoursPolyline[i]);

			outputPoint.x = boundingBox[i].x + boundingBox[i].width / 2;
			outputPoint.y = boundingBox[i].y;
		}

	}

	return outputPoint;
}

vector<vector<int>> detectColor(Mat inputImage) {
	Mat imageInHSV;
	cvtColor(inputImage, imageInHSV, COLOR_BGR2HSV);
	int colorsSize = (int)colorsToDetect.size();
	for (int i = 0; i < colorsSize; i++) {
		Scalar bottom(colorsToDetect[i][0], colorsToDetect[i][1], colorsToDetect[i][2]);
		Scalar up(colorsToDetect[i][3], colorsToDetect[i][4], colorsToDetect[i][5]);
		Mat mask;
		inRange(imageInHSV, bottom, up, mask);
		Point tempPoint = detectContours(mask);
		if (tempPoint.x != 0)
			allPoints.push_back({ tempPoint.x, tempPoint.y, i });
	}

	return allPoints;
}

void draw(vector<vector<int>> allPoints, vector<Scalar> colorsToDetect) {
	int pointsLength = (int)allPoints.size();
	for (int i = 0; i < pointsLength; i++) {
		circle(img, Point(allPoints[i][0], allPoints[i][1]), 10, colorsToDisplay[allPoints[i][2]], FILLED);
	}
}


int main() {
	createHSVTrackbars();
	if (!vCap.isOpened()) return -1;
	
	while(1) {
		vCap.read(img);

		allPoints = detectColor(img);
		draw(allPoints, colorsToDisplay);

		imshow("Virtual Painter", img);
		if(waitKey(30)>=0) break;
	}

	return 0;
}
