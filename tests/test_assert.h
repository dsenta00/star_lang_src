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

#ifndef TEST_ASSERT_H
#define TEST_ASSERT_H

#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>

void assert_true(bool statement,
                 const char *file,
                 int line,
                 const char *fmt,
                 ...);

void assert_false(bool statement,
                  const char *file,
                  int line,
                  const char *fmt,
                  ...);

#define RUN_TEST(__test__) \
  printf("\t-> " #__test__ "::Start\r\n"); \
  do \
  { \
      if (vm) \
      { \
        orm::destroy(vm); \
      } \
      vm = virtual_memory::create(); \
      (__test__); \
      ERROR_LOG_IS_EMPTY; \
  } while(false);

#define ASSERT_TRUE(__statement__, __fmt__, ...) \
  assert_true(__statement__, __FILE__, __LINE__, __fmt__, ##__VA_ARGS__)

#define ASSERT_EQUALS(__statement__, __expected__) \
  assert_true((__statement__) == (__expected__), __FILE__, __LINE__, #__statement__ " and " #__expected__ " are not equal!")

#define ASSERT_NOT_EQUALS(__statement__, __not_expected__) \
  assert_true((__statement__) != (__not_expected__), __FILE__, __LINE__, "#__statement__ and #__not_expected__ are equal!")

#define ASSERT_NULL(__pointer__) \
  assert_true((__pointer__) == nullptr, __FILE__, __LINE__, "#__pointer__ should be null!")

#define ASSERT_NOT_NULL(__pointer__) \
  assert_true((__pointer__) != nullptr, __FILE__, __LINE__, "#__pointer__ shouldn't be null!")

#define ASSERT_FALSE(__statement__, __fmt__, ...) \
  assert_false(__statement__, __FILE__, __LINE__, __fmt__, ##__VA_ARGS__)

#define ASSERT_OK \
  ASSERT_TRUE(ERROR_LOG_IS_EMPTY, "Everything should be OK. (%s)", ERROR_LOG_LAST_ERROR_STRING)

#define ASSERT_ERROR(__error__) \
  ASSERT_TRUE(ERROR_LOG_LAST_ERROR == (__error__), \
              "ERROR_LOG_LAST_ERROR != " #__error__ " (%s)", \
              ERROR_LOG_LAST_ERROR_STRING)

#define ASSERT_VIRTUAL_MEMORY(__VM__, __BYTES__) \
  ASSERT_TRUE((__VM__).get_allocated_total() == (__BYTES__), \
  "Total allocated should be %u (%u)", \
  (__BYTES__), \
  (__VM__).get_allocated_total())

#endif // TEST_ASSERT_H
