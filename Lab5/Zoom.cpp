#include "Zoom.h"
#include <iostream>
using namespace cv;
using namespace std;

Zoom::Zoom(Mat& src, Mat& dst, float vergroting, int x, int y) {
    this->src = src;
    this->dst = dst;
    HEIGHT = src.rows;
    WIDTH = src.cols;
    this->vergroting = vergroting;
    this->x = x;
    this->y = y;
}

void Zoom::process(void) {

    if (vergroting == 0 || vergroting == 1 || vergroting == -1) {
        for (int h = 0; h < HEIGHT; h++) {
            for (int w = 0; w < WIDTH; w++) {
                dst.at<uchar>(h, w) = src.at<uchar>(h, w);
            }
        }
    }

    else if (vergroting < 0) {
        vergroting = -vergroting;

        int breedte = 640;
        int hoogte = 480;

        x = WIDTH / 2;
        y = HEIGHT / 2;

        float newcenterx = x - (WIDTH / vergroting) / 2;
        float newcentery = y - (HEIGHT / vergroting) / 2;

        float verschuiving[3][3]{
            {1,0,newcenterx},
            {0,1,newcentery},
            {0,0,1}
        };

        float zoomer = 1 / vergroting;

        float zoom[3][3]{
            {zoomer,0,0},
            {0,zoomer,0},
            {0,0,1}
        };

        Mat tmp = Mat::ones(480, 640, CV_8U) * 255;

        for (int h = 0; h < HEIGHT; h++) {
            for (int w = 0; w < WIDTH; w++) {
                int nieuww = (float)(zoom[0][0] * h) + (float)(zoom[0][1] * w) + (float)(zoom[0][2] * 1);
                int nieuwh = (float)(zoom[1][0] * h) + (float)(zoom[1][1] * w) + (float)(zoom[1][2] * 1);

                if (nieuww >= 0 && nieuwh >= 0 && nieuww < WIDTH && nieuwh < HEIGHT) {
                    tmp.at<uchar>(nieuwh, nieuww) = src.at<uchar>(h, w);
                }
            }
        }

        for (int h = 0; h < HEIGHT; h++) {
            for (int w = 0; w < WIDTH; w++) {
                float nieuww = (verschuiving[0][0] * h) + (verschuiving[0][1] * w) + (verschuiving[0][2] * 1);
                float nieuwh = (verschuiving[1][0] * h) + (verschuiving[1][1] * w) + (verschuiving[1][2] * 1);

                if (nieuww >= 0 && nieuwh >= 0 && nieuww < WIDTH && nieuwh < HEIGHT) {
                    dst.at<uchar>(nieuwh, nieuww) = tmp.at<uchar>(h, w);
                }
            }
        }
    }

    else {
        float newcenterx = x - (WIDTH / vergroting) / 2;
        float newcentery = y - (HEIGHT / vergroting) / 2;

        float verschuiving[3][3]{
            {1,0,-newcenterx},
            {0,1,-newcentery},
            {0,0,1}
        };

        float zoomer = 1 / vergroting;

        float zoom[3][3]{
            {zoomer,0,0},
            {0,zoomer,0},
            {0,0,1}
        };

        Mat tmp = Mat::ones(HEIGHT, WIDTH, CV_8U) * 0;

        for (int h = 0; h < HEIGHT; h++) {
            for (int w = 0; w < WIDTH; w++) {
                float nieuww = (verschuiving[0][0] * h) + (verschuiving[0][1] * w) + (verschuiving[0][2] * 1);
                float nieuwh = (verschuiving[1][0] * h) + (verschuiving[1][1] * w) + (verschuiving[1][2] * 1);

                if (nieuww >= 0 && nieuwh >= 0 && nieuww < WIDTH && nieuwh < HEIGHT) {
                    tmp.at<uchar>(nieuwh, nieuww) = src.at<uchar>(h, w);
                }
            }
        }

        for (int h = 0; h < HEIGHT; h++) {
            for (int w = 0; w < WIDTH; w++) {
                int oudw = (float)(zoom[0][0] * h) + (float)(zoom[0][1] * w) + (float)(zoom[0][2] * 1);
                int oudh = (float)(zoom[1][0] * h) + (float)(zoom[1][1] * w) + (float)(zoom[1][2] * 1);

                if (oudw >= 0 && oudh >= 0 && oudw < WIDTH && oudh < HEIGHT) {
                    dst.at<uchar>(h, w) = tmp.at<uchar>(oudh, oudw);
                }
            }
        }
    }
}