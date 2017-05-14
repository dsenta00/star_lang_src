#include "memory.h"
#include <cstring>

/**
 * The constructor.
 *
 * @param address - memory address.
 * @param size - memory size.
 */
memory::memory(uintptr_t address, uint32_t size)
{
  this->address = address;
  this->size = size;
  counting_reference = 1;
}

/**
 * Get cast address.
 *
 * @return address.
 */
template <typename T>
T
memory::get_pointer()
{
  return (T)address;
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
template <typename T>
T &
memory::get_element()
{
  return *(T *)address;
}

/**
 * Align adjacent memory.
 *
 * @param adjacent_memory - adjacent memory
 */
void
memory::allign(memory *adjacent_memory)
{
  memmove((void *)(address + size),
          (void *)adjacent_memory->address,
          adjacent_memory->size);

  adjacent_memory->address = address + size;
}

/**
 * Get memory size.
 *
 * @return size.
 */
uint32_t
memory::get_size()
{
  return size;
}

/**
 * Operator <. Compare memory address.
 *
 * @param mem - another memory.
 *
 * @return true if this memory address is smaller than another.
 */
bool
memory::operator < (const memory& mem) const
{
  return address < mem.address;
}

/**
 * Operator +=. Increase memory size.
 *
 * @param size - size to increase.
 */
void
memory::operator += (uint32_t size)
{
  size += size;
}

/**
 * Operator -=. Decrease memory size.
 *
 * @param size - size to ddecrease.
 */
void
memory::operator -= (uint32_t size)
{
  size += size;
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
  this->address = address;
  this->size = size;
}

/**
 * Increase counting reference.
 */
void
memory::increase_ref()
{
  counting_reference++;
}

/**
 * Decrease counting reference.
 */
void
memory::decrease_ref()
{
  if (counting_reference > 0)
  {
    counting_reference--;
  }
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
  return counting_reference == 0;
}

/*
 * Explicit template instatiation.
 */
template int8_t &memory::get_element();
template int16_t &memory::get_element();
template int32_t &memory::get_element();
template int64_t &memory::get_element();
template float32_t &memory::get_element();
template float64_t &memory::get_element();
template void *memory::get_pointer();
template char *memory::get_pointer();
template const char *memory::get_pointer();
template int8_t *memory::get_pointer();
template int16_t *memory::get_pointer();
template int32_t *memory::get_pointer();
template int64_t *memory::get_pointer();
template float32_t *memory::get_pointer();
template float64_t *memory::get_pointer();

