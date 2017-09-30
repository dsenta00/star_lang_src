#ifndef BOX_MEMORY_CHUNK_IF_H
#define BOX_MEMORY_CHUNK_IF_H

#include "ORM/entity.h"
#include "box_fw.h"
#include <cstdint>
#include <cstdlib>
#include <functional>

class memory_chunk_if : public entity {
public:
  memory_chunk_if();
  void free_memory_add(uintptr_t address, uint32_t size);

  void free_memory_remove(memory *mem);
  memory *free_memory_find(std::function<bool(memory *)> foo);
  memory *free_memory_front();
  uint32_t free_memory_num();
  void free_memory_delete_all();
  void free_memory_union();
  memory *reserved_memory_add(uintptr_t address, uint32_t size);

  void reserved_memory_remove(memory *mem);
  memory *reserved_memory_front();
  memory *reserved_memory_back();
  uint32_t reserved_memory_num();
  void reserved_memory_sort();
};

#endif // BOX_MEMORY_CHUNK_IF_H
