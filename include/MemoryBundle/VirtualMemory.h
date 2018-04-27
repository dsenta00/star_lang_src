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

#pragma once

#include "ORM/Object.h"
#include "fw_decl.h"
#include <cstdint>
#include <functional>

#define CHUNK_MINIMUM_CAPACITY (32768)
#define CHUNK_MAXIMUM_CAPACITY (134217728)

/**
 * Virtual memory object.
 */
class VirtualMemory : public Object {
public:
    explicit VirtualMemory(uint32_t initCapacity = CHUNK_MINIMUM_CAPACITY);

    eObjectType getObjectType() override;

    Memory *alloc(uint32_t size);
    Memory *realloc(Memory *mem, uint32_t newSize);
    void free(Memory *mem);
    uint32_t getAllocatedTotal();

    static VirtualMemory *create(uint32_t initCapacity = CHUNK_MINIMUM_CAPACITY);
protected:
    Memory *addChunkAndAlloc(uint32_t size);
    Memory *solveDefragmentationAndAlloc(uint32_t size);
    MemoryChunk *findMemoryChunk(std::function<bool(MemoryChunk *)> func);
    MemoryChunk *addMemoryChunk(uint32_t capacity);
    Memory *reserve(uint32_t size);
    Memory *reserveFromChunk(MemoryChunk *chunk, uint32_t size);

    uint32_t allocatedTotal;
    uint32_t maxAllocatedBytes;
    Relationship *memoryChunkRelationship;
};