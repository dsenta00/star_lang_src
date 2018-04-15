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

#include "thread/thread.h"
#include <method/method.h>
#include <error_handler/error_log.h>
#include <thread>
#include <ORM/orm.h>

thread::thread(uint64_t id, method *m) : object(id)
{
    this->master_relationship_add("thread", ONE_TO_MANY);
    this->push_method(m);

    this->pause = false;
}

bool
thread::step()
{
    if (this->pause)
    {
        return true;
    }

    method *current_method = this->method_stack.top();

    if (current_method == nullptr)
    {
        return false;
    }

    instruction_result instruction_result = current_method->execute_next();

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
thread::run()
{
    while (this->step())
    {}
}

void
thread::sleep(uint64_t milliseconds)
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

value *
thread::pop_stack()
{
    if (this->value_stack.empty())
    {
        return (value *)orm::get_first(OBJECT_TYPE_NULL);
    }

    value *v = this->value_stack.top();
    this->value_stack.pop();

    return v;
}

void
thread::push_stack(value *v)
{
    this->value_stack.push(v);
}

void
thread::push_method(method *m)
{
    this->master_relationship_add_object("thread", m);
    this->method_stack.push(m);
}

void
thread::pop_method()
{
    method *current_method = this->method_stack.top();

    if (current_method == nullptr)
    {
        return;
    }

    current_method->clear();
    this->master_relationship_remove_object("thread", current_method);

    this->method_stack.pop();
}

thread *
thread::create(uint64_t id, method *m)
{
    return (thread *)orm::create(new thread(id, m));
}

object_type
thread::get_object_type()
{
    return OBJECT_TYPE_THREAD;
}
