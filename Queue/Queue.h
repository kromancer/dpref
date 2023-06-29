#pragma once

#include <cassert>
#include <condition_variable>
#include <mutex>
#include <queue>

#include "Token.h"

namespace dpref {

constexpr size_t MaxQCap = 12;
  
template <typename T> class Queue {
public:

  using Token = dpref::Token<T>;
  
  explicit Queue(size_t Cap=MaxQCap) : Capacity(Cap) { if (Capacity <= 0) throw std::runtime_error("Q Capacity must be > 0"); }

  void push(const T &Value) {
    pushToken(Token(Value));
  }

  void pushLoopTerminationToken(void) {
    pushToken(Token(LoopTerminationToken()));
  }
  
  void pushTerminationToken(void) {
    pushTerminationToken(TerminationToken());
  }

  void pushTerminationToken(TerminationToken Tok) {
    pushToken(Tok);
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

  void pushToken(Token Tok) {
    std::unique_lock<std::mutex> Lock(Mutex);
    ProducerCV.wait(Lock, [this] { return Q.size() < Capacity; });
    Q.push(Tok);
    ConsumerCV.notify_one();
  }
};

} // namespace dpref
