#pragma once

#include <algorithm>
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

  int Add(T obj) {
    data_[next_token_] = obj;
    return next_token_++;
  }

  std::vector<T> GetStream() {
    std::vector<T> stream;

    for (const auto &p : data_) {
      stream.push_back(p.second); // CAUTION: VERY UNSAFE
    }

    return stream;
  }

  void Remove(unsigned int token) {
    data_.erase(token);
  }

private:
  std::unordered_map<unsigned int, T> data_;
  unsigned int next_token_;
};

}
