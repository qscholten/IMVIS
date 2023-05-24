#pragma once
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

class Filter {
public:
	Filter(Mat& src, Mat& dst);
	void hoogdoorlaat(void);
	void laagdoorlaat(void);
	void mediaan(void);
private:
	Mat src, dst;
	int HEIGHT, WIDTH;
};

