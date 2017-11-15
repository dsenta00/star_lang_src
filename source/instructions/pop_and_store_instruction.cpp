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

#include <error_log.h>
#include <method.h>
#include <ORM/relationship.h>
#include <ORM/orm.h>
#include <codecvt>
#include <locale>
#include "instructions/pop_and_store_instruction.h"
#include "primitive_data/primitive_data.h"

/**
 * @inherit
 */
pop_and_store_instruction::pop_and_store_instruction(std::vector<std::wstring> &arg)
    : abstract_instruction(OP_CODE_POP_AND_STORE, arg)
{
}

/**
 * @inherit
 */
abstract_instruction *
pop_and_store_instruction::execute()
{
    if (!this->validate())
    {
        return nullptr;
    }

    auto *m = this->get_method();
    auto *data2 = (primitive_data *) m->pop_stack();

    primitive_data *data = (primitive_data *) m->get_local_object(this->arg[0]);
    *data = *data2;

    return (abstract_instruction *) this->master_relationship_get("next_instruction")->front();
}

/**
 * @inherit
 */
pop_and_store_instruction *
pop_and_store_instruction::create(std::wstring name)
{
    std::vector<std::wstring> arg;
    arg.emplace_back(name);

    return dynamic_cast<pop_and_store_instruction *>(orm::create(
        new pop_and_store_instruction(arg)
    ));
}

/**
 * @inherit
 */
bool
pop_and_store_instruction::validate()
{
    if (this->validated)
    {
        return true;
    }

    auto *m = this->get_method();

    if (!m)
    {
        return false;
    }

    auto *data2 = (primitive_data *) m->pop_stack();

    if (!data2)
    {
        ERROR_LOG_ADD(ERROR_INSTRUCTION_OBJECT_DOES_NOT_EXIST);
        return false;
    }

    primitive_data *data = (primitive_data *) m->get_local_object(this->arg[0]);

    if (!data)
    {
        ERROR_LOG_ADD(ERROR_INSTRUCTION_OBJECT_DOES_NOT_EXIST);
        return false;
    }

    this->validated = true;
    return true;
}
