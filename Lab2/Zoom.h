#pragma once
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

class Zoom {
public:
	Zoom(Mat& src, Mat& dst, float vergroting, int x, int y);
	void process(void);
private:
	Mat src, dst;
	int HEIGHT, WIDTH;
	float vergroting;
	int x, y;
};

