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

#include <variable/primitive_data/float_data.h>
#include <variable/primitive_data/int_data.h>
#include <variable/primitive_data/string_data.h>
#include "../../../include/variable/primitive_data/primitive_data_test.h"
#include "../../../test_assert.h"
#include "error_handler/error_log.h"
#include "ORM/orm.h"

static virtual_memory *vm;

/**
 * Test data as float.
 */
static void
primitive_data_test_float()
{
    ASSERT_VIRTUAL_MEMORY(*vm, 0);

    double float_num = 65.0;
    var &float_data = *float_data::create("float_data", &float_num);
    ASSERT_VIRTUAL_MEMORY(*vm, sizeof(double));

    ASSERT_OK;

    ASSERT_TRUE(++float_data, "float_data should be incremented!");
    ASSERT_OK;
    ASSERT_EQUALS(float_data.to_float(), 66.0);

    ASSERT_TRUE(--float_data, "int_data should be incremented!");
    ASSERT_OK;
    ASSERT_EQUALS(float_data.to_float(), 65.0);

    ASSERT_EQUALS(float_data.to_char(), L'A');
    ASSERT_OK;

    ASSERT_EQUALS(float_data.to_int(), 65);
    ASSERT_OK;

    string_data &str = float_data.to_string();
    ASSERT_TRUE(wcscmp((const wchar_t *) str.get_address(), L"65.000000") == 0,
                "string should be 65.000000! (%ls)",
                (const wchar_t *) str.get_address());
    ASSERT_OK;

    ASSERT_VIRTUAL_MEMORY(*vm,
                          sizeof(double) +
                          sizeof(L"65.000000"));

    int32_t num = 35;
    var &int_data = *int_data::create("int_data", &num);

    ASSERT_VIRTUAL_MEMORY(*vm,
                          sizeof(double) +
                          sizeof(L"65.000000") +
                          sizeof(int));

    ASSERT_FALSE(float_data == int_data, "float_data and int_data should not be equal");
    ASSERT_OK;

    ASSERT_TRUE(float_data != int_data, "float_data and int_data should not be equal");
    ASSERT_OK;

    ASSERT_TRUE(float_data > int_data, "float_data should be bigger than int_data");
    ASSERT_OK;

    ASSERT_TRUE(float_data >= int_data, "float_data should be bigger or equal than int_data");
    ASSERT_OK;

    ASSERT_FALSE(float_data < int_data, "float_data should be bigger than int_data");
    ASSERT_OK;

    ASSERT_FALSE(float_data <= int_data, "float_data should be bigger or equal than int_data");
    ASSERT_OK;

    ASSERT_TRUE(float_data += int_data, "float_data and int_data should add");
    ASSERT_EQUALS(float_data.to_float(), 100.0);

    ASSERT_TRUE(float_data -= int_data, "float_data and int_data should substract");
    ASSERT_EQUALS(float_data.to_float(), 65.0);

    ASSERT_TRUE(float_data *= int_data, "float_data and int_data should multiply");
    ASSERT_EQUALS(float_data.to_float(), 2275.0);

    ASSERT_TRUE(float_data /= int_data, "float_data and int_data should divide");
    ASSERT_EQUALS(float_data.to_float(), 65.0);

    ASSERT_FALSE(float_data %= int_data, "float_data and int_data shouldn't mod");
    ASSERT_EQUALS(float_data.to_float(), 65.0);

    ASSERT_ERROR(ERROR_PRIMITIVE_DATA_INVALID_MODULUS);
    ERROR_LOG_CLEAR;

    /*
     * Putting int_data2 to value of zero.
     */
    num = 0;
    int_data = &num;

    ASSERT_FALSE(float_data /= int_data, "int_data and int_data2 shouldn't divide");
    ASSERT_EQUALS(float_data.to_float(), 65.0);
    ASSERT_ERROR(ERROR_PRIMITIVE_DATA_DIVIDING_ZERO);
    ERROR_LOG_CLEAR;

    ASSERT_FALSE(float_data %= int_data, "int_data and int_data2 shouldn't mod");
    ASSERT_EQUALS(float_data.to_float(), 65.0);
    ASSERT_ERROR(ERROR_PRIMITIVE_DATA_INVALID_MODULUS);
    ERROR_LOG_CLEAR;

    double fnum2 = 35.0;
    var &float_data2 = *float_data::create("float_data2", &fnum2);

    ASSERT_VIRTUAL_MEMORY(*vm,
                          sizeof(double) +
                          sizeof(L"65.000000") +
                          sizeof(int) +
                          sizeof(double));

    ASSERT_NOT_EQUALS(float_data, float_data2);
    ASSERT_OK;

    ASSERT_TRUE(float_data > float_data2,
                "float_data should be bigger than float_data2");
    ASSERT_OK;

    ASSERT_TRUE(float_data >= float_data2,
                "float_data should be bigger or equal than float_data2");
    ASSERT_OK;

    ASSERT_FALSE(float_data < float_data2,
                 "float_data should be bigger than float_data2");
    ASSERT_OK;

    ASSERT_FALSE(float_data <= float_data2,
                 "float_data should be bigger or equal than float_data2");
    ASSERT_OK;

    ASSERT_TRUE(float_data += float_data2, "float_data and float_data2 should add");
    ASSERT_EQUALS(float_data.to_float(), 100.0);

    ASSERT_TRUE(float_data -= float_data2,
                "float_data and float_data2 should substract");
    ASSERT_EQUALS(float_data.to_float(), 65.0);

    ASSERT_TRUE(float_data *= float_data2,
                "float_data and float_data2 should multiply");
    ASSERT_EQUALS(float_data.to_float(), 2275.0);

    ASSERT_TRUE(float_data /= float_data2,
                "float_data and float_data2 should divide");
    ASSERT_EQUALS(float_data.to_float(), 65.0);

    /*
     * setting float_data to zero value
     */
    fnum2 = 0.0;
    float_data2 = &fnum2;

    ASSERT_FALSE(float_data /= float_data2,
                 "int_data and float_data shouldn't divide");
    ASSERT_EQUALS(float_data.to_float(), 65.0);
    ASSERT_ERROR(ERROR_PRIMITIVE_DATA_DIVIDING_ZERO);
    ERROR_LOG_CLEAR;

    ASSERT_FALSE(float_data %= float_data, "int_data and float_data shouldn't mod");
    ASSERT_TRUE(float_data.to_float() == 65.0, "int_data should be 65.0");
    ASSERT_ERROR(ERROR_PRIMITIVE_DATA_INVALID_MODULUS);
    ERROR_LOG_CLEAR;

    /* interaction with string */

    string_data &string_data = *string_data::create("string_data", L"35");

    ASSERT_VIRTUAL_MEMORY(*vm,
                          sizeof(double) +
                          sizeof(L"65.000000") +
                          sizeof(int) +
                          sizeof(double) +
                          sizeof(L"35"));

    ASSERT_FALSE(float_data == string_data, "float_data and string_data should not be equal");
    ASSERT_OK;

    ASSERT_TRUE(float_data != string_data, "float_data and string_data should not be equal");
    ASSERT_OK;

    ASSERT_TRUE(float_data > string_data, "float_data should be bigger than string_data");
    ASSERT_OK;

    ASSERT_TRUE(float_data >= string_data, "float_data should be bigger or equal than string_data");
    ASSERT_OK;

    ASSERT_FALSE(float_data < string_data,
                 "float_data should be bigger than string_data");
    ASSERT_OK;

    ASSERT_FALSE(float_data <= string_data, "float_data should be bigger or equal than string_data");
    ASSERT_OK;

    ASSERT_FALSE(float_data += string_data, "float_data and string_data shouldn't add");
    ASSERT_EQUALS(float_data.to_float(), 65.0);
    ASSERT_ERROR(ERROR_PRIMITIVE_DATA_ADDING_STRING);
    ERROR_LOG_CLEAR;

    ASSERT_FALSE(float_data -= string_data, "float_data and string_data shouldn't subtract");
    ASSERT_EQUALS(float_data.to_float(), 65.0);
    ASSERT_ERROR(ERROR_PRIMITIVE_DATA_SUBTRACTING_STRING);
    ERROR_LOG_CLEAR;

    ASSERT_FALSE(float_data *= string_data, "float_data and string_data shouldn't multiply");
    ASSERT_EQUALS(float_data.to_float(), 65.0);
    ASSERT_ERROR(ERROR_PRIMITIVE_DATA_MULTIPLYING_STRING);
    ERROR_LOG_CLEAR;

    ASSERT_FALSE(float_data /= string_data,
                 "float_data and string_data shouldn't divide");
    ASSERT_EQUALS(float_data.to_float(), 65.0);
    ASSERT_ERROR(ERROR_PRIMITIVE_DATA_DIVIDING_STRING);
    ERROR_LOG_CLEAR;

    string_data = L"35 Grupa Zana rules!";
    ASSERT_TRUE(wcscmp((const wchar_t *) string_data.get_address(),
                       L"35 Grupa Zana rules!") == 0,
                "string should be \"35 Grupa Zana rules!\" (%ls)",
                (const wchar_t *) string_data.get_address());

    ASSERT_VIRTUAL_MEMORY(*vm,
                          sizeof(double) +
                          sizeof(L"65.000000") +
                          sizeof(int) +
                          sizeof(double) +
                          sizeof(L"35 Grupa Zana rules!"));

    ASSERT_FALSE(float_data == string_data, "float_data and string_data should be equal");
    ASSERT_OK;

    ASSERT_TRUE(float_data != string_data, "float_data and string_data should not be equal");
    ASSERT_OK;

    ASSERT_TRUE(float_data > string_data, "float_data should be bigger than string_data");
    ASSERT_OK;

    ASSERT_TRUE(float_data >= string_data, "float_data should be bigger or equal than string_data");
    ASSERT_OK;

    ASSERT_FALSE(float_data < string_data, "float_data should be bigger than string_data");
    ASSERT_OK;

    ASSERT_FALSE(float_data <= string_data, "float_data should be bigger or equal than string_data");
    ASSERT_OK;

    ASSERT_FALSE(float_data += string_data, "float_data and string_data shouldn't add");
    ASSERT_TRUE(float_data.to_float() == 65.0, "string_data should be 65.0!");
    ASSERT_ERROR(ERROR_PRIMITIVE_DATA_ADDING_STRING);
    ERROR_LOG_CLEAR;

    ASSERT_FALSE(float_data -= string_data,
                 "float_data and string_data shouldn't substract");
    ASSERT_EQUALS(float_data.to_float(), 65.0);
    ASSERT_ERROR(ERROR_PRIMITIVE_DATA_SUBTRACTING_STRING);
    ERROR_LOG_CLEAR;

    ASSERT_FALSE(float_data *= string_data,
                 "float_data and string_data shouldn't multiply");
    ASSERT_EQUALS(float_data.to_float(), 65.0);
    ASSERT_ERROR(ERROR_PRIMITIVE_DATA_MULTIPLYING_STRING);
    ERROR_LOG_CLEAR;

    ASSERT_FALSE(float_data /= string_data,
                 "float_data and string_data shouldn't divide");
    ASSERT_EQUALS(float_data.to_float(), 65.0);
    ASSERT_ERROR(ERROR_PRIMITIVE_DATA_DIVIDING_STRING);
    ERROR_LOG_CLEAR;

    string_data = L"0";

    ASSERT_FALSE(float_data /= string_data, "float_data and float_data shouldn't divide");
    ASSERT_EQUALS(float_data.to_float(), 65.0);
    ASSERT_ERROR(ERROR_PRIMITIVE_DATA_DIVIDING_STRING);
    ERROR_LOG_CLEAR;

    ASSERT_FALSE(float_data %= string_data, "float_data and string_data shouldn't mod");
    ASSERT_EQUALS(float_data.to_float(), 65.0);
    ASSERT_ERROR(ERROR_PRIMITIVE_DATA_INVALID_MODULUS);
    ERROR_LOG_CLEAR;

    ASSERT_FALSE(float_data = string_data, "string_data shouldn't assign to float_data");
    ASSERT_EQUALS(float_data.to_float(), 65.0);
    ASSERT_ERROR(ERROR_PRIMITIVE_DATA_ASSIGNING_STRING);
    ERROR_LOG_CLEAR;

    ASSERT_TRUE(float_data = int_data, "int_data should assign to float_data");
    ASSERT_OK;

    ASSERT_EQUALS(float_data.to_float(), 0.0);
    ASSERT_OK;
}

/**
 * Test empty string
 */
static void
primitive_data_test_string_empty()
{
    ASSERT_VIRTUAL_MEMORY(*vm, 0);

    primitive_data &empty_string = *string_data::create("empty_string");

    ASSERT_OK;
    ASSERT_VIRTUAL_MEMORY(*vm, DATA_TYPE_SIZE[OBJECT_TYPE_STRING]);
    ASSERT_TRUE(wcscmp((const wchar_t *) empty_string.get_address(), L"") == 0,
                "string_data should be empty!");
}

/**
 * Test data as string.
 */
static void
primitive_data_test_string()
{
    ASSERT_VIRTUAL_MEMORY(*vm, 0);

    primitive_data &string_data = *string_data::create("string_data", L"32");
    ASSERT_OK;
    ASSERT_VIRTUAL_MEMORY(*vm, sizeof(L"32"));

    ASSERT_FALSE(++string_data, "string_data shouldn't' be incremented!");
    ASSERT_ERROR(ERROR_PRIMITIVE_DATA_INCREMENTING_STRING);
    ERROR_LOG_CLEAR;

    ASSERT_FALSE(--string_data, "string_data shouldn't' be decremented!");
    ASSERT_ERROR(ERROR_PRIMITIVE_DATA_DECREMENTING_STRING);
    ERROR_LOG_CLEAR;

    ASSERT_TRUE(string_data.to_char() == L'3',
                "int_data to char() -> %c instead of %c",
                string_data.to_char(),
                L'3');
    ASSERT_OK;

    ASSERT_TRUE(string_data.to_float() == 32.0,
                "int_data to float() -> %f instead of %f",
                string_data.to_float(),
                32.0);
    ASSERT_OK;

    primitive_data &str = string_data.to_string();
    ASSERT_TRUE(wcscmp((const wchar_t *) str.get_address(), L"32") == 0,
                "string should be 32! (%ls)",
                (const wchar_t *) str.get_address());
    ASSERT_OK;
    ASSERT_VIRTUAL_MEMORY(*vm,
                          sizeof(L"32") +
                          sizeof(L"32"));

    primitive_data &string_data2 = *string_data::create("string_data2", L"31");
    ASSERT_VIRTUAL_MEMORY(*vm,
                          sizeof(L"32") +
                          sizeof(L"32") +
                          sizeof(L"31"));

    ASSERT_FALSE(string_data == string_data2,
                 "string_data and string_data2 should not be equal");
    ASSERT_OK;

    ASSERT_TRUE(string_data != string_data2, "string_data and string_data2 should not be equal");
    ASSERT_OK;

    ASSERT_TRUE(string_data > string_data2, "string_data should be bigger than string_data2");
    ASSERT_OK;

    ASSERT_TRUE(string_data >= string_data2, "string_data should be bigger or equal than string_data2");
    ASSERT_OK;

    ASSERT_FALSE(string_data < string_data2, "string_data should be bigger than string_data2");
    ASSERT_OK;

    ASSERT_FALSE(string_data <= string_data2, "string_data should be bigger or equal than string_data2");
    ASSERT_OK;

    ASSERT_VIRTUAL_MEMORY(*vm,
                          sizeof(L"32") +
                          sizeof(L"32") +
                          sizeof(L"31"));

    ASSERT_TRUE(string_data += string_data2,
                "string_data and string_data2 should add");
    ASSERT_OK;
    ASSERT_TRUE(wcscmp((const wchar_t *) string_data.get_address(), L"3231") == 0,
                "string_data should be 3231! (%ls)",
                (const wchar_t *) string_data.get_address());
    ASSERT_OK;

    ASSERT_VIRTUAL_MEMORY(*vm,
                          sizeof(L"3231") +
                          sizeof(L"32") +
                          sizeof(L"31"));

    ASSERT_FALSE(string_data -= string_data2,
                 "string_data and string_data2 shouldn't substract");
    ASSERT_TRUE(wcscmp((const wchar_t *) string_data.get_address(), L"3231") == 0,
                "string_data should be 3231!");
    ASSERT_ERROR(ERROR_PRIMITIVE_DATA_SUBTRACTING_STRING);
    ERROR_LOG_CLEAR;

    ASSERT_FALSE(string_data *= string_data2, "string_data and string_data2 shouldn't multiply");
    ASSERT_TRUE(wcscmp((const wchar_t *) string_data.get_address(), L"3231") == 0,
                "string_data should be 3231!");
    ASSERT_ERROR(ERROR_PRIMITIVE_DATA_MULTIPLYING_STRING);
    ERROR_LOG_CLEAR;

    ASSERT_FALSE(string_data /= string_data2, "string_data and string_data2 shouldn't divide");
    ASSERT_TRUE(wcscmp((const wchar_t *) string_data.get_address(), L"3231") == 0,
                "string_data should be 3231!");
    ASSERT_ERROR(ERROR_PRIMITIVE_DATA_DIVIDING_STRING);
    ERROR_LOG_CLEAR;

    ASSERT_FALSE(string_data %= string_data2, "string_data and string_data2 shouldn't mod");
    ASSERT_TRUE(wcscmp((const wchar_t *) string_data.get_address(), L"3231") == 0,
                "string_data should be 3231!");
    ASSERT_ERROR(ERROR_PRIMITIVE_DATA_INVALID_MODULUS);
    ERROR_LOG_CLEAR;

    int32_t num = 31;
    primitive_data &int_data = *int_data::create("int_data", &num);

    ASSERT_VIRTUAL_MEMORY(*vm,
                          sizeof(L"3231") +
                          sizeof(L"32") +
                          sizeof(L"31") +
                          DATA_TYPE_SIZE[OBJECT_TYPE_INT]);

    ASSERT_FALSE(string_data == int_data, "string_data and int_data should not be equal");
    ASSERT_OK;

    ASSERT_TRUE(string_data != int_data, "string_data and int_data should not be equal");
    ASSERT_OK;

    ASSERT_TRUE(string_data > int_data, "string_data should be bigger than int_data");
    ASSERT_OK;

    ASSERT_TRUE(string_data >= int_data, "string_data should be bigger or equal than int_data");
    ASSERT_OK;

    ASSERT_FALSE(string_data < int_data, "string_data should be bigger than int_data");
    ASSERT_OK;

    ASSERT_FALSE(string_data <= int_data, "string_data should be bigger or equal than int_data");
    ASSERT_OK;

    ASSERT_TRUE(string_data += int_data, "string_data and int_data should add");
    ASSERT_TRUE(wcscmp((const wchar_t *) string_data.get_address(), L"323131") == 0,
                "string_data should be 323131! (%ls)",
                (const wchar_t *) string_data.get_address());
    ASSERT_OK;

    ASSERT_VIRTUAL_MEMORY(*vm,
                          sizeof(L"323131") +
                          sizeof(L"32") +
                          sizeof(L"31") +
                          DATA_TYPE_SIZE[OBJECT_TYPE_INT]);

    ASSERT_FALSE(string_data -= int_data, "string_data and int_data shouldn't substract");
    ASSERT_TRUE(wcscmp((const wchar_t *) string_data.get_address(), L"323131") == 0, "string_data should be 323131!");
    ASSERT_ERROR(ERROR_PRIMITIVE_DATA_SUBTRACTING_STRING);
    ERROR_LOG_CLEAR;

    ASSERT_FALSE(string_data *= int_data,
                 "string_data and int_data shouldn't multiply");
    ASSERT_TRUE(wcscmp((const wchar_t *) string_data.get_address(), L"323131") == 0, "string_data should be 323131!");
    ASSERT_ERROR(ERROR_PRIMITIVE_DATA_MULTIPLYING_STRING);
    ERROR_LOG_CLEAR;

    ASSERT_FALSE(string_data /= int_data, "string_data and int_data shouldn't divide");
    ASSERT_TRUE(wcscmp((const wchar_t *) string_data.get_address(), L"323131") == 0, "string_data should be 323131!");
    ASSERT_ERROR(ERROR_PRIMITIVE_DATA_DIVIDING_STRING);
    ERROR_LOG_CLEAR;

    ASSERT_FALSE(string_data %= int_data, "string_data and int_data shouldn't mod");
    ASSERT_TRUE(wcscmp((const wchar_t *) string_data.get_address(), L"323131") == 0,
                "string_data should be 323131!");
    ASSERT_ERROR(ERROR_PRIMITIVE_DATA_INVALID_MODULUS);
    ERROR_LOG_CLEAR;

    double float_num = 31.0;
    primitive_data &float_data = *float_data::create("float_data", &float_num);

    ASSERT_VIRTUAL_MEMORY(*vm,
                          sizeof(L"323131") +
                          sizeof(L"32") +
                          sizeof(L"31") +
                          DATA_TYPE_SIZE[OBJECT_TYPE_INT] +
                          DATA_TYPE_SIZE[OBJECT_TYPE_FLOAT]);

    ASSERT_NOT_EQUALS(string_data, float_data);
    ASSERT_OK;

    ASSERT_TRUE(string_data > float_data,
                "string_data should be bigger than float_data");
    ASSERT_OK;

    ASSERT_TRUE(string_data >= float_data,
                "string_data should be bigger or equal than float_data");
    ASSERT_OK;

    ASSERT_FALSE(string_data < float_data,
                 "string_data should be bigger than float_data");
    ASSERT_OK;

    ASSERT_FALSE(string_data <= float_data,
                 "string_data should be bigger or equal than float_data");
    ASSERT_OK;

    ASSERT_TRUE(string_data += float_data, "string_data and float_data should add");
    ASSERT_TRUE(wcscmp((const wchar_t *) string_data.get_address(),
                       L"32313131.000000") == 0,
                "string_data should be 32313131.000000! (%ls)",
                (const wchar_t *) string_data.get_address());
    ASSERT_OK;

    ASSERT_VIRTUAL_MEMORY(*vm,
                          sizeof(L"32313131.000000") +
                          sizeof(L"32") +
                          sizeof(L"31") +
                          DATA_TYPE_SIZE[OBJECT_TYPE_INT] +
                          DATA_TYPE_SIZE[OBJECT_TYPE_FLOAT]);

    ASSERT_FALSE(string_data -= float_data, "string_data and float_data shouldn't substract");
    ASSERT_TRUE(wcscmp((const wchar_t *) string_data.get_address(), L"32313131.000000") == 0,
                "string_data should be 32313131.000000!");
    ASSERT_ERROR(ERROR_PRIMITIVE_DATA_SUBTRACTING_STRING);
    ERROR_LOG_CLEAR;

    ASSERT_FALSE(string_data *= float_data, "string_data and float_data shouldn't multiply");
    ASSERT_TRUE(wcscmp((const wchar_t *) string_data.get_address(), L"32313131.000000") == 0,
                "string_data should be 32313131.000000!");
    ASSERT_ERROR(ERROR_PRIMITIVE_DATA_MULTIPLYING_STRING);
    ERROR_LOG_CLEAR;

    ASSERT_FALSE(string_data /= float_data, "string_data and float_data shouldn't divide");
    ASSERT_TRUE(wcscmp((const wchar_t *) string_data.get_address(),
                       L"32313131.000000") == 0,
                "string_data should be 32313131.000000!");
    ASSERT_ERROR(ERROR_PRIMITIVE_DATA_DIVIDING_STRING);
    ERROR_LOG_CLEAR;

    ASSERT_FALSE(string_data %= float_data, "string_data and float_data shouldn't mod");
    ASSERT_TRUE(wcscmp((const wchar_t *) string_data.get_address(),
                       L"32313131.000000") == 0,
                "string_data should be 32313131.000000!");
    ASSERT_ERROR(ERROR_PRIMITIVE_DATA_INVALID_MODULUS);
    ERROR_LOG_CLEAR;

    ASSERT_NOT_EQUALS(string_data, float_data);
    ASSERT_OK;

    ASSERT_NOT_EQUALS(string_data, int_data);
    ASSERT_TRUE(wcscmp((const wchar_t *) string_data.get_address(), L"31") != 0,
                "string_data should be 31! (%ls)",
                (const wchar_t *) string_data.get_address());
    ASSERT_OK;

    ASSERT_VIRTUAL_MEMORY(*vm,
                          sizeof(L"32313131.000000") + // still reserved
                          sizeof(L"32") +
                          sizeof(L"31") +
                          DATA_TYPE_SIZE[OBJECT_TYPE_INT] +
                          DATA_TYPE_SIZE[OBJECT_TYPE_FLOAT]);
}

/**
 * Test integer data.
 */
static void
primitive_data_test_int()
{
    int32_t num = 65;
    int_data &int_data = *int_data::create("int_data", &num);

    ASSERT_OK;

    ASSERT_TRUE(++int_data, "int_data should be incremented!");
    ASSERT_OK;
    ASSERT_EQUALS(int_data.to_int(), 66);

    ASSERT_TRUE(--int_data, "int_data should be incremented!");
    ASSERT_OK;
    ASSERT_EQUALS(int_data.to_int(), 65);

    printf("debug -> ");
    ASSERT_TRUE(int_data.print(), "int_data should be printed!");
    printf("\n");
    ASSERT_OK;

    printf("debug -> ");
    ASSERT_TRUE(int_data.println(), "int_data should be printed!");
    ASSERT_OK;

    ASSERT_TRUE(int_data.to_char() == L'A',
                "int_data to char() -> %c instead of %c",
                int_data.to_char(),
                'A');
    ASSERT_OK;

    ASSERT_TRUE(int_data.to_float() == 65.0,
                "int_data to float() -> %f instead of %f",
                int_data.to_float(),
                65.0);
    ASSERT_OK;

    primitive_data &str = int_data.to_string();
    ASSERT_OK;
    ASSERT_TRUE(wcscmp((const wchar_t *) str.get_address(), L"65") == 0,
                "string should be 65! (%ls)",
                (const wchar_t *) str.get_address());

    int32_t num2 = 35;
    primitive_data &int_data2 = *int_data::create("int_data", &num2);

    ASSERT_FALSE(int_data == int_data2, "int_data and int_data2 should not be equal");
    ASSERT_OK;

    ASSERT_TRUE(int_data != int_data2, "int_data and int_data2 should not be equal");
    ASSERT_OK;

    ASSERT_TRUE(int_data > int_data2, "int_data should be bigger than int_data2");
    ASSERT_OK;

    ASSERT_TRUE(int_data >= int_data2, "int_data should be bigger or equal than int_data2");
    ASSERT_OK;

    ASSERT_FALSE(int_data < int_data2, "int_data should be bigger than int_data2");
    ASSERT_OK;

    ASSERT_FALSE(int_data <= int_data2,
                 "int_data should be bigger or equal than int_data2");
    ASSERT_OK;

    ASSERT_TRUE(int_data += int_data2, "int_data and int_data2 should add");
    ASSERT_EQUALS(int_data.to_int(), 100);

    ASSERT_TRUE(int_data -= int_data2, "int_data and int_data2 should substract");
    ASSERT_EQUALS(int_data.to_int(), 65);

    ASSERT_TRUE(int_data *= int_data2, "int_data and int_data2 should multiply");
    ASSERT_EQUALS(int_data.to_int(), 2275);

    ASSERT_TRUE(int_data /= int_data2, "int_data and int_data2 should divide");
    ASSERT_EQUALS(int_data.to_int(), 65);

    ASSERT_TRUE(int_data %= int_data2, "int_data and int_data2 should mod");
    ASSERT_EQUALS(int_data.to_int(), 30);

    /*
     * Putting int_data2 to value of zero.
     */
    int_data2.default_value();

    ASSERT_FALSE(int_data /= int_data2, "int_data and int_data2 shouldn't divide");
    ASSERT_EQUALS(int_data.to_int(), 30);
    ASSERT_ERROR(ERROR_PRIMITIVE_DATA_DIVIDING_ZERO);
    ERROR_LOG_CLEAR;

    ASSERT_FALSE(int_data %= int_data2, "int_data and int_data2 shouldn't mod");
    ASSERT_EQUALS(int_data.to_int(), 30);
    ASSERT_ERROR(ERROR_PRIMITIVE_DATA_MODULUS_ZERO);
    ERROR_LOG_CLEAR;

    /*
     * Putting int_data to value of 65.
     */
    *(int32_t *) int_data.get_address() = 65;

    double float_num = 35.0;
    primitive_data &float_data = *float_data::create("float_data", &float_num);

    ASSERT_FALSE(int_data == float_data, "int_data and float_data should not be equal");
    ASSERT_OK;

    ASSERT_TRUE(int_data != float_data, "int_data and float_data should not be equal");
    ASSERT_OK;

    ASSERT_TRUE(int_data > float_data, "int_data should be bigger than float_data");
    ASSERT_OK;

    ASSERT_TRUE(int_data >= float_data, "int_data should be bigger or equal than float_data");
    ASSERT_OK;

    ASSERT_FALSE(int_data < float_data, "int_data should be bigger than float_data");
    ASSERT_OK;

    ASSERT_FALSE(int_data <= float_data, "int_data should be bigger or equal than float_data");
    ASSERT_OK;

    ASSERT_TRUE(int_data += float_data, "int_data and float_data should add");
    ASSERT_EQUALS(int_data.to_int(), 100);

    ASSERT_TRUE(int_data -= float_data, "int_data and float_data should substract");
    ASSERT_EQUALS(int_data.to_int(), 65);

    ASSERT_TRUE(int_data *= float_data, "int_data and float_data should multiply");
    ASSERT_EQUALS(int_data.to_int(), 2275);

    ASSERT_TRUE(int_data /= float_data, "int_data and float_data should divide");
    ASSERT_EQUALS(int_data.to_int(), 65);

    /*
     * setting float_data to zero value
     */
    *(double *) float_data.get_address() = 0.0;

    ASSERT_FALSE(int_data /= float_data, "int_data and float_data shouldn't divide");
    ASSERT_EQUALS(int_data.to_int(), 65);
    ASSERT_ERROR(ERROR_PRIMITIVE_DATA_DIVIDING_ZERO);
    ERROR_LOG_CLEAR;

    ASSERT_FALSE(int_data %= float_data, "int_data and float_data shouldn't mod");
    ASSERT_EQUALS(int_data.to_int(), 65);
    ASSERT_ERROR(ERROR_PRIMITIVE_DATA_INVALID_MODULUS);
    ERROR_LOG_CLEAR;

    /* interaction with string */

    string_data &string_data = *string_data::create("string_data", L"35");

    ASSERT_FALSE(int_data == string_data, "int_data and string_data should not be equal");
    ASSERT_OK;

    ASSERT_TRUE(int_data != string_data, "int_data and string_data should not be equal");
    ASSERT_OK;

    ASSERT_TRUE(int_data > string_data, "int_data should be bigger than string_data");
    ASSERT_OK;

    ASSERT_TRUE(int_data >= string_data, "int_data should be bigger or equal than string_data");
    ASSERT_OK;

    ASSERT_FALSE(int_data < string_data, "int_data should be bigger than string_data");
    ASSERT_OK;

    ASSERT_FALSE(int_data <= string_data, "int_data should be bigger or equal than string_data");
    ASSERT_OK;

    ASSERT_FALSE(int_data += string_data, "int_data and string_data shouldn't add");
    ASSERT_EQUALS(int_data.to_int(), 65);
    ASSERT_ERROR(ERROR_PRIMITIVE_DATA_ADDING_STRING);
    ERROR_LOG_CLEAR;

    ASSERT_FALSE(int_data -= string_data, "int_data and string_data shouldn't substract");
    ASSERT_EQUALS(int_data.to_int(), 65);
    ASSERT_ERROR(ERROR_PRIMITIVE_DATA_SUBTRACTING_STRING);
    ERROR_LOG_CLEAR;

    ASSERT_FALSE(int_data *= string_data, "int_data and string_data shouldn't multiply");
    ASSERT_EQUALS(int_data.to_int(), 65);
    ASSERT_ERROR(ERROR_PRIMITIVE_DATA_MULTIPLYING_STRING);
    ERROR_LOG_CLEAR;

    ASSERT_FALSE(int_data /= string_data, "int_data and string_data shouldn't divide");
    ASSERT_EQUALS(int_data.to_int(), 65);
    ASSERT_ERROR(ERROR_PRIMITIVE_DATA_DIVIDING_STRING);
    ERROR_LOG_CLEAR;

    string_data = L"35 Grupa Zana rules!";
    ASSERT_OK;

    ASSERT_FALSE(int_data == string_data, "int_data and string_data should not be equal");
    ASSERT_OK;

    ASSERT_TRUE(int_data != string_data, "int_data and string_data should not be equal");
    ASSERT_OK;

    ASSERT_TRUE(int_data > string_data, "int_data should be bigger than string_data");
    ASSERT_OK;

    ASSERT_TRUE(int_data >= string_data, "int_data should be bigger or equal than string_data");
    ASSERT_OK;

    ASSERT_FALSE(int_data < string_data, "int_data should be bigger than string_data");
    ASSERT_OK;

    ASSERT_FALSE(int_data <= string_data, "int_data should be bigger or equal than string_data");
    ASSERT_OK;

    ASSERT_FALSE(int_data += string_data, "int_data and string_data shouldn't add");
    ASSERT_EQUALS(int_data.to_int(), 65);
    ASSERT_ERROR(ERROR_PRIMITIVE_DATA_ADDING_STRING);
    ERROR_LOG_CLEAR;

    ASSERT_FALSE(int_data -= string_data, "int_data and string_data shouldn't substract");
    ASSERT_EQUALS(int_data.to_int(), 65);
    ASSERT_ERROR(ERROR_PRIMITIVE_DATA_SUBTRACTING_STRING);
    ERROR_LOG_CLEAR;

    ASSERT_FALSE(int_data *= string_data, "int_data and string_data shouldn't multiply");
    ASSERT_EQUALS(int_data.to_int(), 65);
    ASSERT_ERROR(ERROR_PRIMITIVE_DATA_MULTIPLYING_STRING);
    ERROR_LOG_CLEAR;

    ASSERT_FALSE(int_data /= string_data, "int_data and string_data shouldn't divide");
    ASSERT_EQUALS(int_data.to_int(), 65);
    ASSERT_ERROR(ERROR_PRIMITIVE_DATA_DIVIDING_STRING);
    ERROR_LOG_CLEAR;

    string_data = L"0";

    ASSERT_FALSE(int_data /= string_data,
                 "int_data and float_data shouldn't divide");
    ASSERT_ERROR(ERROR_PRIMITIVE_DATA_DIVIDING_STRING);
    ASSERT_TRUE(int_data.to_int() == 65, "int_data should be 65");
    ERROR_LOG_CLEAR;

    ASSERT_FALSE(int_data %= string_data, "int_data and string_data shouldn't mod");
    ASSERT_ERROR(ERROR_PRIMITIVE_DATA_INVALID_MODULUS);
    ERROR_LOG_CLEAR;

    ASSERT_FALSE(int_data = string_data, "string_data shouldn't assign to int_data");
    ASSERT_EQUALS(int_data.to_int(), 65);
    ASSERT_ERROR(ERROR_PRIMITIVE_DATA_ASSIGNING_STRING);
    ERROR_LOG_CLEAR;

    ASSERT_TRUE(int_data = float_data, "float_data should assign to int_data");
    ASSERT_EQUALS(int_data.to_int(), 0);
    ASSERT_OK;

    int_data.print();
}

/**
 * sub test.
 *
 * @param type
 */
static void
primitive_data_test_references(object_type type)
{
    primitive_data &data = *primitive_data::create("data", type);
    ASSERT_EQUALS(data.get_is_reference(), false);

    primitive_data &data2 = *primitive_data::create("data2", data, true);
    ASSERT_EQUALS(data2.get_is_reference(), true);
    ASSERT_EQUALS(data.get_memory(), data2.get_memory());

    primitive_data &data3 = *primitive_data::create("data3", data, true);
    ASSERT_EQUALS(data3.get_is_reference(), true);
    ASSERT_EQUALS(data3.get_memory(), data2.get_memory());
    ASSERT_EQUALS(data3.get_memory(), data.get_memory());

    ASSERT_NOT_NULL(orm::select(type, [&] (object * obj) { return obj->get_id() == "data"; }));
    ASSERT_NOT_NULL(orm::select(type, [&] (object * obj) { return obj->get_id() == "data2"; }));
    ASSERT_NOT_NULL(orm::select(type, [&] (object * obj) { return obj->get_id() == "data3"; }));

    ORM_DESTROY(&data);

    ASSERT_NULL(orm::select(type, [&] (object * obj) { return obj->get_id() == "data"; }));
    ASSERT_NOT_NULL(orm::select(type, [&] (object * obj) { return obj->get_id() == "data2"; }));
    ASSERT_NOT_NULL(orm::select(type, [&] (object * obj) { return obj->get_id() == "data3"; }));

    ASSERT_EQUALS(data2.get_is_reference(), true);
    ASSERT_EQUALS(data3.get_is_reference(), true);
    ASSERT_EQUALS(data2.get_memory(), data2.get_memory());
}

/**
 * Test data references.
 */
void primitive_data_test_references()
{
    orm::remove_object_repository(OBJECT_TYPE_BOOL);
    orm::remove_object_repository(OBJECT_TYPE_CHAR);
    orm::remove_object_repository(OBJECT_TYPE_INT);
    orm::remove_object_repository(OBJECT_TYPE_FLOAT);
    orm::remove_object_repository(OBJECT_TYPE_STRING);

    string_data &str = *string_data::create("string_data", L"35");
    ASSERT_EQUALS(str.get_is_reference(), true);

    string_data &str2 = *string_data::create("string_data2", str);
    ASSERT_EQUALS(str2.get_is_reference(), true);

    ASSERT_EQUALS(str.get_memory(), str2.get_memory());

    ORM_DESTROY(&str2);

    for (int32_t type = OBJECT_TYPE_BOOL; type < OBJECT_TYPE_STRING; type++)
    {
        primitive_data_test_references((object_type) type);
    }
}

/**
 * Test primitive data class.
 */
void primitive_data_test()
{
    vm = (virtual_memory *) orm::get_first(OBJECT_TYPE_VIRTUAL_MEMORY);

    RUN_TEST_VM(primitive_data_test_int());
    RUN_TEST_VM(primitive_data_test_string_empty());
    RUN_TEST_VM(primitive_data_test_string());
    RUN_TEST_VM(primitive_data_test_float());
    RUN_TEST_VM(primitive_data_test_references());
}
