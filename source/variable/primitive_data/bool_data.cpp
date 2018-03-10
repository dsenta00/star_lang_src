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

#include <ORM/orm.h>
#include <error_handler/error_log.h>
#include <memory_handler/memory.h>
#include <memory_handler/virtual_memory.h>
#include <variable/primitive_data/bool_data.h>
#include <variable/primitive_data/string_data.h>
#include <iostream>

/**
 * The constructor.
 *
 * @param value
 */
bool_data::bool_data(const void *value) : primitive_data(OBJECT_TYPE_BOOL, value)
{
    if (!value)
    {
        this->default_value();
    }
}

/**
 * The constructor.
 *
 * @param data
 */
bool_data::bool_data(bool_data &data) : primitive_data(data)
{
}

/**
 * The constructor.
 *
 * @param value
 */
bool_data::
bool_data(bool value) : primitive_data(OBJECT_TYPE_BOOL, &value)
{
}

/**
 * @param value
 * @return
 */
bool_data *
bool_data::create(const void *value)
{
    return (bool_data *) orm::create((object *) new bool_data(value));
}

/**
 *
 * @param value
 * @return
 */
bool_data *
bool_data::create(bool value)
{
    return bool_data::create(&value);
}

/**
 *
 * @param data
 * @return
 */
bool_data *
bool_data::create(bool_data &data)
{
    return (bool_data *) orm::create((object *) new bool_data(data));
}

/**
 * Convert data to bool and return.
 *
 * @return bool value
 */
bool
bool_data::to_bool()
{
    bool value = false;
    memory *mem = this->get_memory();

    if (!mem)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return value;
    }

    value = mem->get_element<bool>();

    return value;
}

/**
 * Convert data to char and return.
 *
 * @return char value if success, otherwise return null terminating character.
 */
wchar_t
bool_data::to_char()
{
    wchar_t value = 0;
    memory *mem = this->get_memory();

    if (!mem)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return value;
    }

    value = (wchar_t) mem->get_element<bool>();

    return value;
}

/**
 * Convert data to int and return.
 *
 * @return int value if success, otherwise return 0.
 */
int32_t
bool_data::to_int()
{
    int32_t value = 0;
    memory *mem = this->get_memory();

    if (!mem)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return value;
    }

    value = (int32_t) mem->get_element<bool>();

    return value;
}

/**
 * Convert data to float and return.
 *
 * @return float value if success, otherwise return 0.0.
 */
double
bool_data::to_float()
{
    double value = 0.0;
    memory *mem = this->get_memory();

    if (!mem)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return value;
    }

    value = (double) mem->get_element<bool>();

    return value;
}

/**
 * Convert data to string and return.
 *
 * @return the data string
 */
string_data &
bool_data::to_string()
{
    std::wstring string = this->get_string();

    return *string_data::create(string.c_str());
}

/**
 * Set data to default value.
 *
 * @return true if success, otherwise return false.
 */
bool
bool_data::default_value()
{
    memory *mem = this->get_memory();

    if (!mem)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return false;
    }

    memset(mem->get_pointer<void *>(), 0, mem->get_size());

    return true;
}

/**
 * Operator =. Assign value from memory address.
 *
 * @param data - the data memory address.
 *
 * @return true if success, otherwise return false.
 */
bool
bool_data::operator=(const void *data)
{
    memory *mem = this->get_memory();

    if ((!mem) || (!data))
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return false;
    }

    mem->get_element<bool>() = *(bool *) data;

    return true;
}

/**
 * Operator =. Assign value from another data.
 *
 * @param data - the data.
 *
 * @return true if success, otherwise return false.
 */
bool
bool_data::operator=(value &data)
{
    memory *mem = this->get_memory();

    if (!mem)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return false;
    }

    if (data.get_object_type() == OBJECT_TYPE_STRING)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_ASSIGNING_STRING);
        return false;
    }

    mem->get_element<bool>() = data.to_bool();

    return true;
}

/**
 * Operator &=.
 *
 * @param data - the data.
 *
 * @return true if success, otherwise return false.
 */
bool
bool_data::operator&=(value &data)
{
    memory *mem = this->get_memory();
    if (!mem)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return false;
    }

    if (data.get_object_type() == OBJECT_TYPE_STRING)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_LOGICAL_OPERATION_STRING);
        return false;
    }

    mem->get_element<bool>() &= data.to_bool();

    return true;
}

/**
 * Operator |=.
 *
 * @param data - the data.
 *
 * @return true if success, otherwise return false.
 */
bool
bool_data::operator|=(value &data)
{
    memory *mem = this->get_memory();
    if (!mem)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return false;
    }

    if (data.get_object_type() == OBJECT_TYPE_STRING)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_LOGICAL_OPERATION_STRING);
        return false;
    }

    mem->get_element<bool>() |= data.to_bool();

    return true;
}

/**
 * Operator ^=
 *
 * @param data - the data.
 *
 * @return true if success, otherwise return false.
 */
bool
bool_data::operator^=(value &data)
{
    memory *mem = this->get_memory();
    if (!mem)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return false;
    }

    if (data.get_object_type() == OBJECT_TYPE_STRING)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_LOGICAL_OPERATION_STRING);
        return false;
    }

    mem->get_element<bool>() ^= data.to_bool();

    return true;
}

/**
 * Operator +=. Do operation add if primitive data type.
 * On string data type append data.
 *
 * @param data - the data.
 *
 * @return true if success, otherwise return false.
 */
bool
bool_data::operator+=(value &data)
{
    ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_BOOL_ARITHMETIC);

    return false;
}

/**
 * Operator -=. Do subtract operation.
 *
 * @param data - the data.
 *
 * @return true if success, otherwise return false.
 */
bool
bool_data::operator-=(value &data)
{
    ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_BOOL_ARITHMETIC);

    return false;
}

/**
 * Operator *=. Multiply values.
 *
 * @param data - the data.
 *
 * @return true if success, otherwise return false.
 */
bool
bool_data::operator*=(value &data)
{
    ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_BOOL_ARITHMETIC);

    return false;
}

/**
 * Operator /=. Divide values.
 *
 * @param data - the data.
 *
 * @return true if success, otherwise return false.
 */
bool
bool_data::operator/=(value &data)
{
    ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_BOOL_ARITHMETIC);

    return false;
}

/**
 * Operator %=. Do modulus between values.
 *
 * @param data - the data.
 *
 * @return true if success, otherwise return false.
 */
bool
bool_data::operator%=(value &data)
{
    ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_BOOL_ARITHMETIC);

    return false;
}

/**
 * Operator ++. Increment value.
 *
 * @return true if success, otherwise return false.
 */
bool
bool_data::operator++()
{
    ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_BOOL_ARITHMETIC);

    return false;
}

/**
 * Operator --. Decrement value.
 *
 * @return true if success, otherwise return false.
 */
bool
bool_data::operator--()
{
    ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_BOOL_ARITHMETIC);

    return false;
}

/**
 * Operator ==. Check values equality.
 *
 * @param data - the data.
 *
 * @return true if equal, otherwise return false.
 */
bool
bool_data::operator==(value &data)
{
    return this->to_bool() == data.to_bool();
}

/**
 * Operator ==. Check values non equality.
 *
 * @param data - the data.
 *
 * @return true if not equal, otherwise return false.
 */
bool
bool_data::operator!=(value &data)
{
    return this->to_bool() != data.to_bool();
}

/**
 * Operator >. Check if this value is bigger than another.
 *
 * @param data - the data.
 *
 * @return true if this value is bigger, otherwise return false.
 */
bool
bool_data::operator>(value &data)
{
    ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_BOOL_ARITHMETIC);

    return false;
}

/**
 * Operator <. Check if this value is lesser than another.
 *
 * @param data - the data.
 *
 * @return true if this value is lesser, otherwise return false.
 */
bool
bool_data::operator<(value &data)
{
    ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_BOOL_ARITHMETIC);

    return false;
}

/**
 * Operator >=. Check if this value is bigger or equal than another.
 *
 * @param data - the data.
 *
 * @return true if this value is bigger or equal, otherwise return false.
 */
bool
bool_data::operator>=(value &data)
{
    ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_BOOL_ARITHMETIC);

    return false;
}

/**
 * Operator <=. Check if this value is smaller than another.
 *
 * @param data - the data.
 *
 * @return true if this value is smaller, otherwise return false.
 */
bool
bool_data::operator<=(value &data)
{
    ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_BOOL_ARITHMETIC);

    return false;
}

/**
 * Print value on console.
 *
 * @return true if success, otherwise return false.
 */
bool
bool_data::print()
{
    std::cout << (this->to_bool() ? "true" : "false");

    return true;
}

/**
 * Print value on screen together with new line.
 *
 * @return true if success, otherwise return false.
 */
bool
bool_data::println()
{
    if (this->print())
    {
        std::cout << std::endl;
        return true;
    }

    return false;
}

/**
 * Insert value from console terminal into data.
 *
 * @return true if success, otherwise return false.
 */
bool
bool_data::scan()
{
    memory *mem = this->get_memory();
    if (!mem)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return false;
    }

    std::cin >> mem->get_element<bool>();

    return true;
}

/**
 * Get string.
 *
 * @return string.
 */
std::wstring
bool_data::get_string()
{
    std::wstring str = this->to_bool() ? L"true" : L"false";

    return str;
}

/**
 * @inherit
 */
object_type
bool_data::get_object_type()
{
    return OBJECT_TYPE_BOOL;
}

/**
 * Parse boolean from string.
 *
 * @param str
 * @return
 */
bool
bool_data::parse(std::wstring str)
{
    std::for_each(str.begin(), str.end(), [&](wchar_t &c) { c = std::tolower(c); });

    return str == L"true";
}

/**
 * @inherit
 */
bool
bool_data::is_reference()
{
    return false;
}
