#include "GatenLabel.h"
#include <iostream>
using namespace cv;
using namespace std;

int label;
 
GatenLabel::GatenLabel(Mat& src, Mat& dst) {
    this->src = src;
    this->dst = dst;
    HEIGHT = src.rows;
    WIDTH = src.cols;
}

int GatenLabel::labelPixel(int h, int w) {

    if ((src.at<uchar>(h, w) == 255) && (dst.at<uchar>(h, w) == 0)) {
        dst.at<uchar>(h, w) = label;
        for (int i = -1; i < 2; i++) {
            for (int j = -1; j < 2; j++) {
                if (((i != 0) || (j != 0)) && (h + i >= 0) && (h + i < HEIGHT) && (w + j >= 0) && (w + j < WIDTH)) {
                    labelPixel(h + i, w + j);
                }
            }
        }
        return 255;
    }
    return 0;
}

void GatenLabel::process(void) {
    label = 1;
    for (int h = 0; h < HEIGHT; h++) {
        for (int w = 0; w < WIDTH; w++) {
            if (labelPixel(h, w) != 0 && label < 254) {
                label++;
            }
        }
    }
    cout << "Gaten: " << label << endl;
}