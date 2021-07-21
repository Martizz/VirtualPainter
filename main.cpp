#include <opencv2\opencv.hpp>

using namespace cv;
using namespace std;

int main() {
	//Access to the camera
	VideoCapture vCap(0);
	if (!vCap.isOpened()) return -1;
	
	for (;;) {
		Mat frame;
		vCap >> frame;
		imshow("Virtual Painter", frame);
		if(waitKey(30)>=0) break;
	}

	return 0;
}
