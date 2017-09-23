#include "box_memory_chunk.h"
#include "ORM/relationship.h"
#include "memory.h"
#include "ORM/orm.h"

#define DISPERSION_LOW_THRESHOLD (131072)
#define DISPERSION_HIGH_THRESHOLD (401408)

/**
 * The constructor.
 *
 * @param capacity - chunk capacity.
 */
memory_chunk::memory_chunk(uint32_t capacity) : memory_chunk_if::memory_chunk_if()
{
  this->capacity = 0;
  this->start_address = 0;

  if (capacity == 0)
  {
    return;
  }

  this->start_address = (uintptr_t) calloc(capacity, sizeof(uint8_t));

  if (!this->start_address)
  {
    return;
  }

  this->capacity = capacity;
  memory_chunk_if::free_memory_add(start_address, capacity);
}

/**
 * Check if memory is part of memory chunk.
 *
 * @param mem - memory.
 * @return true memory if part of chunk, otherwise false.
 */
bool
memory_chunk::is_parent_of(memory *mem)
{
  return memory_chunk_if::reserved_memory_find([&](memory *mem_it) {
    return mem_it == mem;
  }) != nullptr;
}

/**
 * Reserve new memory from memory chunk.
 *
 * @param size - memory size.
 *
 * @return memory if success, otherwise nullptr.
 */
memory *
memory_chunk::reserve(uint32_t size)
{
  if (!can_reserve(size))
  {
    return nullptr;
  }

  memory *fmem = memory_chunk_if::free_memory_find([&](memory *m) {
    return m->get_size() >= size;
  });

  if (!fmem)
  {
    return nullptr;
  }

  memory *mem = memory_chunk_if::reserved_memory_add(fmem->get_address(), size);
  fmem->assign(fmem->get_address() + size, fmem->get_size() - size);

  if (fmem->get_size() == 0)
  {
    memory_chunk_if::free_memory_remove(fmem);
  }

  return mem;
}

/**
 * Expand existing memory.
 *
 * @param mem - memory.
 * @param new_size - new size.
 *
 * @retval MEMORY_CHUNK_EXPAND_OK - success.
 * @retval MEMORY_CHUNK_EXPAND_FRAGMENTED_MEMORY - chunk is to fragmented to find free memory.
 * @retval MEMORY_CHUNK_EXPAND_NO_MEMORY - not enough memory to expand memory.
 * @retval MEMORY_CHUNK_EXPAND_ZERO_CAPACITY - chunk contains zero capacity.
 * @retval MEMORY_CHUNK_EXPAND_ZERO_SIZE - expanding memory with zero size.
 * @retval MEMORY_CHUNK_EXPAND_NULL_MEMORY - expanding NULL memory
 */
memory_chunk_resize_result
memory_chunk::resize(memory *mem, uint32_t new_size)
{
  if (!mem || (mem->get_address() == 0))
  {
    return MEMORY_CHUNK_RESIZE_NULL_MEMORY;
  }

  if (!is_parent_of(mem))
  {
    return MEMORY_CHUNK_RESIZE_UNKNOWN_ADDRESS;
  }

  if (capacity == 0)
  {
    return MEMORY_CHUNK_RESIZE_ZERO_CAPACITY;
  }

  if (new_size == 0)
  {
    return MEMORY_CHUNK_RESIZE_ZERO_SIZE;
  }

  if (new_size == mem->get_size())
  {
    return MEMORY_CHUNK_RESIZE_OK;
  }
  else if (new_size < mem->get_size())
  {
    /* Find free memory after requested memory */
    memory *fmem = memory_chunk_if::free_memory_find([&](memory *m) {
      return m->get_address() == (mem->get_address() + mem->get_size());
    });

    if (fmem)
    {
      fmem->assign(fmem->get_address() - mem->get_size() + new_size,
                   fmem->get_size() + mem->get_size() - new_size);
      mem->assign(mem->get_address(), new_size);
    }
    else
    {
      /* Not found adjacent free memory, create new free memory */
      memory_chunk_if::free_memory_add(mem->get_address() + new_size, new_size - mem->get_size());
      memory_chunk_if::free_memory_union();
      mem->assign(mem->get_address(), new_size);
    }

    return MEMORY_CHUNK_RESIZE_OK;
  }
  else
  {
    if ((new_size - mem->get_size()) > free)
    {
      return MEMORY_CHUNK_RESIZE_NO_MEMORY;
    }

    memory *fmem = memory_chunk_if::free_memory_find([&](memory *m) {
      return m->get_address() == (mem->get_address() + mem->get_size());
    });

    if (!fmem)
    {
      /*
       * Not found adjacent free memory, memory is fragmented.
       */
      return MEMORY_CHUNK_RESIZE_FRAGMENTED_MEMORY;
    }

    /*
     * Found adjacent free memory,
     * check if new size can spread over free memory
     */
    if (new_size > (fmem->get_size() + mem->get_size()))
    {
      return MEMORY_CHUNK_RESIZE_FRAGMENTED_MEMORY;
    }

    /*
     * Adjacent free memory has enough space.
     * Spread over free memory.
     */
    fmem->assign(fmem->get_address() + new_size - mem->get_size(),
                 fmem->get_size() - new_size + mem->get_size());
    mem->assign(mem->get_address(), new_size);

    if (fmem->get_size() == 0)
    {
      memory_chunk_if::free_memory_remove(fmem);
    }

    return MEMORY_CHUNK_RESIZE_OK;
  }
}

/**
 * Release memory from memory chunk.
 *
 * @param mem - memory.
 *
 * @retval MEMORY_CHUNK_RELEASE_OK - success.
 * @retval MEMORY_CHUNK_RELEASE_NULL_MEMORY - null memory releasing.
 * @retval MEMORY_CHUNK_RELEASE_UNKNOWN_ADDRESS - memory isn't part of memory chunk.
 */
uint32_t
memory_chunk::release(memory *mem)
{
  if (!mem)
  {
    return MEMORY_CHUNK_RELEASE_NULL_MEMORY;
  }

  if (!is_parent_of(mem))
  {
    return MEMORY_CHUNK_RELEASE_UNKNOWN_ADDRESS;
  }

  if (!mem->ready_to_remove())
  {
    return MEMORY_CHUNK_RELEASE_OK;
  }

  memory_chunk_if::free_memory_add(mem->get_address(), mem->get_size());
  memory_chunk_if::free_memory_union();
  memory_chunk_if::reserved_memory_remove(mem);

  return MEMORY_CHUNK_RELEASE_OK;
}

/**
 * Check if memory chunk is worth for defragmentation process.
 * Free memory dispersion should be bigger than 128 ki and less than 392 ki.
 *
 * dispersion = number_of_free_chunks * free_bytes_total
 *
 * Legend:
 *
 * [x] - reserved memory byte
 * [-] - free memory byte
 *
 * Negative situations:
 *
 *  0  1  2  3  4  5  6
 * [x][-][x][-][x][-][x] <- dispersion = 3 * 3 = 9 <- not worth it
 * [x][x][-][-][x][x][-] <- dispersion = 2 * 2 = 4 <- not worth it
 *
 *  0       8ki  8ki+1   16ki
 * [x] ... [x]  [-] ... [-]    [repeat to 224ki].... <- dispersion = 56ki * 7 = 392ki <- not worth it
 *
 *  0  1  2       32765  32766  32Ki
 * [x][-][x] ... [-]    [x]    [-]    <- dispersion = 16ki * 16ki = 2Gi <- not worth it
 *
 * @return true if worth it, otherwise false.
 */
bool
memory_chunk::worth_defragmentation()
{
  if (memory_chunk_if::free_memory_num() == 1)
  {
    return false;
  }

  uint64_t dispersion = free * memory_chunk_if::free_memory_num();
  return (dispersion >= DISPERSION_LOW_THRESHOLD) && (dispersion < DISPERSION_HIGH_THRESHOLD);
}

/**
 * Check if memory chunk can reserve memory in size.
 *
 * @param size - size in bytes.
 * @return true if can, otherwise false.
 */
bool
memory_chunk::can_reserve(uint32_t size)
{
  if (size == 0)
  {
    return false;
  }

  if (this->free < size)
  {
    return false;
  }

  /*
   * Optimization part.
   * I'm so damn smart.
   */
  if (memory_chunk_if::free_memory_num() == 1)
  {
    return true;
  }

  return memory_chunk_if::free_memory_find([&](memory *m) {
    return m->get_size() >= size;
  }) != nullptr;
}

/**
 * Check if memory chunk is fragmented according to this byte size.
 *
 * @param size - size in bytes.
 * @return true fragmented, otherwise false.
 */
bool
memory_chunk::is_fragmented(uint32_t size)
{
  return (this->free >= size) && (!can_reserve(size));
}

/**
 * Defragment all memory.
 */
void
memory_chunk::defragmentation()
{
  if (memory_chunk_if::reserved_memory_num() == 0)
  {
    return;
  }

  memory_chunk_if::reserved_memory_sort();
  memory *mem = memory_chunk_if::reserved_memory_front();

  if (mem->get_address() > start_address)
  {
    /*
     * Active memory isn't first in memory chunk.
     * Move it back to start_address.
     */
    memmove((void *) this->start_address,
            (void *) mem->get_address(),
            mem->get_size());
    mem->assign(this->start_address, mem->get_size());
  }

  /*
   * Real defragmentation part.
   */
  this->reserved_memory->for_each([&](entity *e1, entity *e2) {
    memory *mem = (memory *) e1;
    memory *adjacent_memory = (memory *) e2;

    mem->align(adjacent_memory);
    return FOREACH_CONTINUE;
  });

  uint32_t free_temp = this->free;
  memory_chunk_if::free_memory_delete_all();
  memory *back_reserved = memory_chunk_if::reserved_memory_back();
  memory_chunk_if::free_memory_add(back_reserved->get_address() + back_reserved->get_size(), free_temp);
}

memory_chunk::~memory_chunk()
{
  std::free((void *) start_address);
}
