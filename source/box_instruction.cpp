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

#include "box_instruction.h"
#include "ORM/relationship.h"
#include "box_method.h"
#include "box_data.h"
#include "box_array.h"
#include "box_monitor.h"
#include <regex>

instruction::instruction(box_op_code op_code,
                         instruction *next_instruction,
                         instruction *branch_result_false) : entity::entity("instruction", 0)
{
    this->op_code = op_code;

    this->master_relationship_add("next_instruction", ONE_TO_ONE);
    this->master_relationship_add("branch", ONE_TO_ONE);
    this->master_relationship_add("operands", ONE_TO_MANY);

    if (next_instruction)
    {
        relationship *r = this->master_relationship_get("next_instruction");
        r->add_entity(next_instruction);
    }

    if (branch_result_false)
    {
        relationship *r = this->master_relationship_get("branch_result_false");
        r->add_entity(branch_result_false);
    }
}

box_op_code &
instruction::get_op_code()
{
    return this->op_code;
}

/**
 * create <name> <type>
 */
void
instruction::create()
{
    if (this->arg.size() != 2)
    {
        BOX_ERROR(ERROR_BOX_INSTRUCTION_INVALID_NO_OF_ARGS);
        return;
    }

    auto method = (box_method *) this->slave_relationship_get("method_instructions")->front();

    auto &name = this->arg[0];
    auto &type = this->arg[1];

    entity *data = nullptr;

    if (type == "array")
    {
        data = box_array::create(name);
    }
    else
    {
        data = box_data::create(name, get_from_token(type));
    }

    if (!data)
    {
        return;
    }

    method->add_local_object(data);
}

/**
 * create_and_assign_constant <name> <type> <constant>
 */
void
instruction::create_and_assign_constant()
{
    if (this->arg.size() != 3)
    {
        BOX_ERROR(ERROR_BOX_INSTRUCTION_INVALID_NO_OF_ARGS);
        return;
    }

    auto method = (box_method *) this->slave_relationship_get("method_instructions")->front();

    auto &name = this->arg[0];
    auto &type = this->arg[1];
    auto &constant = this->arg[2];

    auto const_data_type = this->detect_data_type(constant);

    if (const_data_type == BOX_DATA_INVALID)
    {
        BOX_ERROR(ERROR_BOX_INSTRUCTION_UNKNOWN_CONSTANT_FORMAT);
        return;
    }

    entity *data = nullptr;

    if (type == "array")
    {
        this->clean_constant_format(constant, const_data_type);

        auto constant_data = box_data::create(
            name.append(":constant"),
            const_data_type,
            constant.c_str()
        );

        auto &array = *box_array::create(name);
        array += constant_data;
        data = &array;
    }
    else
    {
        auto actual_data_type = get_from_token(type);

        if (const_data_type != actual_data_type)
        {
            BOX_ERROR(ERROR_BOX_INSTRUCTION_NO_MATCH_CONSTANT_FORMAT_WITH_DATA_TYPE);
            return;
        }

        this->clean_constant_format(constant, const_data_type);

        data = box_data::create(name,
                                actual_data_type,
                                constant.c_str());
    }

    method->add_local_object(data);
}

/**
 * create_and_assign_object <name> <obj_name>
 */
void
instruction::create_and_assign_object()
{
    if (this->arg.size() != 2)
    {
        BOX_ERROR(ERROR_BOX_INSTRUCTION_INVALID_NO_OF_ARGS);
        return;
    }

    auto method = (box_method *) this->slave_relationship_get("method_instructions")->front();

    auto &name = this->arg[0];
    auto &obj_name = this->arg[1];

    auto object_to_assign = method->get_local_object(obj_name);

    if (!object_to_assign)
    {
        BOX_ERROR(ERROR_BOX_INSTRUCTION_OBJECT_DOES_NOT_EXIST);
        return;
    }

    entity *object = nullptr;

    if (object_to_assign->get_entity_type() == "box_data")
    {
        object = box_data::create(name, *(box_data *) object_to_assign);
    }
    else if (object_to_assign->get_entity_type() == "box_array")
    {
        object = box_array::create(name, (box_array *) object_to_assign);
    }

    method->add_local_object(object);
}

/**
 * pop_and_store <name>
 */
void
instruction::pop_and_store()
{
    box_method *method = (box_method *) this->master_relationship_get("method_instructions")->front();
    auto *data2 = (box_data *) method->pop_stack();

    if (!data2)
    {
        BOX_ERROR(ERROR_BOX_INSTRUCTION_OBJECT_DOES_NOT_EXIST);
        return;
    }

    box_data *data = (box_data *) method->get_local_object(this->arg[0]);

    if (!data)
    {
        BOX_ERROR(ERROR_BOX_INSTRUCTION_OBJECT_DOES_NOT_EXIST);
        return;
    }

    *data = *data2;
}

/**
 * Instruction execution.
 *
 * @return next instruction to execute.
 */
instruction *
instruction::execute()
{
    switch (this->op_code)
    {
        case CREATE:
            this->create();
            break;
        case CREATE_AND_ASSIGN_CONSTANT:
            this->create_and_assign_constant();
            break;
        case CREATE_AND_ASSIGN_OBJECT:
            this->create_and_assign_object();
            break;
        case POP_AND_STORE:
            this->pop_and_store();
            break;
        default:
            BOX_ERROR(ERROR_BOX_INSTRUCTION_UNKNOWN);
            break;
    }

    return (instruction *) this->master_relationship_get("next_instruction")->front();
}

box_data_type
instruction::detect_data_type(std::string &sample)
{
    if (std::regex_match(sample, std::regex("([-+]?[0-9]+)")))
    {
        return BOX_DATA_INT;
    }
    else if (std::regex_match(sample, std::regex("[-+]?[0-9]*\\.?[0-9]*")))
    {
        return BOX_DATA_FLOAT;
    }
    else if (std::regex_match(sample, std::regex("\'[\\]?(.*)\'")))
    {
        return BOX_DATA_CHAR;
    }
    else if (std::regex_match(sample, std::regex("\"(.*)\"")) or std::regex_match(sample, std::regex("\'(.*)+\'")))
    {
        return BOX_DATA_STRING;
    }
    else if (std::regex_match(sample, std::regex("true|false")))
    {
        return BOX_DATA_BOOL;
    }
    else
    {
        return BOX_DATA_INVALID;
    }
}

std::string
instruction::clean_constant_format(std::string &sample, box_data_type type)
{
    switch (type)
    {
        case BOX_DATA_INT:
        case BOX_DATA_FLOAT:
            /*
             * nothing to do.
             */
            break;
        case BOX_DATA_CHAR:
        case BOX_DATA_STRING:
        {
            sample.erase(sample.begin());
            sample.erase(sample.end());
            break;
        }
        case BOX_DATA_BOOL:
        {
            sample.assign(1, (char) (sample == "true"));
            break;
        }
        default:
            break;
    }

    return sample;
}
