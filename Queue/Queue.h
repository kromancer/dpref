#pragma once

#include <cassert>
#include <condition_variable>
#include <mutex>
#include <queue>

#include "Token.h"

namespace dpref {

template <typename T> class Queue {
public:

  using Token = Token<T>;
  
  explicit Queue(size_t Cap) : Capacity(Cap) { if (Capacity <= 0) throw std::runtime_error("Q Capacity must be > 0"); }

  void push(const T &Value) {
    std::unique_lock<std::mutex> Lock(Mutex);
    ProducerCV.wait(Lock, [this] { return Q.size() < Capacity; });
    Q.push(Token(Value));
    ConsumerCV.notify_one();
  }

  void pushTerminationToken(TerminationToken Tok) {
    std::unique_lock<std::mutex> Lock(Mutex);
    ProducerCV.wait(Lock, [this] { return Q.size() < Capacity; });
    Q.push(Token(Tok));
    Lock.unlock();
    ConsumerCV.notify_one();
  }

  Token pop() {
    std::unique_lock<std::mutex> Lock(Mutex);
    ConsumerCV.wait(Lock, [this] { return !Q.empty(); });
    Token Item = std::move(Q.front());
    Q.pop();
    Lock.unlock();
    ProducerCV.notify_one();
    return Item;
  }
  
  bool isEmpty() {
    std::unique_lock<std::mutex> Lock(Mutex);
    return Q.empty();
  }

private:
  size_t Capacity;
  std::queue<Token> Q;
  std::mutex Mutex;
  std::condition_variable ProducerCV;
  std::condition_variable ConsumerCV;
};

} // namespace dpref
