#include "box_virtual_memory.h"
#include "box_monitor.h"
#include "box_utils.h"
#include <limits.h>

/**
 * Find first bigger number with power of 2.
 *
 * @param number - input number.
 *
 * @return first bigger number with power of 2 if bigger or equal than
 *         CHUNK_MINIMUM_CAPACITY and if less than INT32_MAX
 *         otherwise return CHUNK_MINIMUM_CAPACITY or UINT32_MAX.
 */
static uint32_t
next_power_of_2(uint32_t number)
{
  if (number <= CHUNK_MINIMUM_CAPACITY)
  {
    return CHUNK_MINIMUM_CAPACITY;
  }

  if (number == UINT32_MAX)
  {
    return CHUNK_MAXIMUM_CAPACITY;
  }

  uint32_t power_of_2 = CHUNK_MINIMUM_CAPACITY;

  while (number > power_of_2)
  {
    power_of_2 *= 2;
  }

  return power_of_2;
}

/**
 * Add new memory chunk.
 *
 * @param capacity - requested capacity.
 */
void
box_virtual_memory::add_memory_chunk(uint32_t capacity)
{
  capacity = next_power_of_2(capacity);

  if (capacity > max_allocated_bytes)
  {
    max_allocated_bytes = capacity;
  }

  memory_chunk_list.push_front(new memory_chunk(max_allocated_bytes));
}

/**
 * The constructor.
 *
 * @param init_capacity - initial capacity.
 */
box_virtual_memory::box_virtual_memory(uint32_t init_capacity)
{
  allocated_total = 0;
  max_allocated_bytes = 0;
  add_memory_chunk(init_capacity);
}

/**
 * Reserve new memory from chunk list.
 *
 * @param size - size in bytes.
 * @return memory if found, otherwise NULL.
 */
memory *
box_virtual_memory::reserve(uint32_t size)
{
  memory_chunk *chunk;

  chunk = list_find<memory_chunk *>(memory_chunk_list,
                                    [&] (memory_chunk *chunk)
  {
    return chunk->can_reserve(size);
  });

  if (!chunk)
  {
    return NULL;
  }

  memory *mem = chunk->reserve(size);

  if (mem)
  {
    allocated_total += mem->get_size();
  }

  return mem;
}

/**
 * Find all chunks that can reserve memory in size.
 *
 * @param size - size in bytes.
 *
 * @return memory chunk list.
 */
memory_chunk *
box_virtual_memory::get_fragmented_chunk(uint32_t size)
{
  return list_find<memory_chunk *>(memory_chunk_list,
                                   [&] (memory_chunk *chunk)
  {
    return chunk->is_fragmented(size) && chunk->worth_defragmentation();
  });
}

/**
 * Allocate memory.
 *
 * @param size - size in bytes
 * @return memory if ok, otherwise NULL.
 */
memory *
box_virtual_memory::alloc(uint32_t size)
{
  if (size == 0)
  {
    return NULL;
  }

  memory *mem = reserve(size);

  if (mem)
  {
    return mem;
  }

  memory_chunk *chunk = get_fragmented_chunk(size);

  if (chunk)
  {
    chunk->defragmentation();
    return chunk->reserve(size);
  }

  /*
   * No chunk is worth it (yet).
   * Less expensive is to allocate a new chunk.
   */
  add_memory_chunk(size);
  return reserve(size);
}

/**
 * Get chunk that contains memory.
 *
 * @param mem - the memory.
 * @return memory chunk if found, otherwise NULL.
 */
memory_chunk *
box_virtual_memory::get_chunk_parent(memory *mem)
{
  return list_find<memory_chunk *>(memory_chunk_list,
                                   [&] (memory_chunk *chunk)
  {
    return chunk->is_parent_of(mem);
  });
}

/**
 * Reallocate memory with new size.
 *
 * @param mem - memory.
 * @param new_size - new size in bytes.
 *
 * @return memory with new size if success, otherwise memory with old size.
 */
memory *
box_virtual_memory::realloc(memory *mem,
                            uint32_t new_size)
{
  if (!mem)
  {
    return alloc(new_size);
  }

  memory_chunk *chunk = get_chunk_parent(mem);

  if (!chunk)
  {
    /*
     * Memory is obtained by other mechanism.
     * Return existing memory.
     */
    BOX_ERROR(ERROR_BOX_VIRTUAL_MEMORY_UKNOWN_CHUNK);
    return mem;
  }

  uint32_t old_size = mem->get_size();
  uint32_t result = chunk->resize(mem, new_size);

  switch (result)
  {
  case MEMORY_CHUNK_RESIZE_OK:
    allocated_total += new_size - old_size;
    break;
  case MEMORY_CHUNK_RESIZE_NO_MEMORY:
  case MEMORY_CHUNK_RESIZE_FRAGMENTED_MEMORY:
  {
    memory *new_mem = alloc(new_size);

    if (new_mem)
    {
      memcpy((void *)new_mem->get_address(),
             (void *)mem->get_address(),
             mem->get_size());
      free(mem);
      mem = new_mem;
    }
    break;
  }
  case MEMORY_CHUNK_RESIZE_NULL_MEMORY:
    mem = alloc(new_size);
    break;
  case MEMORY_CHUNK_RESIZE_ZERO_CAPACITY:
    /*
     * This shouldn't happen.
     * If memory belongs to this chunk, it can not have more size than capacity.
     * Return memory.
     */
    BOX_ERROR(ERROR_BOX_VIRTUAL_MEMORY_ZERO_CAPACITY);
    break;
  case MEMORY_CHUNK_RESIZE_ZERO_SIZE:
  case MEMORY_CHUNK_RESIZE_UNKNOWN_ADDRESS:
    /*
     * Do nothing. Keep current memory.
     */
    break;
  default:
    BOX_ERROR(ERROR_BOX_VIRTUAL_MEMORY_UNKNOWN_FAULT);
    break;
  }

  return mem;
}

/**
 * Free memory.
 *
 * @param mem - memory.
 */
void
box_virtual_memory::free(memory *mem)
{
  if (!mem)
  {
    return;
  }

  memory_chunk *chunk = get_chunk_parent(mem);

  if (!chunk)
  {
    BOX_ERROR(ERROR_BOX_VIRTUAL_MEMORY_UKNOWN_CHUNK);
    return;
  }

  size_t size = mem->get_size();

  if (chunk->release(mem) == MEMORY_CHUNK_RELEASE_OK)
  {
    allocated_total -= size;
  }
  else
  {
    BOX_ERROR(ERROR_BOX_VIRTUAL_MEMORY_UNKNOWN_FAULT);
  }
}

/**
 * Get total allocated bytes.
 *
 * @return size in bytes.
 */
uint32_t
box_virtual_memory::get_allocated_total(void)
{
  return allocated_total;
}

/**
 * The destructor.
 */
box_virtual_memory::~box_virtual_memory()
{
  for (memory_chunk *chunk : memory_chunk_list)
  {
    delete chunk;
  };
}

/*
 * Global virtual memory.
 */
box_virtual_memory virtual_memory(CHUNK_MINIMUM_CAPACITY);
