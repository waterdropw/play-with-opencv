#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

#define LOG_TAG     "GST"
#include "utils.h"

using namespace std;
using namespace cv;


void calcGST(const Mat& inputImg, Mat& imgCoherencyOut, Mat& imgOrientationOut, int w);

int main(int argc, char** argv) {
    const int W = 52;
    double chThreshold = 0.43;
    int lowThreshold = 35;
    int highThreshold = 57;

    string ifname = "media/gst_input.jpg";
    Mat input_img = imread(ifname.c_str(), IMREAD_GRAYSCALE);
    if (input_img.empty()) {
        loge("Cannot read file %s", ifname.c_str());
        exit(1);
    }

    Mat coherency, orient, coherency_bin, orient_bin, image_bin;
    calcGST(input_img, coherency, orient, W);
    coherency_bin = coherency > chThreshold;
    inRange(orient, Scalar(lowThreshold), Scalar(highThreshold), orient_bin);
    image_bin = coherency_bin & orient_bin;

    normalize(coherency, coherency, 0, 255, NORM_MINMAX);
    normalize(orient, orient, 0, 255, NORM_MINMAX);

    logd("Processing success");
    
    imwrite("result.jpg", 0.5 * (input_img+image_bin));
    imwrite("coherency.jpg", coherency);
    imwrite("orient.jpg", orient);

    return 0;
}

void calcGST(const Mat& inputImg, Mat& imgCoherencyOut, Mat& imgOrientationOut, int w) {
    Mat img;
    inputImg.convertTo(img, CV_32F);

    // GST components calculation (start)
    // J =  (J11 J12; J12 J22) - GST
    Mat dx, dy, dxy, dxx, dyy;
    Sobel(img, dx, CV_32F, 1, 0, 3);
    Sobel(img, dy, CV_32F, 0, 1, 3);
    multiply(dx, dy, dxy);
    multiply(dx, dx, dxx);
    multiply(dy, dy, dyy);

    // J11, J22 and J12 are GST components
    Mat J11, J22, J12;
    boxFilter(dx, J11, CV_32F, Size(w, w));
    boxFilter(dy, J22, CV_32F, Size(w, w));
    boxFilter(dxy, J12, CV_32F, Size(w, w));
    // GST components calculation (stop)

    // eigenvalue calculation (start)
    // lambda1 = J11 + J22 + sqrt((J11-J22)^2 + 4*J12^2)
    // lambda2 = J11 + J22 - sqrt((J11-J22)^2 + 4*J12^2)
    Mat tmp1, tmp2, tmp3, tmp4;
    tmp1 = J11 + J22;
    tmp2 = J11 - J22;
    multiply(tmp2, tmp2, tmp2);
    multiply(J12, J12, tmp3);
    sqrt(tmp2 + 4.0 * tmp3, tmp4);

    Mat lambda1, lambda2;
    lambda1 = tmp1 + tmp4;  // biggest eigenvalue
    lambda2 = tmp1 - tmp4;  // smallest eigenvalue
    // eigenvalue calculation (stop)

    // Coherency calculation (start)
    // Coherency = (lambda1 - lambda2)/(lambda1 + lambda2)) - measure of anisotropism
    // Coherency is anisotropy degree (consistency of local orientation)
    divide(lambda1 - lambda2, lambda1 + lambda2, imgCoherencyOut);
    // Coherency calculation (stop)

    // orientation angle calculation (start)
    // tan(2*Alpha) = 2*J12/(J22 - J11)
    // Alpha = 0.5 atan2(2*J12/(J22 - J11))
    phase(J22 - J11, 2.0 * J12, imgOrientationOut, true);
    imgOrientationOut *= 0.5;
    // orientation angle calculation (stop)
}