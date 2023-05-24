#pragma once
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

class GatenLabel {
public:
	GatenLabel(Mat& src, Mat& dst);
	int labelPixel(int h, int w);
	void process(void);
private:
	Mat src, dst;
	int HEIGHT, WIDTH;
};

