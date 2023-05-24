#include "Mier.h"

Mier::Mier(Mat& src1, Mat& src2, Mat& dst) {
    this->src1 = src1;
    this->src2 = src2;
    this->dst = dst;
    HEIGHT = src1.rows;
    WIDTH = src1.cols;
}

void XOR(Mat& src1, Mat& src2, Mat& dst) {
    if (src1.rows == src2.rows && src1.cols == src2.cols) {
        for (int h = 0; h < src1.rows; h++) {
            for (int w = 0; w < src1.cols; w++) {
                dst.at<uchar>(h, w) = src2.at<uchar>(h, w) ^ src1.at<uchar>(h, w);
            }
        }
    }
}

void Erosion(Mat& src, Mat& dst) {
    for (int h = 0; h < src.rows; h++) {
        for (int w = 0; w < src.cols; w++) {
            int som = 0;
            for (int x = -1; x < 2; x++) {
                for (int y = -1; y < 2; y++) {
                    int som1 = h + y;
                    int som2 = w + x;
                    if (som1 < 0 || som2 < 0 || som1 >= src.rows || som2 >= src.cols) {
                        continue;
                    }
                    if (src.at<uchar>(som1, som2) == 0) {
                        som++;
                    }
                }
            }
            if (som > 0) {
                dst.at<uchar>(h, w) = 0;
            }
            else {
                dst.at<uchar>(h, w) = 255;
            }
        }
    }
}

void Dilation(Mat& src, Mat& dst) {
    for (int h = 0; h < src.rows; h++) {
        for (int w = 0; w < src.cols; w++) {
            int som = 0;
            for (int x = -1; x < 2; x++) {
                for (int y = -1; y < 2; y++) {
                    int som1 = h + y;
                    int som2 = w + x;
                    if (som1 < 0 || som2 < 0 || som1 >= src.rows || som2 >= src.cols) {
                        continue;
                    }
                    if (src.at<uchar>(som1, som2) == 255) {
                        som++;
                    }
                }
            }
            if (som > 0) {
                dst.at<uchar>(h, w) = 255;
            }
            else {
                dst.at<uchar>(h, w) = 0;
            }
        }
    }
}

void Mier::process(void) {
    Mat tmp1 = Mat::ones(HEIGHT, WIDTH, CV_8U) * 0;
    XOR(src1, src2, tmp1);
    Mat tmp2 = Mat::ones(HEIGHT, WIDTH, CV_8U) * 0;
    Erosion(tmp1, tmp2);
    Mat tmp3 = Mat::ones(HEIGHT, WIDTH, CV_8U) * 0;
    Erosion(tmp2, tmp3);
    Dilation(tmp3, dst);
}