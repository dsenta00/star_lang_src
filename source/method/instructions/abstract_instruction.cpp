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

#include <error_handler/error_log.h>
#include <ORM/relationship.h>
#include <method/instructions/abstract_instruction.h>
#include <method/method.h>
#include <regex>

/**
 * An abstract instruction constructor.
 *
 * @param op
 * @param arg
 */
abstract_instruction::abstract_instruction(op_code op, std::vector<std::wstring> &arg) : object::object(op)
{
    this->validated = false;
    this->op = op;
    this->arg = std::move(arg);
    this->master_relationship_add("next_instruction", ONE_TO_ONE);
    this->master_relationship_add("branch", ONE_TO_ONE);
}

/**
 * Get op_code.
 *
 * @return
 */
op_code &
abstract_instruction::get_op_code()
{
    return this->op;
}

/**
 * Get method where instruction is.
 *
 * @return
 */
method *
abstract_instruction::get_method()
{
    auto *r = this->slave_relationship_get("method_instructions");

    if (!r)
    {
        ERROR_LOG_ADD(ERROR_INSTRUCTION_NO_METHOD);
        return nullptr;
    }

    auto *m = (method *) r->front();

    if (!m)
    {
        ERROR_LOG_ADD(ERROR_INSTRUCTION_NO_METHOD);
    }

    return m;
}

/**
 * Check if object name is valid.
 *
 * @param sample
 * @return
 */
bool
abstract_instruction::object_name_is_valid(std::wstring &sample)
{
    return !sample.empty() &&
           sample != L"collection" &&
           data_type_get_from_token(sample) == OBJECT_TYPE_NULL;
}

/**
 * @inherit
 */
object_type
abstract_instruction::get_object_type()
{
    return OBJECT_TYPE_INSTRUCTION;
}
