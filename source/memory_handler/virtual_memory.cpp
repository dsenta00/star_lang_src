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

#include "memory_handler/virtual_memory.h"
#include "memory_handler/memory_chunk.h"
#include "memory_handler/memory.h"
#include "error_handler/error_log.h"
#include "ORM/relationship.h"
#include "ORM/orm.h"

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
 * The constructor.
 *
 * @param init_capacity - initial capacity.
 */
virtual_memory::virtual_memory(uint32_t init_capacity) : object::object("MAIN")
{
    this->master_relationship_add("memory_chunk_relationship", ONE_TO_MANY);
    this->memory_chunk_relationship = this->master_relationship_get("memory_chunk_relationship");

    this->allocated_total = 0;
    this->max_allocated_bytes = 0;
    this->add_memory_chunk(init_capacity);
}



/**
 * @brief virtual_memory::find_memory_chunk
 * @param func
 * @return
 */
memory_chunk *
virtual_memory::find_memory_chunk(std::function<bool(memory_chunk *)> func)
{
    if (this->memory_chunk_relationship->empty())
    {
        return nullptr;
    }

    for (object *e : *this->memory_chunk_relationship)
    {
        auto *chunk = (memory_chunk *) e;

        if (func(chunk))
        {
            return chunk;
        }
    }

    return nullptr;
}

/**
 * Add new memory chunk.
 *
 * @param capacity - requested capacity.
 */
memory_chunk *
virtual_memory::add_memory_chunk(uint32_t capacity)
{
    capacity = next_power_of_2(capacity);

    if (capacity > max_allocated_bytes)
    {
        max_allocated_bytes = capacity;
    }

    memory_chunk *chunk = memory_chunk::create(max_allocated_bytes);
    this->master_relationship_add_object("memory_chunk_relationship", chunk);

    return chunk;
}

/**
 * Reserve memory from chunk.
 *
 * @param chunk
 * @param size
 * @return memory if found, otherwise return NULL.
 */
memory *
virtual_memory::reserve_from_chunk(memory_chunk *chunk, uint32_t size)
{
    memory *mem = chunk->reserve(size);

    if (mem)
    {
        this->allocated_total += mem->get_size();
    }

    return mem;
}

/**
 * Reserve new memory from chunk list.
 *
 * @param size - size in bytes.
 * @return memory if found, otherwise NULL.
 */
memory *
virtual_memory::reserve(uint32_t size)
{
    memory_chunk *chunk = this->find_memory_chunk([&](memory_chunk *chunk) {
        return chunk->can_reserve(size);
    });

    if (!chunk)
    {
        return nullptr;
    }

    return this->reserve_from_chunk(chunk, size);
}

memory *
virtual_memory::add_new_chunk_and_alloc(uint32_t size)
{
    memory_chunk *chunk = this->add_memory_chunk(size);
    if (chunk->can_reserve(size))
    {
        return this->reserve_from_chunk(chunk, size);
    }

    /*
     * New chunk is not allocated.
     * Remove previously allocated chunk and defragment all memory.
     */
    this->master_relationship_remove_object("memory_chunk_relationship", chunk);

    for (object *o : *this->memory_chunk_relationship)
    {
        memory_chunk *chunk = (memory_chunk *) o;
        chunk->defragmentation();
    }

    return this->reserve(size);
}

memory *
virtual_memory::solve_defragmentation_and_alloc(uint32_t size)
{
    /*
     * Check for fragmentation according to size.
     */
    memory_chunk *chunk = this->find_memory_chunk([&](memory_chunk *chunk) {
        return chunk->is_fragmented(size) && chunk->worth_defragmentation();
    });

    if (!chunk)
    {
        /*
         * No chunk is worth it (yet).
         * Less expensive is to allocate a new chunk.
         */
        return this->add_new_chunk_and_alloc(size);
    }

    chunk->defragmentation();

    if (chunk->can_reserve(size))
    {
        return this->reserve_from_chunk(chunk, size);
    }
    else
    {
        return this->reserve(size);
    }
}

/**
 * Allocate memory.
 *
 * @param size - size in bytes
 * @return memory if ok, otherwise NULL.
 */
memory *
virtual_memory::alloc(uint32_t size)
{
    if ((size == 0) || (size == UINT32_MAX))
    {
        return nullptr;
    }

    memory *mem = this->reserve(size);

    if (mem)
    {
        return mem;
    }

    return this->solve_defragmentation_and_alloc(size);
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
virtual_memory::realloc(memory *mem, uint32_t new_size)
{
    if (!mem)
    {
        /*
         * realloc(nullptr, size) == alloc(size);
         */
        return this->alloc(new_size);
    }

    memory_chunk *chunk = this->find_memory_chunk([&](memory_chunk *chunk) {
        return chunk->is_parent_of(mem);
    });

    if (!chunk)
    {
        /*
         * Memory is obtained by other mechanism.
         * Return existing memory.
         */
        ERROR_LOG_ADD(ERROR_VIRTUAL_MEMORY_UNKNOWN_CHUNK);
        return mem;
    }

    uint32_t old_size = mem->get_size();
    uint32_t result = chunk->resize(mem, new_size);

    switch (result)
    {
        case MEMORY_CHUNK_RESIZE_OK:
        {
            allocated_total += new_size - old_size;
            break;
        }
        case MEMORY_CHUNK_RESIZE_NO_MEMORY:
        {
            memory *new_mem = this->add_new_chunk_and_alloc(new_size);

            if (new_mem)
            {
                memcpy((void *) new_mem->get_address(),
                       (void *) mem->get_address(),
                       mem->get_size());

                this->free(mem);
                mem = new_mem;
            }
            break;
        }
        case MEMORY_CHUNK_RESIZE_FRAGMENTED_MEMORY:
        {
            memory *new_mem = this->solve_defragmentation_and_alloc(new_size);

            if (new_mem)
            {
                memcpy((void *) new_mem->get_address(),
                       (void *) mem->get_address(),
                       mem->get_size());

                this->free(mem);
                mem = new_mem;
            }
            break;
        }
        case MEMORY_CHUNK_RESIZE_NULL_MEMORY:
        {
            mem = alloc(new_size);
            break;
        }
        case MEMORY_CHUNK_RESIZE_ZERO_CAPACITY:
        {
            /*
             * This shouldn't happen.
             * If memory belongs to this chunk, it can not have more size than capacity.
             * Return memory.
             */
            ERROR_LOG_ADD(ERROR_VIRTUAL_MEMORY_ZERO_CAPACITY);
            break;
        }
        case MEMORY_CHUNK_RESIZE_ZERO_SIZE:
        case MEMORY_CHUNK_RESIZE_UNKNOWN_ADDRESS:
        {
            /*
           * Do nothing. Keep current memory.
           */
            break;
        }
        default:
            ERROR_LOG_ADD(ERROR_VIRTUAL_MEMORY_UNKNOWN_FAULT);
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
virtual_memory::free(memory *mem)
{
    if (!mem)
    {
        return;
    }

    memory_chunk *chunk = this->find_memory_chunk([&](memory_chunk *chunk) {
        return chunk->is_parent_of(mem);
    });

    if (!chunk)
    {
        ERROR_LOG_ADD(ERROR_VIRTUAL_MEMORY_UNKNOWN_CHUNK);
        return;
    }

    size_t size = mem->get_size();

    if (chunk->release(mem) == MEMORY_CHUNK_RELEASE_OK)
    {
        allocated_total -= size;
    }
    else
    {
        ERROR_LOG_ADD(ERROR_VIRTUAL_MEMORY_UNKNOWN_FAULT);
    }
}

/**
 * Get total allocated bytes.
 *
 * @return size in bytes.
 */
uint32_t
virtual_memory::get_allocated_total(void)
{
    return allocated_total;
}

/**
 * Create virtual memory.
 *
 * @param init_capacity
 * @return
 */
virtual_memory *
virtual_memory::create(uint32_t init_capacity)
{
    return (virtual_memory *) orm::create(new virtual_memory(init_capacity));
}

/**
 * @inherit
 */
object_type
virtual_memory::get_object_type()
{
    return OBJECT_TYPE_VIRTUAL_MEMORY;
}
