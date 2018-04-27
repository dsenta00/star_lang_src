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

#include "../../include/MemoryBundle/memory_chunk_test.h"
#include "MemoryBundle/MemoryChunk.h"
#include "../../test_assert.h"
#include "MemoryBundle/Memory.h"
#include "ErrorBundle/ErrorLog.h"
#include "ORM/ORM.h"

/**
 * Advanced memory chunk test.
 */
static void
memory_chunk_test_advanced()
{
#define BIGGER_CAPACITY (262144)
#define RESERVATION     (32768)

    std::vector<Memory *> to_keep;
    std::vector<Memory *> to_free;

    MemoryChunk &chunk = *MemoryChunk::create(BIGGER_CAPACITY);

    for (size_t i = 0; i < BIGGER_CAPACITY / RESERVATION; i++)
    {
        Memory *mem = chunk.reserve(RESERVATION);
        ASSERT_NOT_NULL(mem);

        if ((i % 2) == 1)
        {
            to_free.push_back(mem);
        }
        else
        {
            to_keep.push_back(mem);
        }
    }

    ASSERT_EQUALS(chunk.getFree(), 0);
    uint32_t free = 0;

    while (!to_free.empty())
    {
        chunk.release(to_free.front());
        to_free.erase(to_free.begin());
        free += RESERVATION;

        ASSERT_EQUALS(chunk.getFree(), free);
    }

    ASSERT_FALSE(chunk.worthDefragmentation(),
                 "chunk shouldn't be worth of defragmentation");

    for (size_t i = 0; i < to_keep.size() - 1; i++)
    {
        Memory *mem1 = to_keep[i];
        Memory *mem2 = to_keep[i + 1];
        ASSERT_EQUALS(mem1->getAddress() + (2 * RESERVATION), mem2->getAddress());
    }

    chunk.defragmentation();
    ASSERT_EQUALS(chunk.getFree(), free);
    ASSERT_TRUE(chunk.canReserve(free), "chunk should be able to reserve %u", free);

    for (size_t i = 0; i < to_keep.size() - 1; i++)
    {
        Memory *mem1 = to_keep[i];
        Memory *mem2 = to_keep[i + 1];
        ASSERT_EQUALS(mem1->getAddress() + RESERVATION, mem2->getAddress());
    }

    while (!to_keep.empty())
    {
        chunk.release(to_keep.front());
        to_keep.erase(to_keep.begin());

        free += RESERVATION;
        ASSERT_EQUALS(chunk.getFree(), free);
    }

    ASSERT_TRUE(chunk.canReserve(BIGGER_CAPACITY),
                "chunk should be able to reserve %u", BIGGER_CAPACITY);
    while (true)
    {
        Memory *mem_rand = chunk.reserve(rand() % 72);

        if (mem_rand == nullptr)
        {
            break;
        }

        to_keep.push_back(mem_rand);
    }

    for (uint32_t i = 0; i < to_keep.size(); i += 2)
    {
        Memory *mem_rand = to_keep[i];
        chunk.release(mem_rand);
        to_keep.erase(to_keep.begin() + i);
    }

    chunk.defragmentation();

    for (uint32_t i = 0; i < to_keep.size() - 1; i++)
    {
        Memory *mem1 = to_keep[i];
        Memory *mem2 = to_keep[i + 1];
        ASSERT_EQUALS(mem1->getAddress() + mem1->getSize(), mem2->getAddress());
    }
}

/**
 * Basic memory chunk test.
 */
static void
memory_chunk_test_basic()
{
#define MEMORY_CHUNK_SIZE         (100)
#define BYTES_RESERVATION_30      (30)
#define BYTES_RESERVATION_20      (20)
#define BYTES_FINAL_FREE_CAPACITY (2 * BYTES_RESERVATION_20)

    uint32_t capacity = MEMORY_CHUNK_SIZE;
    MemoryChunk &chunk = *MemoryChunk::create(capacity);

    chunk.defragmentation();

    /* Negative test cases */
    ASSERT_TRUE(chunk.canReserve(capacity),
                "Chunk should be able to reserve %u bytes.",
                capacity);
    ASSERT_FALSE(chunk.worthDefragmentation(),
                 "Chunk shouldn't be worth defragmentation");
    ASSERT_FALSE(chunk.isParentOf(nullptr), "is_part_of should return false");
    ASSERT_NULL(chunk.reserve(0));
    ASSERT_NULL(chunk.reserve(capacity + 1));

    for (uint32_t i = 1; i < UINT8_MAX; i++)
    {
        ASSERT_FALSE(chunk.isFragmented(i),
                     "Chunk shouldn't be fragmented according to %u bytes.",
                     i);
    }

    Memory &dummy_memory = *Memory::create(0xDEADBEEF, 43);

    ASSERT_EQUALS(chunk.release(&dummy_memory), MEMORY_CHUNK_RELEASE_UNKNOWN_ADDRESS);
    ASSERT_EQUALS(chunk.release(nullptr), MEMORY_CHUNK_RELEASE_NULL_MEMORY);
    ASSERT_EQUALS(chunk.resize(&dummy_memory, 0), MEMORY_CHUNK_RESIZE_UNKNOWN_ADDRESS);
    ASSERT_EQUALS(chunk.resize(nullptr, 0), MEMORY_CHUNK_RESIZE_NULL_MEMORY);
    ERROR_LOG_CLEAR;

    /* Reservation #1 */
    /* Result: [x][x][x][-][-][-][-][-][-][-] */

    capacity -= BYTES_RESERVATION_30;
    Memory *mem = chunk.reserve(BYTES_RESERVATION_30);

    ASSERT_OK;
    ASSERT_NOT_NULL(mem);
    ASSERT_NOT_EQUALS(mem->getAddress(), 0);
    ASSERT_EQUALS(mem->getSize(), BYTES_RESERVATION_30);
    ASSERT_TRUE(chunk.canReserve(capacity),
                "Chunk should be able to reserve %u bytes.",
                capacity);
    ASSERT_FALSE(chunk.worthDefragmentation(),
                 "Chunk shouldn't be worth defragmentation");
    ASSERT_TRUE(chunk.isParentOf(mem), "isParentOf should return true");
    ASSERT_NULL(chunk.reserve(0));
    ASSERT_NULL(chunk.reserve(capacity + 1));

    /* Reservation #2 */
    /* Result: [x][x][x][x][x][-][-][-][-][-] */

    capacity -= BYTES_RESERVATION_20;
    Memory *mem2 = chunk.reserve(BYTES_RESERVATION_20);

    ASSERT_OK;
    ASSERT_NOT_NULL(mem2);
    ASSERT_NOT_EQUALS(mem2->getAddress(), 0);
    ASSERT_EQUALS(mem2->getSize(), BYTES_RESERVATION_20);
    ASSERT_TRUE(chunk.canReserve(capacity),
                "Chunk should be able to reserve %u bytes.",
                capacity);
    ASSERT_FALSE(chunk.worthDefragmentation(),
                 "Chunk shouldn't be worth defragmentation");
    ASSERT_TRUE(chunk.isParentOf(mem2), "is_part_of should return true");
    ASSERT_NULL(chunk.reserve(0));
    ASSERT_NULL(chunk.reserve(capacity + 1));

    /* Reservation #3 */
    /* Result: [x][x][x][x][x][x][x][x][-][-] */

    capacity -= BYTES_RESERVATION_30;
    Memory *mem3 = chunk.reserve(BYTES_RESERVATION_30);

    ASSERT_OK;
    ASSERT_NOT_NULL(mem3);
    ASSERT_NOT_EQUALS(mem3->getAddress(), 0);
    ASSERT_EQUALS(mem3->getSize(), BYTES_RESERVATION_30);
    ASSERT_TRUE(chunk.canReserve(capacity),
                "Chunk should be able to reserve %u bytes.",
                capacity);
    ASSERT_FALSE(chunk.worthDefragmentation(),
                 "Chunk shouldn't be worth defragmentation");
    ASSERT_TRUE(chunk.isParentOf(mem3), "is_part_of should return true");
    ASSERT_NULL(chunk.reserve(0));
    ASSERT_NULL(chunk.reserve(capacity + 1));

    /* Deletion of #2 */
    /* Result: [x][x][x][-][-][x][x][x][-][-] */

    capacity += BYTES_RESERVATION_20;
    ASSERT_EQUALS(chunk.release(mem2), MEMORY_CHUNK_RELEASE_OK);
    ASSERT_TRUE(chunk.canReserve(BYTES_RESERVATION_20),
                "Chunk should be able to reserve %u bytes.",
                BYTES_RESERVATION_20);
    ASSERT_FALSE(chunk.worthDefragmentation(),
                 "Chunk shouldn't be worth defragmentation");
    ASSERT_NULL(chunk.reserve(0));
    ASSERT_NULL(chunk.reserve(capacity + 1));

    ASSERT_FALSE(chunk.canReserve(BYTES_FINAL_FREE_CAPACITY),
                 "Chunk should be able to reserve %u bytes.");
    ASSERT_NULL(chunk.reserve(BYTES_FINAL_FREE_CAPACITY));

    /*
     * resize #1 Memory negative case
     *
     * Result: [x][x][x][-][-][x][x][x][-][-]
     *          |-----|>>>>>>>>|
     *          |-----|>>>>>>>>>>>>>>>>>>>>|
     *          |-----|>>>>>>>>>>>>>>>>>>>>>>|
     */

    uint32_t status;

    status = chunk.resize(mem, 60);
    ASSERT_EQUALS(status, MEMORY_CHUNK_RESIZE_FRAGMENTED_MEMORY);

    status = chunk.resize(mem, MEMORY_CHUNK_SIZE);
    ASSERT_EQUALS(status, MEMORY_CHUNK_RESIZE_NO_MEMORY);

    status = chunk.resize(mem, MEMORY_CHUNK_SIZE + 1);
    ASSERT_EQUALS(status, MEMORY_CHUNK_RESIZE_NO_MEMORY);

    /*
     * Resize #1 Memory negative case
     *
     * Result: [x][x][x][x][x][x][x][x][-][-]
     */

    capacity -= BYTES_RESERVATION_20;
    status = chunk.resize(mem, BYTES_RESERVATION_30 + BYTES_RESERVATION_20);

    ASSERT_EQUALS(status, MEMORY_CHUNK_RESIZE_OK);
    ASSERT_NOT_NULL(mem);
    ASSERT_NOT_EQUALS(mem->getAddress(), 0);
    ASSERT_EQUALS(mem->getSize(), BYTES_RESERVATION_30 + BYTES_RESERVATION_20);
    ASSERT_TRUE(chunk.canReserve(capacity),
                "Chunk should be able to reserve %u bytes.",
                capacity);
    ASSERT_FALSE(chunk.worthDefragmentation(),
                 "Chunk shouldn't be worth defragmentation");
    ASSERT_TRUE(chunk.isParentOf(mem), "is_part_of should return true");
    ASSERT_NULL(chunk.reserve(0));
    ASSERT_NULL(chunk.reserve(capacity + 1));

    ORM_DESTROY(&chunk);
}

/**
 * Test memory chunk.
 */
void
memory_chunk_test()
{
    RUN_TEST(memory_chunk_test_basic());
    RUN_TEST(memory_chunk_test_advanced());
}
