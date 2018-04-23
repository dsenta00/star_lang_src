/*
 * Copyright 2017 Duje Senta
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

#include <ORM/ORM.h>
#include <ErrorBundle/ErrorLog.h>
#include <ORM/Relationship.h>
#include <ORM/MasterRelationships.h>
#include <MemoryBundle/Memory.h>
#include <MemoryBundle/MemoryChunk.h>
#include <MemoryBundle/VirtualMemory.h>
#include <cstring>

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
 * @param initCapacity - initial capacity.
 */
VirtualMemory::VirtualMemory(uint32_t initCapacity) : Object::Object("MAIN")
{
    MasterRelationships *master = this->getMaster();

    master->init("memoryChunkRelationship", ONE_TO_MANY);
    this->memoryChunkRelationship = master->get("memoryChunkRelationship");

    this->allocatedTotal = 0;
    this->maxAllocatedBytes = 0;
    this->addMemoryChunk(initCapacity);
}

/**
 * @brief virtual_memory::find_memory_chunk
 * @param func
 * @return
 */
MemoryChunk *
VirtualMemory::findMemoryChunk(std::function<bool(MemoryChunk *)> func)
{
    if (this->memoryChunkRelationship->empty())
    {
        return nullptr;
    }

    for (Object *e : *this->memoryChunkRelationship)
    {
        auto *chunk = (MemoryChunk *) e;

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
MemoryChunk *
VirtualMemory::addMemoryChunk(uint32_t capacity)
{
    capacity = next_power_of_2(capacity);

    if (capacity > maxAllocatedBytes)
    {
        maxAllocatedBytes = capacity;
    }

    MemoryChunk *chunk = MemoryChunk::create(maxAllocatedBytes);
    this->getMaster()->add("memoryChunkRelationship", chunk);

    return chunk;
}

/**
 * Reserve memory from chunk.
 *
 * @param chunk
 * @param size
 * @return memory if found, otherwise return NULL.
 */
Memory *
VirtualMemory::reserveFromChunk(MemoryChunk *chunk, uint32_t size)
{
    Memory *mem = chunk->reserve(size);

    if (mem)
    {
        this->allocatedTotal += mem->getSize();
    }

    return mem;
}

/**
 * Reserve new memory from chunk list.
 *
 * @param size - size in bytes.
 * @return memory if found, otherwise NULL.
 */
Memory *
VirtualMemory::reserve(uint32_t size)
{
    MemoryChunk *chunk = this->findMemoryChunk([&](MemoryChunk *chunk) {
        return chunk->canReserve(size);
    });

    if (!chunk)
    {
        return nullptr;
    }

    return this->reserveFromChunk(chunk, size);
}

Memory *
VirtualMemory::addChunkAndAlloc(uint32_t size)
{
    MemoryChunk *chunk = this->addMemoryChunk(size);
    if (chunk->canReserve(size))
    {
        return this->reserveFromChunk(chunk, size);
    }

    /*
     * New chunk is not allocated.
     * Remove previously allocated chunk and defragment all Memory.
     */
    this->getMaster()->remove("memoryChunkRelationship", chunk);

    for (Object *o : *this->memoryChunkRelationship)
    {
        auto *chunk = (MemoryChunk *) o;
        chunk->defragmentation();
    }

    return this->reserve(size);
}

Memory *
VirtualMemory::solveDefragmentationAndAlloc(uint32_t size)
{
    /*
     * Check for fragmentation according to size.
     */
    MemoryChunk *chunk = this->findMemoryChunk([&](MemoryChunk *chunk) {
        return chunk->isFragmented(size) && chunk->worthDefragmentation();
    });

    if (!chunk)
    {
        /*
         * No chunk is worth it (yet).
         * Less expensive is to allocate a new chunk.
         */
        return this->addChunkAndAlloc(size);
    }

    chunk->defragmentation();

    if (chunk->canReserve(size))
    {
        return this->reserveFromChunk(chunk, size);
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
Memory *
VirtualMemory::alloc(uint32_t size)
{
    if ((size == 0) || (size == UINT32_MAX))
    {
        return nullptr;
    }

    Memory *mem = this->reserve(size);

    if (mem)
    {
        return mem;
    }

    return this->solveDefragmentationAndAlloc(size);
}

/**
 * Reallocate memory with new size.
 *
 * @param mem - memory.
 * @param newSize - new size in bytes.
 *
 * @return memory with new size if success, otherwise memory with old size.
 */
Memory *
VirtualMemory::realloc(Memory *mem, uint32_t newSize)
{
    if (!mem)
    {
        /*
         * realloc(nullptr, size) == alloc(size);
         */
        return this->alloc(newSize);
    }

    MemoryChunk *chunk = this->findMemoryChunk([&](MemoryChunk *chunk) {
        return chunk->isParentOf(mem);
    });

    if (!chunk)
    {
        /*
         * Memory is obtained by other mechanism.
         * Return existing Memory.
         */
        ERROR_LOG_ADD(ERROR_VIRTUAL_MEMORY_UNKNOWN_CHUNK);
        return mem;
    }

    uint32_t oldSize = mem->getSize();
    uint32_t result = chunk->resize(mem, newSize);

    switch (result)
    {
        case MEMORY_CHUNK_RESIZE_OK:
        {
            allocatedTotal += newSize - oldSize;
            break;
        }
        case MEMORY_CHUNK_RESIZE_NO_MEMORY:
        {
            Memory *newMem = this->addChunkAndAlloc(newSize);

            if (newMem)
            {
                memcpy((void *) newMem->getAddress(),
                       (void *) mem->getAddress(),
                       mem->getSize());

                this->free(mem);
                mem = newMem;
            }
            break;
        }
        case MEMORY_CHUNK_RESIZE_FRAGMENTED_MEMORY:
        {
            Memory *newMem = this->solveDefragmentationAndAlloc(newSize);

            if (newMem)
            {
                memcpy((void *) newMem->getAddress(),
                       (void *) mem->getAddress(),
                       mem->getSize());

                this->free(mem);
                mem = newMem;
            }
            break;
        }
        case MEMORY_CHUNK_RESIZE_NULL_MEMORY:
        {
            mem = alloc(newSize);
            break;
        }
        case MEMORY_CHUNK_RESIZE_ZERO_CAPACITY:
        {
            /*
             * This shouldn't happen.
             * If Memory belongs to this chunk, it can not have more size than capacity.
             * Return Memory.
             */
            ERROR_LOG_ADD(ERROR_VIRTUAL_MEMORY_ZERO_CAPACITY);
            break;
        }
        case MEMORY_CHUNK_RESIZE_ZERO_SIZE:
        case MEMORY_CHUNK_RESIZE_UNKNOWN_ADDRESS:
        {
            /*
           * Do nothing. Keep current Memory.
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
VirtualMemory::free(Memory *mem)
{
    if (!mem)
    {
        return;
    }

    MemoryChunk *chunk = this->findMemoryChunk([&](MemoryChunk *chunk) {
        return chunk->isParentOf(mem);
    });

    if (!chunk)
    {
        ERROR_LOG_ADD(ERROR_VIRTUAL_MEMORY_UNKNOWN_CHUNK);
        return;
    }

    size_t size = mem->getSize();

    if (chunk->release(mem) == MEMORY_CHUNK_RELEASE_OK)
    {
        allocatedTotal -= size;
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
VirtualMemory::getAllocatedTotal()
{
    return allocatedTotal;
}

/**
 * Create virtual memory.
 *
 * @param initCapacity
 * @return
 */
VirtualMemory *
VirtualMemory::create(uint32_t initCapacity)
{
    return (VirtualMemory *) ORM::create(new VirtualMemory(initCapacity));
}

/**
 * @inherit
 */
eObjectType
VirtualMemory::getObjectType()
{
    return OBJECT_TYPE_VIRTUAL_MEMORY;
}
