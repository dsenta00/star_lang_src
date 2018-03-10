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

#include <cstdio>
#include <ORM/orm.h>
#include <error_handler/error_log.h>
#include "../../../include/variable/primitive_data/data_type_test.h"
#include "../../../test_assert.h"
#include "variable/primitive_data/data_type.h"

/**
 * @test data_type_detect()
 */
static void
data_type_detect_test()
{
    ASSERT_EQUALS(data_type_detect(L"9"), OBJECT_TYPE_INT);
    ASSERT_EQUALS(data_type_detect(L"+9"), OBJECT_TYPE_INT);
    ASSERT_EQUALS(data_type_detect(L"\'a\'"), OBJECT_TYPE_CHAR);
    ASSERT_EQUALS(data_type_detect(L"\'jabukica\'"), OBJECT_TYPE_STRING);
    ASSERT_EQUALS(data_type_detect(L"\"jabukica\""), OBJECT_TYPE_STRING);
    ASSERT_EQUALS(data_type_detect(L"4.09"), OBJECT_TYPE_FLOAT);
    ASSERT_EQUALS(data_type_detect(L"+4.09"), OBJECT_TYPE_FLOAT);
    ASSERT_EQUALS(data_type_detect(L"-4.09"), OBJECT_TYPE_FLOAT);
    ASSERT_EQUALS(data_type_detect(L"true"), OBJECT_TYPE_BOOL);
    ASSERT_EQUALS(data_type_detect(L"false"), OBJECT_TYPE_BOOL);
}

static void
data_type_clean_constant_format_test()
{
    std::wstring str;

    str = L"\"miljenko\"";
    data_type_clean_constant_format(str, OBJECT_TYPE_STRING);
    ASSERT_EQUALS(str, L"miljenko");

    str = L"\'miljenko\'";
    data_type_clean_constant_format(str, OBJECT_TYPE_STRING);
    ASSERT_EQUALS(str, L"miljenko");

    str = L"\'m\'";
    data_type_clean_constant_format(str, OBJECT_TYPE_CHAR);
    ASSERT_EQUALS(str, L"m");
}

/**
 * @covers data_type
 */
void data_type_test()
{
    RUN_TEST(data_type_detect_test());
    RUN_TEST(data_type_clean_constant_format_test());
}
