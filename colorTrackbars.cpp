#include <opencv2\opencv.hpp>

#include "colorTrackbars.h"

#include <iostream>

using namespace cv;
using namespace std;

void createHSVTrackbars() {
	VideoCapture vCap(0);
	Mat img, imgInHSV, imgColor, mask;
	int hueMin = 0, saturationMin = 0, valueMin = 0;
	int hueMax = 0, saturationMax = 0, valueMax = 0;

	string name = "Color Trackbars";
	namedWindow(name, (300, 200));
	createTrackbar("Hmin", name, &hueMin, 179);
	createTrackbar("Hmax", name, &hueMax, 179);
	createTrackbar("Smin", name, &saturationMin, 255);
	createTrackbar("Smax", name, &saturationMax, 255);
	createTrackbar("Vmin", name, &valueMin, 255);
	createTrackbar("Vmax", name, &valueMax, 255);

	while (1) {
		vCap.read(img);
		cvtColor(img, imgInHSV, COLOR_BGR2HSV);

		Scalar mins(hueMin, saturationMin, valueMin);
		Scalar maxes(hueMax, saturationMax, valueMax);
		inRange(imgInHSV, mins, maxes, mask);
		cout << hueMin << "," << saturationMin << "," << valueMin << "," <<
			hueMax << "," << saturationMax << "," << valueMax<<'\n';
		imshow("IMG", img);
		imshow("Mask", mask);
		waitKey(30);
	}


}