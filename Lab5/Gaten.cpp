#include "Gaten.h"
#include <iostream>
using namespace cv;
using namespace std;

Gaten::Gaten(Mat& src) {
    this->src = src;
    HEIGHT = src.rows;
    WIDTH = src.cols;
}

void Gaten::process(void) {
    int binnenhoeken = 0;
    int binnenhoek[4][4]{
        {0, 255, 255, 255},
        {255, 0, 255, 255},
        {255, 255, 0, 255},
        {255, 255, 255, 0}
    };

    int buitenhoeken = 0;
    int buitenhoek[4][4]{
        {255, 0, 0, 0},
        {0, 255, 0, 0},
        {0, 0, 255, 0},
        {0, 0, 0, 255}
    };

    for (int h = 0; h < HEIGHT; h++) {
        for (int w = 0; w < WIDTH; w++) {
            for (int rand = 0; rand < 4; rand++) {
                if (h + 1 < HEIGHT && w + 1 < WIDTH) {
                    if (src.at<uchar>(h, w) == binnenhoek[rand][0] && src.at<uchar>(h + 1, w) == binnenhoek[rand][1] &&
                        src.at<uchar>(h, w + 1) == binnenhoek[rand][2] && src.at<uchar>(h + 1, w + 1) == binnenhoek[rand][3]) {
                        binnenhoeken++;
                    }
                    if (src.at <uchar>(h, w) == buitenhoek[rand][0] && src.at<uchar>(h + 1, w) == buitenhoek[rand][1] &&
                        src.at<uchar>(h, w + 1) == buitenhoek[rand][2] && src.at<uchar>(h + 1, w + 1) == buitenhoek[rand][3]) {
                        buitenhoeken++;
                    }
                }
            }
        }
    }

    float som = (buitenhoeken - binnenhoeken) / 4;

    cout << "Gaten: " << som << endl;
}