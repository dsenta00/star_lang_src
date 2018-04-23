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
#include <ORM/MasterRelationships.h>
#include <VariableBundle/Primitive/Primitive.h>
#include <VariableBundle/Collection/Collection.h>
#include <VariableBundle/Var.h>
#include <MethodBundle/Instruction/CreateInstruction.h>
#include <MethodBundle/Instruction/CreateAndAssignObjectInstruction.h>
#include <MethodBundle/Method.h>
#include <codecvt>
#include <locale>

/**
 * @inherit
 */
CreateAndAssignObjectInstruction::CreateAndAssignObjectInstruction(std::vector<std::wstring> &arg)
    : Instruction(OP_CODE_CREATE_AND_ASSIGN_OBJECT, arg)
{
}

/**
 * @inherit
 */
Instruction *
CreateAndAssignObjectInstruction::execute()
{
    if (!this->validate())
    {
        return nullptr;
    }

    auto &name_w = this->arg[0];
    auto &obj_name = this->arg[1];
    auto *m = this->getMethod();

    using convert_type = std::codecvt_utf8<wchar_t>;
    std::wstring_convert<convert_type, wchar_t> converter;
    std::string name = converter.to_bytes(name_w);

    Var *object_to_assign = m->getVar(obj_name);
    Var *object = Var::create(name, object_to_assign->get());

    m->addVar(object);

    return (Instruction *) this->getMaster()->get("next_instruction")->front();
}

/**
 * @inherit
 */
CreateAndAssignObjectInstruction *
CreateAndAssignObjectInstruction::create(std::wstring name, std::wstring obj_name)
{
    std::vector<std::wstring> arg;
    arg.emplace_back(name);
    arg.emplace_back(obj_name);

    return dynamic_cast<CreateAndAssignObjectInstruction *>(ORM::create(
        new CreateAndAssignObjectInstruction(arg))
    );
}

/**
 * @inherit
 */
bool
CreateAndAssignObjectInstruction::validate()
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

    auto &obj_name = this->arg[1];
    auto object_to_assign = m->getVar(obj_name);

    if (!object_to_assign)
    {
        ERROR_LOG_ADD(ERROR_INSTRUCTION_OBJECT_DOES_NOT_EXIST);
        return false;
    }

    this->validated = true;
    return true;
}
