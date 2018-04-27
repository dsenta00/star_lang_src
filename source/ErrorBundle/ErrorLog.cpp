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

#include <ErrorBundle/ErrorLog.h>
#include <queue>
#include <memory>

#define MAX_BOX_ERROR_QUEUE (32)

using error_info_p = std::shared_ptr<ErrorInfo>;
static std::queue<error_info_p> error_queue;

/**
 * Add error.
 *
 * @param func - function name where error occurs.
 * @param status - error status.
 */
void
errorLogAdd(const char *func, eErrorStatus status)
{
    if (error_queue.size() >= MAX_BOX_ERROR_QUEUE)
    {
        error_queue.pop();
    }

    error_queue.push(error_info_p(new ErrorInfo(status, func)));
}

/**
 * Check if program is okay.
 *
 * @return true if okay, otherwise false.
 */
bool
errorLogIsEmpty()
{
    return error_queue.empty();
}

/**
 * Get last error.
 *
 * @return last error if exist otherwise return BOX_STATUS_OK.
 */
eErrorStatus
errorLogLastError()
{
    return errorLogIsEmpty() ?
           STATUS_OK :
           error_queue.back()->getStatus();
}

/**
 * Get last error in string format.
 *
 * @return last error string if exist otherwise return "BOX_STATUS_OK".
 */
const char *
errorLogLastErrorString()
{
    return errorLogIsEmpty() ?
           "STATUS_OK" :
           error_queue.back()->getStatusStr();
}

/**
 * Clear monitor error log.
 */
void
errorLogClear()
{
    while (!error_queue.empty())
    {
        error_queue.pop();
    }
}


