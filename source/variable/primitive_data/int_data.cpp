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
#include <error_handler/error_log.h>
#include <memory_handler/memory.h>
#include <memory_handler/virtual_memory.h>
#include <variable/primitive_data/int_data.h>
#include <variable/primitive_data/string_data.h>
#include <iostream>

/**
 * The constructor.
 *
 * @param id
 * @param value
 */
int_data::int_data(std::string id, const void *value) : primitive_data::primitive_data(
    std::move(id), OBJECT_TYPE_INT, value
)
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
int_data::int_data(std::string id, int_data &data, bool is_reference) : primitive_data::primitive_data(
    std::move(id), data, is_reference
)
{
}

/**
 * @param id
 * @param type
 * @param value
 * @return
 */
int_data *
int_data::create(std::string id, const void *value)
{
    return (int_data *) orm::create((object *) new int_data(std::move(id), value));
}

/**
 * @param id
 * @param data
 * @param is_reference
 * @return
 */
int_data *
int_data::create(std::string id, int_data &data, bool is_reference)
{
    return (int_data *) orm::create((object *) new int_data(std::move(id), data, is_reference));
}

/**
 * Convert data to bool and return.
 *
 * @return bool value
 */
bool
int_data::to_bool()
{
    bool value = false;
    memory *mem = this->get_memory();

    if (!mem)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return value;
    }

    value = (bool) mem->get_element<int32_t>();
    return value;
}

/**
 * Convert data to char and return.
 *
 * @return char value if success, otherwise return null terminating character.
 */
wchar_t
int_data::to_char()
{
    wchar_t value = 0;
    memory *mem = this->get_memory();

    if (!mem)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return value;
    }

    value = (wchar_t) mem->get_element<int32_t>();

    return value;
}

/**
 * Convert data to int and return.
 *
 * @return int value if success, otherwise return 0.
 */
int32_t
int_data::to_int()
{
    int32_t value = 0;
    memory *mem = this->get_memory();

    if (!mem)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return value;
    }

    value = mem->get_element<int32_t>();

    return value;
}

/**
 * Convert data to float and return.
 *
 * @return float value if success, otherwise return 0.0.
 */
double
int_data::to_float()
{
    double value = 0.0;
    memory *mem = this->get_memory();

    if (!mem)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return value;
    }

    value = (double) mem->get_element<int32_t>();

    return value;
}

/**
 * Convert data to string and return.
 *
 * @return the data string
 */
string_data &
int_data::to_string()
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
int_data::default_value()
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
int_data::operator=(const void *data)
{
    memory *mem = this->get_memory();

    if ((!mem) || (!data))
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return false;
    }

    mem->get_element<int32_t>() = *(int32_t *) data;

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
int_data::operator=(var &data)
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

    mem->get_element<int32_t>() = data.to_int();

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
int_data::operator&=(var &data)
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

    mem->get_element<int32_t>() &= data.to_int();
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
int_data::operator|=(var &data)
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

    mem->get_element<int32_t>() |= data.to_int();

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
int_data::operator^=(var &data)
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

    mem->get_element<int32_t>() ^= data.to_int();

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
int_data::operator+=(var &data)
{
    memory *mem = this->get_memory();

    if (!mem)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return false;
    }

    if (data.get_object_type() == OBJECT_TYPE_STRING)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_ADDING_STRING);
        return false;
    }

    mem->get_element<int32_t>() += data.to_int();

    return true;
}

/**
 * Operator -=. Do subtract operation.
 *
 * @param data - the data.
 *
 * @return true if success, otherwise return false.
 */
bool
int_data::operator-=(var &data)
{
    memory *mem = this->get_memory();
    if (!mem)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return false;
    }

    if (data.get_object_type() == OBJECT_TYPE_STRING)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_SUBTRACTING_STRING);
        return false;
    }

    mem->get_element<int32_t>() -= data.to_int();

    return true;
}

/**
 * Operator *=. Multiply values.
 *
 * @param data - the data.
 *
 * @return true if success, otherwise return false.
 */
bool
int_data::operator*=(var &data)
{
    memory *mem = this->get_memory();
    if (!mem)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return false;
    }

    if (data.get_object_type() == OBJECT_TYPE_STRING)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_MULTIPLYING_STRING);
        return false;
    }

    mem->get_element<int32_t>() *= data.to_int();

    return true;
}

/**
 * Operator /=. Divide values.
 *
 * @param data - the data.
 *
 * @return true if success, otherwise return false.
 */
bool
int_data::operator/=(var &data)
{
    memory *mem = this->get_memory();
    if (!mem)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return false;
    }

    if (data.get_object_type() == OBJECT_TYPE_STRING)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_DIVIDING_STRING);
        return false;
    }

    if (data.to_int() == 0)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_DIVIDING_ZERO);
        return false;
    }

    mem->get_element<int32_t>() /= data.to_int();

    return true;
}

/**
 * Operator %=. Do modulus between values.
 *
 * @param data - the data.
 *
 * @return true if success, otherwise return false.
 */
bool
int_data::operator%=(var &data)
{
    memory *mem = this->get_memory();
    if (!mem)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return false;
    }

    if ((data.get_object_type() == OBJECT_TYPE_FLOAT) ||
        (data.get_object_type() == OBJECT_TYPE_STRING) ||
        (data.get_object_type() == OBJECT_TYPE_BOOL))
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_INVALID_MODULUS);
        return false;
    }

    if (data.to_int() == 0)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_MODULUS_ZERO);
        return false;
    }

    mem->get_element<int32_t>() %= data.to_int();

    return true;
}

/**
 * Operator ++. Increment value.
 *
 * @return true if success, otherwise return false.
 */
bool
int_data::operator++()
{
    memory *mem = this->get_memory();
    if (!mem)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return false;
    }

    mem->get_element<int32_t>()++;

    return true;
}

/**
 * Operator --. Decrement value.
 *
 * @return true if success, otherwise return false.
 */
bool
int_data::operator--()
{
    memory *mem = this->get_memory();
    if (!mem)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return false;
    }

    mem->get_element<int32_t>()--;

    return true;
}

/**
 * Operator ==. Check values equality.
 *
 * @param data - the data.
 *
 * @return true if equal, otherwise return false.
 */
bool
int_data::operator==(var &data)
{
    return to_int() == data.to_int();
}

/**
 * Operator ==. Check values non equality.
 *
 * @param data - the data.
 *
 * @return true if not equal, otherwise return false.
 */
bool
int_data::operator!=(var &data)
{
    return to_int() != data.to_int();
}

/**
 * Operator >. Check if this value is bigger than another.
 *
 * @param data - the data.
 *
 * @return true if this value is bigger, otherwise return false.
 */
bool
int_data::operator>(var &data)
{
    return to_int() > data.to_int();
}

/**
 * Operator <. Check if this value is lesser than another.
 *
 * @param data - the data.
 *
 * @return true if this value is lesser, otherwise return false.
 */
bool
int_data::operator<(var &data)
{
    return to_int() < data.to_int();
}

/**
 * Operator >=. Check if this value is bigger or equal than another.
 *
 * @param data - the data.
 *
 * @return true if this value is bigger or equal, otherwise return false.
 */
bool
int_data::operator>=(var &data)
{
    return to_int() >= data.to_int();
}

/**
 * Operator <=. Check if this value is smaller than another.
 *
 * @param data - the data.
 *
 * @return true if this value is smaller, otherwise return false.
 */
bool
int_data::operator<=(var &data)
{
    return this->to_int() <= data.to_int();
}

/**
 * Print value on console.
 *
 * @return true if success, otherwise return false.
 */
bool
int_data::print()
{
    std::cout << this->to_int();

    return true;
}

/**
 * Print value on screen together with new line.
 *
 * @return true if success, otherwise return false.
 */
bool int_data::println()
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
int_data::scan()
{
    memory *mem = this->get_memory();
    if (!mem)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return false;
    }

    std::cin >> mem->get_element<int32_t>();

    return true;
}

/**
 * Get string.
 *
 * @return string.
 */
std::wstring
int_data::get_string()
{
    std::wstring str;

    str = std::to_wstring(this->to_int());

    return str;
}

/**
 * @inherit
 */
object_type
int_data::get_object_type()
{
    return OBJECT_TYPE_INT;
}
