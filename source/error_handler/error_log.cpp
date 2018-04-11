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

#include "error_handler/error_log.h"
#include <queue>

#define MAX_BOX_ERROR_QUEUE (32)

using error_info_p = std::shared_ptr<error_info>;
static std::queue<error_info_p> error_queue;

/**
 * Add error.
 *
 * @param func - function name where error occurs.
 * @param status - error status.
 */
void
error_log_add(const char *func, error_status status)
{
    if (error_queue.size() >= MAX_BOX_ERROR_QUEUE)
    {
        error_queue.pop();
    }

    error_queue.push(error_info_p(new error_info(status, func)));
}

/**
 * Check if program is okay.
 *
 * @return true if okay, otherwise false.
 */
bool
error_log_is_empty()
{
    return error_queue.empty();
}

/**
 * Get last error.
 *
 * @return last error if exist otherwise return BOX_STATUS_OK.
 */
error_status
error_log_last_error()
{
    return error_log_is_empty() ?
           STATUS_OK :
           error_queue.back()->get_status();
}

/**
 * Get last error in string format.
 *
 * @return last error string if exist otherwise return "BOX_STATUS_OK".
 */
const char *
error_log_last_error_string()
{
    return error_log_is_empty() ?
           "STATUS_OK" :
           error_queue.back()->get_status_str();
}

/**
 * Clear monitor error log.
 */
void
error_log_clear()
{
    while (!error_queue.empty())
    {
        error_queue.pop();
    }
}


