#ifndef CONTAINERS_STACK_H_
#define CONTAINERS_STACK_H_

#include <memory>
#include <utility>

namespace containers
{

/**
 * Стек. Время чтения с вершины, добавление и
 * исключение из стека за O(1)
 **/
template<typename ElType>
class Stack
{
public:
  Stack()
    : last_(nullptr)
    , size_(0)
  {};
  ~Stack() { clear(); }

  const ElType& top() const { return last_->el; }
  const ElType& min() const { return last_->min->el; }
  const ElType& max() const { return last_->max->el; }

  void push(const ElType &e) {
    Node *node = new Node(e, last_, size_);
    last_ = node;
    ++size_;
  }
  void push(ElType &&e) {
    Node *node = new Node(std::move(e), last_, size_);
    last_ = node;
    ++size_;
  }
  bool push(Stack &source) noexcept {
    bool result = false;
    if (!source.empty()) {
      Node *node = source.pop_();
      node->prev = last_;
      node->index = size_;
      node->setMinMax();
      last_ = node;
      ++size_;
      result = true;
    }
    return result;
  }

  bool pop() noexcept {
    bool result = false;
    if (last_ != nullptr) {
      Node *node = last_;
      last_ = node->prev;
      delete node;
      --size_;
      result = true;
    }
    return result;
  }

  ElType move() {
    Node *node = last_;
    last_ = node->prev;
    --size_;
    return std::move(std::unique_ptr<Node>(node)->el);
  }

  bool empty() const { return last_ == nullptr; }
  void clear() { while (pop()); }
  size_t size() const { return size_; }
  size_t topIdx() const { return last_->index; }
  size_t minIdx() const { return last_->min->index; }
  size_t maxIdx() const { return last_->max->index; }
private:
  struct Node
  {
    Node(const ElType &e, Node *p, size_t id)
      : el(e)
      , prev(p)
      , min(nullptr)
      , max(nullptr)
      , index(id)
    { setMinMax(); }
    Node(ElType &&e, Node *p, size_t id)
      : el(std::move(e))
      , prev(p)
      , min(nullptr)
      , max(nullptr)
      , index(id)
    { setMinMax(); }
    void setMinMax() {
      if (prev != nullptr) {
        if (el <= prev->min->el) min = this;
        else min = prev->min;
        if (el >= prev->max->el) max = this;
        else max = prev->max;
      } else {
        min = max = this;
      }
    }
    ElType el;
    Node *prev;
    Node *min;
    Node *max;
    size_t index;
  };

  Node* pop_() {
    Node *node = last_;
    last_ = node->prev;
    --size_;
    return node;
  }

  Node *last_;
  size_t size_;
};

} // namespace containers

#endif // CONTAINERS_STACK_H_
