#ifndef BOX_MEMORY_CHUNK_H
#define BOX_MEMORY_CHUNK_H
#include <stdint.h>
#include <cstdlib>
#include <vector>
#include "memory.h"

#define MEMORY_CHUNK_RELEASE_OK                (0)
#define MEMORY_CHUNK_RELEASE_UNKNOWN_ADDRESS   (1)
#define MEMORY_CHUNK_RELEASE_NULL_MEMORY       (2)

typedef enum {
  MEMORY_CHUNK_RESIZE_OK,
  MEMORY_CHUNK_RESIZE_ZERO_CAPACITY,
  MEMORY_CHUNK_RESIZE_ZERO_SIZE,
  MEMORY_CHUNK_RESIZE_NO_MEMORY,
  MEMORY_CHUNK_RESIZE_FRAGMENTED_MEMORY,
  MEMORY_CHUNK_RESIZE_NULL_MEMORY,
  MEMORY_CHUNK_RESIZE_UNKNOWN_ADDRESS
} memory_chunk_resize_result;

class memory_chunk {
protected:
  std::vector<memory *> free_memory;
  std::vector<memory *> reserved_memory;
  uint32_t capacity;
  uint32_t free;
  uintptr_t start_address;
  void free_memory_union();
  memory *find_free_memory(uint32_t size);
  memory *find_adjacent_free(const memory *mem);
public:
  memory_chunk();
  memory_chunk(uint32_t capacity);
  memory *reserve(uint32_t size);
  memory_chunk_resize_result resize(memory *mem, uint32_t new_size);
  uint32_t release(memory * mem);
  bool is_parent_of(memory *mem);
  bool can_reserve(uint32_t size);
  bool is_fragmented(uint32_t size);
  bool worth_defragmentation();
  void defragmentation();
  ~memory_chunk();
};

#endif // BOX_MEMORY_CHUNK_H
