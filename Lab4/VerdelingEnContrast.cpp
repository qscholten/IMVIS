#include "VerdelingEnContrast.h"
#include <iostream>
using namespace cv;
using namespace std;

VerdelingEnContrast::VerdelingEnContrast(Mat& src, Mat& dst) {
    this->src = src;
    this->dst = dst;
    HEIGHT = src.rows;
    WIDTH = src.cols;
}

void VerdelingEnContrast::process(void) {
    float mystery[2][2]{
        {0.2, 0.1},
        {0, 0.2}
    };

    for (int h = 0; h < HEIGHT; h++) {
        for (int w = 0; w < WIDTH; w++) {
            float nieuww = (mystery[0][0] * w) + (mystery[0][1] * h) + 300;
            float nieuwh = (mystery[1][0] * w) + (mystery[1][1] * h) + 200;
            dst.at<uchar>(nieuwh, nieuww) = src.at<uchar>(h, w);
        }
    }
}