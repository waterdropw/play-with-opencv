
#include <opencv2/opencv.hpp>
#include <benchmark/benchmark.h>

#define LOG_TAG     "OPRTEST"
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



static void ocv_add(benchmark::State& state) {
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

static void ocv_addWeighted(benchmark::State& state) {
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

static void ocv_subtract(benchmark::State& state) {
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

static void ocv_absdiff(benchmark::State& state) {
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

static void ocv_split(benchmark::State& state) {
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

static void ocv_merge(benchmark::State& state) {
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



constexpr int WIDTH = 4000, HEIGHT = 3000;

BENCHMARK(ocv_add)->RangeMultiplier(2)->Range(1, 1<<12);
BENCHMARK(ocv_addWeighted)->RangeMultiplier(2)->Range(1, 1<<12);
BENCHMARK(ocv_subtract)->RangeMultiplier(2)->Range(1, 1<<12);
BENCHMARK(ocv_absdiff)->RangeMultiplier(2)->Range(1, 1<<12);
BENCHMARK(ocv_split)->RangeMultiplier(2)->Range(1, 1<<12);
BENCHMARK(ocv_merge)->RangeMultiplier(2)->Range(1, 1<<12);




BENCHMARK_MAIN();