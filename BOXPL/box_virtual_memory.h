#ifndef BOX_VIRTUAL_MEMORY_H
#define BOX_VIRTUAL_MEMORY_H
#include <stdint.h>
#include <list>
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
  ~box_virtual_memory();
protected:
  void add_memory_chunk(uint32_t capacity);
  memory *reserve(uint32_t size);
  memory_chunk *get_chunk_parent(memory *mem);
  memory_chunk *get_fragmented_chunk(uint32_t size);

  uint32_t allocated_total;
  uint32_t max_allocated_bytes;
  std::list<memory_chunk *> memory_chunk_list;
};

#endif // BOX_VIRTUAL_MEMORY_H
