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

#include <error_handler/error_log.h>
#include <variable/primitive_data/primitive_data.h>
#include <variable/collection/collection.h>
#include <method/instructions/create_instruction.h>
#include <method/instructions/create_and_assign_object_instruction.h>
#include <ORM/orm.h>
#include <ORM/relationship.h>
#include <method/method.h>
#include <codecvt>
#include <locale>

/**
 * @inherit
 */
create_and_assign_object_instruction::create_and_assign_object_instruction(std::vector<std::wstring> &arg)
    : abstract_instruction(OP_CODE_CREATE_AND_ASSIGN_OBJECT, arg)
{
}

/**
 * @inherit
 */
abstract_instruction *
create_and_assign_object_instruction::execute()
{
    if (!this->validate())
    {
        return nullptr;
    }

    auto &name_w = this->arg[0];
    auto &obj_name = this->arg[1];
    auto *m = this->get_method();

    using convert_type = std::codecvt_utf8<wchar_t>;
    std::wstring_convert<convert_type, wchar_t> converter;
    std::string name = converter.to_bytes(name_w);

    auto *object_to_assign = m->get_local_object(obj_name);
    object *object = nullptr;

    if (primitive_data::is_primitive((var *)object_to_assign))
    {
        auto *data_to_assign = dynamic_cast<primitive_data *>(object_to_assign);

        object = primitive_data::create(name, *data_to_assign);

        if (!object)
        {
            return nullptr;
        }
    }
    else if (object_to_assign->get_object_type() == OBJECT_TYPE_COLLECTION)
    {
        object = collection::create(name, (collection *) object_to_assign);
    }

    m->add_local_object(object);

    return (abstract_instruction *) this->master_relationship_get("next_instruction")->front();
}

/**
 * @inherit
 */
create_and_assign_object_instruction *
create_and_assign_object_instruction::create(std::wstring name, std::wstring obj_name)
{
    std::vector<std::wstring> arg;
    arg.emplace_back(name);
    arg.emplace_back(obj_name);

    return dynamic_cast<create_and_assign_object_instruction *>(orm::create(
        new create_and_assign_object_instruction(arg))
    );
}

/**
 * @inherit
 */
bool
create_and_assign_object_instruction::validate()
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

    if (this->arg.size() != 2)
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

    auto &obj_name = this->arg[1];
    auto object_to_assign = m->get_local_object(obj_name);

    if (!object_to_assign)
    {
        ERROR_LOG_ADD(ERROR_INSTRUCTION_OBJECT_DOES_NOT_EXIST);
        return false;
    }

    this->validated = true;
    return true;
}
