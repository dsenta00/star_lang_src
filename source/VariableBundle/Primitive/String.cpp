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

#include <ORM/ORM.h>
#include <ErrorBundle/ErrorLog.h>
#include <MemoryBundle/Memory.h>
#include <MemoryBundle/VirtualMemory.h>
#include <VariableBundle/Primitive/String.h>
#include <iostream>
#include <utility>
#include <cstring>

/**
 * The constructor.
 *
 * @param value
 */
String::String(const void *value) : Primitive::Primitive(OBJECT_TYPE_STRING, value)
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
String::String(String &data) : Primitive::Primitive(data)
{
}

/**
 *
 * @param value
 * @return
 */
String *
String::create(const void *value)
{
    return (String *) ORM::create((Object *) new String(value));
}

/**
 *
 * @param data
 * @return
 */
String *
String::create(String &data)
{
    return (String *) ORM::create((Object *) new String(data));
}

/**
 * Convert data to bool and return.
 *
 * @return bool value
 */
bool
String::toBool()
{
    bool value = false;
    Memory *mem = this->getMemory();

    if (!mem)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return value;
    }

    value = !this->getString().empty();

    return value;
}

/**
 * Convert data to char and return.
 *
 * @return char value if success, otherwise return null terminating character.
 */
wchar_t
String::toChar()
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
String::toInt()
{
    int32_t value = 0;
    Memory *mem = this->getMemory();

    if (!mem)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return value;
    }

    swscanf(mem->getPointer<const wchar_t *>(),
            DataType::FORMAT[OBJECT_TYPE_INT],
            &value);

    return value;
}

/**
 * Convert data to float and return.
 *
 * @return float value if success, otherwise return 0.0.
 */
double
String::toFloat()
{
    double value = 0.0;
    Memory *mem = this->getMemory();

    if (!mem)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return value;
    }

    swscanf(mem->getPointer<const wchar_t *>(),
            DataType::FORMAT[OBJECT_TYPE_FLOAT],
            &value);

    return value;
}

/**
 * Convert data to string and return.
 *
 * @return the data string
 */
String &
String::toString()
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
String::defaultValue()
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
String::operator=(const void *data)
{
    Memory *mem = this->getMemory();

    if ((!mem) || (!data))
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return false;
    }

    const auto *strTemp = (const wchar_t *) data;
    auto strSize = static_cast<uint32_t>((wcslen(strTemp) + 1) * sizeof(wchar_t));

    if (mem->getSize() < strSize)
    {
        Memory *newMem = this->vm->alloc(strSize);

        Object::masterRelationshipRemoveObject("primitive_data_memory", (Object *) mem);
        Object::masterRelationshipAddObject("primitive_data_memory", (Object *) newMem);

        this->vm->free(mem);
        mem = newMem;
    }

    wcsncpy(mem->getPointer<wchar_t *>(), strTemp, strSize);

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
String::operator=(Value &data)
{
    Memory *mem = this->getMemory();

    if (!mem)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return false;
    }

    mem->getElement<wchar_t>() = 0;
    return (*this) += (data);
}

/**
 * Operator &=.
 *
 * @param data - the data.
 *
 * @return true if success, otherwise return false.
 */
bool
String::operator&=(Value &data)
{
    ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_LOGICAL_OPERATION_STRING);

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
String::operator|=(Value &data)
{
    ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_LOGICAL_OPERATION_STRING);

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
String::operator^=(Value &data)
{
    ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_LOGICAL_OPERATION_STRING);

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
String::operator+=(Value &data)
{
    Memory *mem = this->getMemory();

    if (!mem)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return false;
    }

    std::wstring string = data.getString();
    auto requestSize = static_cast<uint32_t>(wcslen(mem->getPointer<const wchar_t *>()) * sizeof(wchar_t) +
                                              (wcslen(string.c_str()) + 1) * sizeof(wchar_t));

    if (mem->getSize() < requestSize)
    {
        Memory *newMem = vm->realloc(mem, requestSize);

        if (newMem->getSize() != requestSize)
        {
            /* Something really bad happened. */
            return false;
        }

        if (newMem != mem)
        {
            /*
             * newMem is different than mem,
             * switch relations.
             */
            Object::masterRelationshipRemoveObject("primitive_data_memory", (Object *) mem);
            Object::masterRelationshipAddObject("primitive_data_memory", (Object *) newMem);

            mem = newMem;
        }
    }

    wcscat((wchar_t *) mem->getAddress(), string.c_str());

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
String::operator-=(Value &data)
{
    ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_SUBTRACTING_STRING);

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
String::operator*=(Value &data)
{
    ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_MULTIPLYING_STRING);

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
String::operator/=(Value &data)
{
    ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_DIVIDING_STRING);

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
String::operator%=(Value &data)
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
String::operator++()
{
    ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_INCREMENTING_STRING);

    return false;
}

/**
 * Operator --. Decrement value.
 *
 * @return true if success, otherwise return false.
 */
bool
String::operator--()
{
    ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_DECREMENTING_STRING);

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
String::operator==(Value &data)
{
    Memory *mem = this->getMemory();
    if (!mem)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return false;
    }

    if (data.getObjectType() == OBJECT_TYPE_STRING)
    {
        String &str = (String &)data;

        return wcscmp(mem->getPointer<const wchar_t *>(),
                      (const wchar_t *) str.getAddress()) == 0;
    }
    else
    {
        std::wstring string = data.getString();
        return string == mem->getPointer<const wchar_t *>();
    }
}

/**
 * Operator ==. Check values non equality.
 *
 * @param data - the data.
 *
 * @return true if not equal, otherwise return false.
 */
bool
String::operator!=(Value &data)
{
    Memory *mem = this->getMemory();
    if (!mem)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return false;
    }

    if (data.getObjectType() == OBJECT_TYPE_STRING)
    {
        String &str = (String &)data;

        return wcscmp(mem->getPointer<const wchar_t *>(),
                      (const wchar_t *) str.getAddress()) != 0;
    }
    else
    {
        std::wstring string = data.getString();

        return string != mem->getPointer<const wchar_t *>();
    }
}

/**
 * Operator >. Check if this value is bigger than another.
 *
 * @param data - the data.
 *
 * @return true if this value is bigger, otherwise return false.
 */
bool
String::operator>(Value &data)
{
    Memory *mem = this->getMemory();

    if (!mem)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return false;
    }

    if (data.getObjectType() == OBJECT_TYPE_STRING)
    {
        String &str = (String &)data;

        return wcscmp(mem->getPointer<const wchar_t *>(),
                      (const wchar_t *) str.getAddress()) > 0;
    }
    else
    {
        std::wstring string = data.getString();
        return string.compare(mem->getPointer<const wchar_t *>()) < 0;
    }
}

/**
 * Operator <. Check if this value is lesser than another.
 *
 * @param data - the data.
 *
 * @return true if this value is lesser, otherwise return false.
 */
bool
String::operator<(Value &data)
{
    Memory *mem = this->getMemory();
    if (!mem)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return false;
    }

    if (data.getObjectType() == OBJECT_TYPE_STRING)
    {
        String &str = (String &)data;

        return wcscmp(mem->getPointer<const wchar_t *>(),
                      (const wchar_t *) str.getAddress()) < 0;
    }
    else
    {
        std::wstring string = data.getString();
        return string.compare(mem->getPointer<const wchar_t *>()) > 0;
    }
}

/**
 * Operator >=. Check if this value is bigger or equal than another.
 *
 * @param data - the data.
 *
 * @return true if this value is bigger or equal, otherwise return false.
 */
bool
String::operator>=(Value &data)
{
    Memory *mem = this->getMemory();
    if (!mem)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return false;
    }

    if (data.getObjectType() == OBJECT_TYPE_STRING)
    {
        String &str = (String &)data;

        return wcscmp(mem->getPointer<const wchar_t *>(),
                      (const wchar_t *) str.getAddress()) >= 0;
    }
    else
    {
        std::wstring string = data.getString();
        return string.compare(mem->getPointer<const wchar_t *>()) <= 0;
    }
}

/**
 * Operator <=. Check if this value is smaller than another.
 *
 * @param data - the data.
 *
 * @return true if this value is smaller, otherwise return false.
 */
bool
String::operator<=(Value &data)
{
    Memory *mem = this->getMemory();
    if (!mem)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return false;
    }

    if (data.getObjectType() == OBJECT_TYPE_STRING)
    {
        String &str = (String &)data;

        return wcscmp(mem->getPointer<const wchar_t *>(),
                      (const wchar_t *) str.getAddress()) <= 0;
    }
    else
    {
        std::wstring string = data.getString();
        return string.compare(mem->getPointer<const wchar_t *>()) >= 0;
    }
}

/**
 * Print value on console.
 *
 * @return true if success, otherwise return false.
 */
bool
String::print()
{
    Memory *mem = this->getMemory();
    if (!mem)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return false;
    }

    std::wcout << mem->getPointer<wchar_t *>();

    return true;
}

/**
 * Print value on screen together with new line.
 *
 * @return true if success, otherwise return false.
 */
bool
String::println()
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
String::scan()
{
    Memory *mem = this->getMemory();
    if (!mem)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return false;
    }

    std::wcin >> mem->getPointer<wchar_t *>();

    return true;
}

/**
 * Get string.
 *
 * @return string.
 */
std::wstring
String::getString()
{
    std::wstring str;
    Memory *mem = this->getMemory();

    if (!mem)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return str;
    }

    str.assign(mem->getPointer<const wchar_t *>());

    return str;
}

/**
 * @inherit
 */
eObjectType
String::getObjectType()
{
    return OBJECT_TYPE_STRING;
}

/**
 * @inherit
 */
bool
String::isReference()
{
    return true;
}
