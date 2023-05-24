#pragma once

#include <cassert>
#include <optional>

namespace dpref {
template <typename T> class Token {
public:
  explicit Token(T Dat) : Data(std::move(Dat)) {}

  Token(Token &&Other) noexcept : Data(std::move(Other.Data)) {}

  Token &operator=(Token &&Other) noexcept {
    if (this != &Other) {
      Data = std::move(Other.Data);
    }
    return *this;
  }

  Token() : Data(std::nullopt) {}

  bool isControlToken() const { return !Data.has_value(); }

  T getData() const {
    assert(Data.has_value());
    return Data.value();
  }

private:
  std::optional<T> Data;
};
} // namespace dpref
