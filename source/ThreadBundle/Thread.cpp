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
#include <ORM/MasterRelationships.h>
#include <ErrorBundle/ErrorLog.h>
#include <MethodBundle/Method.h>
#include <ThreadBundle/Thread.h>
#include <InterpreterBundle/Interpreter.h>
#include <thread>

Thread::Thread(uint64_t id, Method *m) : Object(id)
{
    this->getMaster()->init("Thread", ONE_TO_MANY);
    this->pushMethod(m);

    this->pause = false;
}

bool
Thread::step()
{
    if (this->pause)
    {
        return true;
    }

    Method *current_method = this->methodStack.top();

    if (current_method == nullptr)
    {
        return false;
    }

    instruction_result instruction_result = current_method->step();

    switch (instruction_result)
    {
        case INSTRUCTION_OK:
        case INSTRUCTION_FINISHED:
        {
            return true;
        }
        case INSTRUCTION_ERROR:
        default:
        {
            ERROR_LOG_ADD(ERROR_THREAD_INSTRUCTION_ERROR);

            return false;
        }
    }
}

/**
 * Run thread.
 */
void
Thread::run()
{
    while (this->step())
    {}
}

void
Thread::sleep(uint64_t milliseconds)
{
    clock_t start = clock();

    this->pause = true;

    std::thread t([&]() {
        clock_t now;

        do
        { now = clock(); }
        while (now - start < milliseconds);

        this->pause = false;
    });

    t.join();
}

Value *
Thread::popStack()
{
    if (this->valueStack.empty())
    {
        return (Value *) ORM::getFirst(OBJECT_TYPE_NULL);
    }

    Value *v = this->valueStack.top();
    this->valueStack.pop();

    return v;
}

void
Thread::pushStack(Value *v)
{
    this->valueStack.push(v);
}

void
Thread::pushMethod(Method *m)
{
    this->getMaster()->add("Thread", m);
    this->methodStack.push(m);
}

void
Thread::popMethod()
{
    Method *current_method = this->methodStack.top();

    if (current_method == nullptr)
    {
        return;
    }

    current_method->clear();
    this->getMaster()->remove("Thread", current_method);

    this->methodStack.pop();
}

Thread *
Thread::create(uint64_t id, Method *m)
{
    return (Thread *)ORM::create(new Thread(id, m));
}

eObjectType
Thread::getObjectType()
{
    return OBJECT_TYPE_THREAD;
}

Interpreter *
Thread::getInterpreter()
{
    return (Interpreter *)this->getSlave()->front("InterpreterThreads");
}
