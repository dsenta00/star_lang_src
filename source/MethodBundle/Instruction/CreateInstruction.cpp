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

#include <ORM/ORM.h>
#include <ORM/Relationship.h>
#include <ORM/MasterRelationships.h>
#include <ErrorBundle/ErrorLog.h>
#include <VariableBundle/Var.h>
#include <VariableBundle/Collection/Collection.h>
#include <VariableBundle/Primitive/Primitive.h>
#include <MethodBundle/Method.h>
#include <MethodBundle/Instruction/CreateInstruction.h>
#include <codecvt>
#include <locale>

/**
 * @inherit
 */
CreateInstruction::CreateInstruction(std::vector<std::wstring> &arg) : Instruction(OP_CODE_CREATE, arg)
{
}

/**
 * @inherit
 */
Instruction *
CreateInstruction::execute()
{
    if (!this->validate())
    {
        return nullptr;
    }

    auto &name_w = this->arg[0];
    auto &type = this->arg[1];
    Var *data = nullptr;

    using convert_type = std::codecvt_utf8<wchar_t>;
    std::wstring_convert<convert_type, wchar_t> converter;
    std::string name = converter.to_bytes(name_w);

    if (type == L"Collection")
    {
        data = Var::create(name, Collection::create());
    }
    else
    {
        data = Var::create(name, Primitive::create(DataType::getFromToken(type)));
    }

    if (!data)
    {
        return nullptr;
    }

    auto *m = this->getMethod();
    m->addVar(data);

    return dynamic_cast<Instruction *>(this->getMaster()->get("next_instruction")->front());
}

/**
 * @inherit
 */
CreateInstruction *
CreateInstruction::create(std::wstring name, std::wstring type)
{
    std::vector<std::wstring> arg;
    arg.emplace_back(name);
    arg.emplace_back(type);

    return dynamic_cast<CreateInstruction *>(ORM::create(new CreateInstruction(arg)));
}

/**
 * @inherit
 */
bool
CreateInstruction::validate()
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

    if (this->arg.size() != 2)
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

    this->validated = true;
    return true;
}
