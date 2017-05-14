#include "box_memory_chunk.h"
#include "box_monitor.h"
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
  return reserved_memory.find_if([&] (memory *mem_it)
  {
    return mem_it == mem;
  }) != NULL;
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

  memory *fmem;

  fmem = free_memory.find_if([&] (memory *mem)
  {
    return mem->get_size() >= size;
  });;

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
    free_memory.delete_element(fmem);
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
    memory *fmem;

    fmem = free_memory.find_if([&] (memory *m)
    {
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

    memory *fmem;

    fmem = free_memory.find_if([&] (memory *m)
    {
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
      free_memory.delete_element(fmem);
    }

    return MEMORY_CHUNK_RESIZE_OK;
  }
}

/**
 * Perform memory union of free memory container.
 */
void
memory_chunk::free_memory_union()
{
  free_memory.sort([&] (const memory *m1, const memory *m2)
  {
    return m1->get_address() < m2->get_address();
  });

  free_memory.for_each([&] (memory *m1, memory *m2)
  {
    if (m2->get_address() == (m1->get_address() + m1->get_size()))
    {
      m1->assign(m1->get_address(), m1->get_size() + m2->get_size());
      free_memory.delete_element(m2);
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

  reserved_memory.delete_element(mem);

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

  return free_memory.find_if([&] (memory *mem)
  {
    return mem->get_size() >= size;
  }) != NULL;
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

  reserved_memory.sort([&] (const memory *m1, const memory *m2)
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
  reserved_memory.for_each([&](memory *mem, memory *adjacent_memory)
  {
    mem->allign(adjacent_memory);
    return true;
  });

  memory *new_fmem;
  new_fmem = new memory(free_memory.front()->get_address() + mem->get_size(),
                        free);
  free_memory.delete_all();
  free_memory.push_back(new_fmem);
}

/**
 * The destructor.
 */
memory_chunk::~memory_chunk()
{
  std::free((void *)start_address);
}
