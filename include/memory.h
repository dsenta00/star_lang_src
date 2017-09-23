#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>
#include "ORM/entity.h"

/**
 * The memory object.
 *
 * Memory is obtained by memory chunk in order to data can keep
 * memory information.
 */
class memory : public entity {
public:
  memory(uintptr_t address, uint32_t size);

  template<typename T>
  T get_pointer();

  uintptr_t get_address() const;

  template<typename T>
  T &get_element();

  void align(memory *adjacent_memory);
  uint32_t get_size();
  bool operator<(const memory &mem) const;
  void operator+=(uint32_t size);
  void operator-=(uint32_t size);
  void assign(uintptr_t address, uint32_t size);
  bool ready_to_remove();

  static memory *create(uintptr_t address, uint32_t size);
protected:
  uintptr_t address;
  uint32_t size;
};

#endif // MEMORY_H
