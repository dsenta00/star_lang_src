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

#include "ORM/ORM.h"
#include "ErrorBundle/ErrorLog.h"
#include "MemoryBundle/VirtualMemory.h"
#include "MemoryBundle/Memory.h"
#include "../../test_assert.h"
#include "../../include/MemoryBundle/virtual_memory_test.h"
#include <ctime>

/**
 * Test virtual memory basic.
 */
static void
virtual_memory_test_basic()
{
    srand((unsigned) time(nullptr));

    VirtualMemory &vm_zero_cap = *VirtualMemory::create(0);
    ASSERT_VIRTUAL_MEMORY(vm_zero_cap, 0);

    Memory *data1 = vm_zero_cap.alloc(64);
    ASSERT_TRUE(data1 != nullptr,
                "should allocate %u bytes!",
                64);
    ASSERT_VIRTUAL_MEMORY(vm_zero_cap, 64);

    Memory *data2 = vm_zero_cap.realloc(nullptr, 64);
    ASSERT_TRUE(data2 != nullptr,
                "should allocate %u bytes!",
                64);
    ASSERT_VIRTUAL_MEMORY(vm_zero_cap, 128);

    vm_zero_cap.free(data1);
    ASSERT_VIRTUAL_MEMORY(vm_zero_cap, 64);
    vm_zero_cap.free(data2);
    ASSERT_VIRTUAL_MEMORY(vm_zero_cap, 0);

    VirtualMemory &vm = *VirtualMemory::create(CHUNK_MINIMUM_CAPACITY);

    ASSERT_TRUE(vm.alloc(0) == nullptr, "should not allocate 0 bytes!");
    ASSERT_TRUE(vm.alloc(UINT32_MAX) == nullptr,
                "should not allocate %u bytes!",
                UINT32_MAX);

    std::vector<Memory *> memory_array;

    for (uint32_t i = 0; i < UINT8_MAX; i++)
    {
        size_t size = (rand() % 8192) + 1;
        Memory *mem = vm.alloc(size);

        ASSERT_OK;
        ASSERT_TRUE(mem != nullptr, "failed to allocate %u bytes", size);
        memory_array.push_back(mem);
    }

    for (uint32_t i = 0; i < UINT8_MAX; i += 2)
    {
        vm.free(memory_array[i]);
        ASSERT_OK;
    }

    for (uint32_t i = 1; i < UINT8_MAX; i += 2)
    {
        Memory *mem = memory_array[i];
        size_t old_size = mem->getSize();
        size_t new_size = mem->getSize() * 3;

        mem = vm.realloc(mem, new_size);

        ASSERT_OK;
        ASSERT_TRUE(mem != nullptr, "Memory isn't reallocated! %u", new_size);
        ASSERT_TRUE(mem->getSize() == new_size,
                    "Memory should have new size %u (%u) old_size -> %u",
                    new_size,
                    mem->getSize(),
                    old_size);
    }

    size_t size = 64;
    Memory *mem = nullptr;
    mem = vm.realloc(mem, size);
    ASSERT_TRUE(mem != nullptr, "Memory isn't reallocated! %u", size);
    ASSERT_TRUE(mem->getSize() == size,
                "Memory should have new size %u (%u) old_size -> (none)",
                size,
                mem->getSize());

    Memory &mem_unknown = *Memory::create(0x204, 32);
    vm.realloc(&mem_unknown, 32);
    ASSERT_ERROR(ERROR_VIRTUAL_MEMORY_UNKNOWN_CHUNK);

    vm.free(&mem_unknown);
    ASSERT_ERROR(ERROR_VIRTUAL_MEMORY_UNKNOWN_CHUNK);
    ERROR_LOG_CLEAR;

    ORM::destroy(&vm);
    ORM::destroy(&vm_zero_cap);
}

/**
 * Test virtual memory.
 */
void
virtual_memory_test()
{
    RUN_TEST(virtual_memory_test_basic());
}
