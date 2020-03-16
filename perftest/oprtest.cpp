#include <stdio.h>
#include <stdlib.h>

#include <opencv2/opencv.hpp>
#include <benchmark/benchmark.h>

#define LOG_TAG     "OPRTEST"
#include "utils.h"



static void ocv_add(benchmark::State& state) {
    int rows = state.range(1);
    int cols = state.range(0);
    int type = state.range(2);

    cv::Mat src1(rows, cols, type);
    cv::Mat src2(rows, cols, type);
    cv::Mat dst(rows, cols, type);
    cv::Mat mask = cv::Mat::eye(rows, cols, CV_8U);
    for (auto _ : state)
        cv::add(src1, src2, dst, mask);
}

static void ocv_addWeighted(benchmark::State& state) {
    int rows = state.range(1);
    int cols = state.range(0);
    int type = state.range(2);

    cv::Mat src1(rows, cols, type);
    cv::Mat src2(rows, cols, type);
    cv::Mat dst(rows, cols, type);
    double alpha = 0.4;
    double beta = 0.1;
    double gamma = 0.3;
    for (auto _ : state)
        cv::addWeighted(src1, alpha, src2, beta, gamma, dst);
}

static void ocv_subtract(benchmark::State& state) {
    int rows = state.range(1);
    int cols = state.range(0);
    int type = state.range(2);

    cv::Mat src1(rows, cols, type);
    cv::Mat src2(rows, cols, type);
    cv::Mat dst(rows, cols, type);
    for (auto _ : state)
        cv::subtract(src1, src2, dst);
}

static void ocv_absdiff(benchmark::State& state) {
    int rows = state.range(1);
    int cols = state.range(0);
    int type = state.range(2);

    cv::Mat src1(rows, cols, type);
    cv::Mat src2(rows, cols, type);
    cv::Mat dst(rows, cols, type);
    for (auto _ : state)
        cv::absdiff(src1, src2, dst);
}

static void ocv_split(benchmark::State& state) {
    int rows = state.range(1);
    int cols = state.range(0);
    int type = state.range(2);

    cv::Mat src(rows, cols, type);
    std::vector<cv::Mat> output;
    for (auto _ : state)
        cv::split(src, output);
}

static void ocv_merge(benchmark::State& state) {
    int rows = state.range(1);
    int cols = state.range(0);
    int type = state.range(2);

    cv::Mat src(rows, cols, type);
    std::vector<cv::Mat> tmp;
    cv::split(src, tmp);
    for (auto _ : state)
        cv::merge(tmp, src);
}



constexpr int WIDTH = 4000, HEIGHT = 3000;

BENCHMARK(ocv_add)->Args({WIDTH, HEIGHT, CV_8UC3});
BENCHMARK(ocv_addWeighted)->Args({WIDTH, HEIGHT, CV_8UC3});
BENCHMARK(ocv_subtract)->Args({WIDTH, HEIGHT, CV_8UC3});
BENCHMARK(ocv_absdiff)->Args({WIDTH, HEIGHT, CV_8UC3});
BENCHMARK(ocv_split)->Args({WIDTH, HEIGHT, CV_8UC3});
BENCHMARK(ocv_merge)->Args({WIDTH, HEIGHT, CV_8UC3});

BENCHMARK(ocv_add)->Args({WIDTH, HEIGHT, CV_16U});
BENCHMARK(ocv_addWeighted)->Args({WIDTH, HEIGHT, CV_16U});
BENCHMARK(ocv_subtract)->Args({WIDTH, HEIGHT, CV_16U});
BENCHMARK(ocv_absdiff)->Args({WIDTH, HEIGHT, CV_16U});
BENCHMARK(ocv_split)->Args({WIDTH, HEIGHT, CV_16U});
BENCHMARK(ocv_merge)->Args({WIDTH, HEIGHT, CV_16U});

BENCHMARK_MAIN();