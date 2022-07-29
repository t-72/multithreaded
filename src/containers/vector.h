#ifndef CONTAINERS_VECTOR_H_
#define CONTAINERS_VECTOR_H_

#include <cstddef>
#include <initializer_list>
#include <memory>
#include <utility>

#include "memoryblock.h"

namespace containers
{

template<typename T>
class Vector
{	
public:
  Vector() = default;
  
  Vector(std::initializer_list<T> l) : data_(l.size()) {
    for (const T &elem : l)
      pushBack(elem);
  }

  Vector(size_t n) : data_(n) {
    std::uninitialized_value_construct_n(
      data_.buf,
      n
    );
    sz_ = n;
  }
  
  Vector(const Vector& other) : data_(other.sz_) {
    std::uninitialized_copy_n(
      other.data_.buf,
      other.sz_,
      data_.buf
    );
    sz_ = other.sz_;
  }
  
  Vector(Vector&& other) noexcept {
    operator = (std::move(other));
  }
  
  ~Vector() {
    std::destroy_n(data_.buf, sz_);
  }

  /* basic guarantee
   * (stays consistent)
   */
  Vector& operator = (const Vector& other) {
    if (other.sz_ > data_.cp) {
      Vector tmp(other);
      swap(tmp);
    } else {
      for (size_t i = 0; i < sz_ && i < other.sz_; ++i) {
        data_[i] = other.data_[i];
      }
      if (sz_ < other.sz_) {
        std::uninitialized_copy_n(
          other.data_ + sz_,
          other.sz_ - sz_,
          data_ + sz_
        );
      } else if (sz_ > other.sz_) {
        std::destroy_n(
          data_ + other.sz_,
          sz_ - other.sz_
        );
      }
      sz_ = other.sz_;
    }
    return *this;
  }
  
  Vector& operator = (Vector&& other) noexcept {
    data_ = std::move(other.data_);
    sz_ = other.sz_;
    other.sz_ = 0;
    return *this;
  }

  const T& operator [] (size_t i) const {
    return data_[i];
  }
  
  T& operator [] (size_t i) {
    return data_[i];
  }

  size_t size() const {
    return sz_;
  }
  
  size_t capacity() const {
    return data_.cp;
  }
  
  /* strict guarantee
   * (transaction)
   */
  void reserve(size_t n) {
    if (n > data_.cp) {
      MemoryBlock<T> data(n);      
      std::uninitialized_move_n(
        data_.buf,
        sz_,
        data.buf
      );
      std::destroy_n(data_.buf, sz_);
      data_ = std::move(data);
    }
  }
  
  /* strict guarantee
   * (transaction)
   */
  void resize(size_t n) {
    reserve(n);
    if (sz_ < n) {
      std::uninitialized_value_construct_n(
        data_ + sz_,
        n - sz_
    );
    } else if (sz_ > n) {
      std::destroy_n(
        data_ + n,
        sz_ - n
      );
    }
    sz_ = n;
  }
  
  /* strict guarantee
   * (transaction)
   */
  void pushBack(const T& elem) {
    if (sz_ == data_.cp) {
      reserve(sz_ == 0 ? 1 : sz_ * 2);
    }
    new (data_ + sz_) T(elem);
    ++sz_;
  }
  
  /* strict guarantee
   * (transaction)
   */
  void pushBack(T&& elem) {
    if (sz_ == data_.cp) {
      reserve(sz_ == 0 ? 1 : sz_ * 2);
    }
    new (data_ + sz_) T(std::move(elem));
    ++sz_;
  }
  
  void popBack() {
    std::destroy_at(data_ + sz_ - 1);
    --sz_;
  }
  
  template<typename ... Args>
  T& emplaceBack(Args&& ... args) {
    if (sz_ == data_.cp) {
      reserve(sz_ == 0 ? 1 : sz_ * 2);
    }
    auto elem = new (data_ + sz_) T(std::forward<Args>(args)...);
    ++sz_;
    return *elem;
  }
  
  void swap(Vector& other) noexcept {
    data_.swap(other.data_);
    std::swap(sz_, other.sz_);
  }

private:
  static void construct(void* buf) {
    new (buf) T;
  }
  
  static void construct(void* buf, const T& elem) {
    new (buf) T(elem);
  }
  
  static void construct(void* buf, T&& elem) {
    new (buf) T(std::move(elem));
  }
  
  static void destroy(T* buf) {
    buf->~T();
  }
  
  MemoryBlock<T> data_;
  size_t sz_ = 0;
};

} // namespace containers

#endif // CONTAINERS_VECTOR_H_
