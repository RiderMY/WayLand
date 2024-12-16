#pragma once

#include <algorithm>
#include <memory>
#include <unordered_map>

namespace wispy {

// COAT-CHECKING SYSTEM
// one file b/c implementation for templates
template <typename T>
class CCS {
public:
  CCS() : data_(), next_token_(1u) { }

  ~CCS() {
    data_.clear();
  }

  int Add(std::weak_ptr<T> obj) {
    data_[next_token_] = obj;
    return next_token_++;
  }

  std::vector<std::weak_ptr<T>> GetStream() {
    std::vector<std::weak_ptr<T>> stream;

    for (const auto &p : data_) {
      if (!p.second.expired()) stream.push_back(p.second);
    }

    return stream;
  }

  void Remove(unsigned int token) {
    data_.erase(token);
  }

private:
  std::unordered_map<unsigned int, std::weak_ptr<T>> data_;
  unsigned int next_token_;
};

}
