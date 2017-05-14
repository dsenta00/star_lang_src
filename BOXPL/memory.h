#ifndef MEMORY_H
#define MEMORY_H
#include <stdint.h>

typedef float float32_t;
typedef double float64_t;

/**
 * The memory object.
 *
 * Memory is obtained by memory chunk in order to data can keep
 * memory information.
 */
class memory {
protected:
  uintptr_t address;
  uint32_t size;
  uint32_t counting_reference;
public:
  memory(uintptr_t address, uint32_t size);
  template <typename T>
  T get_pointer();
  uintptr_t get_address() const;
  template <typename T>
  T get_element();
  void allign(memory *adjacent_memory);
  uint32_t get_size();
  bool operator < (const memory& mem) const;
  void operator += (uint32_t size);
  void operator -= (uint32_t size);
  void assign(uintptr_t address, uint32_t size);
  void increase_ref();
  void decrease_ref();
  bool ready_to_remove();
};

#endif // MEMORY_H
