
#include <benchmark/benchmark.h>

#define LOG_TAG     "OPRTEST"
#include "utils.h"
#include "Profiler.h"

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/video/tracking.hpp>

#include <opencv2/gapi/core.hpp>
#include <opencv2/gapi/imgproc.hpp>

// #define __PMU_RAW_EVENT__


#ifdef __PMU_RAW_EVENT__
#include "Profiler.h"
#define PMU_RECORD_BEGIN()                                  \
    bool valid = true;                                      \
    utils::Profiler::Counters counters;                     \
    utils::Profiler& profiler = utils::Profiler::get();     \
    profiler.resetEvents(utils::Profiler::EV_CPU_CYCLES     \
                        | utils::Profiler::EV_L1I_RATES     \
                        | utils::Profiler::EV_L1D_RATES     \
                        | utils::Profiler::EV_BPU_RATES);   \
    if (profiler.isValid()) {                              \
        profiler.start();                                   \
    } else {                                                \
        loge("performance counters not enabled. try \"setprop security.perf_harden 0\"\n"); \
        valid = false;                                      \
    }                                                       

#define PMU_RECORD_END()                                    \
if (valid) {                                                \
    profiler.stop();                                        \
    profiler.readCounters(&counters);                       \
    state.counters.insert({                                 \
        {"instructions", counters.getInstructions()},       \
        {"cpu-cycles", counters.getCpuCycles()},            \
        {"ipc", counters.getIPC()},                         \
        {"cpi", counters.getCPI()},                         \
        {"L1i-hit-rate", counters.getL1IHitRate()},         \
        {"L1i-miss-rate", counters.getL1IMissRate()},       \
        {"L1d-hit-rate", counters.getL1IHitRate()},         \
        {"L1d-miss-rate", counters.getL1IMissRate()},       \
        {"L1i-mpki", counters.getMPKI(counters.getL1IMisses())},    \
    });                                                     \
}
#else
#define PMU_RECORD_BEGIN()
#define PMU_RECORD_END()
#endif



static void BM_add_ocv(benchmark::State& state) {
    int rows = state.range(0);
    int cols = state.range(0);
    int type = CV_8UC3;

    cv::Mat src1(rows, cols, type);
    cv::Mat src2(rows, cols, type);
    cv::Mat dst(rows, cols, type);
    cv::Mat mask = cv::Mat::eye(rows, cols, CV_8U);

    PMU_RECORD_BEGIN();
    for (auto _ : state)
        cv::add(src1, src2, dst, mask);
    PMU_RECORD_END();
}

static void BM_addWeighted_ocv(benchmark::State& state) {
    int rows = state.range(0);
    int cols = state.range(0);
    int type = CV_8UC3;

    cv::Mat src1(rows, cols, type);
    cv::Mat src2(rows, cols, type);
    cv::Mat dst(rows, cols, type);
    double alpha = 0.4;
    double beta = 0.1;
    double gamma = 0.3;

    PMU_RECORD_BEGIN();
    for (auto _ : state)
        cv::addWeighted(src1, alpha, src2, beta, gamma, dst);
    PMU_RECORD_END();
}

static void BM_subtract_ocv(benchmark::State& state) {
    int rows = state.range(0);
    int cols = state.range(0);
    int type = CV_8UC3;

    cv::Mat src1(rows, cols, type);
    cv::Mat src2(rows, cols, type);
    cv::Mat dst(rows, cols, type);

    PMU_RECORD_BEGIN();
    for (auto _ : state)
        cv::subtract(src1, src2, dst);
    PMU_RECORD_END();
}

static void BM_absdiff_ocv(benchmark::State& state) {
    int rows = state.range(0);
    int cols = state.range(0);
    int type = CV_8UC3;

    cv::Mat src1(rows, cols, type);
    cv::Mat src2(rows, cols, type);
    cv::Mat dst(rows, cols, type);

    PMU_RECORD_BEGIN();
    for (auto _ : state)
        cv::absdiff(src1, src2, dst);
    PMU_RECORD_END();
}

static void BM_split_ocv(benchmark::State& state) {
    int rows = state.range(0);
    int cols = state.range(0);
    int type = CV_8UC3;

    cv::Mat src(rows, cols, type);
    std::vector<cv::Mat> output;

    PMU_RECORD_BEGIN();
    for (auto _ : state)
        cv::split(src, output);
    PMU_RECORD_END();
}

static void BM_merge_ocv(benchmark::State& state) {
    int rows = state.range(0);
    int cols = state.range(0);
    int type = CV_8UC3;

    cv::Mat src(rows, cols, type);
    std::vector<cv::Mat> tmp;
    cv::split(src, tmp);

    PMU_RECORD_BEGIN();
    for (auto _ : state)
        cv::merge(tmp, src);
    PMU_RECORD_END();
}

static void BM_medianBlur_ocv(benchmark::State& state) {
    int rows = state.range(0);
    int cols = state.range(0);
    int type = CV_8UC3;

    cv::Mat src(rows, cols, type);
    cv::Mat dst(rows, cols, type);
    int ksize = 3;  // 3x3

    PMU_RECORD_BEGIN();
    for (auto _ : state)
        cv::medianBlur(src, dst, ksize);
    PMU_RECORD_END();
}
static void BM_medianBlur_gapi(benchmark::State& state) {
    int rows = state.range(0);
    int cols = state.range(0);
    int type = CV_8UC3;

    cv::Mat src(rows, cols, type);
    cv::Mat dst(rows, cols, type);
    int ksize = 3;  // 3x3
    cv::GMat in;
    cv::GMat out = cv::gapi::medianBlur(in, ksize);
    cv::GComputation blur(GIn(in), GOut(out));

    PMU_RECORD_BEGIN();
    for (auto _ : state)
        blur.apply(src, dst);

    PMU_RECORD_END();
}


static void BM_GaussianBlur_ocv(benchmark::State& state) {
    int rows = state.range(0);
    int cols = state.range(0);
    int type = CV_8UC3;

    cv::Mat src(rows, cols, type);
    cv::Mat dst(rows, cols, type);
    cv::Size ksize = {3, 3};  // 3x3
    double sigmaX = 0.0;
    double sigmaY = 0.0;

    PMU_RECORD_BEGIN();
    for (auto _ : state)
        cv::GaussianBlur(src, dst, ksize, sigmaX, sigmaY);
    PMU_RECORD_END();
}
static void BM_GaussianBlur_gapi(benchmark::State& state) {
    int rows = state.range(0);
    int cols = state.range(0);
    int type = CV_8UC3;

    cv::Mat src(rows, cols, type);
    cv::Mat dst(rows, cols, type);
    cv::Size ksize = {3, 3};  // 3x3
    double sigmaX = 0.0;
    double sigmaY = 0.0;

    cv::GMat in;
    cv::GMat out = cv::gapi::gaussianBlur(in, ksize, sigmaX, sigmaY);
    cv::GComputation blur(GIn(in), GOut(out));

    PMU_RECORD_BEGIN();
    for (auto _ : state)
        blur.apply(src, dst);

    PMU_RECORD_END();
}


static void BM_Sobel_ocv(benchmark::State& state) {
    int rows = state.range(0);
    int cols = state.range(0);
    int type = CV_8UC3;

    cv::Mat src(rows, cols, type);
    cv::Mat dst(rows, cols, type);
    int ksize = 3;  // 3x3
    int ddepth = -1;    // as the input
    int orderX = 0;
    int orderY = 1;

    PMU_RECORD_BEGIN();
    for (auto _ : state)
        cv::Sobel(src, dst, ddepth, orderX, orderY, ksize);
    PMU_RECORD_END();
}
static void BM_Sobel_gapi(benchmark::State& state) {
    int rows = state.range(0);
    int cols = state.range(0);
    int type = CV_8UC3;

    cv::Mat src(rows, cols, type);
    cv::Mat dst(rows, cols, type);
    int ksize = 3;  // 3x3
    int ddepth = -1;    // as the input
    int orderX = 0;
    int orderY = 1;

    cv::GMat in;
    cv::GMat out = cv::gapi::Sobel(in, ddepth, orderX, orderY, ksize);
    cv::GComputation sobel(GIn(in), GOut(out));

    PMU_RECORD_BEGIN();
    for (auto _ : state)
        sobel.apply(src, dst);

    PMU_RECORD_END();
}


static void BM_Canny_ocv(benchmark::State& state) {
    int rows = state.range(0);
    int cols = state.range(0);
    int type = CV_8UC3;
    double thresh1 = 100;
    double thresh2 = 150;

    cv::Mat src(rows, cols, type);
    cv::Mat dst(rows, cols, CV_8UC1);

    PMU_RECORD_BEGIN();
    for (auto _ : state)
        cv::Canny(src, dst, thresh1, thresh2);
    PMU_RECORD_END();
}
static void BM_Canny_gapi(benchmark::State& state) {
    int rows = state.range(0);
    int cols = state.range(0);
    int type = CV_8UC3;
    double thresh1 = 100;
    double thresh2 = 150;

    cv::Mat src(rows, cols, type);
    cv::Mat dst(rows, cols, CV_8UC1);

    cv::GMat in;
    cv::GMat out = cv::gapi::Canny(in, thresh1, thresh2);
    cv::GComputation canny(GIn(in), GOut(out));

    PMU_RECORD_BEGIN();
    for (auto _ : state)
        canny.apply(src, dst);
    
    PMU_RECORD_END();
}


static void BM_dilate_ocv(benchmark::State& state) {
    int rows = state.range(0);
    int cols = state.range(0);
    int type = CV_8UC3;

    cv::Mat src(rows, cols, type);
    cv::Mat dst(rows, cols, type);
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));

    PMU_RECORD_BEGIN();
    for (auto _ : state)
        cv::dilate(src, dst, kernel);
    PMU_RECORD_END();
}
static void BM_dilate_gapi(benchmark::State& state) {
    int rows = state.range(0);
    int cols = state.range(0);
    int type = CV_8UC3;

    cv::Mat src(rows, cols, type);
    cv::Mat dst(rows, cols, type);
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));

    cv::GMat in;
    cv::GMat out = cv::gapi::dilate(in, kernel);
    cv::GComputation dilate(GIn(in), GOut(out));

    PMU_RECORD_BEGIN();
    for (auto _ : state)
        dilate.apply(src, dst);

    PMU_RECORD_END();
}


static void BM_erode_ocv(benchmark::State& state) {
    int rows = state.range(0);
    int cols = state.range(0);
    int type = CV_8UC3;

    cv::Mat src(rows, cols, type);
    cv::Mat dst(rows, cols, type);
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));

    PMU_RECORD_BEGIN();
    for (auto _ : state)
        cv::erode(src, dst, kernel);
    PMU_RECORD_END();
}
static void BM_erode_gapi(benchmark::State& state) {
    int rows = state.range(0);
    int cols = state.range(0);
    int type = CV_8UC3;

    cv::Mat src(rows, cols, type);
    cv::Mat dst(rows, cols, type);
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));

    cv::GMat in;
    cv::GMat out = cv::gapi::erode(in, kernel);
    cv::GComputation erode(GIn(in), GOut(out));

    PMU_RECORD_BEGIN();
    for (auto _ : state)
        erode.apply(src, dst);

    PMU_RECORD_END();
}


static void BM_integral_ocv(benchmark::State& state) {
    int rows = state.range(0);
    int cols = state.range(0);
    int type = CV_8UC3;

    cv::Mat src(rows, cols, type);
    cv::Mat dst(rows, cols, CV_32SC3);

    PMU_RECORD_BEGIN();
    for (auto _ : state)
        cv::integral(src, dst);
    PMU_RECORD_END();
}
// static void BM_integral_gapi(benchmark::State& state) {
//     int rows = state.range(0);
//     int cols = state.range(0);
//     int type = CV_8UC3;

//     cv::Mat src(rows, cols, type);
//     cv::Mat dst(rows, cols, CV_32SC3);

//     cv::GMat in;
//     std::tuple<cv::GMat, cv::GMat> out = cv::gapi::integral(in);
//     cv::GComputation integral(GIn(in), GOut(out));

//     PMU_RECORD_BEGIN();
//     for (auto _ : state)
//         integral.apply(src, dst);

//     PMU_RECORD_END();
// }


static void BM_boxFilter_ocv(benchmark::State& state) {
    int rows = state.range(0);
    int cols = state.range(0);
    int type = CV_8UC3;
    int ddepth = -1;    // same as src
    cv::Size ksize(3, 3);
    cv::Mat src(rows, cols, type);
    cv::Mat dst(rows, cols, type);

    PMU_RECORD_BEGIN();
    for (auto _ : state)
        cv::boxFilter(src, dst, ddepth, ksize);
    PMU_RECORD_END();
}
static void BM_boxFilter_gapi(benchmark::State& state) {
    int rows = state.range(0);
    int cols = state.range(0);
    int type = CV_8UC3;
    int ddepth = -1;    // same as src
    cv::Size ksize(3, 3);
    cv::Mat src(rows, cols, type);
    cv::Mat dst(rows, cols, type);

    cv::GMat in;
    cv::GMat out = cv::gapi::boxFilter(in, ddepth, ksize);
    cv::GComputation boxFilter(GIn(in), GOut(out));

    PMU_RECORD_BEGIN();
    for (auto _ : state)
        boxFilter.apply(src, dst);

    PMU_RECORD_END();
}


static void BM_bilateralFilter_ocv(benchmark::State& state) {
    int rows = state.range(0);
    int cols = state.range(0);
    int type = CV_8UC3;
    int d = 5;
    double sigmaColor = 10;
    double sigmaSpace = 10;
    cv::Mat src(rows, cols, type);
    cv::Mat dst(rows, cols, type);

    PMU_RECORD_BEGIN();
    for (auto _ : state)
        cv::bilateralFilter(src, dst, d, sigmaColor, sigmaSpace);
    PMU_RECORD_END();
}
// static void BM_bilateralFilter_gapi(benchmark::State& state) {
//     int rows = state.range(0);
//     int cols = state.range(0);
//     int type = CV_8UC3;
//     int d = 5;
//     double sigmaColor = 10;
//     double sigmaSpace = 10;
//     cv::Mat src(rows, cols, type);
//     cv::Mat dst(rows, cols, type);

//     cv::GMat in;
//     cv::GMat out = cv::gapi::bilateralFilter(in, d, sigmaColor, sigmaSpace);
//     cv::GComputation bilateralFilter(GIn(in), GOut(out));

//     PMU_RECORD_BEGIN();
//     for (auto _ : state)
//         bilateralFilter.apply(src, dst);

//     PMU_RECORD_END();
// }


static void BM_calcHist_ocv(benchmark::State& state) {
    int rows = state.range(0);
    int cols = state.range(0);
    int type = CV_8UC3;
    cv::Mat src(rows, cols, type);
    cv::Mat dst(rows, cols, type);

    cv::Mat hsv;
    cvtColor(src, hsv, cv::COLOR_BGR2HSV);
    // Quantize the hue to 30 levels
    // and the saturation to 32 levels
    int hbins = 30, sbins = 32;
    int histSize[] = {hbins, sbins};
    // hue varies from 0 to 179, see cvtColor
    float hranges[] = { 0, 180 };
    // saturation varies from 0 (black-gray-white) to
    // 255 (pure spectrum color)
    float sranges[] = { 0, 256 };
    const float* ranges[] = { hranges, sranges };
    // we compute the histogram from the 0-th and 1-st channels
    int channels[] = {0, 1};
    cv::MatND hist;

    PMU_RECORD_BEGIN();
    for (auto _ : state)
        cv::calcHist( &hsv, 1, channels, cv::Mat(), // do not use mask
             hist, 2, histSize, ranges,
             true, // the histogram is uniform
             false );

    PMU_RECORD_END();
}


static void BM_pyrDown_ocv(benchmark::State& state) {
    int rows = state.range(0);
    int cols = state.range(0);
    int type = CV_8UC3;
    cv::Size dstsize( (cols+1)/2, (rows+1)/2 );
    cv::Mat src(rows, cols, type);
    cv::Mat dst;

    PMU_RECORD_BEGIN();
    for (auto _ : state)
        cv::pyrDown( src, dst, dstsize);

    PMU_RECORD_END();
}


static void BM_pyrUp_ocv(benchmark::State& state) {
    int rows = state.range(0);
    int cols = state.range(0);
    int type = CV_8UC3;
    cv::Size dstsize( cols*2, rows*2 );
    cv::Mat src(rows, cols, type);
    cv::Mat dst;

    PMU_RECORD_BEGIN();
    for (auto _ : state)
        cv::pyrUp( src, dst, dstsize);

    PMU_RECORD_END();
}

static void BM_calcOpticalFlowPyrLK_ocv(benchmark::State& state) {
    int rows = state.range(0);
    int cols = state.range(0);
    int type = CV_8UC1;

    cv::Mat gray_orig = cv::imread("bin/media/gray.jpg", cv::IMREAD_GRAYSCALE);
    cv::Mat prevGray_orig  = cv::imread("bin/media/prev_gray.jpg", cv::IMREAD_GRAYSCALE);

    if (gray_orig.empty() || prevGray_orig.empty()) {
        loge("Read input image failed");
        return;
    }
    std::vector<uchar> status;
    std::vector<float> err;
    cv::Mat gray, prevGray;
    std::vector<cv::Point2f> points[2];
    cv::TermCriteria termcrit(cv::TermCriteria::COUNT | cv::TermCriteria::EPS,20,0.03);
    cv::Size subPixWinSize(5,5), winSize(31,31);
    const int MAX_COUNT = 500;

    cv::resize(gray_orig, gray, cv::Size(cols, rows));
    cv::resize(prevGray_orig, prevGray, cv::Size(cols, rows));
    cv::goodFeaturesToTrack(prevGray, points[0], MAX_COUNT, 0.01, 10, cv::Mat(), 3, 3, false, 0.04);
    cv::cornerSubPix(prevGray, points[0], subPixWinSize, cv::Size(-1,-1), termcrit);
    cv::goodFeaturesToTrack(gray, points[1], MAX_COUNT, 0.01, 10, cv::Mat(), 3, 3, false, 0.04);
    cv::cornerSubPix(gray, points[1], subPixWinSize, cv::Size(-1,-1), termcrit);

    PMU_RECORD_BEGIN();
    for (auto _ : state)
        cv::calcOpticalFlowPyrLK(prevGray, gray, points[0], points[1], status, err, winSize,
                                 3, termcrit, 0, 0.001);

    PMU_RECORD_END();
}


static void BM_resizeBilinear_ocv(benchmark::State& state) {
    int rows = state.range(0);
    int cols = state.range(0);
    int type = CV_8UC3;
    cv::Size dstsize( cols*2, rows*2 );
    cv::Mat src(rows, cols, type);
    cv::Mat dst;

    PMU_RECORD_BEGIN();
    for (auto _ : state)
        cv::resize( src, dst, dstsize, 0, 0, cv::INTER_LINEAR);

    PMU_RECORD_END();
}


static void BM_resizeBicubic_ocv(benchmark::State& state) {
    int rows = state.range(0);
    int cols = state.range(0);
    int type = CV_8UC3;
    cv::Size dstsize( cols*2, rows*2 );
    cv::Mat src(rows, cols, type);
    cv::Mat dst;

    PMU_RECORD_BEGIN();
    for (auto _ : state)
        cv::resize( src, dst, dstsize, 0, 0, cv::INTER_CUBIC);

    PMU_RECORD_END();
}

static void BM_warpAffine_ocv(benchmark::State& state) {
    int rows = state.range(0);
    int cols = state.range(0);
    int type = CV_8UC3;
    cv::Size dstsize( cols*2, rows*2 );
    cv::Mat src(rows, cols, type);
    cv::Mat dst;

    cv::Mat warpGround;
    cv::RNG rng(0);
    warpGround = (cv::Mat_<float>(2,3) << (1-rng.uniform(-0.05f, 0.05f)),
                (rng.uniform(-0.03f, 0.03f)), (rng.uniform(10.f, 20.f)),
                (rng.uniform(-0.03f, 0.03f)), (1-rng.uniform(-0.05f, 0.05f)),
                (rng.uniform(10.f, 20.f)));

    PMU_RECORD_BEGIN();
    for (auto _ : state)
        cv::warpAffine( src, dst, warpGround, cv::Size(200, 200), cv::INTER_LINEAR + cv::WARP_INVERSE_MAP);

    PMU_RECORD_END();
}

static void BM_warpPerspective_ocv(benchmark::State& state) {
    int rows = state.range(0);
    int cols = state.range(0);
    int type = CV_8UC3;
    cv::Size dstsize( cols*2, rows*2 );
    cv::Mat src(rows, cols, type);
    cv::Mat dst;

    cv::Mat warpGround;
    cv::RNG rng(0);
    warpGround = (cv::Mat_<float>(3,3) << (1-rng.uniform(-0.05f, 0.05f)),
                (rng.uniform(-0.03f, 0.03f)), (rng.uniform(10.f, 20.f)),
                (rng.uniform(-0.03f, 0.03f)), (1-rng.uniform(-0.05f, 0.05f)),(rng.uniform(10.f, 20.f)),
                (rng.uniform(0.0001f, 0.0003f)), (rng.uniform(0.0001f, 0.0003f)), 1.f);

    PMU_RECORD_BEGIN();
    for (auto _ : state)
        cv::warpPerspective( src, dst, warpGround, cv::Size(200, 200), cv::INTER_LINEAR + cv::WARP_INVERSE_MAP);

    PMU_RECORD_END();
}



// width:height = 4:3, so width=4/3*height
constexpr int MIN_HEIGHT = 1<<4, MAX_HEIGHT = 1<<12;

// #define REAL_IMAGE_SIZE

#ifndef REAL_IMAGE_SIZE
#define BENCHMARK_IMAGE(test_name)      \
    BENCHMARK(test_name)->RangeMultiplier(2)->Range(MIN_HEIGHT, MAX_HEIGHT)
#else
// refer to https://zh.wikipedia.org/wiki/%E6%98%BE%E7%A4%BA%E5%88%86%E8%BE%A8%E7%8E%87%E5%88%97%E8%A1%A8
#define BENCHMARK_IMAGE(test_name)      \
    BENCHMARK(test_name)                \
    ->Args({120, 160})                  \
    ->Args({160, 240})                  \
    ->Args({240, 320})                  \
    ->Args({240, 360})                  \
    ->Args({240, 384})                  \
    ->Args({240, 400})                  \
    ->Args({240, 432})                  \
    ->Args({320, 480})                  \
    ->Args({360, 640})                  \
    ->Args({480, 640})                  \
    ->Args({480, 720})                  \
    ->Args({480, 768})                  \
    ->Args({480, 800})                  \
    ->Args({480, 854})                  \
    ->Args({540, 960})                  \
    ->Args({576, 1024})                 \
    ->Args({600, 800})                  \
    ->Args({600, 1024})                 \
    ->Args({640, 960})                  \
    ->Args({720, 1080})                 \
    ->Args({768, 1024})                 \
    ->Args({768, 1152})                 \
    ->Args({768, 1280})                 \
    ->Args({768, 1366})                 \
    ->Args({800, 1280})                 \
    ->Args({864, 1152})                 \
    ->Args({900, 1440})                 \
    ->Args({900, 1600})                 \
    ->Args({960, 1280})                 \
    ->Args({960, 1440})                 \
    ->Args({1024, 1280})                \
    ->Args({1024, 1600})                \
    ->Args({1050, 1400})                \
    ->Args({1050, 1680})                \
    ->Args({1080, 1920})                \

#endif

BENCHMARK_IMAGE(BM_medianBlur_ocv);
BENCHMARK_IMAGE(BM_GaussianBlur_ocv);
BENCHMARK_IMAGE(BM_Sobel_ocv);
BENCHMARK_IMAGE(BM_Canny_ocv);
BENCHMARK_IMAGE(BM_dilate_ocv);
BENCHMARK_IMAGE(BM_erode_ocv);
BENCHMARK_IMAGE(BM_integral_ocv);
BENCHMARK_IMAGE(BM_boxFilter_ocv);
BENCHMARK_IMAGE(BM_bilateralFilter_ocv);
BENCHMARK_IMAGE(BM_calcHist_ocv);
BENCHMARK_IMAGE(BM_pyrDown_ocv);
BENCHMARK_IMAGE(BM_pyrUp_ocv);
BENCHMARK_IMAGE(BM_calcOpticalFlowPyrLK_ocv);
BENCHMARK_IMAGE(BM_resizeBilinear_ocv);
BENCHMARK_IMAGE(BM_resizeBicubic_ocv);
BENCHMARK_IMAGE(BM_warpAffine_ocv);
BENCHMARK_IMAGE(BM_warpPerspective_ocv);



BENCHMARK_MAIN();