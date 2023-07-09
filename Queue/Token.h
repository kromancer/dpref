#pragma once

#include <stdexcept>
#include <variant>

namespace dpref {

class TerminationToken {
public:
  explicit TerminationToken(bool Black = false) : IsBlack(Black) {}
  void setBlack() { IsBlack = true; }
  void setWhite() { IsBlack = false; }
  bool isBlack() const { return IsBlack; }
  bool isWhite() const { return !IsBlack; }

private:
  bool IsBlack;
};

class LoopTerminationToken {
  // Empty
};

template <typename T> class Token {
public:
  Token(const T &D) : Data(D) {}
  Token(TerminationToken TermToken) : Data(TermToken) {}
  Token(LoopTerminationToken LoopToken) : Data(LoopToken) {}

  bool isTerminationToken() const {
    return std::holds_alternative<TerminationToken>(Data);
  }

  bool isLoopTerminationToken() const {
    return std::holds_alternative<LoopTerminationToken>(Data);
  }

  bool isControlToken() const {
    return isTerminationToken() || isLoopTerminationToken();
  }

  T getData() const {
    if (isControlToken())
      throw std::runtime_error("Control token has no data!");
    return std::get<T>(Data);
  }

  TerminationToken getTerminationToken() const {
    if (!isTerminationToken())
      throw std::runtime_error("Not a control token!");
    return std::get<TerminationToken>(Data);
  }

private:
  std::variant<T, TerminationToken, LoopTerminationToken> Data;
};

} // namespace dpref
