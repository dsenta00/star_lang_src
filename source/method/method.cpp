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

#include "method/method.h"

#include <error_handler/error_log.h>
#include <ORM/relationship.h>
#include <ORM/orm.h>
#include <method/instructions/abstract_instruction.h>
#include <variable/var.h>
#include <locale>
#include <codecvt>
#include <thread/thread.h>

/**
 * The constructor.
 *
 * @param id
 * @param instructions
 */
method::method(std::string id, std::vector<abstract_instruction *> &instructions) : object::object(std::move(id))
{
    /*
     * - value
     * - array
     * - function
     */
    this->master_relationship_add("method_vars", ONE_TO_MANY);

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
 * Push value to stack.
 *
 * @param o
 */
void
method::push_stack(value *v)
{
    auto *thread_relationship = this->slave_relationship_get("thread");

    if (!thread_relationship)
    {
        ERROR_LOG_ADD(ERROR_METHOD_NOT_PART_OF_THREAD);
        return;
    }

    auto *t = (thread *)thread_relationship->front();
    t->push_stack(v);
}

/**
 * Pop value from stack.
 *
 * @return value.
 */
value *
method::pop_stack()
{
    auto *thread_relationship = this->slave_relationship_get("thread");

    if (!thread_relationship)
    {
        ERROR_LOG_ADD(ERROR_METHOD_NOT_PART_OF_THREAD);

        return (value *)orm::get_first(OBJECT_TYPE_NULL);
    }

    auto *t = (thread *)thread_relationship->front();

    return t->pop_stack();
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

void
method::clear()
{
    this->current_instruction = (abstract_instruction *) this->master_relationship_front("method_instructions");
    this->master_relationships_clear_objects("method_vars");
}

void
method::add_var(var *v)
{
    relationship *r = this->master_relationship_get("method_vars");

    var *v2 = (var *)r->find(v->get_id());

    if (v2)
    {
        ERROR_LOG_ADD(ERROR_METHOD_ADD_OBJECTS_OF_SAME_NAME);
        return;
    }

    this->master_relationship_add_object("method_vars", v);
}

var *
method::get_var(std::wstring id)
{
    relationship *r = this->master_relationship_get("method_vars");

    using convert_type = std::codecvt_utf8<wchar_t>;
    std::wstring_convert<convert_type, wchar_t> converter;
    std::string name = converter.to_bytes(id);

    return (var *)r->find(name);
}
