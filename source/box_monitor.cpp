/*
 * Copyright 2017 Duje Senta, Tomislav Radanovic
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

#include "box_monitor.h"
#include <queue>

#define MAX_BOX_ERROR_QUEUE (32)

typedef std::shared_ptr<box_error> box_error_p;
static std::queue<box_error_p> box_error_queue;

/**
 * Add error.
 *
 * @param func - function name where error occurs.
 * @param status - box status.
 */
void
box_monitor_add_error(const char *func, box_status status)
{
    if (box_error_queue.size() >= MAX_BOX_ERROR_QUEUE)
    {
        box_error_queue.pop();
    }

    box_error_queue.push(box_error_p(new box_error(status, func)));
}

/**
 * Check if program is okay.
 *
 * @return true if okay, otherwise false.
 */
bool
box_monitor_ok()
{
    return box_error_queue.empty();
}

/**
 * Get last error.
 *
 * @return last error if exist otherwise return BOX_STATUS_OK.
 */
box_status
box_monitor_last_error()
{
    return box_monitor_ok() ?
           BOX_STATUS_OK :
           box_error_queue.back().get()->get_status();
}

/**
 * Get last error in string format.
 *
 * @return last error string if exist otherwise return "BOX_STATUS_OK".
 */
const char *
box_monitor_last_error_string()
{
    return box_monitor_ok() ?
           "BOX_STATUS_OK" :
           box_error_queue.back().get()->get_status_str();
}

/**
 * Clear monitor error log.
 */
void
box_monitor_clear()
{
    while (!box_error_queue.empty())
    {
        box_error_queue.pop();
    }
}


