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

#include "method.h"
#include "ORM/relationship.h"
#include "error_log.h"
#include "instruction.h"

method::method(std::string id,
                       std::vector<instruction *> &instructions) : object::object("method", id)
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
        r->add_object(i);
        i->master_relationship_add_object("method", (object *) this);
    }

    this->current_instruction = instructions[0];
}


instruction_result
method::execute_next()
{
    this->current_instruction = this->current_instruction->execute();

    if (!ERROR_LOG_IS_EMPTY)
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
method::add_local_object(object *e)
{
    if (this->local_objects[e->get_id()])
    {
        ERROR_LOG_ADD(ERROR_METHOD_ADD_OBJECTS_OF_SAME_NAME);
        return;
    }

    this->local_objects[e->get_id()] = e;
    this->master_relationship_add_object("method_objects", e);

    /*
     * object can be used in many methods.
     */
    e->master_relationship_add("method_objects", ONE_TO_MANY);
    e->master_relationship_add_object("method_objects", (object *) this);
}

object *
method::get_local_object(std::string id)
{
    return this->local_objects[id];
}

void
method::push_stack(object *e)
{
    this->stack.push_back(e);
}

object *
method::pop_stack()
{
    object *e = this->stack.back();
    this->stack.pop_back();
    return e;
}
