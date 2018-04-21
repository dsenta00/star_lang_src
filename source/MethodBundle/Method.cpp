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
#include <ErrorBundle/ErrorLog.h>
#include <MethodBundle/Method.h>
#include <MethodBundle/Instruction/Instruction.h>
#include <VariableBundle/Var.h>
#include <ThreadBundle/Thread.h>
#include <locale>
#include <codecvt>

/**
 * The constructor.
 *
 * @param id
 * @param instructions
 */
Method::Method(std::string id, std::vector<Instruction *> &instructions) : Object::Object(std::move(id))
{
    /*
     * - Value
     * - array
     * - function
     */
    this->masterRelationshipAdd("method_vars", ONE_TO_MANY);

    /*
     * - Instruction
     */
    this->masterRelationshipAdd("method_instructions", ONE_TO_MANY);

    if (instructions.empty())
    {
        this->current_instruction = nullptr;
        return;
    }

    for (Instruction *i : instructions)
    {
        this->masterRelationshipAddObject("method_instructions", i);
    }

    Relationship *r = this->masterRelationshipGet("method_instructions");

    r->forEach([&](Object *o1, Object *o2) {
        o1->masterRelationshipAddObject("next_instruction", o2);
        return FOREACH_CONTINUE;
    });

    this->current_instruction = instructions[0];
}

/**
 * Execute instruction.
 *
 * @return next instruction.
 */
instruction_result
Method::execute_next()
{
    if (!this->current_instruction)
    {
        return INSTRUCTION_ERROR;
    }

    this->current_instruction = this->current_instruction->execute();

    if (!ERROR_LOG_IS_EMPTY)
    {
        return INSTRUCTION_ERROR;
    }

    if (this->current_instruction == nullptr)
    {
        return INSTRUCTION_FINISHED;
    }

    return INSTRUCTION_OK;
}

/**
 * Push value to stack.
 *
 * @param o
 */
void
Method::push_stack(Value *v)
{
    auto *thread_relationship = this->slaveRelationshipGet("Thread");

    if (!thread_relationship)
    {
        ERROR_LOG_ADD(ERROR_METHOD_NOT_PART_OF_THREAD);
        return;
    }

    auto *t = (Thread *)thread_relationship->front();
    t->pushStack(v);
}

/**
 * Pop value from stack.
 *
 * @return value.
 */
Value *
Method::pop_stack()
{
    auto *thread_relationship = this->slaveRelationshipGet("Thread");

    if (!thread_relationship)
    {
        ERROR_LOG_ADD(ERROR_METHOD_NOT_PART_OF_THREAD);

        return (Value *) ORM::getFirst(OBJECT_TYPE_NULL);
    }

    auto *t = (Thread *)thread_relationship->front();

    return t->popStack();
}

/**
 * Create method.
 *
 * @param id
 * @param instructions
 * @return
 */
Method *
Method::create(std::string id, std::vector<Instruction *> &instructions)
{
    return (Method *) ORM::create(new Method(std::move(id), instructions));
}

/**
 * @inherit
 */
eObjectType
Method::getObjectType()
{
    return OBJECT_TYPE_METHOD;
}

void
Method::clear()
{
    this->current_instruction = (Instruction *) this->masterRelationshipFront("method_instructions");
    this->masterRelationshipsClearObjects("method_vars");
}

void
Method::add_var(Var *v)
{
    Relationship *r = this->masterRelationshipGet("method_vars");

    Var *v2 = (Var *)r->find(v->getId());

    if (v2)
    {
        ERROR_LOG_ADD(ERROR_METHOD_ADD_OBJECTS_OF_SAME_NAME);
        return;
    }

    this->masterRelationshipAddObject("method_vars", v);
}

Var *
Method::get_var(std::wstring id)
{
    Relationship *r = this->masterRelationshipGet("method_vars");

    using convert_type = std::codecvt_utf8<wchar_t>;
    std::wstring_convert<convert_type, wchar_t> converter;
    std::string name = converter.to_bytes(id);

    return (Var *)r->find(name);
}
