#include "Rotate.h"
#include <iostream>
#include <cmath>
#define _USE_MATH_DEFINES
#include <math.h>

Rotate::Rotate(Mat& src, Mat& dst, float rotatie) {
    this->src = src;
    this->dst = dst;
    HEIGHT = src.rows;
    WIDTH = src.cols;
    this->rotatie = rotatie/180 * M_PI;
}

void Rotate::process(void) {

    float roteren[3][3]{
        {cos(-rotatie), -sin(-rotatie), 0},
        {sin(-rotatie), cos(-rotatie), 0},
        (0, 0, 1)
    };

    for (int h = 0; h < HEIGHT; h++) {
        for (int w = 0; w < WIDTH; w++) {
            int oudh = (roteren[0][0] * h) + (roteren[0][1] * w) + (roteren[0][2] * 1);
            int oudw = (roteren[1][0] * h) + (roteren[1][1] * w) + (roteren[1][2] * 1);

            if (oudw >= 0 && oudh >= 0 && oudw < WIDTH && oudh < HEIGHT) {
                dst.at<uchar>(h, w) = src.at<uchar>(oudh, oudw);
            }
        }
    }
}