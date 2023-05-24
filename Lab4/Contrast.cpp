#include "Contrast.h"
#include <iostream>
using namespace cv;
using namespace std;

Contrast::Contrast(Mat& src, Mat& dst) {
    this->src = src;
    this->dst = dst;
    HEIGHT = src.rows;
    WIDTH = src.cols;
}

void Contrast::process(void) {
    int h, w, temp = 0;
    int mapping[256] = { 0 };
    for (h = 0; h < HEIGHT; h++) {
        for (w = 0; w < WIDTH; w++) {

            temp = src.at<uchar>(h, w);
            //temp = *src.ptr(h, w); // hetzelfde als hierboven, maar op een andere manier.
            mapping[temp]++;
        }
    }

    //min value
    int min = 0;
    for (int i = 0; i < 256; i++) {
        if (mapping[i] > 100) {
            min = i;
            break;
        }
    }
    
    //max value
    int max = 255;
    for (int i = 255; i > 0; i--) {
        if (mapping[i] > 100) {
            max = i;
            break;
        }
    }

    int bereik = max - min;

    cout << "Min = " << min << endl;
    cout << "Max = " << max << endl;
    cout << "Bereik = " << bereik << endl;

    for (h = 0; h < HEIGHT; h++) {
        for (w = 0; w < WIDTH; w++) {

            temp = src.at<uchar>(h, w);
            //temp = *src.ptr(h, w); // hetzelfde als hierboven, maar op een andere manier.
            int nieuw = (float)(temp - min) / bereik * 255;
            if (nieuw > 255) {
                nieuw = 255;
            }
            else if (nieuw < 0) {
                nieuw = 0;
            }

            dst.at<uchar>(h, w) = nieuw;//HEIGHT - h - 1, w) = temp;
            //*dst.ptr(HEIGHT - h - 1, w) = temp; // hetzelfde als hierboven, maar op een andere manier.
        }
    }
}