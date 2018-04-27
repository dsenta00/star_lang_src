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
#include <cstdlib>
#include <functional>

class MemoryChunkIf : public Object {
public:
    MemoryChunkIf();

    eObjectType getObjectType() override;

    void freeMemoryAdd(uintptr_t address, uint32_t size);
    void freeMemoryRemove(Memory *mem);
    Memory *freeMemoryFind(std::function<bool(Memory *)> foo);
    Memory *freeMemoryFront();
    uint32_t freeMemoryCount();
    void freeMemoryDeleteAll();
    void freeMemoryUnion();

    Memory *reservedMemoryAdd(uintptr_t address, uint32_t size);
    void reservedMemoryRemove(Memory *mem);
    Memory *reservedMemoryFront();
    Memory *reservedMemoryBack();
    uint32_t reservedMemoryCount();
    void reservedMemorySort();
};