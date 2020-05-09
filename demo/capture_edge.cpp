#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/gapi/core.hpp>
#include <opencv2/gapi/imgproc.hpp>


using namespace cv;


int main(int argc, char** argv) {
    VideoCapture cap;
    cap.open(0);

    CV_Assert(cap.isOpened());

    GMat in;
    GMat vga = gapi::resize(in, Size(), 0.5, 0.5);
    GMat gray = gapi::BGR2Gray(vga);
    GMat blurred = gapi::blur(gray, Size(5, 5));
    GMat edges = gapi::Canny(blurred, 32, 128);
    GMat b, g, r;
    std::tie(b, g, r) = gapi::split3(vga);
    GMat out = gapi::merge3(b, g, edges);
    GComputation ac(in, out);

    Mat input_frame, output_frame;
    CV_Assert(cap.read(input_frame));
    do {
        ac.apply(input_frame, output_frame);
        imshow("output", output_frame);
    } while (cap.read(input_frame) && waitKey(30) < 0);

    return 0;
}