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

#ifndef ERROR_LOG_H
#define ERROR_LOG_H

#include "error_info.h"

void error_log_add(const char *func, error_status status);
bool error_log_is_empty();
error_status error_log_last_error();
const char *error_log_last_error_string();
void error_log_clear();

#define ERROR_LOG_ADD(__stat) (error_log_add(__func__, __stat))
#define ERROR_LOG_IS_EMPTY (error_log_is_empty())
#define ERROR_LOG_LAST_ERROR (error_log_last_error())
#define ERROR_LOG_LAST_ERROR_STRING (error_log_last_error_string())
#define ERROR_LOG_CLEAR (error_log_clear())

#endif // ERROR_LOG_H
