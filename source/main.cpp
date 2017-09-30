#include "../tests/test.h"
#include "ORM/orm.h"
#include "box_virtual_memory.h"
#include <cstdlib>

/**
 * Main program.
 *
 * @param argc
 * @param argv
 * @return
 */
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

  orm::remove_entity_repository("memory");
  orm::remove_entity_repository("box_memory_chunk");
  orm::remove_entity_repository("box_virtual_memory");
  orm::remove_entity_repository("box_data");
  orm::remove_entity_repository("box_array");
  orm::remove_entity_repository("box_method");
  orm::remove_entity_repository("instruction");

  return EXIT_SUCCESS;
}

