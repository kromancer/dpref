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

template <typename T> class Token {
public:
  Token(const T &D) : Data(D) {}
  Token(TerminationToken TermToken) : Data(TermToken) {}

  bool isTerminationToken() const {
    return std::holds_alternative<TerminationToken>(Data);
  }

  T getData() const {
    if (isTerminationToken())
      throw std::runtime_error("Control token has no data!");
    return std::get<T>(Data);
  }

  TerminationToken getTerminationToken() const {
    if (!isTerminationToken())
      throw std::runtime_error("Not a control token!");
    return std::get<TerminationToken>(Data);
  }

private:
  std::variant<T, TerminationToken> Data;
};

} // namespace dpref
