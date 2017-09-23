#include "../tests/test.h"
#include "ORM/orm.h"
#include "box_virtual_memory.h"
#include <cstdlib>

int main(int argc, char *argv[])
{
  (void) argc;
  (void) argv;

  orm::add_entity_repository("memory");
  orm::add_entity_repository("box_memory_chunk");
  orm::add_entity_repository("box_array");
  orm::add_entity_repository("box_virtual_memory");
  orm::add_entity_repository("box_data");
  orm::add_entity_repository("box_method");
  orm::add_entity_repository("instruction");

  /*
   * Create global virtual memory.
   */
  box_virtual_memory::create();

  run_tests();

  return EXIT_SUCCESS;
}

