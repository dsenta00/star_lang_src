/*
 * Copyright 2017 Duje Senta
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <ORM/ORM.h>
#include <ORM/Relationship.h>
#include <ORM/SlaveRelationships.h>
#include <MemoryBundle/Memory.h>
#include <cstring>

/**
 * The constructor.
 *
 * @param address - memory address.
 * @param size - memory size.
 */
Memory::Memory(uintptr_t address, uint32_t size) : Object::Object(address)
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
Memory::getPointer()
{
    return (T) address;
}

/**
 * Get memory address.
 *
 * @return address.
 */
uintptr_t
Memory::getAddress() const
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
Memory::getElement()
{
    return *(T *) address;
}

/**
 * Align adjacent memory.
 *
 * @param adjacentMemory - adjacent memory
 */
void
Memory::align(Memory *adjacentMemory)
{
    memmove((void *) (this->address + size),
            (void *) adjacentMemory->address,
            adjacentMemory->size);

    adjacentMemory->assign(this->address + this->size, adjacentMemory->getSize());
}

/**
 * Get memory size.
 *
 * @return size.
 */
uint32_t
Memory::getSize()
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
Memory::operator<(const Memory &mem) const
{
    return address < mem.address;
}

/**
 * Operator +=. Increase memory size.
 *
 * @param size - size to increase.
 */
void
Memory::operator+=(uint32_t size)
{
    this->size += size;
}

/**
 * Operator -=. Decrease memory size.
 *
 * @param size - size to ddecrease.
 */
void
Memory::operator-=(uint32_t size)
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
Memory::assign(uintptr_t address, uint32_t size)
{
    std::string newId = std::to_string(address);

    if (newId != this->id)
    {
        ORM::changeId(this, newId);
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
Memory::isReadyToRemove()
{
    auto r = this->getSlave()->get("primitive_data_memory");

    if (!r)
    {
        return true;
    }

    return r->empty();
}

Memory *
Memory::create(uintptr_t address, uint32_t size)
{
    return (Memory *) ORM::create(new Memory(address, size));
}

/**
 * @inherit
 */
eObjectType
Memory::getObjectType()
{
    return OBJECT_TYPE_MEMORY;
}

/*
 * Explicit template instatiation.
 */
template wchar_t &Memory::getElement();

template int16_t &Memory::getElement();

template int32_t &Memory::getElement();

template int64_t &Memory::getElement();

template double &Memory::getElement();

template bool &Memory::getElement();

template void *Memory::getPointer();

template wchar_t *Memory::getPointer();

template const wchar_t *Memory::getPointer();

template int16_t *Memory::getPointer();

template int32_t *Memory::getPointer();

template int64_t *Memory::getPointer();

template double *Memory::getPointer();
