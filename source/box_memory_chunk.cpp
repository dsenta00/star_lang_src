/*
 * Copyright 2017 Duje Senta, Tomislav Radanovic
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

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
  this->free = 0;
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

  this->free = capacity;
  this->capacity = capacity;
  this->free_memory_add(start_address, capacity);
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
  if (!mem)
  {
    return false;
  }

  relationship *r = mem->slave_relationship_get("reserved_memory");

  if (!r)
  {
    r = mem->slave_relationship_get("free_memory");
  }

  if (!r)
  {
    return false;
  }

  return r->find([&](entity *e) { return e == this; }) != nullptr;
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
  if (!this->can_reserve(size))
  {
    return nullptr;
  }

  auto free_mem = this->free_memory_find([&](memory *m) {
    return m->get_size() >= size;
  });

  if (!free_mem)
  {
    return nullptr;
  }

  uintptr_t address = free_mem->get_address();

  if (free_mem->get_size() - size == 0)
  {
    this->free_memory_remove(free_mem);
  }
  else
  {
    free_mem->assign(free_mem->get_address() + size, free_mem->get_size() - size);
  }

  auto mem = this->reserved_memory_add(address, size);

  if (mem)
  {
    this->free -= size;
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

  if (!this->is_parent_of(mem))
  {
    return MEMORY_CHUNK_RESIZE_UNKNOWN_ADDRESS;
  }

  if (this->capacity == 0)
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
    memory *free_memory = this->free_memory_find([&](memory *m) {
      return m->get_address() == (mem->get_address() + mem->get_size());
    });

    if (free_memory)
    {
      /*
       * Found adjacent free_memory.
       * Spread over free_memory.
       */
      this->free -= new_size - mem->get_size();
      free_memory->assign(free_memory->get_address() - mem->get_size() + new_size,
                          free_memory->get_size() + mem->get_size() - new_size);
      mem->assign(mem->get_address(), new_size);
    }
    else
    {
      /* Not found adjacent free memory, create new free memory */
      this->free -= new_size - mem->get_size();
      this->free_memory_add(mem->get_address() + new_size, new_size - mem->get_size());
      this->free_memory_union();
      mem->assign(mem->get_address(), new_size);
    }

    return MEMORY_CHUNK_RESIZE_OK;
  }
  else
  {
    if ((new_size - mem->get_size()) > this->free)
    {
      return MEMORY_CHUNK_RESIZE_NO_MEMORY;
    }

    auto free_memory = this->free_memory_find([&](memory *m) {
      return m->get_address() == (mem->get_address() + mem->get_size());
    });

    if (!free_memory)
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
    if (new_size > (free_memory->get_size() + mem->get_size()))
    {
      return MEMORY_CHUNK_RESIZE_FRAGMENTED_MEMORY;
    }

    /*
     * Adjacent free memory has enough space.
     * Spread over free memory.
     */
    this->free -= new_size - mem->get_size();
    free_memory->assign(free_memory->get_address() + new_size - mem->get_size(),
                        free_memory->get_size() - new_size + mem->get_size());
    mem->assign(mem->get_address(), new_size);

    if (free_memory->get_size() == 0)
    {
      this->free_memory_remove(free_memory);
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
memory_chunk_release_result
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

  uintptr_t address = mem->get_address();
  uint32_t size = mem->get_size();

  this->free += size;

  this->reserved_memory_remove(mem);
  this->free_memory_add(address, size);
  this->free_memory_union();

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
  if (this->free_memory_num() == 1)
  {
    return false;
  }

  uint64_t dispersion = free * this->free_memory_num();
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
  if (this->free_memory_num() == 1)
  {
    return true;
  }

  return this->free_memory_find([&](memory *m) {
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
  return (this->free >= size) && (!this->can_reserve(size));
}

/**
 * Defragment all memory.
 */
void
memory_chunk::defragmentation()
{
  if (this->reserved_memory_num() == 0)
  {
    return;
  }

  this->reserved_memory_sort();
  memory *mem = this->reserved_memory_front();

  if (mem->get_address() > this->start_address)
  {
    /*
     * Active memory isn't first in memory chunk.
     * Move it master_relationship_back to start_address.
     */
    memmove((void *) this->start_address,
            (void *) mem->get_address(),
            mem->get_size());
    mem->assign(this->start_address, mem->get_size());
  }

  /*
   * Real defragmentation part.
   */
  auto reserved_memory = master_relationship_get("reserved_memory");

  reserved_memory->for_each([&](entity *e1, entity *e2) {
    memory *mem = (memory *) e1;
    memory *adjacent_memory = (memory *) e2;

    mem->align(adjacent_memory);
    return FOREACH_CONTINUE;
  });

  uint32_t free_temp = this->free;
  this->free_memory_delete_all();
  memory *back_reserved = this->reserved_memory_back();
  this->free_memory_add(back_reserved->get_address() + back_reserved->get_size(), free_temp);
}

uint32_t
memory_chunk::get_free()
{
  return this->free;
}

memory_chunk::~memory_chunk()
{
  std::free((void *) start_address);
}

memory_chunk *
memory_chunk::create(uint32_t capacity)
{
  return (memory_chunk *) orm::create(new memory_chunk(capacity));
}
