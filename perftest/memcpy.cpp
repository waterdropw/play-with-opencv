
#include <math.h>
#include <string.h>

#include <array>
#include <vector>
#include <set>
#include <random>

#include <benchmark/benchmark.h>

constexpr int len = 6;
// constexpr function具有inline属性，你应该把它放在头文件中
constexpr auto my_pow(const int i)
{
    return i * i;
}

// 使用operator[]读取元素，依次存入1-6的平方
static void bench_array_operator(benchmark::State &state)
{
    std::array<int, len> arr;
    constexpr int i = 1;
    for (auto _ : state)
    {
        benchmark::DoNotOptimize(arr[i]);
        arr[0] = my_pow(i);
        arr[1] = my_pow(i + 1);
        arr[2] = my_pow(i + 2);
        arr[3] = my_pow(i + 3);
        arr[4] = my_pow(i + 4);
        arr[5] = my_pow(i + 5);
    }
}
BENCHMARK(bench_array_operator);

// 使用at()读取元素，依次存入1-6的平方
static void bench_array_at(benchmark::State &state)
{
    std::array<int, len> arr;
    constexpr int i = 1;
    for (auto _ : state)
    {
        benchmark::DoNotOptimize(arr.at(i));
        arr.at(0) = my_pow(i);
        arr.at(1) = my_pow(i + 1);
        arr.at(2) = my_pow(i + 2);
        arr.at(3) = my_pow(i + 3);
        arr.at(4) = my_pow(i + 4);
        arr.at(5) = my_pow(i + 5);
    }
}
BENCHMARK(bench_array_at);

// std::get<>(array)是一个constexpr function，它会返回容器内元素的引用，并在编译期检查数组的索引是否正确
static void bench_array_get(benchmark::State &state)
{
    std::array<int, len> arr;
    constexpr int i = 1;
    for (auto _ : state)
    {
        benchmark::DoNotOptimize(std::get<i>(arr));
        std::get<0>(arr) = my_pow(i);
        std::get<1>(arr) = my_pow(i + 1);
        std::get<2>(arr) = my_pow(i + 2);
        std::get<3>(arr) = my_pow(i + 3);
        std::get<4>(arr) = my_pow(i + 4);
        std::get<5>(arr) = my_pow(i + 5);
    }
}
BENCHMARK(bench_array_get);

static void BM_memcpy(benchmark::State &state)
{
    char *src = new char[state.range(0)];
    char *dst = new char[state.range(0)];
    memset(src, 'x', state.range(0));
    for (auto _ : state)
    {
        benchmark::DoNotOptimize(src);
        benchmark::DoNotOptimize(dst);
        memcpy(dst, src, state.range(0));
    }
    state.SetBytesProcessed(int64_t(state.iterations()) *
                            int64_t(state.range(0)));
    delete[] src;
    delete[] dst;
}
// BENCHMARK(BM_memcpy)->Arg(8)->Arg(64)->Arg(512)->Arg(1 << 10)->Arg(8 << 10);
// BENCHMARK(BM_memcpy)->Range(8, 8<<10);
BENCHMARK(BM_memcpy)->RangeMultiplier(2)->Range(8, 8 << 10);

static void BM_VecInit(benchmark::State &state)
{
    for (auto _ : state)
    {
        std::vector<int> v(state.range(0), state.range(0));
        benchmark::DoNotOptimize(v.data());
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_VecInit)->DenseRange(0, 1024, 128);

static void bench_N(benchmark::State &state)
{
    int n = 0;
    for ([[maybe_unused]] auto _ : state)
    {
        for (int i = 0; i < state.range(0); ++i)
        {
            benchmark::DoNotOptimize(n += 2); // 这个函数防止编译器将表达式优化，会略微降低一些性能
        }
    }
    state.SetComplexityN(state.range(0));
}
BENCHMARK(bench_N)->RangeMultiplier(10)->Range(10, 10e6)->Complexity();

static void bench_LogN(benchmark::State &state)
{
    int n = 0;
    for ([[maybe_unused]] auto _ : state)
    {
        for (int i = 1; i < state.range(0); i *= 2)
        {
            benchmark::DoNotOptimize(n += 2);
        }
    }
    state.SetComplexityN(state.range(0));
}
BENCHMARK(bench_LogN)->RangeMultiplier(10)->Range(10, 10e6)->Complexity();

static void bench_Square(benchmark::State &state)
{
    int n = 0;
    auto len = state.range(0);
    for ([[maybe_unused]] auto _ : state)
    {
        for (int64_t i = 1; i < len * len; ++i)
        {
            benchmark::DoNotOptimize(n += 2);
        }
    }
    state.SetComplexityN(len);
}
BENCHMARK(bench_Square)->RangeMultiplier(10)->Range(10, 100000)->Complexity();

static void BM_StringCompare(benchmark::State &state)
{
    std::string s1(state.range(0), '-');
    std::string s2(state.range(0), '-');
    for (auto _ : state)
    {
        benchmark::DoNotOptimize(s1.compare(s2));
    }
    state.SetComplexityN(state.range(0));
}
BENCHMARK(BM_StringCompare)
    ->RangeMultiplier(2)
    ->Range(1 << 10, 1 << 18)
    // ->Complexity(benchmark::oN);
    // ->Complexity();
    ->Complexity([](benchmark::IterationCount n) -> double { return n; });

BENCHMARK_MAIN();