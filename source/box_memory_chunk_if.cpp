#include "box_memory_chunk_if.h"
#include "memory.h"
#include "ORM/relationship.h"
#include "ORM/orm.h"

memory_chunk_if::memory_chunk_if() : entity::entity("box_memory_chunk", "chunk")
{
  entity::master_relationship_add("free_memory", ONE_TO_MANY);
  entity::master_relationship_add("reserved_memory", ONE_TO_MANY);
}

void
memory_chunk_if::free_memory_add(uintptr_t address, uint32_t size)
{
  memory *mem = memory::create(address, size);
  this->master_relationship_add_entity("free_memory", mem);
}

void
memory_chunk_if::free_memory_remove(memory *mem)
{
  this->master_relationship_remove_entity("free_memory", mem);
}

memory *
memory_chunk_if::free_memory_find(std::function<bool(memory *)> foo)
{
  auto free_memory = this->master_relationship_get("free_memory");

  return (memory *) free_memory->find([&](entity *e) {
    memory *m = (memory *) e;
    return foo(m);
  });
}

memory *
memory_chunk_if::free_memory_front()
{
  auto free_memory = this->master_relationship_get("free_memory");

  return (memory *) free_memory->front();
}

uint32_t
memory_chunk_if::free_memory_num()
{
  auto free_memory = this->master_relationship_get("free_memory");

  return free_memory->num_of_entities();
}

void
memory_chunk_if::free_memory_delete_all()
{
  while (this->free_memory_num())
  {
    memory *m = this->free_memory_front();
    this->free_memory_remove(m);
  }
}

/**
 * Perform memory union of free memory container.
 */
void
memory_chunk_if::free_memory_union()
{
  auto free_memory = master_relationship_get("free_memory");

  free_memory->sort([&](entity *e1, entity *e2) {
    memory *m1 = (memory *) e1;
    memory *m2 = (memory *) e2;

    return m1->get_address() < m2->get_address();
  });

  free_memory->for_each([&](entity *e1, entity *e2) {
    memory *m1 = (memory *) e1;
    memory *m2 = (memory *) e2;

    if (m2->get_address() == (m1->get_address() + m1->get_size()))
    {
      m1->assign(m1->get_address(), m1->get_size() + m2->get_size());
      this->free_memory_remove(m2);
      return FOREACH_IT2_REMOVED;
    }

    return FOREACH_CONTINUE;
  });
}

memory *
memory_chunk_if::reserved_memory_add(uintptr_t address, uint32_t size)
{
  memory *mem = memory::create(address, size);
  this->master_relationship_add_entity("reserved_memory", (entity *) mem);

  return mem;
}

void
memory_chunk_if::reserved_memory_remove(memory *mem)
{
  this->master_relationship_remove_entity("reserved_memory", mem);
}

memory *
memory_chunk_if::reserved_memory_front()
{
  auto reserved_memory = master_relationship_get("reserved_memory");

  return (memory *) reserved_memory->front();
}

memory *
memory_chunk_if::reserved_memory_back()
{
  auto reserved_memory = master_relationship_get("reserved_memory");

  return (memory *) reserved_memory->back();
}

uint32_t
memory_chunk_if::reserved_memory_num()
{
  auto reserved_memory = master_relationship_get("reserved_memory");

  return reserved_memory->num_of_entities();
}

void
memory_chunk_if::reserved_memory_sort()
{
  auto reserved_memory = master_relationship_get("reserved_memory");

  reserved_memory->sort([&](entity *e1, entity *e2) {
    auto m1 = (memory *) e1;
    auto m2 = (memory *) e2;

    return m1->get_address() < m2->get_address();
  });
}
