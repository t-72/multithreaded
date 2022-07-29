#ifndef PROFILING_LOGDURATION_H_
#define PROFILING_LOGDURATION_H_

#include <chrono>
#include <iostream>
#include <string>
#include <string_view>
#include <type_traits>

#include "profiling/stopwatch.h"

namespace profiling
{

template<typename Resolution = std::chrono::microseconds>
class LogDuration final
{
public:
  LogDuration(std::string_view message = "",
              std::ostream &out = std::cout)
    : message_(message)
    , out_(out)
  {}

  ~LogDuration() {
    stopwatch_.stop();
    if (!message_.empty()) {
      message_.push_back(' ');
    }
    out_ << message_ << stopwatch_.duration() << " "
         << representation() << '\n';
  }

private:
  constexpr static const char* representation()
  {
    return std::is_same<Resolution, std::chrono::nanoseconds>::value ? "ns" :
      std::is_same<Resolution, std::chrono::microseconds>::value ? "mcs" :
      std::is_same<Resolution, std::chrono::milliseconds>::value ? "ms" :
      std::is_same<Resolution, std::chrono::seconds>::value ? "sec" :
      std::is_same<Resolution, std::chrono::minutes>::value ? "min" :
      std::is_same<Resolution, std::chrono::hours>::value ? "hr" : "unk";
  }

  std::string message_;
  std::ostream &out_;
  Stopwatch<Resolution> stopwatch_;
};

} // namespace profiling

#endif // PROFILING_LOGDURATION_H_
