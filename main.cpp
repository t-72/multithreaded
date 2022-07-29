#include <iostream>
#include <numeric>

#include "multit/accumulate.h"
#include "profiling/logduration.h"


int main()
{
  const size_t sizeA = 2000000000;

  int *a = new int[sizeA];
  for (size_t i = 0; i < sizeA; ++i)
    a[i] = 1;

  int summ = 0;
  {
    profiling::LogDuration ld("std::accumulate: ");
    summ = std::accumulate(a, a + sizeA, 0);
  }
  std::cout << "summ: " << summ << std::endl;

  {
    profiling::LogDuration ld("multithread accumulate: ");
    summ = multit::accumulate(a, a + sizeA, 0);
  }
  std::cout << "summ: " << summ << std::endl;

  delete [] a;
}
