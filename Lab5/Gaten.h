#pragma once
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

class Gaten {
public:
	Gaten(Mat& src);
	void process(void);
private:
	Mat src;
	int HEIGHT, WIDTH;
};

