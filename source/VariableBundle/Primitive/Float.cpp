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
#include <VariableBundle/Primitive/String.h>
#include <VariableBundle/Primitive/Float.h>
#include <iostream>

/**
 * The constructor.
 *
 * @param value
 */
Float::Float(const void *value) : Primitive(OBJECT_TYPE_FLOAT, value)
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
Float::Float(double data) : Primitive(OBJECT_TYPE_FLOAT, &data)
{
}

/**
 * The constructor.
 *
 * @param data
 */
Float::Float(Float &data) : Primitive(data)
{
}

/**
 * The constructor.
 *
 * @param value
 * @return
 */
Float *
Float::create(const void *value)
{
    return (Float *) ORM::create((Object *) new Float(value));
}

/**
 * The constructor.
 *
 * @param data
 * @return
 */
Float *
Float::create(Float &data)
{
    return (Float *) ORM::create((Object *) new Float(data));
}

/**
 * Convert data to bool and return.
 *
 * @return bool value
 */
bool
Float::toBool()
{
    bool value = false;
    Memory *mem = this->getMemory();

    if (!mem)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);

        return value;
    }

    value = (bool) mem->getElement<double>();

    return value;
}

/**
 * Convert data to char and return.
 *
 * @return char value if success, otherwise return null terminating character.
 */
wchar_t
Float::toChar()
{
    wchar_t value = 0;
    Memory *mem = this->getMemory();

    if (!mem)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return value;
    }

    value = (wchar_t) mem->getElement<double>();

    return value;
}

/**
 * Convert data to int and return.
 *
 * @return int value if success, otherwise return 0.
 */
int32_t
Float::toInt()
{
    int32_t value = 0;
    Memory *mem = this->getMemory();

    if (!mem)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return value;
    }

    value = (int32_t) mem->getElement<double>();

    return value;
}

/**
 * Convert data to float and return.
 *
 * @return float value if success, otherwise return 0.0.
 */
double
Float::toFloat()
{
    double value = 0.0;
    Memory *mem = this->getMemory();

    if (!mem)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return value;
    }

    value = mem->getElement<double>();

    return value;
}

/**
 * Convert data to string and return.
 *
 * @return the data string
 */
String &
Float::toString()
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
Float::defaultValue()
{
    Memory *mem = this->getMemory();

    if (!mem)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return false;
    }

    mem->getElement<double>() = 0.0;

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
Float::operator=(const void *data)
{
    Memory *mem = this->getMemory();

    if ((!mem) || (!data))
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return false;
    }

    mem->getElement<double>() = *(double *) data;

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
Float::operator=(Value &data)
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

    mem->getElement<double>() = data.toFloat();

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
Float::operator&=(Value &data)
{
    ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_LOGICAL_OPERATION_FLOAT);

    return false;
}

/**
 * Operator |=.
 *
 * @param data - the data.
 *
 * @return true if success, otherwise return false.
 */
bool
Float::operator|=(Value &data)
{
    ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_LOGICAL_OPERATION_FLOAT);

    return false;
}

/**
 * Operator ^=
 *
 * @param data - the data.
 *
 * @return true if success, otherwise return false.
 */
bool
Float::operator^=(Value &data)
{
    ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_LOGICAL_OPERATION_FLOAT);

    return false;
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
Float::operator+=(Value &data)
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

    mem->getElement<double>() += data.toFloat();

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
Float::operator-=(Value &data)
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

    mem->getElement<double>() -= data.toFloat();

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
Float::operator*=(Value &data)
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

    mem->getElement<double>() *= data.toFloat();

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
Float::operator/=(Value &data)
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

    if (data.toFloat() == 0.0)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_DIVIDING_ZERO);
        return false;
    }

    mem->getElement<double>() /= data.toFloat();

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
Float::operator%=(Value &data)
{
    ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_INVALID_MODULUS);

    return false;
}

/**
 * Operator ++. Increment value.
 *
 * @return true if success, otherwise return false.
 */
bool
Float::operator++()
{
    Memory *mem = this->getMemory();
    if (!mem)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return false;
    }

    mem->getElement<double>() += 1.0;

    return true;
}

/**
 * Operator --. Decrement value.
 *
 * @return true if success, otherwise return false.
 */
bool
Float::operator--()
{
    Memory *mem = this->getMemory();
    if (!mem)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return false;
    }

    mem->getElement<double>() -= 1.0;

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
Float::operator==(Value &data)
{
    return this->toFloat() == data.toFloat();
}

/**
 * Operator ==. Check values non equality.
 *
 * @param data - the data.
 *
 * @return true if not equal, otherwise return false.
 */
bool
Float::operator!=(Value &data)
{
    return this->toFloat() != data.toFloat();
}

/**
 * Operator >. Check if this value is bigger than another.
 *
 * @param data - the data.
 *
 * @return true if this value is bigger, otherwise return false.
 */
bool
Float::operator>(Value &data)
{
    return this->toFloat() > data.toFloat();
}

/**
 * Operator <. Check if this value is lesser than another.
 *
 * @param data - the data.
 *
 * @return true if this value is lesser, otherwise return false.
 */
bool
Float::operator<(Value &data)
{
    return this->toFloat() < data.toFloat();
}

/**
 * Operator >=. Check if this value is bigger or equal than another.
 *
 * @param data - the data.
 *
 * @return true if this value is bigger or equal, otherwise return false.
 */
bool
Float::operator>=(Value &data)
{
    return this->toFloat() >= data.toFloat();
}

/**
 * Operator <=. Check if this value is smaller than another.
 *
 * @param data - the data.
 *
 * @return true if this value is smaller, otherwise return false.
 */
bool
Float::operator<=(Value &data)
{
    return this->toFloat() <= data.toFloat();
}

/**
 * Print value on console.
 *
 * @return true if success, otherwise return false.
 */
bool
Float::print()
{
    std::cout << this->toFloat();

    return true;
}

/**
 * Print value on screen together with new line.
 *
 * @return true if success, otherwise return false.
 */
bool
Float::println()
{
    if (print())
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
Float::scan()
{
    Memory *mem = this->getMemory();
    if (!mem)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);

        return false;
    }

    std::cin >> mem->getElement<double>();

    return true;
}

/**
 * Get string.
 *
 * @return string.
 */
std::wstring
Float::getString()
{
    std::wstring str;

    str = std::to_wstring(this->toFloat());

    return str;
}

/**
 * @inherit
 */
eObjectType
Float::getObjectType()
{
    return OBJECT_TYPE_FLOAT;
}

/**
 * Parse float from string.
 *
 * @param str
 * @return
 */
double
Float::parse(std::wstring str)
{
    return std::stof(str);
}

/**
 * @param data
 * @return
 */
Float *
Float::create(double data)
{
    return Float::create(&data);
}

/**
 * @inherit
 */
bool
Float::isReference()
{
    return false;
}
