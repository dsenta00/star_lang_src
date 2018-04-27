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
#include <VariableBundle/Value.h>
#include <VariableBundle/Primitive/String.h>
#include <VariableBundle/Primitive/Char.h>
#include <iostream>
#include <cstring>

/**
 * The constructor.
 *
 * @param value
 */
Char::Char(const void *value) : Primitive::Primitive(OBJECT_TYPE_CHAR, value)
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
Char::Char(Char &data) : Primitive::Primitive(data)
{
}

/**
 *
 * @param value
 * @return
 */
Char *
Char::create(const void *value)
{
    return (Char *) ORM::create((Object *) new Char(value));
}

/**
 * @param id
 * @param data
 * @param is_reference
 * @return
 */
Char *
Char::create(Char &data)
{
    return (Char *) ORM::create((Object *) new Char(data));
}

/**
 * Convert data to bool and return.
 *
 * @return bool value
 */
bool
Char::toBool()
{
    bool value = false;
    Memory *mem = this->getMemory();

    if (!mem)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return value;
    }

    value = (bool) mem->getElement<wchar_t>();

    return value;
}

/**
 * Convert data to char and return.
 *
 * @return char value if success, otherwise return null terminating character.
 */
wchar_t
Char::toChar()
{
    wchar_t value = 0;
    Memory *mem = this->getMemory();

    if (!mem)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return value;
    }

    value = mem->getElement<wchar_t>();

    return value;
}

/**
 * Convert data to int and return.
 *
 * @return int value if success, otherwise return 0.
 */
int32_t
Char::toInt()
{
    int32_t value = 0;
    Memory *mem = this->getMemory();

    if (!mem)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return value;
    }

    value = (int32_t) mem->getElement<wchar_t>();

    return value;
}

/**
 * Convert data to float and return.
 *
 * @return float value if success, otherwise return 0.0.
 */
double
Char::toFloat()
{
    double value = 0.0;
    Memory *mem = this->getMemory();

    if (!mem)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return value;
    }

    value = (double) mem->getElement<wchar_t>();

    return value;
}

/**
 * Convert data to string and return.
 *
 * @return the data string
 */
String &
Char::toString()
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
Char::defaultValue()
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
Char::operator=(const void *data)
{
    Memory *mem = this->getMemory();

    if ((!mem) || (!data))
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return false;
    }

    mem->getElement<wchar_t>() = *(wchar_t *) data;

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
Char::operator=(Value &data)
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

    mem->getElement<wchar_t>() = data.toChar();

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
Char::operator&=(Value &data)
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

    mem->getElement<wchar_t>() &= data.toChar();

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
Char::operator|=(Value &data)
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

    mem->getElement<wchar_t>() |= data.toChar();

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
Char::operator^=(Value &data)
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

    mem->getElement<wchar_t>() ^= data.toChar();

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
Char::operator+=(Value &data)
{
    Memory *mem = this->getMemory();

    if (!mem)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return false;
    }

    if (data.getObjectType() == OBJECT_TYPE_STRING)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_ADDING_STRING);
        return false;
    }

    mem->getElement<wchar_t>() += data.toChar();

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
Char::operator-=(Value &data)
{
    Memory *mem = this->getMemory();
    if (!mem)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return false;
    }

    if (data.getObjectType() == OBJECT_TYPE_STRING)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_SUBTRACTING_STRING);
        return false;
    }

    mem->getElement<wchar_t>() -= data.toChar();

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
Char::operator*=(Value &data)
{
    Memory *mem = this->getMemory();
    if (!mem)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return false;
    }

    if (data.getObjectType() == OBJECT_TYPE_STRING)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_MULTIPLYING_STRING);
        return false;
    }

    mem->getElement<wchar_t>() *= data.toChar();

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
Char::operator/=(Value &data)
{
    Memory *mem = this->getMemory();
    if (!mem)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return false;
    }

    if (data.getObjectType() == OBJECT_TYPE_STRING)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_DIVIDING_STRING);
        return false;
    }

    if (data.toChar() == 0)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_DIVIDING_ZERO);
        return false;
    }

    mem->getElement<wchar_t>() /= data.toChar();

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
Char::operator%=(Value &data)
{
    Memory *mem = this->getMemory();
    if (!mem)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return false;
    }

    if ((data.getObjectType() == OBJECT_TYPE_FLOAT) ||
        (data.getObjectType() == OBJECT_TYPE_STRING) ||
        (data.getObjectType() == OBJECT_TYPE_BOOL))
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_INVALID_MODULUS);
        return false;
    }

    if (data.toChar() == 0)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_MODULUS_ZERO);
        return false;
    }

    mem->getElement<wchar_t>() %= data.toChar();

    return true;
}

/**
 * Operator ++. Increment value.
 *
 * @return true if success, otherwise return false.
 */
bool
Char::operator++()
{
    Memory *mem = this->getMemory();
    if (!mem)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return false;
    }

    mem->getElement<wchar_t>()++;

    return true;
}

/**
 * Operator --. Decrement value.
 *
 * @return true if success, otherwise return false.
 */
bool
Char::operator--()
{
    Memory *mem = this->getMemory();
    if (!mem)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return false;
    }

    mem->getElement<wchar_t>()--;

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
Char::operator==(Value &data)
{
    return this->toChar() == data.toChar();
}

/**
 * Operator ==. Check values non equality.
 *
 * @param data - the data.
 *
 * @return true if not equal, otherwise return false.
 */
bool
Char::operator!=(Value &data)
{
    return this->toChar() != data.toChar();
}

/**
 * Operator >. Check if this value is bigger than another.
 *
 * @param data - the data.
 *
 * @return true if this value is bigger, otherwise return false.
 */
bool
Char::operator>(Value &data)
{
    return this->toChar() > data.toChar();
}

/**
 * Operator <. Check if this value is lesser than another.
 *
 * @param data - the data.
 *
 * @return true if this value is lesser, otherwise return false.
 */
bool
Char::operator<(Value &data)
{
    return this->toChar() < data.toChar();
}

/**
 * Operator >=. Check if this value is bigger or equal than another.
 *
 * @param data - the data.
 *
 * @return true if this value is bigger or equal, otherwise return false.
 */
bool
Char::operator>=(Value &data)
{
    return this->toChar() >= data.toChar();
}

/**
 * Operator <=. Check if this value is smaller than another.
 *
 * @param data - the data.
 *
 * @return true if this value is smaller, otherwise return false.
 */
bool
Char::operator<=(Value &data)
{
    return this->toChar() <= data.toChar();
}

/**
 * Print value on console.
 *
 * @return true if success, otherwise return false.
 */
bool
Char::print()
{
    std::wcout << toChar();

    return true;
}

/**
 * Print value on screen together with new line.
 *
 * @return true if success, otherwise return false.
 */
bool
Char::println()
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
Char::scan()
{
    Memory *mem = this->getMemory();
    if (!mem)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return false;
    }

    std::wcin >> mem->getElement<wchar_t>();

    return true;
}

/**
 * Get string.
 *
 * @return string.
 */
std::wstring
Char::getString()
{
    std::wstring str;

    str = this->toChar();

    return str;
}

/**
 * @inherit
 */
eObjectType
Char::getObjectType()
{
    return OBJECT_TYPE_CHAR;
}

/**
 * @inherit
 */
bool
Char::isReference()
{
    return false;
}
