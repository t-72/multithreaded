#ifndef PROFILING_STOPWATCH_H_
#define PROFILING_STOPWATCH_H_

#include <chrono>

#include "templates/tarithmeticable.h"

namespace profiling
{

template<typename ResolutionType = std::chrono::microseconds>
class Stopwatch final : public templates::TArithmeticable<
    Stopwatch<ResolutionType>, long long int>
{
public:
  Stopwatch(const long long int &in = 0)
    : begin_(ClockType::now())
    , duration_(in)
  {}

  Stopwatch(const Stopwatch& in)
    : begin_(ClockType::now())
    , duration_(in())
  {}

  void start() {
    begin_ = ClockType::now();
  }

  void stop() {
    duration_ = std::chrono::duration_cast<ResolutionType>(
          ClockType::now() - begin_);
  }

  long long int duration() const {
    return duration_.count();
  }

  void reset(const long long int &in = 0) override
  {
    duration_ = DurationType(in);
  }

  long long int operator () () const override
  {
    return duration();
  }

  bool operator < (const Stopwatch& o2) const override
  {
    return duration() < o2.duration();
  }

private:
  using ClockType = std::conditional_t<
    std::chrono::high_resolution_clock::is_steady,
    std::chrono::high_resolution_clock,
    std::chrono::steady_clock>;

  using DurationType =
    decltype(std::chrono::duration_cast<ResolutionType>(
      ClockType::now() - ClockType::now()));

  ClockType::time_point begin_;
  DurationType duration_;
};

} // namespace profiling

#endif // PROFILING_STOPWATCH_H_
