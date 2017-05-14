#include "box_memory_chunk.h"
#include "box_monitor.h"
#include "box_utils.h"
#include <algorithm>
#include <cstring>

#define DISPERSION_LOW_TRESHOLD (131072)
#define DISPERSION_HIGH_TRESHOLD (401408)

/**
 * The constructor.
 */
memory_chunk::memory_chunk()
{
  capacity = 0;
  start_address = 0;
  free = 0;
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
  return (mem) ? std::find(reserved_memory.begin(),
                           reserved_memory.end(),
                           mem) != reserved_memory.end() : false;
}

/**
 * Find adjacent free memory.
 *
 * @param mem - memory information orientation.
 * @return adjacent free memory if exist, otherwise return NULL.
 */
memory *
memory_chunk::find_adjacent_free(memory *mem)
{
  return vector_find<memory *>(free_memory, [&] (memory *m)
  {
    return m->get_address() == (mem->get_address() + mem->get_size());
  });
}

/**
 * The constructor.
 *
 * @param capacity - chunk capacity.
 */
memory_chunk::memory_chunk(uint32_t capacity)
{
  ::memory_chunk();

  if (capacity == 0)
  {
    return;
  }

  start_address = (uintptr_t)calloc(capacity, sizeof(uint8_t));

  if (!start_address)
  {
    return;
  }

  this->capacity = capacity;
  free = capacity;
  free_memory.push_back(new memory(start_address, capacity));
}

/**
 * Reserve new memory from memory chunk.
 *
 * @param size - memory size.
 *
 * @return memory if success, otherwise NULL.
 */
memory *
memory_chunk::reserve(uint32_t size)
{
  if (!can_reserve(size))
  {
    return NULL;
  }

  memory *fmem = find_free_memory(size);

  if (!fmem)
  {
    return NULL;
  }

  memory *mem = new memory(fmem->get_address(), size);

  free -= size;
  reserved_memory.push_back(mem);

  fmem->assign(fmem->get_address() + size,
               fmem->get_size() - size);

  if (fmem->get_size() == 0)
  {
    vector_remove<memory *>(free_memory, fmem);
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
    memory *fmem = find_adjacent_free(mem);

    if (fmem)
    {
      fmem->assign(fmem->get_address() - mem->get_size() + new_size,
                   fmem->get_size() + mem->get_size() - new_size);
      mem->assign(mem->get_address(), new_size);
    }
    else
    {
      /* Not found adjacent free memory, create new free memory */
      free_memory.push_back(new memory(mem->get_address() + new_size,
                                       new_size - mem->get_size()));
      free_memory_union();
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

    memory *fmem = find_adjacent_free(mem);

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
      vector_remove<memory *>(free_memory, fmem);
    }

    return MEMORY_CHUNK_RESIZE_OK;
  }
}

/**
 * Find free memory.
 *
 * @param size - size in bytes.
 * @return memory if found, otherwise return NULL.
 */
memory *
memory_chunk::find_free_memory(uint32_t size)
{
  return vector_find<memory *>(free_memory, [&] (memory *mem)
  {
    return mem->get_size() >= size;
  });
}

/**
 * Perform memory union of free memory container.
 */
void
memory_chunk::free_memory_union()
{
  vector_sort<memory *>(free_memory, [&] (const memory *m1, const memory *m2)
  {
    return m1->get_address() < m2->get_address();
  });

  vector_foreach2<memory *>(free_memory, [&] (memory *m1, memory *m2)
  {
    if (m2->get_address() == (m1->get_address() + m1->get_size()))
    {
      m1->assign(m1->get_address(), m1->get_size() + m2->get_size());
      vector_remove(free_memory, m2);
      return false;
    }

    return true;
  });
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

  free_memory.push_back(new memory(mem->get_address(), mem->get_size()));
  free += mem->get_size();
  free_memory_union();

  vector_remove<memory *>(reserved_memory, mem);

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
  if (free_memory.size() == 1)
  {
    return false;
  }

  uint64_t dispersion = free * free_memory.size();
  return (dispersion >= DISPERSION_LOW_TRESHOLD) &&
      (dispersion < DISPERSION_HIGH_TRESHOLD);
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

  if (free < size)
  {
    return false;
  }

  /*
   * Optimization part.
   * I'm so damn smart.
   */
  if (free_memory.size() == 1)
  {
    return true;
  }

  return find_free_memory(size) != NULL;
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
  return (free >= size) && (!can_reserve(size));
}

/**
 * Defragment all memory.
 */
void
memory_chunk::defragmentation()
{
  if (reserved_memory.size() == 0)
  {
    return;
  }

  vector_sort<memory *>(reserved_memory,
                        [&] (const memory *m1, const memory *m2)
  {
    return m1->get_address() < m2->get_address();
  });

  memory *mem = reserved_memory.front();

  /*
   * Active memory isn't first in memory chunk.
   * Move it back to start_address.
   */
  if (mem->get_address() > start_address)
  {
    mem->assign(start_address, mem->get_size());
  }

  /*
   * Real defragmentation part.
   */
  vector_foreach2<memory *>(reserved_memory,
                            [&] (memory *mem, memory *adjacent_memory)
  {
    mem->allign(adjacent_memory);
    return true;
  });

  memory *new_fmem;
  new_fmem = new memory(free_memory.front()->get_address() + mem->get_size(),
                        free);

  for (memory *mem : free_memory)
  {
    delete mem;
  };

  free_memory.clear();
  free_memory.push_back(new_fmem);
}

/**
 * The destructor.
 */
memory_chunk::~memory_chunk()
{
  std::free((void *)start_address);

  for (memory *mem : reserved_memory)
  {
    delete mem;
  };

  for (memory *mem : free_memory)
  {
    delete mem;
  };
}
