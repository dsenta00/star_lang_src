#ifndef BOX_VECTOR_H
#define BOX_VECTOR_H
#include <vector>
#include <functional>
#include <algorithm>

template <class T>
class box_vector : public std::vector<T> {
public:
  void push_front(T &element);
  void delete_element(T &element);
  void delete_all();
  void sort(const std::function <bool(const T, const T)>& func);
  void for_each(const std::function <void(T)>& func);
  void for_each(const std::function <bool(T, T)>& func);
  T find_if(const std::function <bool(const T)>& func);
  ~box_vector();
};

#endif // BOX_VECTOR_H
