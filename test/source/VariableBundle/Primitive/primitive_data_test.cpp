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

#include "ErrorBundle/ErrorLog.h"
#include <VariableBundle/Primitive/Float.h>
#include <VariableBundle/Primitive/Int.h>
#include <VariableBundle/Primitive/String.h>
#include "../../../include/VariableBundle/Primitive/primitive_data_test.h"
#include "../../../test_assert.h"
#include "ORM/ORM.h"

static VirtualMemory *vm;

/**
 * Test data as float.
 */
static void
primitive_data_test_float()
{
    ASSERT_VIRTUAL_MEMORY(*vm, 0);

    double float_num = 65.0;
    Value &float_data = *Float::create(&float_num);
    ASSERT_VIRTUAL_MEMORY(*vm, sizeof(double));

    ASSERT_OK;

    ASSERT_TRUE(++float_data, "Float should be incremented!");
    ASSERT_OK;
    ASSERT_EQUALS(float_data.toFloat(), 66.0);

    ASSERT_TRUE(--float_data, "Int should be incremented!");
    ASSERT_OK;
    ASSERT_EQUALS(float_data.toFloat(), 65.0);

    ASSERT_EQUALS(float_data.toChar(), L'A');
    ASSERT_OK;

    ASSERT_EQUALS(float_data.toInt(), 65);
    ASSERT_OK;

    String &str = float_data.toString();
    ASSERT_TRUE(wcscmp((const wchar_t *) str.getAddress(), L"65.000000") == 0,
                "string should be 65.000000! (%ls)",
                (const wchar_t *) str.getAddress());
    ASSERT_OK;

    ASSERT_VIRTUAL_MEMORY(*vm,
                          sizeof(double) +
                          sizeof(L"65.000000"));

    int32_t num = 35;
    Value &int_data = *Int::create(&num);

    ASSERT_VIRTUAL_MEMORY(*vm,
                          sizeof(double) +
                          sizeof(L"65.000000") +
                          sizeof(int));

    ASSERT_FALSE(float_data == int_data, "Float and Int should not be equal");
    ASSERT_OK;

    ASSERT_TRUE(float_data != int_data, "Float and Int should not be equal");
    ASSERT_OK;

    ASSERT_TRUE(float_data > int_data, "Float should be bigger than Int");
    ASSERT_OK;

    ASSERT_TRUE(float_data >= int_data, "Float should be bigger or equal than Int");
    ASSERT_OK;

    ASSERT_FALSE(float_data < int_data, "Float should be bigger than Int");
    ASSERT_OK;

    ASSERT_FALSE(float_data <= int_data, "Float should be bigger or equal than Int");
    ASSERT_OK;

    ASSERT_TRUE(float_data += int_data, "Float and Int should init");
    ASSERT_EQUALS(float_data.toFloat(), 100.0);

    ASSERT_TRUE(float_data -= int_data, "Float and Int should substract");
    ASSERT_EQUALS(float_data.toFloat(), 65.0);

    ASSERT_TRUE(float_data *= int_data, "Float and Int should multiply");
    ASSERT_EQUALS(float_data.toFloat(), 2275.0);

    ASSERT_TRUE(float_data /= int_data, "Float and Int should divide");
    ASSERT_EQUALS(float_data.toFloat(), 65.0);

    ASSERT_FALSE(float_data %= int_data, "Float and Int shouldn't mod");
    ASSERT_EQUALS(float_data.toFloat(), 65.0);

    ASSERT_ERROR(ERROR_PRIMITIVE_DATA_INVALID_MODULUS);
    ERROR_LOG_CLEAR;

    /*
     * Putting int_data2 to Value of zero.
     */
    num = 0;
    int_data = &num;

    ASSERT_FALSE(float_data /= int_data, "Int and int_data2 shouldn't divide");
    ASSERT_EQUALS(float_data.toFloat(), 65.0);
    ASSERT_ERROR(ERROR_PRIMITIVE_DATA_DIVIDING_ZERO);
    ERROR_LOG_CLEAR;

    ASSERT_FALSE(float_data %= int_data, "Int and int_data2 shouldn't mod");
    ASSERT_EQUALS(float_data.toFloat(), 65.0);
    ASSERT_ERROR(ERROR_PRIMITIVE_DATA_INVALID_MODULUS);
    ERROR_LOG_CLEAR;

    double fnum2 = 35.0;
    Value &float_data2 = *Float::create(&fnum2);

    ASSERT_VIRTUAL_MEMORY(*vm,
                          sizeof(double) +
                          sizeof(L"65.000000") +
                          sizeof(int) +
                          sizeof(double));

    ASSERT_NOT_EQUALS(float_data, float_data2);
    ASSERT_OK;

    ASSERT_TRUE(float_data > float_data2,
                "Float should be bigger than float_data2");
    ASSERT_OK;

    ASSERT_TRUE(float_data >= float_data2,
                "Float should be bigger or equal than float_data2");
    ASSERT_OK;

    ASSERT_FALSE(float_data < float_data2,
                 "Float should be bigger than float_data2");
    ASSERT_OK;

    ASSERT_FALSE(float_data <= float_data2,
                 "Float should be bigger or equal than float_data2");
    ASSERT_OK;

    ASSERT_TRUE(float_data += float_data2, "Float and float_data2 should init");
    ASSERT_EQUALS(float_data.toFloat(), 100.0);

    ASSERT_TRUE(float_data -= float_data2,
                "Float and float_data2 should substract");
    ASSERT_EQUALS(float_data.toFloat(), 65.0);

    ASSERT_TRUE(float_data *= float_data2,
                "Float and float_data2 should multiply");
    ASSERT_EQUALS(float_data.toFloat(), 2275.0);

    ASSERT_TRUE(float_data /= float_data2,
                "Float and float_data2 should divide");
    ASSERT_EQUALS(float_data.toFloat(), 65.0);

    /*
     * setting Float to zero Value
     */
    fnum2 = 0.0;
    float_data2 = &fnum2;

    ASSERT_FALSE(float_data /= float_data2,
                 "Int and Float shouldn't divide");
    ASSERT_EQUALS(float_data.toFloat(), 65.0);
    ASSERT_ERROR(ERROR_PRIMITIVE_DATA_DIVIDING_ZERO);
    ERROR_LOG_CLEAR;

    ASSERT_FALSE(float_data %= float_data, "Int and Float shouldn't mod");
    ASSERT_TRUE(float_data.toFloat() == 65.0, "Int should be 65.0");
    ASSERT_ERROR(ERROR_PRIMITIVE_DATA_INVALID_MODULUS);
    ERROR_LOG_CLEAR;

    /* interaction with string */

    String &string_data = *String::create(L"35");

    ASSERT_VIRTUAL_MEMORY(*vm,
                          sizeof(double) +
                          sizeof(L"65.000000") +
                          sizeof(int) +
                          sizeof(double) +
                          sizeof(L"35"));

    ASSERT_FALSE(float_data == string_data, "Float and String should not be equal");
    ASSERT_OK;

    ASSERT_TRUE(float_data != string_data, "Float and String should not be equal");
    ASSERT_OK;

    ASSERT_TRUE(float_data > string_data, "Float should be bigger than String");
    ASSERT_OK;

    ASSERT_TRUE(float_data >= string_data, "Float should be bigger or equal than String");
    ASSERT_OK;

    ASSERT_FALSE(float_data < string_data,
                 "Float should be bigger than String");
    ASSERT_OK;

    ASSERT_FALSE(float_data <= string_data, "Float should be bigger or equal than String");
    ASSERT_OK;

    ASSERT_FALSE(float_data += string_data, "Float and String shouldn't init");
    ASSERT_EQUALS(float_data.toFloat(), 65.0);
    ASSERT_ERROR(ERROR_PRIMITIVE_DATA_ADDING_STRING);
    ERROR_LOG_CLEAR;

    ASSERT_FALSE(float_data -= string_data, "Float and String shouldn't subtract");
    ASSERT_EQUALS(float_data.toFloat(), 65.0);
    ASSERT_ERROR(ERROR_PRIMITIVE_DATA_SUBTRACTING_STRING);
    ERROR_LOG_CLEAR;

    ASSERT_FALSE(float_data *= string_data, "Float and String shouldn't multiply");
    ASSERT_EQUALS(float_data.toFloat(), 65.0);
    ASSERT_ERROR(ERROR_PRIMITIVE_DATA_MULTIPLYING_STRING);
    ERROR_LOG_CLEAR;

    ASSERT_FALSE(float_data /= string_data,
                 "Float and String shouldn't divide");
    ASSERT_EQUALS(float_data.toFloat(), 65.0);
    ASSERT_ERROR(ERROR_PRIMITIVE_DATA_DIVIDING_STRING);
    ERROR_LOG_CLEAR;

    string_data = L"35 Grupa Zana rules!";
    ASSERT_TRUE(wcscmp((const wchar_t *) string_data.getAddress(),
                       L"35 Grupa Zana rules!") == 0,
                "string should be \"35 Grupa Zana rules!\" (%ls)",
                (const wchar_t *) string_data.getAddress());

    ASSERT_VIRTUAL_MEMORY(*vm,
                          sizeof(double) +
                          sizeof(L"65.000000") +
                          sizeof(int) +
                          sizeof(double) +
                          sizeof(L"35 Grupa Zana rules!"));

    ASSERT_FALSE(float_data == string_data, "Float and String should be equal");
    ASSERT_OK;

    ASSERT_TRUE(float_data != string_data, "Float and String should not be equal");
    ASSERT_OK;

    ASSERT_TRUE(float_data > string_data, "Float should be bigger than String");
    ASSERT_OK;

    ASSERT_TRUE(float_data >= string_data, "Float should be bigger or equal than String");
    ASSERT_OK;

    ASSERT_FALSE(float_data < string_data, "Float should be bigger than String");
    ASSERT_OK;

    ASSERT_FALSE(float_data <= string_data, "Float should be bigger or equal than String");
    ASSERT_OK;

    ASSERT_FALSE(float_data += string_data, "Float and String shouldn't init");
    ASSERT_TRUE(float_data.toFloat() == 65.0, "String should be 65.0!");
    ASSERT_ERROR(ERROR_PRIMITIVE_DATA_ADDING_STRING);
    ERROR_LOG_CLEAR;

    ASSERT_FALSE(float_data -= string_data,
                 "Float and String shouldn't substract");
    ASSERT_EQUALS(float_data.toFloat(), 65.0);
    ASSERT_ERROR(ERROR_PRIMITIVE_DATA_SUBTRACTING_STRING);
    ERROR_LOG_CLEAR;

    ASSERT_FALSE(float_data *= string_data,
                 "Float and String shouldn't multiply");
    ASSERT_EQUALS(float_data.toFloat(), 65.0);
    ASSERT_ERROR(ERROR_PRIMITIVE_DATA_MULTIPLYING_STRING);
    ERROR_LOG_CLEAR;

    ASSERT_FALSE(float_data /= string_data,
                 "Float and String shouldn't divide");
    ASSERT_EQUALS(float_data.toFloat(), 65.0);
    ASSERT_ERROR(ERROR_PRIMITIVE_DATA_DIVIDING_STRING);
    ERROR_LOG_CLEAR;

    string_data = L"0";

    ASSERT_FALSE(float_data /= string_data, "Float and Float shouldn't divide");
    ASSERT_EQUALS(float_data.toFloat(), 65.0);
    ASSERT_ERROR(ERROR_PRIMITIVE_DATA_DIVIDING_STRING);
    ERROR_LOG_CLEAR;

    ASSERT_FALSE(float_data %= string_data, "Float and String shouldn't mod");
    ASSERT_EQUALS(float_data.toFloat(), 65.0);
    ASSERT_ERROR(ERROR_PRIMITIVE_DATA_INVALID_MODULUS);
    ERROR_LOG_CLEAR;

    ASSERT_FALSE(float_data = string_data, "String shouldn't assign to Float");
    ASSERT_EQUALS(float_data.toFloat(), 65.0);
    ASSERT_ERROR(ERROR_PRIMITIVE_DATA_ASSIGNING_STRING);
    ERROR_LOG_CLEAR;

    ASSERT_TRUE(float_data = int_data, "Int should assign to Float");
    ASSERT_OK;

    ASSERT_EQUALS(float_data.toFloat(), 0.0);
    ASSERT_OK;
}

/**
 * Test empty string
 */
static void
primitive_data_test_string_empty()
{
    ASSERT_VIRTUAL_MEMORY(*vm, 0);

    Primitive &empty_string = *String::create();

    ASSERT_OK;
    ASSERT_VIRTUAL_MEMORY(*vm, DataType::SIZE[OBJECT_TYPE_STRING]);
    ASSERT_TRUE(wcscmp((const wchar_t *) empty_string.getAddress(), L"") == 0,
                "String should be empty!");
}

/**
 * Test data as string.
 */
static void
primitive_data_test_string()
{
    ASSERT_VIRTUAL_MEMORY(*vm, 0);

    Primitive &string_data = *String::create(L"32");
    ASSERT_OK;
    ASSERT_VIRTUAL_MEMORY(*vm, sizeof(L"32"));

    ASSERT_FALSE(++string_data, "String shouldn't' be incremented!");
    ASSERT_ERROR(ERROR_PRIMITIVE_DATA_INCREMENTING_STRING);
    ERROR_LOG_CLEAR;

    ASSERT_FALSE(--string_data, "String shouldn't' be decremented!");
    ASSERT_ERROR(ERROR_PRIMITIVE_DATA_DECREMENTING_STRING);
    ERROR_LOG_CLEAR;

    ASSERT_TRUE(string_data.toChar() == L'3',
                "Int to char() -> %c instead of %c",
                string_data.toChar(),
                L'3');
    ASSERT_OK;

    ASSERT_TRUE(string_data.toFloat() == 32.0,
                "Int to float() -> %f instead of %f",
                string_data.toFloat(),
                32.0);
    ASSERT_OK;

    Primitive &str = string_data.toString();
    ASSERT_TRUE(wcscmp((const wchar_t *) str.getAddress(), L"32") == 0,
                "string should be 32! (%ls)",
                (const wchar_t *) str.getAddress());
    ASSERT_OK;
    ASSERT_VIRTUAL_MEMORY(*vm,
                          sizeof(L"32") +
                          sizeof(L"32"));

    Primitive &string_data2 = *String::create(L"31");
    ASSERT_VIRTUAL_MEMORY(*vm,
                          sizeof(L"32") +
                          sizeof(L"32") +
                          sizeof(L"31"));

    ASSERT_FALSE(string_data == string_data2,
                 "String and string_data2 should not be equal");
    ASSERT_OK;

    ASSERT_TRUE(string_data != string_data2, "String and string_data2 should not be equal");
    ASSERT_OK;

    ASSERT_TRUE(string_data > string_data2, "String should be bigger than string_data2");
    ASSERT_OK;

    ASSERT_TRUE(string_data >= string_data2, "String should be bigger or equal than string_data2");
    ASSERT_OK;

    ASSERT_FALSE(string_data < string_data2, "String should be bigger than string_data2");
    ASSERT_OK;

    ASSERT_FALSE(string_data <= string_data2, "String should be bigger or equal than string_data2");
    ASSERT_OK;

    ASSERT_VIRTUAL_MEMORY(*vm,
                          sizeof(L"32") +
                          sizeof(L"32") +
                          sizeof(L"31"));

    ASSERT_TRUE(string_data += string_data2,
                "String and string_data2 should init");
    ASSERT_OK;
    ASSERT_TRUE(wcscmp((const wchar_t *) string_data.getAddress(), L"3231") == 0,
                "String should be 3231! (%ls)",
                (const wchar_t *) string_data.getAddress());
    ASSERT_OK;

    ASSERT_VIRTUAL_MEMORY(*vm,
                          sizeof(L"3231") +
                          sizeof(L"32") +
                          sizeof(L"31"));

    ASSERT_FALSE(string_data -= string_data2,
                 "String and string_data2 shouldn't substract");
    ASSERT_TRUE(wcscmp((const wchar_t *) string_data.getAddress(), L"3231") == 0,
                "String should be 3231!");
    ASSERT_ERROR(ERROR_PRIMITIVE_DATA_SUBTRACTING_STRING);
    ERROR_LOG_CLEAR;

    ASSERT_FALSE(string_data *= string_data2, "String and string_data2 shouldn't multiply");
    ASSERT_TRUE(wcscmp((const wchar_t *) string_data.getAddress(), L"3231") == 0,
                "String should be 3231!");
    ASSERT_ERROR(ERROR_PRIMITIVE_DATA_MULTIPLYING_STRING);
    ERROR_LOG_CLEAR;

    ASSERT_FALSE(string_data /= string_data2, "String and string_data2 shouldn't divide");
    ASSERT_TRUE(wcscmp((const wchar_t *) string_data.getAddress(), L"3231") == 0,
                "String should be 3231!");
    ASSERT_ERROR(ERROR_PRIMITIVE_DATA_DIVIDING_STRING);
    ERROR_LOG_CLEAR;

    ASSERT_FALSE(string_data %= string_data2, "String and string_data2 shouldn't mod");
    ASSERT_TRUE(wcscmp((const wchar_t *) string_data.getAddress(), L"3231") == 0,
                "String should be 3231!");
    ASSERT_ERROR(ERROR_PRIMITIVE_DATA_INVALID_MODULUS);
    ERROR_LOG_CLEAR;

    int32_t num = 31;
    Primitive &int_data = *Int::create(&num);

    ASSERT_VIRTUAL_MEMORY(*vm,
                          sizeof(L"3231") +
                          sizeof(L"32") +
                          sizeof(L"31") +
                                  DataType::SIZE[OBJECT_TYPE_INT]);

    ASSERT_FALSE(string_data == int_data, "String and Int should not be equal");
    ASSERT_OK;

    ASSERT_TRUE(string_data != int_data, "String and Int should not be equal");
    ASSERT_OK;

    ASSERT_TRUE(string_data > int_data, "String should be bigger than Int");
    ASSERT_OK;

    ASSERT_TRUE(string_data >= int_data, "String should be bigger or equal than Int");
    ASSERT_OK;

    ASSERT_FALSE(string_data < int_data, "String should be bigger than Int");
    ASSERT_OK;

    ASSERT_FALSE(string_data <= int_data, "String should be bigger or equal than Int");
    ASSERT_OK;

    ASSERT_TRUE(string_data += int_data, "String and Int should init");
    ASSERT_TRUE(wcscmp((const wchar_t *) string_data.getAddress(), L"323131") == 0,
                "String should be 323131! (%ls)",
                (const wchar_t *) string_data.getAddress());
    ASSERT_OK;

    ASSERT_VIRTUAL_MEMORY(*vm,
                          sizeof(L"323131") +
                          sizeof(L"32") +
                          sizeof(L"31") +
                                  DataType::SIZE[OBJECT_TYPE_INT]);

    ASSERT_FALSE(string_data -= int_data, "String and Int shouldn't substract");
    ASSERT_TRUE(wcscmp((const wchar_t *) string_data.getAddress(), L"323131") == 0, "String should be 323131!");
    ASSERT_ERROR(ERROR_PRIMITIVE_DATA_SUBTRACTING_STRING);
    ERROR_LOG_CLEAR;

    ASSERT_FALSE(string_data *= int_data,
                 "String and Int shouldn't multiply");
    ASSERT_TRUE(wcscmp((const wchar_t *) string_data.getAddress(), L"323131") == 0, "String should be 323131!");
    ASSERT_ERROR(ERROR_PRIMITIVE_DATA_MULTIPLYING_STRING);
    ERROR_LOG_CLEAR;

    ASSERT_FALSE(string_data /= int_data, "String and Int shouldn't divide");
    ASSERT_TRUE(wcscmp((const wchar_t *) string_data.getAddress(), L"323131") == 0, "String should be 323131!");
    ASSERT_ERROR(ERROR_PRIMITIVE_DATA_DIVIDING_STRING);
    ERROR_LOG_CLEAR;

    ASSERT_FALSE(string_data %= int_data, "String and Int shouldn't mod");
    ASSERT_TRUE(wcscmp((const wchar_t *) string_data.getAddress(), L"323131") == 0,
                "String should be 323131!");
    ASSERT_ERROR(ERROR_PRIMITIVE_DATA_INVALID_MODULUS);
    ERROR_LOG_CLEAR;

    double float_num = 31.0;
    Primitive &float_data = *Float::create(&float_num);

    ASSERT_VIRTUAL_MEMORY(*vm,
                          sizeof(L"323131") +
                          sizeof(L"32") +
                          sizeof(L"31") +
                                  DataType::SIZE[OBJECT_TYPE_INT] +
                                  DataType::SIZE[OBJECT_TYPE_FLOAT]);

    ASSERT_NOT_EQUALS(string_data, float_data);
    ASSERT_OK;

    ASSERT_TRUE(string_data > float_data,
                "String should be bigger than Float");
    ASSERT_OK;

    ASSERT_TRUE(string_data >= float_data,
                "String should be bigger or equal than Float");
    ASSERT_OK;

    ASSERT_FALSE(string_data < float_data,
                 "String should be bigger than Float");
    ASSERT_OK;

    ASSERT_FALSE(string_data <= float_data,
                 "String should be bigger or equal than Float");
    ASSERT_OK;

    ASSERT_TRUE(string_data += float_data, "String and Float should init");
    ASSERT_TRUE(wcscmp((const wchar_t *) string_data.getAddress(),
                       L"32313131.000000") == 0,
                "String should be 32313131.000000! (%ls)",
                (const wchar_t *) string_data.getAddress());
    ASSERT_OK;

    ASSERT_VIRTUAL_MEMORY(*vm,
                          sizeof(L"32313131.000000") +
                          sizeof(L"32") +
                          sizeof(L"31") +
                                  DataType::SIZE[OBJECT_TYPE_INT] +
                                  DataType::SIZE[OBJECT_TYPE_FLOAT]);

    ASSERT_FALSE(string_data -= float_data, "String and Float shouldn't substract");
    ASSERT_TRUE(wcscmp((const wchar_t *) string_data.getAddress(), L"32313131.000000") == 0,
                "String should be 32313131.000000!");
    ASSERT_ERROR(ERROR_PRIMITIVE_DATA_SUBTRACTING_STRING);
    ERROR_LOG_CLEAR;

    ASSERT_FALSE(string_data *= float_data, "String and Float shouldn't multiply");
    ASSERT_TRUE(wcscmp((const wchar_t *) string_data.getAddress(), L"32313131.000000") == 0,
                "String should be 32313131.000000!");
    ASSERT_ERROR(ERROR_PRIMITIVE_DATA_MULTIPLYING_STRING);
    ERROR_LOG_CLEAR;

    ASSERT_FALSE(string_data /= float_data, "String and Float shouldn't divide");
    ASSERT_TRUE(wcscmp((const wchar_t *) string_data.getAddress(),
                       L"32313131.000000") == 0,
                "String should be 32313131.000000!");
    ASSERT_ERROR(ERROR_PRIMITIVE_DATA_DIVIDING_STRING);
    ERROR_LOG_CLEAR;

    ASSERT_FALSE(string_data %= float_data, "String and Float shouldn't mod");
    ASSERT_TRUE(wcscmp((const wchar_t *) string_data.getAddress(),
                       L"32313131.000000") == 0,
                "String should be 32313131.000000!");
    ASSERT_ERROR(ERROR_PRIMITIVE_DATA_INVALID_MODULUS);
    ERROR_LOG_CLEAR;

    ASSERT_NOT_EQUALS(string_data, float_data);
    ASSERT_OK;

    ASSERT_NOT_EQUALS(string_data, int_data);
    ASSERT_TRUE(wcscmp((const wchar_t *) string_data.getAddress(), L"31") != 0,
                "String should be 31! (%ls)",
                (const wchar_t *) string_data.getAddress());
    ASSERT_OK;

    ASSERT_VIRTUAL_MEMORY(*vm,
                          sizeof(L"32313131.000000") + // still reserved
                          sizeof(L"32") +
                          sizeof(L"31") +
                                  DataType::SIZE[OBJECT_TYPE_INT] +
                                  DataType::SIZE[OBJECT_TYPE_FLOAT]);
}

/**
 * Test integer data.
 */
static void
primitive_data_test_int()
{
    int32_t num = 65;
    Int &int_data = *Int::create(&num);

    ASSERT_OK;

    ASSERT_TRUE(++int_data, "Int should be incremented!");
    ASSERT_OK;
    ASSERT_EQUALS(int_data.toInt(), 66);

    ASSERT_TRUE(--int_data, "Int should be incremented!");
    ASSERT_OK;
    ASSERT_EQUALS(int_data.toInt(), 65);

    printf("debug -> ");
    ASSERT_TRUE(int_data.print(), "Int should be printed!");
    printf("\n");
    ASSERT_OK;

    printf("debug -> ");
    ASSERT_TRUE(int_data.println(), "Int should be printed!");
    ASSERT_OK;

    ASSERT_TRUE(int_data.toChar() == L'A',
                "Int to char() -> %c instead of %c",
                int_data.toChar(),
                'A');
    ASSERT_OK;

    ASSERT_TRUE(int_data.toFloat() == 65.0,
                "Int to float() -> %f instead of %f",
                int_data.toFloat(),
                65.0);
    ASSERT_OK;

    String &str = int_data.toString();
    ASSERT_OK;
    ASSERT_TRUE(wcscmp((const wchar_t *) str.getAddress(), L"65") == 0,
                "string should be 65! (%ls)",
                (const wchar_t *) str.getAddress());

    int32_t num2 = 35;
    Int &int_data2 = *Int::create(&num2);

    ASSERT_FALSE(int_data == int_data2, "Int and int_data2 should not be equal");
    ASSERT_OK;

    ASSERT_TRUE(int_data != int_data2, "Int and int_data2 should not be equal");
    ASSERT_OK;

    ASSERT_TRUE(int_data > int_data2, "Int should be bigger than int_data2");
    ASSERT_OK;

    ASSERT_TRUE(int_data >= int_data2, "Int should be bigger or equal than int_data2");
    ASSERT_OK;

    ASSERT_FALSE(int_data < int_data2, "Int should be bigger than int_data2");
    ASSERT_OK;

    ASSERT_FALSE(int_data <= int_data2,
                 "Int should be bigger or equal than int_data2");
    ASSERT_OK;

    ASSERT_TRUE(int_data += int_data2, "Int and int_data2 should init");
    ASSERT_EQUALS(int_data.toInt(), 100);

    ASSERT_TRUE(int_data -= int_data2, "Int and int_data2 should substract");
    ASSERT_EQUALS(int_data.toInt(), 65);

    ASSERT_TRUE(int_data *= int_data2, "Int and int_data2 should multiply");
    ASSERT_EQUALS(int_data.toInt(), 2275);

    ASSERT_TRUE(int_data /= int_data2, "Int and int_data2 should divide");
    ASSERT_EQUALS(int_data.toInt(), 65);

    ASSERT_TRUE(int_data %= int_data2, "Int and int_data2 should mod");
    ASSERT_EQUALS(int_data.toInt(), 30);

    /*
     * Putting int_data2 to Value of zero.
     */
    int_data2.defaultValue();

    ASSERT_FALSE(int_data /= int_data2, "Int and int_data2 shouldn't divide");
    ASSERT_EQUALS(int_data.toInt(), 30);
    ASSERT_ERROR(ERROR_PRIMITIVE_DATA_DIVIDING_ZERO);
    ERROR_LOG_CLEAR;

    ASSERT_FALSE(int_data %= int_data2, "Int and int_data2 shouldn't mod");
    ASSERT_EQUALS(int_data.toInt(), 30);
    ASSERT_ERROR(ERROR_PRIMITIVE_DATA_MODULUS_ZERO);
    ERROR_LOG_CLEAR;

    /*
     * Putting Int to Value of 65.
     */
    *(int32_t *) int_data.getAddress() = 65;

    double float_num = 35.0;
    Primitive &float_data = *Float::create(&float_num);

    ASSERT_FALSE(int_data == float_data, "Int and Float should not be equal");
    ASSERT_OK;

    ASSERT_TRUE(int_data != float_data, "Int and Float should not be equal");
    ASSERT_OK;

    ASSERT_TRUE(int_data > float_data, "Int should be bigger than Float");
    ASSERT_OK;

    ASSERT_TRUE(int_data >= float_data, "Int should be bigger or equal than Float");
    ASSERT_OK;

    ASSERT_FALSE(int_data < float_data, "Int should be bigger than Float");
    ASSERT_OK;

    ASSERT_FALSE(int_data <= float_data, "Int should be bigger or equal than Float");
    ASSERT_OK;

    ASSERT_TRUE(int_data += float_data, "Int and Float should init");
    ASSERT_EQUALS(int_data.toInt(), 100);

    ASSERT_TRUE(int_data -= float_data, "Int and Float should substract");
    ASSERT_EQUALS(int_data.toInt(), 65);

    ASSERT_TRUE(int_data *= float_data, "Int and Float should multiply");
    ASSERT_EQUALS(int_data.toInt(), 2275);

    ASSERT_TRUE(int_data /= float_data, "Int and Float should divide");
    ASSERT_EQUALS(int_data.toInt(), 65);

    /*
     * setting Float to zero Value
     */
    *(double *) float_data.getAddress() = 0.0;

    ASSERT_FALSE(int_data /= float_data, "Int and Float shouldn't divide");
    ASSERT_EQUALS(int_data.toInt(), 65);
    ASSERT_ERROR(ERROR_PRIMITIVE_DATA_DIVIDING_ZERO);
    ERROR_LOG_CLEAR;

    ASSERT_FALSE(int_data %= float_data, "Int and Float shouldn't mod");
    ASSERT_EQUALS(int_data.toInt(), 65);
    ASSERT_ERROR(ERROR_PRIMITIVE_DATA_INVALID_MODULUS);
    ERROR_LOG_CLEAR;

    /* interaction with string */

    String &string_data = *String::create(L"35");

    ASSERT_FALSE(int_data == string_data, "Int and String should not be equal");
    ASSERT_OK;

    ASSERT_TRUE(int_data != string_data, "Int and String should not be equal");
    ASSERT_OK;

    ASSERT_TRUE(int_data > string_data, "Int should be bigger than String");
    ASSERT_OK;

    ASSERT_TRUE(int_data >= string_data, "Int should be bigger or equal than String");
    ASSERT_OK;

    ASSERT_FALSE(int_data < string_data, "Int should be bigger than String");
    ASSERT_OK;

    ASSERT_FALSE(int_data <= string_data, "Int should be bigger or equal than String");
    ASSERT_OK;

    ASSERT_FALSE(int_data += string_data, "Int and String shouldn't init");
    ASSERT_EQUALS(int_data.toInt(), 65);
    ASSERT_ERROR(ERROR_PRIMITIVE_DATA_ADDING_STRING);
    ERROR_LOG_CLEAR;

    ASSERT_FALSE(int_data -= string_data, "Int and String shouldn't substract");
    ASSERT_EQUALS(int_data.toInt(), 65);
    ASSERT_ERROR(ERROR_PRIMITIVE_DATA_SUBTRACTING_STRING);
    ERROR_LOG_CLEAR;

    ASSERT_FALSE(int_data *= string_data, "Int and String shouldn't multiply");
    ASSERT_EQUALS(int_data.toInt(), 65);
    ASSERT_ERROR(ERROR_PRIMITIVE_DATA_MULTIPLYING_STRING);
    ERROR_LOG_CLEAR;

    ASSERT_FALSE(int_data /= string_data, "Int and String shouldn't divide");
    ASSERT_EQUALS(int_data.toInt(), 65);
    ASSERT_ERROR(ERROR_PRIMITIVE_DATA_DIVIDING_STRING);
    ERROR_LOG_CLEAR;

    string_data = L"35 Grupa Zana rules!";
    ASSERT_OK;

    ASSERT_FALSE(int_data == string_data, "Int and String should not be equal");
    ASSERT_OK;

    ASSERT_TRUE(int_data != string_data, "Int and String should not be equal");
    ASSERT_OK;

    ASSERT_TRUE(int_data > string_data, "Int should be bigger than String");
    ASSERT_OK;

    ASSERT_TRUE(int_data >= string_data, "Int should be bigger or equal than String");
    ASSERT_OK;

    ASSERT_FALSE(int_data < string_data, "Int should be bigger than String");
    ASSERT_OK;

    ASSERT_FALSE(int_data <= string_data, "Int should be bigger or equal than String");
    ASSERT_OK;

    ASSERT_FALSE(int_data += string_data, "Int and String shouldn't init");
    ASSERT_EQUALS(int_data.toInt(), 65);
    ASSERT_ERROR(ERROR_PRIMITIVE_DATA_ADDING_STRING);
    ERROR_LOG_CLEAR;

    ASSERT_FALSE(int_data -= string_data, "Int and String shouldn't substract");
    ASSERT_EQUALS(int_data.toInt(), 65);
    ASSERT_ERROR(ERROR_PRIMITIVE_DATA_SUBTRACTING_STRING);
    ERROR_LOG_CLEAR;

    ASSERT_FALSE(int_data *= string_data, "Int and String shouldn't multiply");
    ASSERT_EQUALS(int_data.toInt(), 65);
    ASSERT_ERROR(ERROR_PRIMITIVE_DATA_MULTIPLYING_STRING);
    ERROR_LOG_CLEAR;

    ASSERT_FALSE(int_data /= string_data, "Int and String shouldn't divide");
    ASSERT_EQUALS(int_data.toInt(), 65);
    ASSERT_ERROR(ERROR_PRIMITIVE_DATA_DIVIDING_STRING);
    ERROR_LOG_CLEAR;

    string_data = L"0";

    ASSERT_FALSE(int_data /= string_data,
                 "Int and Float shouldn't divide");
    ASSERT_ERROR(ERROR_PRIMITIVE_DATA_DIVIDING_STRING);
    ASSERT_TRUE(int_data.toInt() == 65, "Int should be 65");
    ERROR_LOG_CLEAR;

    ASSERT_FALSE(int_data %= string_data, "Int and String shouldn't mod");
    ASSERT_ERROR(ERROR_PRIMITIVE_DATA_INVALID_MODULUS);
    ERROR_LOG_CLEAR;

    ASSERT_FALSE(int_data = string_data, "String shouldn't assign to Int");
    ASSERT_EQUALS(int_data.toInt(), 65);
    ASSERT_ERROR(ERROR_PRIMITIVE_DATA_ASSIGNING_STRING);
    ERROR_LOG_CLEAR;

    ASSERT_TRUE(int_data = float_data, "Float should assign to Int");
    ASSERT_EQUALS(int_data.toInt(), 0);
    ASSERT_OK;

    int_data.print();
}

/**
 * Test primitive data class.
 */
void primitive_data_test()
{
    vm = (VirtualMemory *) ORM::getFirst(OBJECT_TYPE_VIRTUAL_MEMORY);

    RUN_TEST_VM(primitive_data_test_int());
    RUN_TEST_VM(primitive_data_test_string_empty());
    RUN_TEST_VM(primitive_data_test_string());
    RUN_TEST_VM(primitive_data_test_float());
}
