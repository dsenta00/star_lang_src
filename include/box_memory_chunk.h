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

#ifndef BOX_MEMORY_CHUNK_H
#define BOX_MEMORY_CHUNK_H

#include "box_memory_chunk_if.h"
#include <cstdint>
#include <cstdlib>

typedef enum {
    MEMORY_CHUNK_RELEASE_OK,
    MEMORY_CHUNK_RELEASE_UNKNOWN_ADDRESS,
    MEMORY_CHUNK_RELEASE_NULL_MEMORY
} memory_chunk_release_result;

typedef enum {
    MEMORY_CHUNK_RESIZE_OK,
    MEMORY_CHUNK_RESIZE_ZERO_CAPACITY,
    MEMORY_CHUNK_RESIZE_ZERO_SIZE,
    MEMORY_CHUNK_RESIZE_NO_MEMORY,
    MEMORY_CHUNK_RESIZE_FRAGMENTED_MEMORY,
    MEMORY_CHUNK_RESIZE_NULL_MEMORY,
    MEMORY_CHUNK_RESIZE_UNKNOWN_ADDRESS
} memory_chunk_resize_result;

class memory_chunk : public memory_chunk_if {
public:
    memory_chunk(uint32_t capacity = 0);
    memory *reserve(uint32_t size);
    memory_chunk_resize_result resize(memory *mem, uint32_t new_size);
    memory_chunk_release_result release(memory *mem);
    bool is_parent_of(memory *mem);
    bool can_reserve(uint32_t size);
    bool is_fragmented(uint32_t size);
    bool worth_defragmentation();
    void defragmentation();
    uint32_t get_free();

    static memory_chunk *create(uint32_t capacity = 0);

    virtual ~memory_chunk();
protected:
    uint32_t free;
    uint32_t capacity;
    uintptr_t start_address;
};

#endif // BOX_MEMORY_CHUNK_H
