#pragma once
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

class VerdelingEnContrast {
public:
	VerdelingEnContrast(Mat& src, Mat& dst);
	void process(void);
private:
	Mat src, dst;
	int HEIGHT, WIDTH;
	float vergroting;
	int x, y;
};

