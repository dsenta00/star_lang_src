#include "box_memory_chunk_if.h"
#include "box_monitor.h"
#include "memory.h"
#include <cstring>
#include "ORM/relationship.h"
#include "ORM/orm.h"

memory_chunk_if::memory_chunk_if() : entity::entity("box_memory_chunk", "chunk")
{
  entity::add_relationship("free_memory", ONE_TO_MANY);
  entity::add_relationship("reserved_memory", ONE_TO_MANY);

  this->free_memory = entity::get_relationship("free_memory");
  this->reserved_memory = entity::get_relationship("reserved_memory");
  this->free = 0;
}

void
memory_chunk_if::free_memory_add(uintptr_t address, uint32_t size)
{
  memory *mem = (memory *)orm::create((entity *)new memory(address, size));

  this->free_memory->add_entity((entity *)mem);
  mem->add_entity("free_memory", (entity *)this);

  this->free += size;
}

void
memory_chunk_if::free_memory_remove(memory *mem)
{
  this->free -= mem->get_size();
  orm::destroy(mem);
}

memory *
memory_chunk_if::free_memory_find(std::function<bool(memory *)> foo)
{
  return (memory *)this->free_memory->find([&] (entity *e) {
    memory *m = (memory *)e;
    return foo(m);
  });
}

memory *
memory_chunk_if::free_memory_front()
{
  return (memory *)this->free_memory->front();
}

uint32_t
memory_chunk_if::free_memory_num()
{
  return this->free_memory->num_of_entities();
}

void
memory_chunk_if::free_memory_delete_all()
{
  while (this->free_memory->num_of_entities())
  {
    entity *e = this->free_memory_front();
    this->free -= ((memory *)e)->get_size();
    orm::destroy(e);
  }
}

/**
 * Perform memory union of free memory container.
 */
void
memory_chunk_if::free_memory_union()
{
  this->free_memory->sort([&](entity *e1, entity *e2)
  {
    memory *m1 = (memory *)e1;
    memory *m2 = (memory *)e2;

    return m1->get_address() < m2->get_address();
  });

  this->free_memory->for_each([&] (entity *e1, entity *e2)
  {
    memory *m1 = (memory *)e1;
    memory *m2 = (memory *)e2;

    if (m2->get_address() == (m1->get_address() + m1->get_size()))
    {
      this->free += m2->get_size();
      m1->assign(m1->get_address(), m1->get_size() + m2->get_size());
      memory_chunk_if::free_memory_remove(m2);
      return FOREACH_IT2_REMOVED;
    }

    return FOREACH_CONTINUE;
  });
}

memory *
memory_chunk_if::reserved_memory_add(uintptr_t address, uint32_t size)
{
  memory *mem = (memory *)orm::create((entity *)new memory(address, size));

  this->reserved_memory->add_entity((entity *)mem);
  mem->add_entity("reserved_memory", (entity *)this);

  this->free -= size;
  return mem;
}

void
memory_chunk_if::reserved_memory_remove(memory *mem)
{
  orm::destroy(mem);
}

memory *
memory_chunk_if::reserved_memory_find(std::function<bool(memory *)> foo)
{
  return (memory *)this->reserved_memory->find([&] (entity *e) {
    memory *m = (memory *)e;
    return foo(m);
  });
}

memory *
memory_chunk_if::reserved_memory_front()
{
  return (memory *)this->reserved_memory->front();
}

memory *
memory_chunk_if::reserved_memory_back()
{
  return (memory *)this->reserved_memory->back();
}

uint32_t
memory_chunk_if::reserved_memory_num()
{
  return this->reserved_memory->num_of_entities();
}

void
memory_chunk_if::reserved_memory_sort()
{
  this->reserved_memory->sort([&] (entity *e1, entity *e2)
  {
    memory *m1 = (memory *)e1;
    memory *m2 = (memory *)e2;

    return m1->get_address() < m2->get_address();
  });
}


uint32_t
memory_chunk_if::get_free()
{
  return this->free;
}
