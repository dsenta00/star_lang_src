/*
 * Copyright 2018 Duje Senta
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

#include <ORM/Relationship.h>
#include <ORM/MasterRelationships.h>
#include <MemoryBundle/Memory.h>
#include <MemoryBundle/MemoryChunkIf.h>

/**
 * The constructor.
 */
MemoryChunkIf::MemoryChunkIf() : Object::Object("chunk")
{
    MasterRelationships *master = this->getMaster();

    master->init("freeMemory", ONE_TO_MANY);
    master->init("reservedMemory", ONE_TO_MANY);
}

/**
 * Add free memory.
 *
 * @param address
 * @param size
 */
void
MemoryChunkIf::freeMemoryAdd(uintptr_t address, uint32_t size)
{
    Memory *mem = Memory::create(address, size);

    this->getMaster()->add("freeMemory", mem);
}

/**
 * Remove free memory.
 * @param mem
 */
void
MemoryChunkIf::freeMemoryRemove(Memory *mem)
{
    this->getMaster()->remove("freeMemory", mem);
}

/**
 * Find free memory.
 *
 * @param foo
 * @return
 */
Memory *
MemoryChunkIf::freeMemoryFind(std::function<bool(Memory *)> foo)
{
    auto freeMemory = this->getMaster()->get("freeMemory");

    return (Memory *) freeMemory->find([&](Object *e) {
        auto *m = (Memory *) e;
        return foo(m);
    });
}

/**
 * Get first free memory.
 *
 * @return
 */
Memory *
MemoryChunkIf::freeMemoryFront()
{
    auto freeMemory = this->getMaster()->get("freeMemory");

    return (Memory *) freeMemory->front();
}

/**
 * Get number of free memory.
 *
 * @return
 */
uint32_t
MemoryChunkIf::freeMemoryCount()
{
    auto freeMemory = this->getMaster()->get("freeMemory");

    return static_cast<uint32_t>(freeMemory->size());
}

/**
 * Delete all free_memory.
 */
void
MemoryChunkIf::freeMemoryDeleteAll()
{
    while (this->freeMemoryCount())
    {
        Memory *m = this->freeMemoryFront();
        this->freeMemoryRemove(m);
    }
}

/**
 * Perform memory union of free memory container.
 */
void
MemoryChunkIf::freeMemoryUnion()
{
    auto freeMemory = this->getMaster()->get("freeMemory");

    freeMemory->sort([&](Object *e1, Object *e2) {
        auto *m1 = (Memory *) e1;
        auto m2 = (Memory *) e2;

        return m1->getAddress() < m2->getAddress();
    });

    freeMemory->forEach([&](Object *e1, Object *e2) {
        auto *m1 = (Memory *) e1;
        auto *m2 = (Memory *) e2;

        if (m2->getAddress() == (m1->getAddress() + m1->getSize())) {
            m1->assign(m1->getAddress(), m1->getSize() + m2->getSize());
            this->freeMemoryRemove(m2);
            return FOREACH_IT2_REMOVED;
        }

        return FOREACH_CONTINUE;
    });
}

/**
 * Add reserved memory.
 *
 * @param address
 * @param size
 * @return
 */
Memory *
MemoryChunkIf::reservedMemoryAdd(uintptr_t address, uint32_t size)
{
    Memory *mem = Memory::create(address, size);

    this->getMaster()->add("reservedMemory", (Object *) mem);

    return mem;
}

/**
 * Remove reserved memory.
 * @param mem
 */
void
MemoryChunkIf::reservedMemoryRemove(Memory *mem)
{
    this->getMaster()->remove("reservedMemory", mem);
}

/**
 * Get fist reserved memory.
 *
 * @return
 */
Memory *
MemoryChunkIf::reservedMemoryFront()
{
    auto reservedMemory = this->getMaster()->get("reservedMemory");

    return (Memory *) reservedMemory->front();
}

/**
 * Get last reserved memory.
 *
 * @return
 */
Memory *
MemoryChunkIf::reservedMemoryBack()
{
    auto reservedMemory = this->getMaster()->get("reservedMemory");

    return (Memory *) reservedMemory->back();
}

/**
 * Get number of reserved memory.
 * @return
 */
uint32_t
MemoryChunkIf::reservedMemoryCount()
{
    auto reservedMemory = this->getMaster()->get("reservedMemory");

    return static_cast<uint32_t>(reservedMemory->size());
}

/**
 * Sort reserved memory.
 */
void
MemoryChunkIf::reservedMemorySort()
{
    auto reservedMemory = this->getMaster()->get("reservedMemory");

    reservedMemory->sort([&](Object *e1, Object *e2) {
        auto m1 = (Memory *) e1;
        auto m2 = (Memory *) e2;

        return m1->getAddress() < m2->getAddress();
    });
}

/**
 * @inherit
 */
eObjectType
MemoryChunkIf::getObjectType()
{
    return OBJECT_TYPE_MEMORY_CHUNK;
}
