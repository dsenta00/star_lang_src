#include "box_vector.h"
#include "memory.h"
#include "box_memory_chunk.h"
#include "box_data.h"

template <class T>
/**
 * Push front element.
 *
 * @param element - The element.
 */
void
box_vector<T>::push_front(T &element)
{
  this->insert(this->begin(), element);
}

template <class T>
/**
 * Delete element.
 *
 * @param element - the element.
 */
void
box_vector<T>::delete_element(T &element)
{
  this->erase(std::remove(this->begin(), this->end(), element), this->end());
  delete element;
}

template <class T>
/**
 * Delete all elements.
 */
void
box_vector<T>::delete_all()
{
  for (T &data : *this)
  {
    delete data;
  }

  this->clear();
}

template <class T>
/**
 * Sort elements.
 *
 * @param func - sorting function rule.
 */
void
box_vector<T>::sort(const std::function <bool(const T, const T)>& func)
{
  std::sort(this->begin(), this->end(), func);
}

template <class T>
/**
 * for each element.
 *
 * @param func - foreach body for one element.
 */
void
box_vector<T>::for_each(const std::function <void(T)>& func)
{
  std::for_each(this->begin(), this->end(), func);
}

template <class T>
/**
 * for each neighbout elements.
 *
 * @param func - for each body for two neighbour elements.
 */
void
box_vector<T>::for_each(const std::function <bool(T, T)>& func)
{
  for (auto i = this->begin() + 1;
       i != this->end();
       i++)
  {
    if (!func(*(i - 1), *i))
    {
      i--;
    }
  }
}

template <class T>
/**
 * Find if.
 *
 * @param func - function rule.
 *
 * @return pointer if found, otherwise NULL.
 */
T
box_vector<T>::find_if(const std::function <bool(const T)>& func)
{
  auto position = std::find_if(this->begin(), this->end(), func);

  if (position != this->end())
  {
    return *position;
  }

  return NULL;
}

template <class T>
/**
 * The destructor.
 */
box_vector<T>::~box_vector()
{
  this->delete_all();
}

/*
 * Explicit template instatiation.
 */
template box_vector<box_data *>::~box_vector();
template box_vector<memory_chunk *>::~box_vector();
template box_vector<memory *>::~box_vector();
template void box_vector<box_data *>::delete_all();
template void box_vector<memory_chunk *>::delete_all();
template void box_vector<memory *>::delete_all();
template void box_vector<memory_chunk *>::push_front(memory_chunk *&element);
template void box_vector<memory *>::delete_element(memory *&element);
template void box_vector<memory *>::sort(
    const std::function <bool(memory *, memory *)>& func);
template void box_vector<memory *>::for_each(
    const std::function <void(memory *)>& func);
template void box_vector<memory *>::for_each(
    const std::function <bool(memory *, memory *)>& func);
template memory *box_vector<memory *>::find_if(
    const std::function <bool(memory *)>& func);
template memory_chunk *box_vector<memory_chunk *>::find_if(
    const std::function <bool(memory_chunk *)>& func);

