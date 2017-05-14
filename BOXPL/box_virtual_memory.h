#ifndef BOX_VIRTUAL_MEMORY_H
#define BOX_VIRTUAL_MEMORY_H
#include <stdint.h>
#include "box_vector.h"
#include "box_memory_chunk.h"

#define CHUNK_MINIMUM_CAPACITY (32768)
#define CHUNK_MAXIMUM_CAPACITY (134217728)

class box_virtual_memory {
public:
  box_virtual_memory(uint32_t init_capacity);
  memory *alloc(uint32_t size);
  memory *realloc(memory *mem, uint32_t new_size);
  void free(memory *mem);
  uint32_t get_allocated_total(void);
protected:
  void add_memory_chunk(uint32_t capacity);
  memory *reserve(uint32_t size);

  uint32_t allocated_total;
  uint32_t max_allocated_bytes;
  box_vector<memory_chunk *> memory_chunks;
};

#endif // BOX_VIRTUAL_MEMORY_H
