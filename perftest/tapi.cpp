
#include <stdio.h>
#include <stdlib.h>

#include <opencv2/opencv.hpp>

#define LOG_TAG     "TAPI"
#include "utils.h"


cv::Mat api() {
    cv::Mat img, gray;
    img = cv::imread("./media/rgb.png", cv::IMREAD_COLOR);

    cvtColor(img, gray, cv::COLOR_BGR2GRAY);
    GaussianBlur(gray, gray, cv::Size(5, 5), 1.5);
    Canny(gray, gray, 0, 50);

    return gray;
}

cv::UMat tapi() {
    cv::UMat img, gray;
    cv::imread("./media/rgb.png", cv::IMREAD_COLOR).copyTo(img);

    cvtColor(img, gray, cv::COLOR_BGR2GRAY);
    GaussianBlur(gray, gray, cv::Size(5, 5), 1.5);
    Canny(gray, gray, 0, 50);

    return gray;
}

int main(int argc, char** argv)
{
    xbwee::perf::Timer timer;

    cv::Mat apiRes = api();
    double apiTime = timer.get_msecs_reset();

    cv::UMat tapiRes = tapi();
    double tapiTime = timer.get_msecs_reset();

    logp("Time cost: \n\tapi: %.2f ms\n\ttapi: %.2f ms\n", apiTime, tapiTime);

    imshow("api", apiRes);
    imshow("tapi", tapiRes);

    cv::waitKey();
    return 0;
}