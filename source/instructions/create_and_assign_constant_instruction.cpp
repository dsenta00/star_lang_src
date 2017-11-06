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
#include <primitive_data/primitive_data.h>
#include <collection.h>
#include <instructions/create_instruction.h>
#include <ORM/orm.h>
#include <ORM/relationship.h>
#include <method.h>
#include "instructions/create_and_assign_constant_instruction.h"

/**
 * @inherit
 */
create_and_assign_constant_instruction::create_and_assign_constant_instruction(std::vector<std::string> &arg)
    : abstract_instruction(OP_CODE_CREATE_AND_ASSIGN_CONSTANT, arg)
{
}

/**
 * @inherit
 */
abstract_instruction *
create_and_assign_constant_instruction::execute()
{
    if (!this->validate())
    {
        return nullptr;
    }

    auto &name = this->arg[0];
    auto &type = this->arg[1];
    auto &constant = this->arg[2];
    auto const_data_type = this->detect_data_type(constant);

    object *data = nullptr;

    if (type == "collection")
    {
        this->clean_constant_format(constant, const_data_type);
        primitive_data *constant_data = primitive_data::create(name, const_data_type, constant.c_str());

        if (!constant_data)
        {
            return nullptr;
        }

        auto &array = *collection::create(name);
        array += constant_data;
        data = &array;
    }
    else
    {
        auto actual_data_type = get_from_token(type);

        if (const_data_type != actual_data_type)
        {
            ERROR_LOG_ADD(ERROR_INSTRUCTION_NO_MATCH_CONSTANT_FORMAT_WITH_DATA_TYPE);
            return nullptr;
        }

        this->clean_constant_format(constant, const_data_type);

        data = primitive_data::create(name, actual_data_type, constant.c_str());

        if (!data)
        {
            return nullptr;
        }
    }

    auto *m = this->get_method();
    m->add_local_object(data);

    return (abstract_instruction *) this->master_relationship_get("next_instruction")->front();
}

/**
 * @inherit
 */
create_and_assign_constant_instruction *
create_and_assign_constant_instruction::create(std::string name, std::string type, std::string constant)
{
    std::vector<std::string> arg;
    arg.emplace_back(name);
    arg.emplace_back(type);
    arg.emplace_back(constant);

    return dynamic_cast<create_and_assign_constant_instruction *>(orm::create(
        new create_and_assign_constant_instruction(arg))
    );
}

/**
 * @inherit
 */
bool
create_and_assign_constant_instruction::validate()
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

    if (this->arg.size() != 3)
    {
        ERROR_LOG_ADD(ERROR_INSTRUCTION_INVALID_NO_OF_ARGS);
        return false;
    }

    auto &name = this->arg[0];

    if (!this->object_name_is_valid(name))
    {
        ERROR_LOG_ADD(ERROR_INSTRUCTION_INVALID_OBJECT_NAME);
        return false;
    }

    auto &constant = this->arg[2];
    auto const_data_type = this->detect_data_type(constant);

    if (const_data_type == DATA_TYPE_INVALID)
    {
        ERROR_LOG_ADD(ERROR_INSTRUCTION_UNKNOWN_CONSTANT_FORMAT);
        return false;
    }

    this->validated = true;
    return true;
}
