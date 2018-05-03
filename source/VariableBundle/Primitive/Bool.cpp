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

#include <ORM/ORM.h>
#include <ErrorBundle/ErrorLog.h>
#include <MemoryBundle/Memory.h>
#include <MemoryBundle/VirtualMemory.h>
#include <VariableBundle/Primitive/Bool.h>
#include <VariableBundle/Primitive/String.h>
#include <VariableBundle/Value.h>
#include <iostream>
#include <cstring>
#include <algorithm>

/**
 * The constructor.
 *
 * @param value
 */
Bool::Bool(const void *value) : Primitive(OBJECT_TYPE_BOOL, value)
{
    if (!value)
    {
        this->defaultValue();
    }
}

/**
 * The constructor.
 *
 * @param data
 */
Bool::Bool(Bool &data) : Primitive(data)
{
}

/**
 * The constructor.
 *
 * @param value
 */
Bool::Bool(bool value) : Primitive(OBJECT_TYPE_BOOL, &value)
{
}

/**
 * @param value
 * @return
 */
Bool *
Bool::create(const void *value)
{
    return (Bool *) ORM::create((Object *) new Bool(value));
}

/**
 *
 * @param value
 * @return
 */
Bool *
Bool::create(bool value)
{
    return Bool::create(&value);
}

/**
 *
 * @param data
 * @return
 */
Bool *
Bool::create(Bool &data)
{
    return (Bool *) ORM::create((Object *) new Bool(data));
}

/**
 * Convert data to bool and return.
 *
 * @return bool value
 */
bool
Bool::toBool()
{
    bool value = false;
    Memory *mem = this->getMemory();

    if (!mem)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return value;
    }

    value = mem->getElement<bool>();

    return value;
}

/**
 * Convert data to char and return.
 *
 * @return char value if success, otherwise return null terminating character.
 */
wchar_t
Bool::toChar()
{
    wchar_t value = 0;
    Memory *mem = this->getMemory();

    if (!mem)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return value;
    }

    value = (wchar_t) mem->getElement<bool>();

    return value;
}

/**
 * Convert data to int and return.
 *
 * @return int value if success, otherwise return 0.
 */
int32_t
Bool::toInt()
{
    int32_t value = 0;
    Memory *mem = this->getMemory();

    if (!mem)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return value;
    }

    value = (int32_t) mem->getElement<bool>();

    return value;
}

/**
 * Convert data to float and return.
 *
 * @return float value if success, otherwise return 0.0.
 */
double
Bool::toFloat()
{
    double value = 0.0;
    Memory *mem = this->getMemory();

    if (!mem)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return value;
    }

    value = (double) mem->getElement<bool>();

    return value;
}

/**
 * Convert data to string and return.
 *
 * @return the data string
 */
String &
Bool::toString()
{
    std::wstring string = this->getString();

    return *String::create(string.c_str());
}

/**
 * Set data to default value.
 *
 * @return true if success, otherwise return false.
 */
bool
Bool::defaultValue()
{
    Memory *mem = this->getMemory();

    if (!mem)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return false;
    }

    memset(mem->getPointer<void *>(), 0, mem->getSize());

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
Bool::operator=(const void *data)
{
    Memory *mem = this->getMemory();

    if ((!mem) || (!data))
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return false;
    }

    mem->getElement<bool>() = *(bool *) data;

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
Bool::operator=(Value & data)
{
    Memory *mem = this->getMemory();

    if (!mem)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return false;
    }

    if (data.getObjectType() == OBJECT_TYPE_STRING)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_ASSIGNING_STRING);
        return false;
    }

    mem->getElement<bool>() = data.toBool();

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
Bool::operator&=(Value & data)
{
    Memory *mem = this->getMemory();
    if (!mem)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return false;
    }

    if (data.getObjectType() == OBJECT_TYPE_STRING)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_LOGICAL_OPERATION_STRING);
        return false;
    }

    mem->getElement<bool>() &= data.toBool();

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
Bool::operator|=(Value & data)
{
    Memory *mem = this->getMemory();
    if (!mem)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return false;
    }

    if (data.getObjectType() == OBJECT_TYPE_STRING)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_LOGICAL_OPERATION_STRING);
        return false;
    }

    mem->getElement<bool>() |= data.toBool();

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
Bool::operator^=(Value & data)
{
    Memory *mem = this->getMemory();
    if (!mem)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return false;
    }

    if (data.getObjectType() == OBJECT_TYPE_STRING)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_LOGICAL_OPERATION_STRING);
        return false;
    }

    mem->getElement<bool>() ^= data.toBool();

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
Bool::operator+=(Value & data)
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
Bool::operator-=(Value & data)
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
Bool::operator*=(Value & data)
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
Bool::operator/=(Value & data)
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
Bool::operator%=(Value & data)
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
Bool::operator++()
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
Bool::operator--()
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
Bool::operator==(Value & data)
{
    return this->toBool() == data.toBool();
}

/**
 * Operator ==. Check values non equality.
 *
 * @param data - the data.
 *
 * @return true if not equal, otherwise return false.
 */
bool
Bool::operator!=(Value & data)
{
    return this->toBool() != data.toBool();
}

/**
 * Operator >. Check if this value is bigger than another.
 *
 * @param data - the data.
 *
 * @return true if this value is bigger, otherwise return false.
 */
bool
Bool::operator>(Value & data)
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
Bool::operator<(Value & data)
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
Bool::operator>=(Value & data)
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
Bool::operator<=(Value & data)
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
Bool::print()
{
    std::cout << (this->toBool() ? "true" : "false");

    return true;
}

/**
 * Print value on screen together with new line.
 *
 * @return true if success, otherwise return false.
 */
bool
Bool::println()
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
Bool::scan()
{
    Memory *mem = this->getMemory();
    if (!mem)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return false;
    }

    std::cin >> mem->getElement<bool>();

    return true;
}

/**
 * Get string.
 *
 * @return string.
 */
std::wstring
Bool::getString()
{
    std::wstring str = this->toBool() ? L"true" : L"false";

    return str;
}

/**
 * @inherit
 */
eObjectType
Bool::getObjectType()
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
Bool::parse(std::wstring str)
{
    std::for_each(str.begin(), str.end(), [&](wchar_t &c) { c = (wchar_t)std::tolower(c); });

    return str == L"true";
}

/**
 * @inherit
 */
bool
Bool::isReference()
{
    return false;
}
