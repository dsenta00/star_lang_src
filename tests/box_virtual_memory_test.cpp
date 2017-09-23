#include "box_virtual_memory_test.h"
#include "box_virtual_memory.h"
#include "box_assert.h"
#include "box_monitor.h"
#include "memory.h"
#include "ORM/orm.h"
#include <ctime>

/**
 * Test virtual memory basic.
 */
static void
box_virtual_memory_test_basic()
{
  srand((unsigned)time(nullptr));

  box_virtual_memory &vm_zero_cap = *box_virtual_memory::create(0);
  ASSERT_VIRTUAL_MEMORY(vm_zero_cap, 0);

  memory *data1 = vm_zero_cap.alloc(64);
  ASSERT_TRUE(data1 != nullptr,
              "should allocate %u bytes!",
              64);
  ASSERT_VIRTUAL_MEMORY(vm_zero_cap, 64);

  memory *data2 = vm_zero_cap.realloc(nullptr, 64);
  ASSERT_TRUE(data2 != nullptr,
              "should allocate %u bytes!",
              64);
  ASSERT_VIRTUAL_MEMORY(vm_zero_cap, 128);

  vm_zero_cap.free(data1);
  ASSERT_VIRTUAL_MEMORY(vm_zero_cap, 64);
  vm_zero_cap.free(data2);
  ASSERT_VIRTUAL_MEMORY(vm_zero_cap, 0);

  box_virtual_memory &vm = *box_virtual_memory::create(CHUNK_MINIMUM_CAPACITY);

  ASSERT_TRUE(vm.alloc(0) == nullptr, "should not allocate 0 bytes!");
  ASSERT_TRUE(vm.alloc(UINT32_MAX) == nullptr,
              "should not allocate %u bytes!",
              UINT32_MAX);

  std::vector<memory *> memory_array;

  for (uint32_t i = 0; i < UINT8_MAX; i++)
  {
    size_t size = (rand() % 8192) + 1;
    memory *mem = vm.alloc(size);

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
    memory *mem = memory_array[i];
    size_t old_size = mem->get_size();
    size_t new_size = mem->get_size() * 3;

    mem = vm.realloc(mem, new_size);

    ASSERT_OK;
    ASSERT_TRUE(mem != nullptr, "memory isn't reallocated! %u", new_size);
    ASSERT_TRUE(mem->get_size() == new_size,
                "memory should have new size %u (%u) old_size -> %u",
                new_size,
                mem->get_size(),
                old_size);
  }

  size_t size = 64;
  memory *mem = nullptr;
  mem = vm.realloc(mem, size);
  ASSERT_TRUE(mem != nullptr, "memory isn't reallocated! %u", size);
  ASSERT_TRUE(mem->get_size() == size,
              "memory should have new size %u (%u) old_size -> (none)",
              size,
              mem->get_size());

  memory &mem_unknown = *memory::create(0x204, 32);
  vm.realloc(&mem_unknown, 32);
  ASSERT_ERROR(ERROR_BOX_VIRTUAL_MEMORY_UNKNOWN_CHUNK);

  vm.free(&mem_unknown);
  ASSERT_ERROR(ERROR_BOX_VIRTUAL_MEMORY_UNKNOWN_CHUNK);
  BOX_ERROR_CLEAR;

  orm::destroy(&vm);
  orm::destroy(&vm_zero_cap);

  printf("\t-> %s()::OK\n", __FUNCTION__);
}

/**
 * Test virtual memory.
 */
void
box_virtual_memory_test()
{
  printf("%s()\r\n", __FUNCTION__);
  box_virtual_memory_test_basic();
  printf("\r\n\r\n");
}
