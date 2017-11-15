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

#include <cstdio>
#include <ORM/orm.h>
#include <error_log.h>
#include "data_type_test.h"
#include "test_assert.h"
#include "data_type.h"

/**
 *
 */
static void data_type_detect_test()
{
    ASSERT_EQUALS(data_type_detect(L"9"), DATA_TYPE_INT);
    ASSERT_EQUALS(data_type_detect(L"+9"), DATA_TYPE_INT);
    ASSERT_EQUALS(data_type_detect(L"\'a\'"), DATA_TYPE_CHAR);
    ASSERT_EQUALS(data_type_detect(L"\'jabukica\'"), DATA_TYPE_STRING);
    ASSERT_EQUALS(data_type_detect(L"4.09"), DATA_TYPE_FLOAT);
    ASSERT_EQUALS(data_type_detect(L"+4.09"), DATA_TYPE_FLOAT);
    ASSERT_EQUALS(data_type_detect(L"-4.09"), DATA_TYPE_FLOAT);
}

void data_type_test()
{
    RUN_TEST(data_type_detect_test());
}
