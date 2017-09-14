#include "test.h"
#include "ORM/orm.h"
#include "box_virtual_memory.h"
#include <cstdlib>

int main(int argc, char *argv[])
{
  (void)argc;
  (void)argv;

  orm::addEntityRepostiory("memory");
  orm::addEntityRepostiory("box_memory_chunk");
  orm::addEntityRepostiory("box_array");
  orm::addEntityRepostiory("box_virtual_memory");
  orm::addEntityRepostiory("box_data");

  orm::create((entity *)new box_virtual_memory(CHUNK_MINIMUM_CAPACITY));

  run_tests();

  return EXIT_SUCCESS;
}

