
#include <stdio.h>
#include <stdlib.h>

#include "opencv2/opencv.hpp"
using namespace cv;
 
Mat api() {
    Mat img, gray;
    img = imread("data/rgb.png", IMREAD_COLOR);

    cvtColor(img, gray, COLOR_BGR2GRAY);
    GaussianBlur(gray, gray,Size(5, 5), 1.5);
    Canny(gray, gray, 0, 50);

    return gray;
}

UMat tapi() {
    UMat img, gray;
    imread("data/rgb.png", IMREAD_COLOR).copyTo(img);

    cvtColor(img, gray, COLOR_BGR2GRAY);
    GaussianBlur(gray, gray,Size(5, 5), 1.5);
    Canny(gray, gray, 0, 50);

    return gray;
}

int main(int argc, char** argv)
{
    uint64_t t1 = getTickCount();
    Mat apiRes = api();
    int apiTime = (getTickCount() - t1) / getTickFrequency() * 1000;
    t1 = getTickCount();
    UMat tapiRes = tapi();
    int tapiTime = (getTickCount() - t1) / getTickFrequency() * 1000;

    printf("Time cost: \n\tapi: %d ms\n\ttapi: %d ms\n", apiTime, tapiTime);

    imshow("api", apiRes);
    imshow("tapi", tapiRes);

    waitKey();
    return 0;
}