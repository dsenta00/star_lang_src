#include "test.h"
#include "../ORM/orm_test.h"
#include "../box_data_test.h"
#include "box_array_test.h"
#include "box_memory_chunk_test.h"
#include "box_virtual_memory_test.h"
#include <cstdio>

void run_tests()
{
  orm_test();
  box_memory_chunk_test();
  box_virtual_memory_test();
  box_data_test();
  box_array_test();

  printf("TESTS ARE OK!\n");
}
