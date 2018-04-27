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

#include <ORM/ORM.h>
#include <ORM/Relationship.h>
#include <ORM/SlaveRelationships.h>
#include <ORM/MasterRelationships.h>
#include <ErrorBundle/ErrorLog.h>
#include <MethodBundle/Method.h>
#include <MethodBundle/Instruction/Instruction.h>
#include <VariableBundle/Var.h>
#include <ThreadBundle/Thread.h>
#include <locale>
#include <codecvt>
#include <VariableBundle/Primitive/String.h>
#include <iostream>

/**
 * The constructor.
 *
 * @param id
 * @param instructions
 */
Method::Method(std::string id, std::vector<Instruction *> &instructions) : Value::Value()
{
    this->setId(id);

    MasterRelationships *master = this->getMaster();

    /*
     * - Value
     * - array
     * - function
     */
    master->init("method_vars", ONE_TO_MANY);

    /*
     * - Instruction
     */
    master->init("method_instructions", ONE_TO_MANY);

    if (instructions.empty())
    {
        this->currentInstruction = nullptr;

        return;
    }

    for (Instruction *i : instructions)
    {
        master->add("method_instructions", i);
    }

    master->get("method_instructions")->forEach([&](Object *o1, Object *o2) {
        o1->getMaster()->add("next_instruction", o2);

        return FOREACH_CONTINUE;
    });

    this->currentInstruction = instructions[0];
}

/**
 * Execute instruction.
 *
 * @return next instruction.
 */
instruction_result
Method::step()
{
    if (!this->currentInstruction)
    {
        return INSTRUCTION_ERROR;
    }

    this->currentInstruction = this->currentInstruction->executeIt();

    if (!ERROR_LOG_IS_EMPTY)
    {
        return INSTRUCTION_ERROR;
    }

    if (this->currentInstruction == nullptr)
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
Method::push(Value *v)
{
    Thread *thread = this->getThread();

    if (!thread)
    {
        return;
    }

    thread->pushStack(v);
}

/**
 * Pop value from stack.
 *
 * @return value.
 */
Value *
Method::pop()
{
    Thread *thread = this->getThread();

    if (!thread)
    {
        return (Value *) ORM::getFirst(OBJECT_TYPE_NULL);
    }

    return thread->popStack();
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
    MasterRelationships *master = this->getMaster();

    this->currentInstruction = (Instruction *) master->front("method_instructions");
    master->clearObjects("method_vars");
}

void
Method::addVar(Var *v)
{
    MasterRelationships *master = this->getMaster();
    Relationship *r = master->get("method_vars");

    Var *v2 = (Var *) r->find(v->getId());

    if (v2)
    {
        ERROR_LOG_ADD(ERROR_METHOD_ADD_OBJECTS_OF_SAME_NAME);
        return;
    }

    master->add("method_vars", v);
}

Var *
Method::getVar(std::wstring id)
{
    Relationship *r = this->getMaster()->get("method_vars");

    using convert_type = std::codecvt_utf8<wchar_t>;
    std::wstring_convert<convert_type, wchar_t> converter;
    std::string name = converter.to_bytes(id);

    return (Var *) r->find(name);
}

/**
 * @inherit
 */
bool
Method::toBool()
{
    return true;
}

/**
 * @inherit
 */
wchar_t
Method::toChar()
{
    return 0;
}

/**
 * @inherit
 */
int32_t
Method::toInt()
{
    return 0;
}

/**
 * @inherit
 */
double
Method::toFloat()
{
    return 0;
}

/**
 * @inherit
 */
String &
Method::toString()
{
    return *String::create(this->getId().c_str());
}

/**
 * @inherit
 */
bool
Method::isReference()
{
    return false;
}

/**
 * @inherit
 */
bool
Method::operator=(const void *data)
{
    ERROR_LOG_ADD(ERROR_METHOD_INVALID_OPERATION);

    return false;
}

/**
 * @inherit
 */
bool
Method::operator=(Value &data)
{
    ERROR_LOG_ADD(ERROR_METHOD_INVALID_OPERATION);

    return false;
}

/**
 * @inherit
 */
bool
Method::operator&=(Value &data)
{
    ERROR_LOG_ADD(ERROR_METHOD_INVALID_OPERATION);

    return false;
}

/**
 * @inherit
 */
bool
Method::operator|=(Value &data)
{
    ERROR_LOG_ADD(ERROR_METHOD_INVALID_OPERATION);

    return false;
}

/**
 * @inherit
 */
bool
Method::operator^=(Value &data)
{
    ERROR_LOG_ADD(ERROR_METHOD_INVALID_OPERATION);

    return false;
}

/**
 * @inherit
 */
bool
Method::operator+=(Value &var)
{
    ERROR_LOG_ADD(ERROR_METHOD_INVALID_OPERATION);

    return false;
}

/**
 * @inherit
 */
bool
Method::operator-=(Value &data)
{
    ERROR_LOG_ADD(ERROR_METHOD_INVALID_OPERATION);

    return false;
}

/**
 * @inherit
 */
bool
Method::operator*=(Value &data)
{
    ERROR_LOG_ADD(ERROR_METHOD_INVALID_OPERATION);

    return false;
}

/**
 * @inherit
 */
bool
Method::operator/=(Value &data)
{
    ERROR_LOG_ADD(ERROR_METHOD_INVALID_OPERATION);

    return false;
}

/**
 * @inherit
 */
bool
Method::operator%=(Value &data)
{
    ERROR_LOG_ADD(ERROR_METHOD_INVALID_OPERATION);

    return false;
}

/**
 * @inherit
 */
bool
Method::operator++()
{
    ERROR_LOG_ADD(ERROR_METHOD_INVALID_OPERATION);

    return false;
}

/**
 * @inherit
 */
bool
Method::operator--()
{
    ERROR_LOG_ADD(ERROR_METHOD_INVALID_OPERATION);

    return false;
}

/**
 * @inherit
 */
bool
Method::operator==(Value &data)
{
    if (data.getObjectType() != OBJECT_TYPE_METHOD)
    {
        return false;
    }

    return data.getId() == this->getId();
}

/**
 * @inherit
 */
bool
Method::operator!=(Value &data)
{
    if (data.getObjectType() != OBJECT_TYPE_METHOD)
    {
        return true;
    }

    return data.getId() != this->getId();
}

/**
 * @inherit
 */
bool
Method::operator>(Value &data)
{
    ERROR_LOG_ADD(ERROR_METHOD_INVALID_OPERATION);

    return false;
}

/**
 * @inherit
 */
bool
Method::operator<(Value &data)
{
    ERROR_LOG_ADD(ERROR_METHOD_INVALID_OPERATION);

    return false;
}

/**
 * @inherit
 */
bool
Method::operator>=(Value &data)
{
    ERROR_LOG_ADD(ERROR_METHOD_INVALID_OPERATION);

    return false;
}

/**
 * @inherit
 */
bool
Method::operator<=(Value &data)
{
    ERROR_LOG_ADD(ERROR_METHOD_INVALID_OPERATION);

    return false;
}

/**
 * @inherit
 */
bool
Method::print()
{
    std::cout << this->getId();

    return true;
}

/**
 * @inherit
 */
bool
Method::println()
{
    std::cout << this->getId() << std::endl;

    return true;
}

/**
 * @inherit
 */
bool
Method::scan()
{
    ERROR_LOG_ADD(ERROR_METHOD_INVALID_OPERATION);

    return false;
}

/**
 * @inherit
 */
std::wstring
Method::getString()
{
    return std::wstring(this->getId().begin(), this->getId().end());
}

Thread *
Method::getThread()
{
    auto *threadRelationship = this->getSlave()->get("Thread");

    if (!threadRelationship)
    {
        ERROR_LOG_ADD(ERROR_METHOD_NOT_PART_OF_THREAD);

        return nullptr;
    }

    return (Thread *)threadRelationship->front();
}
