#include "tests/test.h"
#include "ORM/orm.h"
#include "box_virtual_memory.h"
#include <cstdlib>

int main(int argc, char *argv[])
{
  (void)argc;
  (void)argv;

  orm::add_entity_repostiory("memory");
  orm::add_entity_repostiory("box_memory_chunk");
  orm::add_entity_repostiory("box_array");
  orm::add_entity_repostiory("box_virtual_memory");
  orm::add_entity_repostiory("box_data");
  orm::add_entity_repostiory("box_method");
  orm::add_entity_repostiory("instruction");

  orm::create((entity *)new box_virtual_memory(CHUNK_MINIMUM_CAPACITY));

  run_tests();

  return EXIT_SUCCESS;
}

