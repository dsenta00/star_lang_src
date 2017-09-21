#include "box_memory_chunk_test.h"
#include "box_memory_chunk.h"
#include "box_assert.h"
#include "memory.h"
#include <limits.h>
#include <vector>
#include "ORM/orm.h"

static memory_chunk &
alloc_memory_chunk(uint32_t capacity)
{
  return *(memory_chunk *)orm::create((entity *)new memory_chunk(capacity));
}

static memory &
alloc_memory(uintptr_t address, uint32_t size)
{
  return *(memory *)orm::create((entity *)new memory(address, size));
}

/**
 * Advanced memory chunk test.
 */
static void
box_memory_chunk_test_advanced(void)
{
#define BIGGER_CAPACITY (262144)
#define RESERVATION     (32768)

  std::vector<memory *> to_keep;
  std::vector<memory *> to_free;

  memory_chunk &chunk = alloc_memory_chunk(BIGGER_CAPACITY);

  for (size_t i = 0; i < BIGGER_CAPACITY / RESERVATION; i++)
  {
    memory *mem = chunk.reserve(RESERVATION);
    ASSERT_TRUE(mem != NULL, "mem should be reserved");

    if ((i % 2) == 1)
    {
      to_free.push_back(mem);
    }
    else
    {
      to_keep.push_back(mem);
    }
  }

  ASSERT_TRUE(chunk.get_free() == 0, "there should no be enough room for reservation");
  uint32_t free = 0;

  while (to_free.size() > 0)
  {
    chunk.release(to_free.front());
    to_free.erase(to_free.begin());
    free += RESERVATION;

    ASSERT_TRUE(chunk.get_free() == free, "there should be free memory %u (%u)", free, chunk.get_free());
  }

  ASSERT_FALSE(chunk.worth_defragmentation(),
               "chunk shouldn't be worth of defragmentation");

  for (size_t i = 0; i < to_keep.size() - 1; i++)
  {
    memory *mem1 = to_keep[i];
    memory *mem2 = to_keep[i + 1];
    ASSERT_TRUE(((mem1->get_address() + (2 * RESERVATION)) == mem2->get_address()),
                "reserved memories should be %u bytes far",
                RESERVATION);
  }

  chunk.defragmentation();
  ASSERT_TRUE(chunk.get_free() == free, "there should be free memory %u (%u)", free, chunk.get_free());
  ASSERT_TRUE(chunk.can_reserve(free),
              "chunk should be able to reserve %u", free);
  for (size_t i = 0; i < to_keep.size() - 1; i++)
  {
    memory *mem1 = to_keep[i];
    memory *mem2 = to_keep[i + 1];
    ASSERT_TRUE(((mem1->get_address() + RESERVATION) == mem2->get_address()),
                "reserved memories should be %u bytes far (%u)",
                RESERVATION, mem2->get_address() - mem1->get_address());
  }

  while (to_keep.size() > 0)
  {
    chunk.release(to_keep.front());
    to_keep.erase(to_keep.begin());

    free += RESERVATION;
    ASSERT_TRUE(chunk.get_free() == free, "there should be free memory %u (%u)", free, chunk.get_free());
  }

  ASSERT_TRUE(chunk.can_reserve(BIGGER_CAPACITY),
              "chunk should be able to reserve %u", BIGGER_CAPACITY);
  while (true)
  {
    memory *mem_rand = chunk.reserve(rand() % 72);

    if (mem_rand == NULL)
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
    ASSERT_TRUE(((mem1->get_address() + mem1->get_size()) == mem2->get_address()),
                "reserved memories should be %u bytes far",
                RESERVATION);
  }

  printf("\t-> %s()::OK \n", __FUNCTION__);
}

/**
 * Basic memory chunk test.
 */
static void
box_memory_chunk_test_basic(void)
{
#define MEMORY_CHUNK_SIZE         (100)
#define BYTES_RESERVATION_30      (30)
#define BYTES_RESERVATION_20      (20)
#define BYTES_FINAL_FREE_CAPACITY (2 * BYTES_RESERVATION_20)

  size_t capacity = MEMORY_CHUNK_SIZE;
  memory_chunk &chunk = alloc_memory_chunk(capacity);

  chunk.defragmentation();

  /* Negative test cases */
  ASSERT_TRUE(chunk.can_reserve(capacity),
              "Chunk should be able to reserve %u bytes.",
              capacity);
  ASSERT_FALSE(chunk.worth_defragmentation(),
               "Chunk shouldn't be worth refragmentation");
  ASSERT_FALSE(chunk.is_parent_of(NULL), "is_part_of should return false");
  ASSERT_TRUE(chunk.reserve(0) == NULL, "Chunk shouldn't reserve 0 bytes.");
  ASSERT_TRUE(chunk.reserve(capacity + 1) == NULL,
              "Chunk shouldn't reserve %u bytes.",
              capacity + 1);

  for (uint32_t i = 1; i < UINT8_MAX; i++)
  {
    ASSERT_FALSE(chunk.is_fragmented(i),
                 "Chunk shouldn't be fragmented according to %u bytes.",
                 i);
  }

  memory &dummy_memory = alloc_memory(0xDEADBEEF, 43);

  ASSERT_TRUE(chunk.release(&dummy_memory) == MEMORY_CHUNK_RELEASE_UNKNOWN_ADDRESS,
              "Chunk should return MEMORY_CHUNK_RELEASE_UNKNOWN_ADDRESS");
  ASSERT_TRUE(chunk.release(NULL) == MEMORY_CHUNK_RELEASE_NULL_MEMORY,
              "Chunk should return MEMORY_CHUNK_RELEASE_NULL_MEMORY");
  ASSERT_TRUE(chunk.resize(&dummy_memory, 0) == MEMORY_CHUNK_RESIZE_UNKNOWN_ADDRESS,
              "Chunk should return MEMORY_CHUNK_RESIZE_UNKNOWN_ADDRESS");
  ASSERT_TRUE(chunk.resize(NULL, 0) == MEMORY_CHUNK_RESIZE_NULL_MEMORY,
              "Chunk should return MEMORY_CHUNK_RESIZE_NULL_MEMORY");

  /* Reservation #1 */
  /* Result: [x][x][x][-][-][-][-][-][-][-] */

  capacity -= BYTES_RESERVATION_30;
  memory *mem = chunk.reserve(BYTES_RESERVATION_30);

  ASSERT_TRUE(mem != NULL,
              "Reserved memory should be different than NULL");
  ASSERT_TRUE(mem->get_address() != 0,
              "Reserved address should be different than NULL");
  ASSERT_TRUE(mem->get_size() == BYTES_RESERVATION_30,
              "Reserved size should be %u",
              BYTES_RESERVATION_30);
  ASSERT_TRUE(chunk.can_reserve(capacity),
              "Chunk should be able to reserve %u bytes.",
              capacity);
  ASSERT_FALSE(chunk.worth_defragmentation(),
               "Chunk shouldn't be worth refragmentation");
  ASSERT_TRUE(chunk.is_parent_of(mem), "is_part_of should return true");
  ASSERT_TRUE(chunk.reserve(0) == NULL, "Chunk shouldn't reserve 0 bytes.");
  ASSERT_TRUE(chunk.reserve(capacity + 1) == NULL,
              "Chunk shouldn't reserve capacity + 1 bytes.");

  /* Reservation #2 */
  /* Result: [x][x][x][x][x][-][-][-][-][-] */

  capacity -= BYTES_RESERVATION_20;
  memory *mem2 = chunk.reserve(BYTES_RESERVATION_20);

  ASSERT_TRUE(mem2 != NULL, "Reserved memory should be different than NULL");
  ASSERT_TRUE(mem2->get_address() != 0,
              "Reserved address should be different than NULL");
  ASSERT_TRUE(mem2->get_size() == BYTES_RESERVATION_20,
              "Reserved size should be %u",
              BYTES_RESERVATION_20);
  ASSERT_TRUE(chunk.can_reserve(capacity),
              "Chunk should be able to reserve %u bytes.", capacity);
  ASSERT_FALSE(chunk.worth_defragmentation(),
               "Chunk shouldn't be worth refragmentation");
  ASSERT_TRUE(chunk.is_parent_of(mem2), "is_parent_of should return true");
  ASSERT_TRUE(chunk.reserve(0) == NULL, "Chunk shouldn't reserve 0 bytes.");
  ASSERT_TRUE(chunk.reserve(capacity + 1) == NULL,
              "Chunk shouldn't reserve capacity + 1 bytes.");

  /* Reservation #3 */
  /* Result: [x][x][x][x][x][x][x][x][-][-] */

  capacity -= BYTES_RESERVATION_30;
  memory *mem3 = chunk.reserve(BYTES_RESERVATION_30);

  ASSERT_TRUE(mem3 != NULL, "Reserved memory should be different than NULL");
  ASSERT_TRUE(mem3->get_address() != 0,
              "Reserved address should be different than NULL");
  ASSERT_TRUE(mem3->get_size() == BYTES_RESERVATION_30,
              "Reserved size should be %u",
              BYTES_RESERVATION_30);
  ASSERT_TRUE(chunk.can_reserve(capacity),
              "Chunk should be able to reserve %u bytes.",
              capacity);
  ASSERT_FALSE(chunk.worth_defragmentation(),
               "Chunk shouldn't be worth refragmentation");
  ASSERT_TRUE(chunk.is_parent_of(mem3), "is_part_of should return true");
  ASSERT_TRUE(chunk.reserve(0) == NULL, "Chunk shouldn't reserve 0 bytes.");
  ASSERT_TRUE(chunk.reserve(capacity + 1) == NULL,
              "Chunk shouldn't reserve capacity + 1 bytes.");

  /* Deletion of #2 */
  /* Result: [x][x][x][-][-][x][x][x][-][-] */

  capacity += BYTES_RESERVATION_20;
  ASSERT_TRUE(chunk.release(mem2) == MEMORY_CHUNK_RELEASE_OK,
              "Release should be OK.");
  ASSERT_TRUE(chunk.can_reserve(BYTES_RESERVATION_20),
              "Chunk should be able to reserve %u bytes.",
              BYTES_RESERVATION_20);
  ASSERT_FALSE(chunk.worth_defragmentation(),
               "Chunk shouldn't be worth refragmentation");
  ASSERT_TRUE(chunk.reserve(0) == NULL,
              "Chunk shouldn't reserve 0 bytes.");
  ASSERT_TRUE(chunk.reserve(capacity + 1) == NULL,
              "Chunk shouldn't reserve capacity + 1 bytes.");

  ASSERT_FALSE(chunk.can_reserve(BYTES_FINAL_FREE_CAPACITY),
               "Chunk should be able to reserve %u bytes.");
  memory *mem4 = chunk.reserve(BYTES_FINAL_FREE_CAPACITY);
  ASSERT_TRUE(mem4 == NULL, "chunk should not reserve %u memory",
              BYTES_FINAL_FREE_CAPACITY);

  /*
   * Expand #1 memory negative case
   *
   * Result: [x][x][x][-][-][x][x][x][-][-]
   *          |--------------|
   *          |--------------------------|
   *          |----------------------------|
   */

  uint32_t status;

  status = chunk.resize(mem, 60);
  ASSERT_TRUE(status == MEMORY_CHUNK_RESIZE_FRAGMENTED_MEMORY,
              "expand should return MEMORY_CHUNK_EXPAND_FRAGMENTED_MEMORY (%u)",
              status);

  status = chunk.resize(mem, MEMORY_CHUNK_SIZE);
  ASSERT_TRUE(status == MEMORY_CHUNK_RESIZE_NO_MEMORY,
              "expand should return MEMORY_CHUNK_EXPAND_NO_MEMORY (%u)",
              status);

  status = chunk.resize(mem, MEMORY_CHUNK_SIZE + 1);
  ASSERT_TRUE(status == MEMORY_CHUNK_RESIZE_NO_MEMORY,
              "expand should return MEMORY_CHUNK_EXPAND_NO_MEMORY (%u)",
              status);

  /*
   * Expand #1 memory negative case
   *
   * Result: [x][x][x][x][x][x][x][x][-][-]
   */

  capacity -= BYTES_RESERVATION_20;
  status = chunk.resize(mem, (BYTES_RESERVATION_30 + BYTES_RESERVATION_20));

  ASSERT_TRUE(status == MEMORY_CHUNK_RESIZE_OK,
              "expand should return MEMORY_CHUNK_RESIZE_OK (%u)",
              status);
  ASSERT_TRUE(mem != NULL, "Reserved memory should be different than NULL");
  ASSERT_TRUE(mem->get_address() != 0, "Reserved address should be different than NULL");
  ASSERT_TRUE(mem->get_size() == (BYTES_RESERVATION_30 + BYTES_RESERVATION_20),
              "Reserved size should be %u (%u)",
              BYTES_RESERVATION_30 + BYTES_RESERVATION_20,
              mem->get_size());
  ASSERT_TRUE(chunk.can_reserve(capacity),
              "Chunk should be able to reserve %u bytes.",
              capacity);
  ASSERT_FALSE(chunk.worth_defragmentation(),
               "Chunk shouldn't be worth refragmentation");
  ASSERT_TRUE(chunk.is_parent_of(mem), "is_part_of should return true");
  ASSERT_TRUE(chunk.reserve(0) == NULL, "Chunk shouldn't reserve 0 bytes.");
  ASSERT_TRUE(chunk.reserve(capacity + 1) == NULL,
              "Chunk shouldn't reserve capacity + 1 bytes.");

  printf("\t-> %s()::OK \n", __FUNCTION__);
}

/**
 * Test memory chunk.
 */
void
box_memory_chunk_test(void)
{
  printf("%s()\r\n", __FUNCTION__);
  box_memory_chunk_test_basic();
  box_memory_chunk_test_advanced();
  printf("\r\n\r\n");
}
