//
// Created by Duje Senta on 12/24/17.
//

#include "thread/thread.h"
#include <method/method.h>
#include <error_handler/error_log.h>
#include <thread>

thread::thread(uint64_t id, method *m) : object(id)
{
    this->master_relationship_add("thread", ONE_TO_ONE);
    this->master_relationship_add_object("thread", m);

    this->push(m);
    this->pause = false;
}

bool
thread::step()
{
    if (this->pause)
    {
        return true;
    }

    method *current_method = this->top();

    if (current_method == nullptr)
    {
        return false;
    }

    instruction_result instruction_result = current_method->execute_next();

    switch (instruction_result)
    {
        case INSTRUCTION_OK:
            break;
        case INSTRUCTION_ERROR:
        {
            ERROR_LOG_ADD(ERROR_THREAD_INSTRUCTION_ERROR);

            return false;
        }
        case INSTRUCTION_FINISHED:
        {
            value *result = current_method->get_result();
            this->pop();

            if (result)
            {
                method *parent_method = this->top();

                if (parent_method == nullptr)
                {
                    return false;
                }

                parent_method->push_stack(result);
            }

            break;
        }
    }

    return true;
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
