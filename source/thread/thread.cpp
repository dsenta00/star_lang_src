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

thread::thread(uint64_t id, method *m) : object(id)
{
    this->master_relationship_add("thread", ONE_TO_ONE);
    this->master_relationship_add_object("thread", m);

    this->methodStack.push(m);
    this->pause = false;
}

bool
thread::step()
{
    if (this->pause)
    {
        return true;
    }

    method *current_method = this->methodStack.top();

    if (current_method == nullptr)
    {
        return false;
    }

    instruction_result instruction_result = current_method->execute_next();

    switch (instruction_result)
    {
        case INSTRUCTION_OK:
        {
            return true;
        }
        case INSTRUCTION_ERROR:
        default:
        {
            ERROR_LOG_ADD(ERROR_THREAD_INSTRUCTION_ERROR);

            return false;
        }
        case INSTRUCTION_FINISHED:
        {
            value *result = current_method->get_result();
            this->methodStack.pop();

            if (result)
            {
                method *parent_method = this->methodStack.top();

                if (parent_method == nullptr)
                {
                    return false;
                }

                parent_method->push_stack(result);
            }

            return true;
        }
    }
}

/**
 * Run thread.
 */
void
thread::run()
{
    while (this->step());
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
