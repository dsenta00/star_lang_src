#ifndef BOX_VIRTUAL_MEMORY_H
#define BOX_VIRTUAL_MEMORY_H

#include "ORM/entity.h"
#include "box_fw.h"
#include <cstdint>
#include <functional>

#define CHUNK_MINIMUM_CAPACITY (32768)
#define CHUNK_MAXIMUM_CAPACITY (134217728)

class box_virtual_memory : public entity {
public:
  box_virtual_memory(uint32_t init_capacity = CHUNK_MINIMUM_CAPACITY);
  memory *alloc(uint32_t size);
  memory *realloc(memory *mem, uint32_t new_size);
  void free(memory *mem);
  uint32_t get_allocated_total();

  static box_virtual_memory *create(uint32_t init_capacity = CHUNK_MINIMUM_CAPACITY);
protected:
  memory *add_new_chunk_and_alloc(uint32_t size);
  memory *solve_defragmentation_and_alloc(uint32_t size);
  memory_chunk *find_memory_chunk(std::function<bool(memory_chunk *)> func);
  memory_chunk *add_memory_chunk(uint32_t capacity);
  memory *reserve(uint32_t size);
  memory *reserve_from_chunk(memory_chunk *chunk, uint32_t size);

  uint32_t allocated_total;
  uint32_t max_allocated_bytes;
  relationship *box_memory_chunk;
};

#endif // BOX_VIRTUAL_MEMORY_H
