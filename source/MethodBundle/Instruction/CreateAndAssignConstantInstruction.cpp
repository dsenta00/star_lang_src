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

#include <ErrorBundle/ErrorLog.h>
#include <ORM/ORM.h>
#include <ORM/Relationship.h>
#include <VariableBundle/Primitive/Primitive.h>
#include <VariableBundle/Collection/Collection.h>
#include <VariableBundle/Var.h>
#include <MethodBundle/Instruction/CreateInstruction.h>
#include <MethodBundle/Instruction/CreateAndAssignConstantInstruction.h>
#include <MethodBundle/Method.h>
#include <codecvt>
#include <locale>

/**
 * @inherit
 */
CreateAndAssignConstantInstruction::CreateAndAssignConstantInstruction(std::vector<std::wstring> &arg)
    : Instruction(OP_CODE_CREATE_AND_ASSIGN_CONSTANT, arg)
{
}

/**
 * @inherit
 */
Instruction *
CreateAndAssignConstantInstruction::execute()
{
    if (!this->validate())
    {
        return nullptr;
    }

    auto name_w = this->arg[0];
    auto &type = this->arg[1];
    auto &constant = this->arg[2];
    auto const_data_type = DataType::detect(constant);

    using convert_type = std::codecvt_utf8<wchar_t>;
    std::wstring_convert<convert_type, wchar_t> converter;
    std::string name = converter.to_bytes(name_w);

    Var *data = nullptr;

    if (type == L"Collection")
    {
        /*
         * 1. Create constant
         */
        DataType::cleanConstantFormat(constant, const_data_type);
        auto &constant_data = *Primitive::create(const_data_type, constant.c_str());

        Collection &array = *Collection::create();
        array += constant_data;
        data = Var::create(name, &array);
    }
    else
    {
        auto actual_data_type = DataType::getFromToken(type);

        if (const_data_type != actual_data_type)
        {
            ERROR_LOG_ADD(ERROR_INSTRUCTION_NO_MATCH_CONSTANT_FORMAT_WITH_DATA_TYPE);
            return nullptr;
        }

        DataType::cleanConstantFormat(constant, const_data_type);

        data = Var::create(
            name,
            Primitive::create(
                actual_data_type, constant.c_str()
            )
        );
    }

    auto *m = this->getMethod();
    m->add_var(data);

    return (Instruction *) this->masterRelationshipGet("next_instruction")->front();
}

/**
 * @inherit
 */
CreateAndAssignConstantInstruction *
CreateAndAssignConstantInstruction::create(std::wstring name, std::wstring type, std::wstring constant)
{
    std::vector<std::wstring> arg;

    arg.emplace_back(name);
    arg.emplace_back(type);
    arg.emplace_back(constant);

    return dynamic_cast<CreateAndAssignConstantInstruction *>(ORM::create(
        new CreateAndAssignConstantInstruction(arg))
    );
}

/**
 * @inherit
 */
bool
CreateAndAssignConstantInstruction::validate()
{
    if (this->validated)
    {
        return true;
    }

    auto *m = this->getMethod();

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

    if (!this->objectNameIsValid(name))
    {
        ERROR_LOG_ADD(ERROR_INSTRUCTION_INVALID_OBJECT_NAME);
        return false;
    }

    auto &constant = this->arg[2];
    auto const_data_type = DataType::detect(constant);

    if (const_data_type == OBJECT_TYPE_NULL)
    {
        ERROR_LOG_ADD(ERROR_INSTRUCTION_UNKNOWN_CONSTANT_FORMAT);
        return false;
    }

    this->validated = true;
    return true;
}
