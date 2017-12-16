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

#include <method/instructions/create_instruction.h>
#include <ORM/relationship.h>
#include <error_handler/error_log.h>
#include <variable/collection/collection.h>
#include <variable/primitive_data/primitive_data.h>
#include <method/method.h>
#include <ORM/orm.h>
#include <codecvt>
#include <locale>
#include <variable/var.h>

/**
 * @inherit
 */
create_instruction::create_instruction(std::vector<std::wstring> &arg) : abstract_instruction(OP_CODE_CREATE, arg)
{
}

/**
 * @inherit
 */
abstract_instruction *
create_instruction::execute()
{
    if (!this->validate())
    {
        return nullptr;
    }

    auto &name_w = this->arg[0];
    auto &type = this->arg[1];
    var *data = nullptr;

    using convert_type = std::codecvt_utf8<wchar_t>;
    std::wstring_convert<convert_type, wchar_t> converter;
    std::string name = converter.to_bytes(name_w);

    if (type == L"collection")
    {
        data = var::create(name, collection::create());
    }
    else
    {
        data = var::create(name, primitive_data::create(data_type_get_from_token(type)));
    }

    if (!data)
    {
        return nullptr;
    }

    auto *m = this->get_method();
    m->add_variable(data);

    return dynamic_cast<abstract_instruction *>(this->master_relationship_get("next_instruction")->front());
}

/**
 * @inherit
 */
create_instruction *
create_instruction::create(std::wstring name, std::wstring type)
{
    std::vector<std::wstring> arg;
    arg.emplace_back(name);
    arg.emplace_back(type);

    return dynamic_cast<create_instruction *>(orm::create(new create_instruction(arg)));
}

/**
 * @inherit
 */
bool
create_instruction::validate()
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

    this->validated = true;
    return true;
}
