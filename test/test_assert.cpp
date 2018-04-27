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

#include "test_assert.h"

/**
 * Check if statement is true.
 *
 * @param statement - the statement.
 * @param file - file where statement is checked.
 * @param line - line where statement is checked.
 * @param fmt - string format to write if statement is false.
 */
void
assert_true(bool statement,
            const char *file,
            int line,
            const char *fmt,
            ...)
{
  if (!statement)
  {
    va_list arg;

    printf("[%s:%d] -> ", file, line);

    va_start(arg, fmt);
    printf("  ");
    vfprintf(stdout, fmt, arg);
    va_end(arg);

    printf("\n");
    exit(EXIT_FAILURE);
  }
}

/**
 * Check if statement is false.
 *
 * @param statement - the statement.
 * @param file - file where statement is checked.
 * @param line - line where statement is checked.
 * @param fmt - string format to write if statement is true.
 */
void
assert_false(bool statement,
             const char *file,
             int line,
             const char *fmt,
             ...)
{
  if (statement)
  {
    va_list arg;

    printf("[%s:%d] -> ", file, line);

    va_start(arg, fmt);
    printf("  ");
    vfprintf(stdout, fmt, arg);
    va_end(arg);

    printf("\n");
    exit(EXIT_FAILURE);
  }
}