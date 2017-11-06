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

#include "instructions/abstract_instruction.h"
#include "ORM/relationship.h"
#include "method.h"
#include "error_log.h"
#include <regex>

/**
 * An abstract instruction constructor.
 *
 * @param op
 * @param arg
 */
abstract_instruction::abstract_instruction(op_code op, std::vector<std::string> arg) : object::object(
    "abstract_instruction", op)
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
 * Detect data_type.
 *
 * @param sample
 * @return
 */
data_type
abstract_instruction::detect_data_type(std::string &sample)
{
    if (sample.empty())
    {
        return DATA_TYPE_INVALID;
    }

    if (std::regex_match(sample, std::regex("([-+]?[0-9]+)")))
    {
        return DATA_TYPE_INT;
    }
    else if (std::regex_match(sample, std::regex("[-+]?[0-9]*\\.?[0-9]*")))
    {
        return DATA_TYPE_FLOAT;
    }
    else if (std::regex_match(sample, std::regex("\'?.\'")))
    {
        return DATA_TYPE_CHAR;
    }
    else if (std::regex_match(sample, std::regex("\"(.*)\"")) or std::regex_match(sample, std::regex("\'(.*)+\'")))
    {
        return DATA_TYPE_STRING;
    }
    else if (std::regex_match(sample, std::regex("true|false")))
    {
        return DATA_TYPE_BOOL;
    }
    else
    {
        return DATA_TYPE_INVALID;
    }
}

/**
 * Clean constant format.
 *
 * @param sample
 * @param type
 * @return
 */
std::string
abstract_instruction::clean_constant_format(std::string &sample, data_type type)
{
    switch (type)
    {
        case DATA_TYPE_INT:
        case DATA_TYPE_FLOAT:
            /*
             * nothing to do.
             */
            break;
        case DATA_TYPE_CHAR:
        case DATA_TYPE_STRING:
        {
            sample.erase(sample.begin());
            sample.erase(sample.end());
            break;
        }
        case DATA_TYPE_BOOL:
        {
            sample.assign(1, (char) (sample == "true"));
            break;
        }
        default:
            break;
    }

    return sample;
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
abstract_instruction::object_name_is_valid(std::string &sample)
{
    return !sample.empty() &&
           sample != "collection" &&
           get_from_token(sample) == DATA_TYPE_INVALID;
}
