#ifndef CONTAINERS_QUEUE_H_
#define CONTAINERS_QUEUE_H_

#include <algorithm>
#include <utility>

#include "containers/stack.h"

namespace containers
{

/**
 * Очереди на двух стеках. Время чтения с фронта, добавление и
 * исключение из очереди за O(1)
 **/
template<typename ElType>
class Queue
{
public:
  const ElType& front() { return del_().top(); }
  const ElType& min() const {
    if (!d_.empty() && !a_.empty())
      return std::min(d_.min(), a_.min());
    else if (!d_.empty()) return d_.min();
    else return a_.min();
  }
  const ElType& max() const {
    if (!d_.empty() && !a_.empty())
      return std::max(d_.max(), a_.max());
    else if (!d_.empty()) return d_.max();
    else return a_.max();
  }

  void push(const ElType &e) { a_.push(e); }
  void push(ElType &&e) { a_.push(std::move(e)); }

  bool pop() { return del_().pop(); }
  ElType move() { return del_().move(); }

  bool empty() const { return a_.empty() && d_.empty(); }
  void clear() { a_.clear(); d_.clear(); }
  size_t size() const { return a_.size() + d_.size(); }
private:
  void flip_() { while (d_.push(a_)); }
  Stack<ElType>& del_() {
    if (d_.empty()) flip_();
    return d_;
  }
  Stack<ElType> a_;
  Stack<ElType> d_;
};

} // namespace containers

#endif // CONTAINERS_QUEUE_H_
