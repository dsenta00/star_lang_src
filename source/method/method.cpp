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

#include "method/method.h"

#include <error_handler/error_log.h>
#include <ORM/relationship.h>
#include <ORM/orm.h>
#include <method/instructions/abstract_instruction.h>
#include <locale>
#include <codecvt>

/**
 * The constructor.
 *
 * @param id
 * @param instructions
 */
method::method(std::string id, std::vector<abstract_instruction *> &instructions) : object::object(std::move(id))
{
    /*
     * - var
     * - array
     * - function
     */
    this->master_relationship_add("method_objects", ONE_TO_MANY);

    /*
     * - instructions
     */
    this->master_relationship_add("method_instructions", ONE_TO_MANY);

    if (instructions.empty())
    {
        this->current_instruction = nullptr;
        return;
    }

    for (abstract_instruction *i : instructions)
    {
        this->master_relationship_add_object("method_instructions", i);
    }

    relationship *r = this->master_relationship_get("method_instructions");

    r->for_each([&](object *o1, object *o2) {
        o1->master_relationship_add_object("next_instruction", o2);
        return FOREACH_CONTINUE;
    });

    this->current_instruction = instructions[0];
}

/**
 * Execute instruction.
 *
 * @return next instruction.
 */
instruction_result
method::execute_next()
{
    if (!this->current_instruction)
    {
        return INSTRUCTION_ERROR;
    }

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

/**
 * Add local object.
 *
 * @param o - object
 */
void
method::add_local_object(object *o)
{
    std::wstring name;

    /* this is workaround */
    for (char &letter : o->get_id())
    {
        name.push_back((wchar_t) letter);
    }

    if (this->local_objects[name])
    {
        ERROR_LOG_ADD(ERROR_METHOD_ADD_OBJECTS_OF_SAME_NAME);
        return;
    }

    this->local_objects[name] = o;
    this->master_relationship_add_object("method_objects", o);
}

/**
 * Get local object.
 *
 * @param id - object id.
 * @return
 */
object *
method::get_local_object(std::wstring id)
{
    return this->local_objects[id];
}

/**
 * Push object to stack.
 *
 * @param o
 */
void
method::push_stack(object *o)
{
    this->stack.push_back(o);
}

/**
 * Pop object from stack.
 *
 * @return object.
 */
object *
method::pop_stack()
{
    object *e = this->stack.back();
    this->stack.pop_back();

    return e;
}

/**
 * Create method.
 *
 * @param id
 * @param instructions
 * @return
 */
method *
method::create(std::string id, std::vector<abstract_instruction *> &instructions)
{
    return (method *) orm::create(new method(std::move(id), instructions));
}

/**
 * @inherit
 */
object_type
method::get_object_type()
{
    return OBJECT_TYPE_METHOD;
}
