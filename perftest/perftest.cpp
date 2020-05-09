
#include <opencv2/opencv.hpp>
#include <benchmark/benchmark.h>

#define LOG_TAG     "OCVTEST"
#include "utils.h"
#include "Profiler.h"


#define __PMU_RAW_EVENT__


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
        {"branch-hit-rate", counters.getBranchHitRate()},   \
        {"branch-miss-rate", counters.getBranchMissRate()}, \
    });                                                     \
}
#else
#define PMU_RECORD_BEGIN()
#define PMU_RECORD_END()
#endif



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


constexpr int WIDTH = 1<<12, HEIGHT = 1<<12;

BENCHMARK(BM_medianBlur_ocv)->RangeMultiplier(2)->Range(1<<2, WIDTH);
BENCHMARK(BM_GaussianBlur_ocv)->RangeMultiplier(2)->Range(1<<2, WIDTH);
BENCHMARK(BM_Sobel_ocv)->RangeMultiplier(2)->Range(1<<2, WIDTH);
BENCHMARK(BM_Canny_ocv)->RangeMultiplier(2)->Range(1<<2, WIDTH);
BENCHMARK(BM_dilate_ocv)->RangeMultiplier(2)->Range(1<<2, WIDTH);
BENCHMARK(BM_erode_ocv)->RangeMultiplier(2)->Range(1<<2, WIDTH);
BENCHMARK(BM_integral_ocv)->RangeMultiplier(2)->Range(1<<2, WIDTH);
BENCHMARK(BM_boxFilter_ocv)->RangeMultiplier(2)->Range(1<<2, WIDTH);
BENCHMARK(BM_bilateralFilter_ocv)->RangeMultiplier(2)->Range(1<<2, WIDTH);




BENCHMARK_MAIN();