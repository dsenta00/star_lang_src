#include "box_memory_chunk.h"
#include "box_monitor.h"
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
memory_chunk::find_adjacent_free(const memory *mem)
{
  std::vector<memory *>::iterator position;

  position = std::find_if(free_memory.begin(),
                       free_memory.end(),
                       [&] (memory *m) -> bool
                       {
                         return m->address == (mem->address + mem->size);
                       });

  return (position != free_memory.end()) ? *position : NULL;
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

  memory *mem = new memory(fmem->address, size);

  free -= size;
  reserved_memory.push_back(mem);

  fmem->address += size;
  fmem->size -= size;

  if (fmem->size == 0)
  {
    free_memory.erase(std::remove(free_memory.begin(),
                                  free_memory.end(),
                                  fmem), free_memory.end());
    delete fmem;
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
  if (!mem || (mem->address == 0))
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

  if (new_size == mem->size)
  {
    return MEMORY_CHUNK_RESIZE_OK;
  }
  else if (new_size < mem->size)
  {
    /* Find free memory after requested memory */
    memory *fmem = find_adjacent_free(mem);

    if (fmem)
    {
      fmem->address -= mem->size - new_size;
      fmem->size += mem->size - new_size;
      mem->size = new_size;
    }
    else
    {
      /* Not found adjacent free memory, create new free memory */
      free_memory.push_back(new memory(mem->address + new_size,
                                       new_size - mem->size));
      free_memory_union();
      mem->size = new_size;
    }

    return MEMORY_CHUNK_RESIZE_OK;
  }
  else
  {
    if ((new_size - mem->size) > free)
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
    if (new_size > (fmem->size + mem->size))
    {
      return MEMORY_CHUNK_RESIZE_FRAGMENTED_MEMORY;
    }

    /*
     * Adjacent free memory has enough space.
     * Spread over free memory.
     */
    fmem->address += new_size - mem->size;
    fmem->size -= new_size - mem->size;
    mem->size = new_size;

    if (fmem->size == 0)
    {
      free_memory.erase(std::remove(free_memory.begin(),
                                    free_memory.end(),
                                    fmem), free_memory.end());
      delete fmem;
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
  std::vector<memory *>::iterator position;

  position = std::find_if(free_memory.begin(),
                          free_memory.end(),
                          [&] (memory *mem)
                          {
                            return mem->size >= size;
                          });

  if (position == free_memory.end())
  {
    return NULL;
  }

  return *position;
}

/**
 * Perform memory union of free memory container.
 */
void
memory_chunk::free_memory_union()
{
  std::sort(free_memory.begin(),
            free_memory.end(),
            [&] (const memory *m1, const memory *m2)
            {
              return m1->address < m2->address;
            });

  for (std::vector<memory *>::const_iterator i = free_memory.begin();
       i != free_memory.end() - 1;
       i++)
  {
    memory *m1 = *i;
    memory *m2 = *(i + 1);

    if (m2->address == (m1->address + m1->size))
    {
      m1->size += m2->size;
      free_memory.erase(i + 1);
      delete m2;
      i--;
    }
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

  memset((void *)mem->address, 0, mem->size);

  free_memory.push_back(new memory(mem->address, mem->size));
  free += mem->size;

  reserved_memory.erase(std::remove(reserved_memory.begin(),
                                    reserved_memory.end(),
                                    mem), reserved_memory.end());
  free_memory_union();

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

  std::sort(reserved_memory.begin(),
            reserved_memory.end(),
            [&] (const memory *m1, const memory *m2)
            {
              return m1->address < m2->address;
            });

  memory *mem = reserved_memory[0];

  /*
   * Active memory isn't first in memory chunk.
   * Move it back to start_address.
   */
  if (mem->address > start_address)
  {
    memcpy((void *)start_address, (void *)mem->address, mem->size);
    mem->address = start_address;
  }

  memory *mem_prev;

  /*
   * Real defragmentation part.
   */
  for (uint32_t i = 1; i < reserved_memory.size(); i++)
  {
    mem = reserved_memory[i];
    mem_prev = reserved_memory[i - 1];

    memcpy((void *)(mem_prev->address + mem_prev->size),
           (void *)mem->address,
           mem->size);

    mem->address = mem_prev->address + mem_prev->size;
  }

  memory *fmem;

  /*
   * Remove all free memory and keep one entry.
   */
  while (free_memory.size() > 1)
  {
    std::vector<memory *>::iterator last = free_memory.end() - 1;
    memory *fmem = *last;
    free_memory.erase(last);
    delete fmem;
  }

  mem = reserved_memory.back();
  fmem = free_memory.front();

  fmem->address = mem->address + mem->size;
  fmem->size = free;
  memset((void *)fmem->address, 0, fmem->size);
}

/**
 * The destructor.
 */
memory_chunk::~memory_chunk()
{
  std::free((void *)start_address);

  while (reserved_memory.size())
  {
    memory *mem = reserved_memory.back();
    reserved_memory.pop_back();
    delete mem;
  }

  while (free_memory.size())
  {
    memory *mem = free_memory.back();
    free_memory.pop_back();
    delete mem;
  }
}
