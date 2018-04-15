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

#include "interpreter/interpreter.h"
#include <error_handler/error_log.h>
#include <ORM/orm.h>


interpreter::interpreter(uint64_t id, std::string fname) : object(id)
{
    //TOOO: do something here
}

void
interpreter::add_thread(method *m)
{
    auto id = interpreter::next_id++;

    thread *t = thread::create(id, m);

    interpreter::threads[id] = std::thread([&]() {
        t->run();

        if (error_log_is_empty())
        {
            interpreter::remove_thread(id);
        }
        else
        {
            interpreter::stop();
        }

        orm::destroy(t);
    });

    interpreter::threads[id].join();
}

void
interpreter::run()
{
    while (!interpreter::threads.empty())
    {}
}

void
interpreter::remove_thread(uint32_t id)
{
    interpreter::threads.erase(id);
}

void
interpreter::stop()
{
    for (auto &t : interpreter::threads) {
        //TOOO: do something here
        t.second.detach();
    }
}
