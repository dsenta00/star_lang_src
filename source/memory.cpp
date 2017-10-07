#include <ORM/orm.h>
#include "memory.h"
#include "ORM/relationship.h"

/**
 * The constructor.
 *
 * @param address - memory address.
 * @param size - memory size.
 */
memory::memory(uintptr_t address, uint32_t size) : entity::entity("memory", address)
{
  this->address = address;
  this->size = size;
}

/**
 * Get cast address.
 *
 * @return address.
 */
template<typename T>
T
memory::get_pointer()
{
  return (T) address;
}

/**
 * Get memory address.
 *
 * @return address.
 */
uintptr_t
memory::get_address() const
{
  return address;
}

/**
 * Get element.
 *
 * @return element.
 */
template<typename T>
T &
memory::get_element()
{
  return *(T *) address;
}

/**
 * Align adjacent memory.
 *
 * @param adjacent_memory - adjacent memory
 */
void
memory::align(memory *adjacent_memory)
{
  memmove((void *) (this->address + size),
          (void *) adjacent_memory->address,
          adjacent_memory->size);

  adjacent_memory->assign(this->address + this->size, adjacent_memory->get_size());
}

/**
 * Get memory size.
 *
 * @return size.
 */
uint32_t
memory::get_size()
{
  return this->size;
}

/**
 * Operator <. Compare memory address.
 *
 * @param mem - another memory.
 *
 * @return true if this memory address is smaller than another.
 */
bool
memory::operator<(const memory &mem) const
{
  return address < mem.address;
}

/**
 * Operator +=. Increase memory size.
 *
 * @param size - size to increase.
 */
void
memory::operator+=(uint32_t size)
{
  this->size += size;
}

/**
 * Operator -=. Decrease memory size.
 *
 * @param size - size to ddecrease.
 */
void
memory::operator-=(uint32_t size)
{
  this->size -= size;
}

/**
 * Assign new memory.
 *
 * @param address - memory address.
 * @param size - memory size.
 */
void
memory::assign(uintptr_t address,
               uint32_t size)
{
  std::string new_id = std::to_string(address);

  if (new_id != this->id)
  {
    orm::change_id(this, new_id);
  }

  this->address = address;
  this->size = size;
}

/**
 * Check if memory is ready to remove.
 *
 * @retval true - memory is ready to remove.
 * @retval false - memory isn't ready to remove.
 */
bool
memory::ready_to_remove()
{
  auto r = this->slave_relationship_get("box_data_memory");

  if (!r)
  {
    return true;
  }

  return r->num_of_entities() == 0;
}

memory *
memory::create(uintptr_t address, uint32_t size)
{
  return (memory *)orm::create(new memory(address, size));
}

/*
 * Explicit template instatiation.
 */
template int8_t &memory::get_element();

template int16_t &memory::get_element();

template int32_t &memory::get_element();

template int64_t &memory::get_element();

template double &memory::get_element();

template bool &memory::get_element();

template void *memory::get_pointer();

template char *memory::get_pointer();

template const char *memory::get_pointer();

template int8_t *memory::get_pointer();

template int16_t *memory::get_pointer();

template int32_t *memory::get_pointer();

template int64_t *memory::get_pointer();

template double *memory::get_pointer();
