#ifndef CONTAINERS_MEMORYBLOCK_H_
#define CONTAINERS_MEMORYBLOCK_H_

#include <cstddef>
#include <new>
#include <utility>

namespace containers
{

template<typename T>
struct MemoryBlock
{
  static T* allocate(size_t n) {
    return static_cast<T*>(operator new(n * sizeof(T)));
  }
  
  static void deallocate(T* buf) {
    operator delete(buf);
  }
  
  MemoryBlock() = default;
  
  MemoryBlock(size_t n)
    : buf(allocate(n))
    , cp(n)
  {}
  
  MemoryBlock(const MemoryBlock&) = delete;
  
  MemoryBlock(MemoryBlock&& other) noexcept {
    operator = (std::move(other));
  }
  
  ~MemoryBlock() {
    deallocate(buf);
  }
  
  MemoryBlock& operator = (const MemoryBlock&) = delete;
  
  MemoryBlock& operator = (MemoryBlock&& other) noexcept {
    swap(other);
    deallocate(other.buf);
    other.buf = nullptr;
    other.cp = 0;
    return *this;
  }
  
  T* operator + (size_t i) {
    return buf + i;
  }
  
  const T* operator + (size_t i) const {
    return buf + i;
  }
  
  T& operator [] (size_t i) {
    return buf[i];
  }
  
  const T& operator [] (size_t i) const {
    return buf[i];
  }
  
  void swap(MemoryBlock& other) noexcept {
    std::swap(buf, other.buf);
    std::swap(cp, other.cp);
  }
  
  T* buf = nullptr;
  size_t cp = 0;
};

} // namespace containers

#endif // CONTAINERS_MEMORYBLOCK_H_
