#include "Filter.h"
#include <iostream>
using namespace cv;
using namespace std;

Filter::Filter(Mat& src, Mat& dst) {
    this->src = src;
    this->dst = dst;
    HEIGHT = src.rows;
    WIDTH = src.cols;
}

void Filter::hoogdoorlaat(void) {
    int kernel[3][3] =
    {
        {-1, -1, -1},
        {-1, 9, -1},
        {-1, -1, -1}
    };

    for (int h = 0; h < HEIGHT; h++) {
        for (int w = 0; w < WIDTH; w++) {
            int som = 0;

            for (int y = 0; y < 3; y++) {
                for (int x = 0; x < 3; x++) {
                    int i = h + y - 1;
                    int j = w + x - 1;

                    if (i < 0) {
                        i = 0;
                    }
                    else if (i >= HEIGHT) {
                        i = HEIGHT - 1;
                    }
                    if (j < 0) {
                        j = 0;
                    }
                    else if (j >= WIDTH) {
                        j = WIDTH - 1;
                    }

                    som = som + kernel[y][x] * src.at<uchar>(i, j);
                }
            }
            if (som < 0) {
                som = 0;
            }
            else if (som > 255) {
                som = 255;
            }

            dst.at<uchar>(h, w) = som;
        }
    }
}

void Filter::laagdoorlaat(void) {
    int kernel[3][3]{
        {1, 1, 1},
        {1, 1, 1},
        {1, 1, 1}
    };

    for (int h = 0; h < HEIGHT; h++) {
        for (int w = 0; w < WIDTH; w++) {
            int som = 0;

            for (int y = 0; y < 3; y++) {
                for (int x = 0; x < 3; x++) {
                    int i = h + y - 1;
                    int j = w + x - 1;

                    if (i < 0) {
                        i = 0;
                    }
                    else if (i >= HEIGHT) {
                        i = HEIGHT - 1;
                    }
                    if (j < 0) {
                        j = 0;
                    }
                    else if (j >= WIDTH) {
                        j = WIDTH - 1;
                    }

                    som = som + kernel[y][x] * src.at<uchar>(i, j);
                }
            }
            som = som / 9;
            if (som < 0) {
                som = 0;
            }
            else if (som > 255) {
                som = 255;
            }

            dst.at<uchar>(h, w) = som;
        }
    }
}

void Filter::mediaan(void) {
    int array[9] = { 0 };

    for (int h = 0; h < HEIGHT; h++) {
        for (int w = 0; w < WIDTH; w++) {
            int teller = 0;

            for (int x = -1; x < 2; x++) {
                for (int y = -1; y < 2; y++) {
                    int som1 = h + x;
                    int som2 = w + y;
                    if (som1 < 0) {
                        som1 = 0;
                    }
                    else if (som1 >= HEIGHT) {
                        som1 = HEIGHT - 1;
                    }
                    if (som2 < 0) {
                        som2 = 0;
                    }
                    else if (som2 >= WIDTH) {
                        som2 = WIDTH - 1;
                    }
                    array[teller] = src.at<uchar>(som1, som2);
                    teller++;
                }
            }

            sort(begin(array), end(array));
            dst.at<uchar>(h, w) = array[4];
        }
    }
}