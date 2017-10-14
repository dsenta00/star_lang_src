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

#ifndef BOX_VIRTUAL_MEMORY_H
#define BOX_VIRTUAL_MEMORY_H

#include "ORM/entity.h"
#include "fw_decl.h"
#include <cstdint>
#include <functional>

#define CHUNK_MINIMUM_CAPACITY (32768)
#define CHUNK_MAXIMUM_CAPACITY (134217728)

class virtual_memory : public entity {
public:
    explicit virtual_memory(uint32_t init_capacity = CHUNK_MINIMUM_CAPACITY);
    memory *alloc(uint32_t size);
    memory *realloc(memory *mem, uint32_t new_size);
    void free(memory *mem);
    uint32_t get_allocated_total();

    static virtual_memory *create(uint32_t init_capacity = CHUNK_MINIMUM_CAPACITY);
protected:
    memory *add_new_chunk_and_alloc(uint32_t size);
    memory *solve_defragmentation_and_alloc(uint32_t size);
    memory_chunk *find_memory_chunk(std::function<bool(memory_chunk *)> func);
    memory_chunk *add_memory_chunk(uint32_t capacity);
    memory *reserve(uint32_t size);
    memory *reserve_from_chunk(memory_chunk *chunk, uint32_t size);

    uint32_t allocated_total;
    uint32_t max_allocated_bytes;
    relationship *memory_chunk_relationship;
};

#endif // BOX_VIRTUAL_MEMORY_H
