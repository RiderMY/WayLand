#pragma once

#include <iostream>
#include <memory>
#include <type_traits>
#include <unordered_map>

namespace wispy {

// FILTERED HETEROGENEOUS LIST
template <typename Filter>
class FHL {
public:
  FHL() : data_() { }

  ~FHL() {
    data_.clear();
  }

  template <typename T>
  void Add(std::shared_ptr<T> obj) noexcept requires std::is_base_of_v<Filter, T> {
    if (!data_.contains(typeid(T).name())) data_[typeid(T).name()] = obj;
    //  else std::cerr << "Error: Element of type " << typeid(T).name() << " already exists\n";
  }

  template <typename T>
  std::weak_ptr<T> Get() noexcept requires std::is_base_of_v<Filter, T> {
    if (data_.contains(typeid(T).name())) return std::dynamic_pointer_cast<T>(data_[typeid(T).name()]);
    return std::weak_ptr<T>();
  }

  template <typename T>
  void Remove() noexcept requires std::is_base_of_v<Filter, T> {
    if (data_.contains(typeid(T).name())) data_.erase(typeid(T).name());
    //  else std::cerr << "Error: Element of type " << typeid(T).name() << " does not exist\n";
  }

private:
  std::unordered_map<const char *, std::shared_ptr<Filter>> data_;
};

}
