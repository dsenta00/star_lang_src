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

#include "MemoryChunkIf.h"
#include <cstdint>
#include <cstdlib>

typedef enum {
    MEMORY_CHUNK_RELEASE_OK,
    MEMORY_CHUNK_RELEASE_UNKNOWN_ADDRESS,
    MEMORY_CHUNK_RELEASE_NULL_MEMORY
} eMemoryChunkReleaseResult;

typedef enum {
    MEMORY_CHUNK_RESIZE_OK,
    MEMORY_CHUNK_RESIZE_ZERO_CAPACITY,
    MEMORY_CHUNK_RESIZE_ZERO_SIZE,
    MEMORY_CHUNK_RESIZE_NO_MEMORY,
    MEMORY_CHUNK_RESIZE_FRAGMENTED_MEMORY,
    MEMORY_CHUNK_RESIZE_NULL_MEMORY,
    MEMORY_CHUNK_RESIZE_UNKNOWN_ADDRESS
} eMemoryChunkResizeResult;

/**
 * Memory chunk.
 */
class MemoryChunk : public MemoryChunkIf {
public:
    explicit MemoryChunk(uint32_t capacity = 0);
    Memory *reserve(uint32_t size);
    eMemoryChunkResizeResult resize(Memory *mem, uint32_t newSize);
    eMemoryChunkReleaseResult release(Memory *mem);
    bool isParentOf(Memory *mem);
    bool canReserve(uint32_t size);
    bool isFragmented(uint32_t size);
    bool worthDefragmentation();
    void defragmentation();
    uint32_t getFree();

    static MemoryChunk *create(uint32_t capacity = 0);
protected:
    uint32_t free;
    uint32_t capacity;
    uintptr_t startAddress;
    std::vector<uint8_t> field;
};