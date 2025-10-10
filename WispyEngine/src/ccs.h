#pragma once

#include <algorithm>
#include <unordered_map>

namespace wispy {

// COAT-CHECKING SYSTEM
template <typename T>
class CCS {
public:
  CCS() : data_(), ids_(), next_id_(0) { }

  ~CCS() {
    data_.clear();
  }

  template <typename U>
  unsigned Add(U&& obj) {
    ids_.push_back(next_id_);
    data_.emplace_back(std::forward<U>(obj));
    next_id_++;
    return ids_[ids_.size() - 1];
  }

  std::vector<T> GetStream() {
      return data_;
  }

  const std::vector<T>& GetStream() const {
      return data_;
  }

  void Remove(unsigned int token) {
    // Find the index that corrosponds to the given id
    // Use binary search since ids_ is gaurenteed to be sorted
    // Upper bound is the token or the end (whichever comes first) because of how the id_ is stored

    auto end_it = ids_.end();
    if (token < ids_.size()) {
        end_it = ids_.begin() + token;
    }

    auto ids_it = std::lower_bound(ids_.begin(), end_it, token);
    
    // Check that it was actually included for safety and bug handling
    if (ids_it == ids_.end()) {
        throw;
    }

    // Calculate index through pointer arithmatic
    std::ptrdiff_t index = &(*ids_it) - &(ids_[0]);

    // Remove from vectors using erase
    ids_.erase(ids_it);
    data_.erase(data_.begin() + index);

  }

private:

  // Holds the data for this
  std::vector<T> data_;

  /// <summary>
  /// Holds the indentifiers for the data members
  /// data_[i] corrosponds to ids_[i
  /// ids_ should be sorted in ascending order at all times since next_id_ always grows and is added to the end
  /// ids_[i] >= i
  /// </summary>
  std::vector<unsigned> ids_;
  unsigned next_id_;
};

}
