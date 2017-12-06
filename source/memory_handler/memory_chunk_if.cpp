/*
 * Copyright 2017 Duje Senta, Tomislav Radanovic
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

#include "memory_handler/memory_chunk_if.h"
#include "memory_handler/memory.h"
#include "ORM/relationship.h"

/**
 * The constructor.
 */
memory_chunk_if::memory_chunk_if() : object::object("chunk")
{
    object::master_relationship_add("free_memory", ONE_TO_MANY);
    object::master_relationship_add("reserved_memory", ONE_TO_MANY);
}

/**
 * Add free memory.
 *
 * @param address
 * @param size
 */
void
memory_chunk_if::free_memory_add(uintptr_t address, uint32_t size)
{
    memory *mem = memory::create(address, size);
    this->master_relationship_add_object("free_memory", mem);
}

/**
 * Remove free memory.
 * @param mem
 */
void
memory_chunk_if::free_memory_remove(memory *mem)
{
    this->master_relationship_remove_object("free_memory", mem);
}

/**
 * Find free memory.
 *
 * @param foo
 * @return
 */
memory *
memory_chunk_if::free_memory_find(std::function<bool(memory *)> foo)
{
    auto free_memory = this->master_relationship_get("free_memory");

    return (memory *) free_memory->find([&](object *e) {
        auto *m = (memory *) e;
        return foo(m);
    });
}

/**
 * Get first free memory.
 *
 * @return
 */
memory *
memory_chunk_if::free_memory_front()
{
    auto free_memory = this->master_relationship_get("free_memory");

    return (memory *) free_memory->front();
}

/**
 * Get number of free memory.
 *
 * @return
 */
uint32_t
memory_chunk_if::free_memory_num()
{
    auto free_memory = this->master_relationship_get("free_memory");

    return static_cast<uint32_t>(free_memory->size());
}

/**
 * Delete all free_memory.
 */
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

    free_memory->sort([&](object *e1, object *e2) {
        auto *m1 = (memory *) e1;
        auto m2 = (memory *) e2;

        return m1->get_address() < m2->get_address();
    });

    free_memory->for_each([&](object *e1, object *e2) {
        auto *m1 = (memory *) e1;
        auto *m2 = (memory *) e2;

        if (m2->get_address() == (m1->get_address() + m1->get_size()))
        {
            m1->assign(m1->get_address(), m1->get_size() + m2->get_size());
            this->free_memory_remove(m2);
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
memory *
memory_chunk_if::reserved_memory_add(uintptr_t address, uint32_t size)
{
    memory *mem = memory::create(address, size);
    this->master_relationship_add_object("reserved_memory", (object *) mem);

    return mem;
}

/**
 * Remove reserved memory.
 * @param mem
 */
void
memory_chunk_if::reserved_memory_remove(memory *mem)
{
    this->master_relationship_remove_object("reserved_memory", mem);
}

/**
 * Get fist reserved memory.
 *
 * @return
 */
memory *
memory_chunk_if::reserved_memory_front()
{
    auto reserved_memory = master_relationship_get("reserved_memory");

    return (memory *) reserved_memory->front();
}

/**
 * Get last reserved memory.
 *
 * @return
 */
memory *
memory_chunk_if::reserved_memory_back()
{
    auto reserved_memory = master_relationship_get("reserved_memory");

    return (memory *) reserved_memory->back();
}

/**
 * Get number of reserved memory.
 * @return
 */
uint32_t
memory_chunk_if::reserved_memory_num()
{
    auto reserved_memory = master_relationship_get("reserved_memory");

    return static_cast<uint32_t>(reserved_memory->size());
}

/**
 * Sort reserved memory.
 */
void
memory_chunk_if::reserved_memory_sort()
{
    auto reserved_memory = master_relationship_get("reserved_memory");

    reserved_memory->sort([&](object *e1, object *e2) {
        auto m1 = (memory *) e1;
        auto m2 = (memory *) e2;

        return m1->get_address() < m2->get_address();
    });
}

/**
 * @inherit
 */
object_type
memory_chunk_if::get_object_type()
{
    return OBJECT_TYPE_MEMORY_CHUNK;
}
