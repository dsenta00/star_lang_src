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

#include <MethodBundle/Instruction/PushConstantInstruction.h>
#include <ORM/ORM.h>
#include <MethodBundle/Method.h>
#include <ThreadBundle/Thread.h>
#include <InterpreterBundle/Interpreter.h>

/**
 * The constructor.
 *
 * @param arg
 */
PushConstantInstruction::PushConstantInstruction(std::vector<std::wstring> &arg) : Instruction(OP_CODE_PUSH_CONSTANT, arg)
{
}

/**
 * Create PushConstantInstruction.
 *
 * @param arg
 * @return
 */
PushConstantInstruction *
PushConstantInstruction::create(std::vector<std::wstring> &arg)
{
    return (PushConstantInstruction *)ORM::create(new PushConstantInstruction(arg));
}

/**
 * @inherit
 */
Instruction *
PushConstantInstruction::execute()
{
    uint32_t constNo = 0;
    wscanf(this->arg[0].c_str(), "%d", &constNo);

    Value *val = this->getMethod()
            ->getThread()
            ->getInterpreter()
            ->getConstants()
            ->get(constNo);

    if (!val)
    {
        return nullptr;
    }

    this->getMethod()->push(val);

    return this->getNext();
}