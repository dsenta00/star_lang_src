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

#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>
#include "ORM/object.h"

/**
 * The memory object.
 *
 * Memory is obtained by memory chunk in order to data can keep
 * memory information.
 */
class memory : public object {
public:
    memory(uintptr_t address, uint32_t size);

    template<typename T>
    T get_pointer();

    uintptr_t get_address() const;

    template<typename T>
    T &get_element();

    void align(memory *adjacent_memory);
    uint32_t get_size();
    bool operator<(const memory &mem) const;
    void operator+=(uint32_t size);
    void operator-=(uint32_t size);
    void assign(uintptr_t address, uint32_t size);
    bool ready_to_remove();

    static memory *create(uintptr_t address, uint32_t size);
protected:
    uintptr_t address;
    uint32_t size;
};

#endif // MEMORY_H
