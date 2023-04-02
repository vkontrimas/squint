#pragma once

#include <utility>

namespace squint {
  template<typename HandleT, typename DeleterT>
  class BasicHandle {
  public:
    explicit BasicHandle(HandleT&& handle) noexcept
      : handle_{std::forward<HandleT>(handle)} {}

    ~BasicHandle() noexcept {
      DeleterT{}(handle_);
    }

    BasicHandle(const BasicHandle&) = delete;
    BasicHandle& operator=(const BasicHandle&) = delete;

    BasicHandle(BasicHandle&& other) noexcept {
      swap(*this, other);
    }

    BasicHandle& operator=(BasicHandle&& other) noexcept {
      swap(*this, other);
      return *this;
    }

    const HandleT& get() const {
      return handle_;
    }

    friend void swap(BasicHandle& a, BasicHandle& b) noexcept {
      using std::swap;
      std::swap(a.handle_, b.handle_);
    }

  private:
    HandleT handle_;
  };
};
