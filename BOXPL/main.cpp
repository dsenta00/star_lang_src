#include <iostream>
#include "box_data_test.h"
#include "box_array_test.h"
#include "box_memory_chunk_test.h"
#include "box_data_member_test.h"
#include "box_member_collection_test.h"
#include "box_virtual_memory_test.h"

using namespace std;

int main(int argc, char *argv[])
{
  (void)argc;
  (void)argv;

  box_memory_chunk_test();
  box_virtual_memory_test();
  box_data_test();
  box_array_test();
  box_data_member_test();
  box_member_collection_test();

  return 0;
}

