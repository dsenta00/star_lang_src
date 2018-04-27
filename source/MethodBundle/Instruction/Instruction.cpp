/*
 * Copyright 2018 Duje Senta
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

#include <ORM/Relationship.h>
#include <ORM/SlaveRelationships.h>
#include <ORM/MasterRelationships.h>
#include <ErrorBundle/ErrorLog.h>
#include <MethodBundle/Instruction/Instruction.h>
#include <MethodBundle/Method.h>
#include <regex>

/**
 * An abstract instruction constructor.
 *
 * @param op
 * @param arg
 */
Instruction::Instruction(eOpCode op, std::vector<std::wstring> &arg) : Object::Object(op)
{
    this->validated = false;
    this->op = op;
    this->arg = std::move(arg);

    MasterRelationships *master = this->getMaster();

    master->init("next_instruction", ONE_TO_ONE);
    master->init("branch", ONE_TO_ONE);
}

/**
 * Get op_code.
 *
 * @return
 */
eOpCode &
Instruction::getOpCode()
{
    return this->op;
}

/**
 * Get method where instruction is.
 *
 * @return
 */
Method *
Instruction::getMethod()
{
    auto *r = this->getSlave()->get("method_instructions");

    if (!r)
    {
        ERROR_LOG_ADD(ERROR_INSTRUCTION_NO_METHOD);
        return nullptr;
    }

    auto *m = (Method *) r->front();

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
Instruction::objectNameIsValid(std::wstring &sample)
{
    return !sample.empty() &&
           sample != L"Collection" &&
            DataType::getFromToken(sample) == OBJECT_TYPE_NULL;
}

/**
 * @inherit
 */
eObjectType
Instruction::getObjectType()
{
    return OBJECT_TYPE_INSTRUCTION;
}

/**
 * Execute command.
 *
 * @return
 */
Instruction *
Instruction::executeIt()
{
    if (!this->validated)
    {
        if (!this->validate())
        {
            return nullptr;
        }
    }

    this->execute();
}

/**
 * Get next instruction.
 *
 * @return
 */
Instruction *
Instruction::getNext()
{
    return (Instruction *) this->getMaster()->get("next_instruction")->front();
}
