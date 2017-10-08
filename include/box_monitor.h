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

#ifndef BOX_MONITOR_H
#define BOX_MONITOR_H

#include "box_error.h"

void box_monitor_add_error(const char *func, box_status status);
bool box_monitor_ok();
box_status box_monitor_last_error();
const char *box_monitor_last_error_string();
void box_monitor_clear();

#define BOX_ERROR(__stat) (box_monitor_add_error(__func__, __stat))
#define BOX_OK (box_monitor_ok())
#define BOX_LAST_ERROR (box_monitor_last_error())
#define BOX_LAST_ERROR_STRING (box_monitor_last_error_string())
#define BOX_ERROR_CLEAR (box_monitor_clear())

#endif // BOX_MONITOR_H
