#pragma once
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

class Rotate {
public:
	Rotate(Mat& src, Mat& dst, float rotatie);
	void process(void);
private:
	Mat src, dst;
	int HEIGHT, WIDTH;
	float rotatie;
};