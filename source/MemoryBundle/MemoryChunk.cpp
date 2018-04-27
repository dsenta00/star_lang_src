/*
 * Copyright 2018 Duje Senta
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
#include <ORM/Relationship.h>
#include <ORM/SlaveRelationships.h>
#include <ORM/MasterRelationships.h>
#include <MemoryBundle/MemoryChunk.h>
#include <MemoryBundle/Memory.h>
#include <cstring>

#define DISPERSION_LOW_THRESHOLD (131072)
#define DISPERSION_HIGH_THRESHOLD (401408)

/**
 * The constructor.
 *
 * @param capacity - chunk capacity.
 */
MemoryChunk::MemoryChunk(uint32_t capacity) : MemoryChunkIf::MemoryChunkIf()
{
    this->free = 0;
    this->capacity = 0;
    this->startAddress = 0;

    if (capacity == 0)
    {
        return;
    }

    this->field.reserve(capacity);
    this->startAddress = reinterpret_cast<uintptr_t>(&*this->field.begin());

    if (!this->startAddress)
    {
        return;
    }

    this->free = capacity;
    this->capacity = capacity;
    this->freeMemoryAdd(startAddress, capacity);
}

/**
 * Check if memory is part of memory chunk.
 *
 * @param mem - memory.
 * @return true memory if part of chunk, otherwise false.
 */
bool
MemoryChunk::isParentOf(Memory *mem)
{
    if (!mem)
    {
        return false;
    }

    Relationship *r = mem->getSlave()->get("reservedMemory");

    if (!r)
    {
        r = mem->getSlave()->get("freeMemory");
    }

    if (!r)
    {
        return false;
    }

    return r->find([&](Object *o) { return o == this; }) != nullptr;
}

/**
 * Reserve new memory from memory chunk.
 *
 * @param size - memory size.
 *
 * @return memory if success, otherwise nullptr.
 */
Memory *
MemoryChunk::reserve(uint32_t size)
{
    if (!this->canReserve(size))
    {
        return nullptr;
    }

    auto freeMem = this->freeMemoryFind([&](Memory *m) {
        return m->getSize() >= size;
    });

    if (!freeMem)
    {
        return nullptr;
    }

    uintptr_t address = freeMem->getAddress();

    if (freeMem->getSize() - size == 0)
    {
        this->freeMemoryRemove(freeMem);
    }
    else
    {
        freeMem->assign(freeMem->getAddress() + size, freeMem->getSize() - size);
    }

    auto mem = this->reservedMemoryAdd(address, size);

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
 * @param newSize - new size.
 *
 * @retval MEMORY_CHUNK_EXPAND_OK - success.
 * @retval MEMORY_CHUNK_EXPAND_FRAGMENTED_MEMORY - chunk is to fragmented to find free memory.
 * @retval MEMORY_CHUNK_EXPAND_NO_MEMORY - not enough memory to expand memory.
 * @retval MEMORY_CHUNK_EXPAND_ZERO_CAPACITY - chunk contains zero capacity.
 * @retval MEMORY_CHUNK_EXPAND_ZERO_SIZE - expanding memory with zero size.
 * @retval MEMORY_CHUNK_EXPAND_NULL_MEMORY - expanding NULL memory
 */
eMemoryChunkResizeResult
MemoryChunk::resize(Memory *mem, uint32_t newSize)
{
    if (!mem || (mem->getAddress() == 0))
    {
        return MEMORY_CHUNK_RESIZE_NULL_MEMORY;
    }

    if (!this->isParentOf(mem))
    {
        return MEMORY_CHUNK_RESIZE_UNKNOWN_ADDRESS;
    }

    if (this->capacity == 0)
    {
        return MEMORY_CHUNK_RESIZE_ZERO_CAPACITY;
    }

    if (newSize == 0)
    {
        return MEMORY_CHUNK_RESIZE_ZERO_SIZE;
    }

    if (newSize == mem->getSize())
    {
        return MEMORY_CHUNK_RESIZE_OK;
    }
    else if (newSize < mem->getSize())
    {
        /* Find free Memory after requested Memory */
        Memory *freeMemory = this->freeMemoryFind([&](Memory *m) {
            return m->getAddress() == (mem->getAddress() + mem->getSize());
        });

        if (freeMemory)
        {
            /*
             * Found adjacent freeMemory.
             * Spread over freeMemory.
             */
            this->free -= newSize - mem->getSize();
            freeMemory->assign(freeMemory->getAddress() - mem->getSize() + newSize,
                                freeMemory->getSize() + mem->getSize() - newSize);
            mem->assign(mem->getAddress(), newSize);
        }
        else
        {
            /* Not found adjacent free Memory, create new free Memory */
            this->free -= newSize - mem->getSize();
            this->freeMemoryAdd(mem->getAddress() + newSize, newSize - mem->getSize());
            this->freeMemoryUnion();
            mem->assign(mem->getAddress(), newSize);
        }

        return MEMORY_CHUNK_RESIZE_OK;
    }
    else
    {
        if ((newSize - mem->getSize()) > this->free)
        {
            return MEMORY_CHUNK_RESIZE_NO_MEMORY;
        }

        auto freeMemory = this->freeMemoryFind([&](Memory *m) {
            return m->getAddress() == (mem->getAddress() + mem->getSize());
        });

        if (!freeMemory)
        {
            /*
             * Not found adjacent free Memory, Memory is fragmented.
             */
            return MEMORY_CHUNK_RESIZE_FRAGMENTED_MEMORY;
        }

        /*
         * Found adjacent free Memory,
         * check if new size can spread over free Memory
         */
        if (newSize > (freeMemory->getSize() + mem->getSize()))
        {
            return MEMORY_CHUNK_RESIZE_FRAGMENTED_MEMORY;
        }

        /*
         * Adjacent free Memory has enough space.
         * Spread over free Memory.
         */
        this->free -= newSize - mem->getSize();
        freeMemory->assign(freeMemory->getAddress() + newSize - mem->getSize(),
                            freeMemory->getSize() - newSize + mem->getSize());
        mem->assign(mem->getAddress(), newSize);

        if (freeMemory->getSize() == 0)
        {
            this->freeMemoryRemove(freeMemory);
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
eMemoryChunkReleaseResult
MemoryChunk::release(Memory *mem)
{
    if (!mem)
    {
        return MEMORY_CHUNK_RELEASE_NULL_MEMORY;
    }

    if (!isParentOf(mem))
    {
        return MEMORY_CHUNK_RELEASE_UNKNOWN_ADDRESS;
    }

    if (!mem->isReadyToRemove())
    {
        return MEMORY_CHUNK_RELEASE_OK;
    }

    uintptr_t address = mem->getAddress();
    uint32_t size = mem->getSize();

    this->free += size;

    this->reservedMemoryRemove(mem);
    this->freeMemoryAdd(address, size);
    this->freeMemoryUnion();

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
MemoryChunk::worthDefragmentation()
{
    if (this->freeMemoryCount() == 1)
    {
        return false;
    }

    uint64_t dispersion = free * this->freeMemoryCount();
    return (dispersion >= DISPERSION_LOW_THRESHOLD) && (dispersion < DISPERSION_HIGH_THRESHOLD);
}

/**
 * Check if memory chunk can reserve memory in size.
 *
 * @param size - size in bytes.
 * @return true if can, otherwise false.
 */
bool
MemoryChunk::canReserve(uint32_t size)
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
    if (this->freeMemoryCount() == 1)
    {
        return true;
    }

    return this->freeMemoryFind([&](Memory *m) {
        return m->getSize() >= size;
    }) != nullptr;
}

/**
 * Check if memory chunk is fragmented according to this byte size.
 *
 * @param size - size in bytes.
 * @return true fragmented, otherwise false.
 */
bool
MemoryChunk::isFragmented(uint32_t size)
{
    return (this->free >= size) && (!this->canReserve(size));
}

/**
 * Defragment all memory.
 */
void
MemoryChunk::defragmentation()
{
    if (this->reservedMemoryCount() == 0)
    {
        return;
    }

    this->reservedMemorySort();
    Memory *mem = this->reservedMemoryFront();

    if (mem->getAddress() > this->startAddress)
    {
        /*
         * Active Memory isn't first in Memory chunk.
         * Move it masterRelationshipBack to startAddress.
         */
        memmove((void *) this->startAddress,
                (void *) mem->getAddress(),
                mem->getSize());
        mem->assign(this->startAddress, mem->getSize());
    }

    /*
     * Real defragmentation part.
     */
    auto reservedMemory = this->getMaster()->get("reservedMemory");

    reservedMemory->forEach([&](Object *o1, Object *o2) {
        auto *m1 = (Memory *) o1;
        auto *m2 = (Memory *) o2;

        m1->align(m2);
        return FOREACH_CONTINUE;
    });

    uint32_t free_temp = this->free;
    this->freeMemoryDeleteAll();
    Memory *back_reserved = this->reservedMemoryBack();
    this->freeMemoryAdd(back_reserved->getAddress() + back_reserved->getSize(), free_temp);
}

/**
 * Get free memory in bytes.
 *
 * @return
 */
uint32_t
MemoryChunk::getFree()
{
    return this->free;
}

/**
 * Create memory chunk.
 *
 * @param capacity
 * @return
 */
MemoryChunk *
MemoryChunk::create(uint32_t capacity)
{
    return (MemoryChunk *) ORM::create(new MemoryChunk(capacity));
}
