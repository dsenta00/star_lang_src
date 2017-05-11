#ifndef MEMORY_H
#define MEMORY_H
#include <stdint.h>

class memory {
public:
  uintptr_t address;
  uint32_t size;
  uint32_t counting_reference;

  memory(uintptr_t address, uint32_t size)
  {
    this->address = address;
    this->size = size;
    this->counting_reference = 1;
  }

  bool operator < (const memory& mem) const
  {
    return this->address < mem.address;
  }

  void operator += (uint32_t size)
  {
    this->size += size;
  }

  void operator -= (uint32_t size)
  {
    this->size += size;
  }

  void increase_ref()
  {
    this->counting_reference++;
  }

  void decrease_ref()
  {
    if (this->counting_reference > 0)
    {
      this->counting_reference--;
    }
  }

  bool ready_to_remove()
  {
    return this->counting_reference == 0;
  }
};

#endif // MEMORY_H
