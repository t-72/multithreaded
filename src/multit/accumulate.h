#ifndef MULTIT_ACCUMULATE_H_
#define MULTIT_ACCUMULATE_H_

#include <iterator>
#include <numeric>
#include <thread>
#include <vector>

namespace multit
{

template<typename Iterator, typename T>
struct Accumulate {
  void operator () (Iterator first, Iterator last, T &result) {
    result = std::accumulate(first, last, result);
  }
};

template<typename Iterator, typename T>
T accumulate(Iterator first, Iterator last, T init)
{
  const size_t length = std::distance(first, last);
  if (length == 0)
    return init;

  const size_t minPerThread = 25;
  const size_t maxThreads =
      (length + minPerThread - 1) /
      minPerThread;
  const size_t hThreads =
      std::thread::hardware_concurrency();
  const size_t numThreads =
      std::min(hThreads != 0 ? hThreads : 2, maxThreads);
  const size_t blockSize = length / numThreads;

  std::vector<T> results(numThreads);
  std::vector<std::thread> threads(numThreads - 1);

  Iterator begin = first;
  for (size_t i = 0; i < threads.size(); ++i) {
    Iterator end = begin;
    std::advance(end, blockSize);
    threads[i] = std::thread(
          Accumulate<Iterator, T>(),
          begin, end,
          std::ref(results[i]));
    begin = end;
  }

  Accumulate<Iterator, T>()(begin,
                            last,
                            results[numThreads - 1]);

  for (auto &item : threads)
    item.join();

  return std::accumulate(
        results.begin(),
        results.end(),
        init);
}

} // namespace multithreading

#endif // MULTIT_ACCUMULATE_H_
