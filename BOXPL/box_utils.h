#ifndef BOX_UTILS_H
#define BOX_UTILS_H
#include <algorithm>
#include <vector>
#include <list>
#include <functional>

template <typename T>
T
vector_find(std::vector<T> &vector,
            const std::function <bool(T)>& func)
{
  auto position = std::find_if(vector.begin(),
                               vector.end(),
                               func);

  if (position != vector.end())
  {
    return *position;
  }

  return NULL;
}

template <typename T>
T
list_find(std::list<T> &list,
          const std::function <bool(const T)>& func)
{
  auto position = std::find_if(list.begin(),
                               list.end(),
                               func);

  if (position != list.end())
  {
    return *position;
  }

  return NULL;
}

template <typename T>
void
vector_foreach(std::vector<T> &vector,
               const std::function <void(T)>& func)
{
  std::for_each(vector.begin(), vector.end(), func);
}

template <typename T>
void
list_foreach(std::list<T> &list,
               const std::function <void(T)>& func)
{
  std::for_each(list.begin(), list.end(), func);
}

template <typename T>
void
vector_foreach2(std::vector<T> &vector,
                const std::function <bool(T, T)>& func)
{
  for (auto i = vector.begin() + 1;
       i != vector.end();
       i++)
  {
    if (!func(*(i - 1), *i))
    {
      i--;
    }
  }
}

template <typename T>
void
vector_remove(std::vector<T> &vector, T &element)
{
  vector.erase(std::remove(vector.begin(),
                           vector.end(),
                           element), vector.end());
  delete element;
}

template <typename T>
void
vector_sort(std::vector<T> &vector,
            const std::function <bool(const T, const T)>& func)
{
  std::sort(vector.begin(), vector.end(), func);
}


#endif // BOX_UTILS_H
