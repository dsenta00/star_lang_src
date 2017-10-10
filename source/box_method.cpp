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

#include "box_method.h"
#include "ORM/relationship.h"
#include "box_monitor.h"
#include "box_instruction.h"

box_method::box_method(std::string id,
                       std::vector<instruction *> &instructions) : entity::entity("box_method", id)
{
    /*
     * - variable
     * - array
     * - function
     */
    this->master_relationship_add("method_objects", ONE_TO_MANY);

    /*
     * - instructions
     */
    this->master_relationship_add("method_instructions", ONE_TO_MANY);

    relationship *r = this->master_relationship_get("method_instructions");

    for (instruction *i : instructions)
    {
        r->add_entity(i);
        i->master_relationship_add_entity("box_method", (entity *) this);
    }

    this->current_instruction = instructions[0];
}


instruction_result
box_method::execute_next()
{
    this->current_instruction = this->current_instruction->execute();

    if (!BOX_OK)
    {
        return INSTRUCTION_ERROR;
    }

    if (this->current_instruction == nullptr)
    {
        return INSTRUCTION_FINISHED;
    }

    return INSTRUCTION_OK;
}

void
box_method::add_local_object(entity *e)
{
    if (this->local_objects[e->get_id()])
    {
        BOX_ERROR(ERROR_BOX_METHOD_ADD_OBJECTS_OF_SAME_NAME);
        return;
    }

    this->local_objects[e->get_id()] = e;
    this->master_relationship_add_entity("method_objects", e);

    /*
     * object can be used in many methods.
     */
    e->master_relationship_add("method_objects", ONE_TO_MANY);
    e->master_relationship_add_entity("method_objects", (entity *) this);
}

entity *
box_method::get_local_object(std::string id)
{
    return this->local_objects[id];
}

void
box_method::push_stack(entity *e)
{
    this->stack.push_back(e);
}

entity *
box_method::pop_stack()
{
    entity *e = this->stack.back();
    this->stack.pop_back();
    return e;
}
