#include "box_memory_chunk_test.h"
#include "box_memory_chunk.h"
#include "box_assert.h"
#include "memory.h"
#include "box_monitor.h"
#include "ORM/orm.h"

/**
 * Advanced memory chunk test.
 */
static void
box_memory_chunk_test_advanced()
{
#define BIGGER_CAPACITY (262144)
#define RESERVATION     (32768)

  std::vector<memory *> to_keep;
  std::vector<memory *> to_free;

  memory_chunk &chunk = *memory_chunk::create(BIGGER_CAPACITY);

  for (size_t i = 0; i < BIGGER_CAPACITY / RESERVATION; i++)
  {
    memory *mem = chunk.reserve(RESERVATION);
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

  ASSERT_EQUALS(chunk.get_free(), 0);
  uint32_t free = 0;

  while (!to_free.empty())
  {
    chunk.release(to_free.front());
    to_free.erase(to_free.begin());
    free += RESERVATION;

    ASSERT_EQUALS(chunk.get_free(), free);
  }

  ASSERT_FALSE(chunk.worth_defragmentation(),
               "chunk shouldn't be worth of defragmentation");

  for (size_t i = 0; i < to_keep.size() - 1; i++)
  {
    memory *mem1 = to_keep[i];
    memory *mem2 = to_keep[i + 1];
    ASSERT_EQUALS(mem1->get_address() + (2 * RESERVATION), mem2->get_address());
  }

  chunk.defragmentation();
  ASSERT_EQUALS(chunk.get_free(), free);
  ASSERT_TRUE(chunk.can_reserve(free), "chunk should be able to reserve %u", free);

  for (size_t i = 0; i < to_keep.size() - 1; i++)
  {
    memory *mem1 = to_keep[i];
    memory *mem2 = to_keep[i + 1];
    ASSERT_EQUALS(mem1->get_address() + RESERVATION, mem2->get_address());
  }

  while (!to_keep.empty())
  {
    chunk.release(to_keep.front());
    to_keep.erase(to_keep.begin());

    free += RESERVATION;
    ASSERT_EQUALS(chunk.get_free(), free);
  }

  ASSERT_TRUE(chunk.can_reserve(BIGGER_CAPACITY),
              "chunk should be able to reserve %u", BIGGER_CAPACITY);
  while (true)
  {
    memory *mem_rand = chunk.reserve(rand() % 72);

    if (mem_rand == nullptr)
    {
      break;
    }

    to_keep.push_back(mem_rand);
  }

  for (uint32_t i = 0; i < to_keep.size(); i += 2)
  {
    memory *mem_rand = to_keep[i];
    chunk.release(mem_rand);
    to_keep.erase(to_keep.begin() + i);
  }

  chunk.defragmentation();

  for (uint32_t i = 0; i < to_keep.size() - 1; i++)
  {
    memory *mem1 = to_keep[i];
    memory *mem2 = to_keep[i + 1];
    ASSERT_EQUALS(mem1->get_address() + mem1->get_size(), mem2->get_address());
  }

  printf("\t-> %s()::OK \n", __FUNCTION__);
}

/**
 * Basic memory chunk test.
 */
static void
box_memory_chunk_test_basic()
{
#define MEMORY_CHUNK_SIZE         (100)
#define BYTES_RESERVATION_30      (30)
#define BYTES_RESERVATION_20      (20)
#define BYTES_FINAL_FREE_CAPACITY (2 * BYTES_RESERVATION_20)

  uint32_t capacity = MEMORY_CHUNK_SIZE;
  memory_chunk &chunk = *memory_chunk::create(capacity);

  chunk.defragmentation();

  /* Negative test cases */
  ASSERT_TRUE(chunk.can_reserve(capacity),
              "Chunk should be able to reserve %u bytes.",
              capacity);
  ASSERT_FALSE(chunk.worth_defragmentation(),
               "Chunk shouldn't be worth defragmentation");
  ASSERT_FALSE(chunk.is_parent_of(nullptr), "is_part_of should return false");
  ASSERT_NULL(chunk.reserve(0));
  ASSERT_NULL(chunk.reserve(capacity + 1));

  for (uint32_t i = 1; i < UINT8_MAX; i++)
  {
    ASSERT_FALSE(chunk.is_fragmented(i),
                 "Chunk shouldn't be fragmented according to %u bytes.",
                 i);
  }

  memory &dummy_memory = *memory::create(0xDEADBEEF, 43);

  ASSERT_EQUALS(chunk.release(&dummy_memory), MEMORY_CHUNK_RELEASE_UNKNOWN_ADDRESS);
  ASSERT_EQUALS(chunk.release(nullptr), MEMORY_CHUNK_RELEASE_NULL_MEMORY);
  ASSERT_EQUALS(chunk.resize(&dummy_memory, 0), MEMORY_CHUNK_RESIZE_UNKNOWN_ADDRESS);
  ASSERT_EQUALS(chunk.resize(nullptr, 0), MEMORY_CHUNK_RESIZE_NULL_MEMORY);
  BOX_ERROR_CLEAR;

  /* Reservation #1 */
  /* Result: [x][x][x][-][-][-][-][-][-][-] */

  capacity -= BYTES_RESERVATION_30;
  memory *mem = chunk.reserve(BYTES_RESERVATION_30);

  ASSERT_OK;
  ASSERT_NOT_NULL(mem);
  ASSERT_NOT_EQUALS(mem->get_address(), 0);
  ASSERT_EQUALS(mem->get_size(), BYTES_RESERVATION_30);
  ASSERT_TRUE(chunk.can_reserve(capacity),
              "Chunk should be able to reserve %u bytes.",
              capacity);
  ASSERT_FALSE(chunk.worth_defragmentation(),
               "Chunk shouldn't be worth defragmentation");
  ASSERT_TRUE(chunk.is_parent_of(mem), "is_parent_of should return true");
  ASSERT_NULL(chunk.reserve(0));
  ASSERT_NULL(chunk.reserve(capacity + 1));

  /* Reservation #2 */
  /* Result: [x][x][x][x][x][-][-][-][-][-] */

  capacity -= BYTES_RESERVATION_20;
  memory *mem2 = chunk.reserve(BYTES_RESERVATION_20);

  ASSERT_OK;
  ASSERT_NOT_NULL(mem2);
  ASSERT_NOT_EQUALS(mem2->get_address(), 0);
  ASSERT_EQUALS(mem2->get_size(), BYTES_RESERVATION_20);
  ASSERT_TRUE(chunk.can_reserve(capacity),
              "Chunk should be able to reserve %u bytes.",
              capacity);
  ASSERT_FALSE(chunk.worth_defragmentation(),
               "Chunk shouldn't be worth defragmentation");
  ASSERT_TRUE(chunk.is_parent_of(mem2), "is_part_of should return true");
  ASSERT_NULL(chunk.reserve(0));
  ASSERT_NULL(chunk.reserve(capacity + 1));

  /* Reservation #3 */
  /* Result: [x][x][x][x][x][x][x][x][-][-] */

  capacity -= BYTES_RESERVATION_30;
  memory *mem3 = chunk.reserve(BYTES_RESERVATION_30);

  ASSERT_OK;
  ASSERT_NOT_NULL(mem3);
  ASSERT_NOT_EQUALS(mem3->get_address(), 0);
  ASSERT_EQUALS(mem3->get_size(), BYTES_RESERVATION_30);
  ASSERT_TRUE(chunk.can_reserve(capacity),
              "Chunk should be able to reserve %u bytes.",
              capacity);
  ASSERT_FALSE(chunk.worth_defragmentation(),
               "Chunk shouldn't be worth defragmentation");
  ASSERT_TRUE(chunk.is_parent_of(mem3), "is_part_of should return true");
  ASSERT_NULL(chunk.reserve(0));
  ASSERT_NULL(chunk.reserve(capacity + 1));

  /* Deletion of #2 */
  /* Result: [x][x][x][-][-][x][x][x][-][-] */

  capacity += BYTES_RESERVATION_20;
  ASSERT_EQUALS(chunk.release(mem2), MEMORY_CHUNK_RELEASE_OK);
  ASSERT_TRUE(chunk.can_reserve(BYTES_RESERVATION_20),
              "Chunk should be able to reserve %u bytes.",
              BYTES_RESERVATION_20);
  ASSERT_FALSE(chunk.worth_defragmentation(),
               "Chunk shouldn't be worth defragmentation");
  ASSERT_NULL(chunk.reserve(0));
  ASSERT_NULL(chunk.reserve(capacity + 1));

  ASSERT_FALSE(chunk.can_reserve(BYTES_FINAL_FREE_CAPACITY),
               "Chunk should be able to reserve %u bytes.");
  ASSERT_NULL(chunk.reserve(BYTES_FINAL_FREE_CAPACITY));

  /*
   * resize #1 memory negative case
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
   * Resize #1 memory negative case
   *
   * Result: [x][x][x][x][x][x][x][x][-][-]
   */

  capacity -= BYTES_RESERVATION_20;
  status = chunk.resize(mem, BYTES_RESERVATION_30 + BYTES_RESERVATION_20);

  ASSERT_EQUALS(status, MEMORY_CHUNK_RESIZE_OK);
  ASSERT_NOT_NULL(mem);
  ASSERT_NOT_EQUALS(mem->get_address(), 0);
  ASSERT_EQUALS(mem->get_size(), BYTES_RESERVATION_30 + BYTES_RESERVATION_20);
  ASSERT_TRUE(chunk.can_reserve(capacity),
              "Chunk should be able to reserve %u bytes.",
              capacity);
  ASSERT_FALSE(chunk.worth_defragmentation(),
               "Chunk shouldn't be worth defragmentation");
  ASSERT_TRUE(chunk.is_parent_of(mem), "is_part_of should return true");
  ASSERT_NULL(chunk.reserve(0));
  ASSERT_NULL(chunk.reserve(capacity + 1));

  ORM_DESTROY(&chunk);
  printf("\t-> %s()::OK \n", __FUNCTION__);
}

/**
 * Test memory chunk.
 */
void
box_memory_chunk_test()
{
  printf("%s()\r\n", __FUNCTION__);
  box_memory_chunk_test_basic();
  box_memory_chunk_test_advanced();
  printf("\r\n\r\n");
}
