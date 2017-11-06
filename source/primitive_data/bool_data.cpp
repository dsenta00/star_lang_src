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

#include <ORM/orm.h>
#include <error_log.h>
#include <memory.h>
#include <virtual_memory.h>
#include <iostream>
#include <primitive_data/string_data.h>
#include "primitive_data/bool_data.h"

/**
 * The constructor.
 *
 * @param id
 * @param value
 */
bool_data::bool_data(std::string id, const void *value) : primitive_data::primitive_data(
    std::move(id), DATA_TYPE_CHAR, value)
{
    if (!value)
    {
        this->default_value();
    }
}

/**
 * The constructor.
 *
 * @param id
 * @param data
 * @param is_reference
 */
bool_data::bool_data(std::string id, bool_data &data, bool is_reference) : primitive_data::primitive_data(
    std::move(id), data, is_reference)
{
}

/**
 * @param id
 * @param value
 * @return
 */
bool_data *
bool_data::create(std::string id, const void *value)
{
    return (bool_data *) orm::create((object *) new bool_data(std::move(id), value));
}

/**
 * @param id
 * @param data
 * @param is_reference
 * @return
 */
bool_data *
bool_data::create(std::string id, bool_data &data, bool is_reference)
{
    return (bool_data *) orm::create((object *) new bool_data(std::move(id), data, is_reference));
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
primitive_data &
bool_data::to_string()
{
    std::wstring string = this->get_string();

    return *string_data::create(this->id.append(" as string"), string.c_str());
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
bool_data::operator=(primitive_data &data)
{
    memory *mem = this->get_memory();

    if ((!mem) || (data.get_address() == 0))
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return false;
    }

    if (data.get_type() == DATA_TYPE_STRING)
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
bool_data::operator&=(primitive_data &data)
{
    memory *mem = this->get_memory();
    if ((!mem) || (data.get_address() == 0))
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return false;
    }

    if (data.get_type() == DATA_TYPE_STRING)
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
bool_data::operator|=(primitive_data &data)
{
    memory *mem = this->get_memory();
    if ((!mem) || (data.get_address() == 0))
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return false;
    }

    if (data.get_type() == DATA_TYPE_STRING)
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
bool_data::operator^=(primitive_data &data)
{
    memory *mem = this->get_memory();
    if ((!mem) || (data.get_address() == 0))
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return false;
    }

    if (data.get_type() == DATA_TYPE_STRING)
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
bool_data::operator+=(primitive_data &data)
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
bool_data::operator-=(primitive_data &data)
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
bool_data::operator*=(primitive_data &data)
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
bool_data::operator/=(primitive_data &data)
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
bool_data::operator%=(primitive_data &data)
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
bool_data::operator==(primitive_data &data)
{
    memory *mem = this->get_memory();
    if ((!mem) || (data.get_address() == 0))
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return false;
    }

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
bool_data::operator!=(primitive_data &data)
{
    memory *mem = this->get_memory();
    if ((!mem) || (data.get_address() == 0))
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return false;
    }

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
bool_data::operator>(primitive_data &data)
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
bool_data::operator<(primitive_data &data)
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
bool_data::operator>=(primitive_data &data)
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
bool_data::operator<=(primitive_data &data)
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
    memory *mem = this->get_memory();
    if (!mem)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return false;
    }

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
    std::wstring str;
    memory *mem = this->get_memory();

    if (!mem)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return str;
    }

    str = this->to_bool() ? L"true" : L"false";

    return str;
}
