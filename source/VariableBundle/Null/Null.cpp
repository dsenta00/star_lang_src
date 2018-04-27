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
#include <VariableBundle/Primitive/String.h>
#include "VariableBundle/Null/Null.h"
#include <iostream>

/**
 * The constructor.
 */
Null::Null() : Value()
{}

/**
 * To string.
 *
 * @return
 */
String &
Null::toString()
{
    return *String::create( L"(Null)");
}

/**
 * Put to default value.
 *
 * @return
 */
bool
Null::defaultValue()
{
    return true;
}

/**
 * To bool.
 *
 * @return
 */
bool
Null::toBool()
{
    return false;
}

/**
 * To char.
 *
 * @return
 */
wchar_t
Null::toChar()
{
    return 0;
}

/**
 * To integer.
 *
 * @return
 */
int32_t
Null::toInt()
{
    return 0;
}

/**
 * To float.
 *
 * @return
 */
double
Null::toFloat()
{
    return 0;
}

/**
 * @inherit
 */
eObjectType
Null::getObjectType()
{
    return OBJECT_TYPE_NULL;
}

/**
 * @inherit
 */
bool
Null::print()
{
    std::cout << "(Null)";
    return true;
}

/**
 * @inherit
 */
bool
Null::println()
{
    this->print();
    std::cout << std::endl;

    return true;
}

/**
 * @inherit
 */
std::wstring
Null::getString()
{
    return std::wstring(L"(Null)");
}

/**
 * Create null.
 */
Null *
Null::create()
{
    return (Null *) ORM::create(new Null());
}

/**
 * Operator =.
 *
 * @param data
 * @return
 */
bool
Null::operator=(const void *data)
{
    ERROR_LOG_ADD(ERROR_NULL_OPERATION);

    return false;
}

/**
 * Operator =.
 *
 * @param data
 * @return
 */
bool
Null::operator=(Value &data)
{
    ERROR_LOG_ADD(ERROR_NULL_OPERATION);

    return false;
}

/**
 * Operator &=.
 *
 * @param data
 * @return
 */
bool
Null::operator&=(Value &data)
{
    ERROR_LOG_ADD(ERROR_NULL_OPERATION);

    return false;
}

/**
 * Operator |=.
 *
 * @param data
 * @return
 */
bool
Null::operator|=(Value &data)
{
    ERROR_LOG_ADD(ERROR_NULL_OPERATION);

    return false;
}

/**
 * Operator ^=
 *
 * @param data
 * @return
 */
bool
Null::operator^=(Value &data)
{
    ERROR_LOG_ADD(ERROR_NULL_OPERATION);

    return false;
}

/**
 * Operator +=.
 *
 * @param data
 * @return
 */
bool
Null::operator+=(Value &data)
{
    ERROR_LOG_ADD(ERROR_NULL_OPERATION);

    return false;
}

/**
 * Operator -=.
 *
 * @param data
 * @return
 */
bool
Null::operator-=(Value &data)
{
    ERROR_LOG_ADD(ERROR_NULL_OPERATION);

    return false;
}

/**
 * Operator *=.
 *
 * @param data
 * @return
 */
bool
Null::operator*=(Value &data)
{
    ERROR_LOG_ADD(ERROR_NULL_OPERATION);

    return false;
}

/**
 * Operator /=.
 *
 * @param data
 * @return
 */
bool
Null::operator/=(Value &data)
{
    ERROR_LOG_ADD(ERROR_NULL_OPERATION);

    return false;
}

/**
 * Operator /=.
 *
 * @param data
 * @return
 */
bool
Null::operator%=(Value &data)
{
    ERROR_LOG_ADD(ERROR_NULL_OPERATION);

    return false;
}

/**
 * Operator ++.
 *
 * @return
 */
bool
Null::operator++()
{
    ERROR_LOG_ADD(ERROR_NULL_OPERATION);

    return false;
}

/**
 * Operator --.
 *
 * @return
 */
bool
Null::operator--()
{
    ERROR_LOG_ADD(ERROR_NULL_OPERATION);

    return false;
}

/**
 * Operator ==.
 *
 * @param data
 * @return
 */
bool
Null::operator==(Value &data)
{
    return data.getObjectType() == OBJECT_TYPE_NULL;
}

/**
 * Operator !=.
 *
 * @param data
 * @return
 */
bool
Null::operator!=(Value &data)
{
    return data.getObjectType() != OBJECT_TYPE_NULL;
}

/**
 * Operator >.
 *
 * @param data
 * @return
 */
bool
Null::operator>(Value &data)
{
    ERROR_LOG_ADD(ERROR_NULL_OPERATION);

    return false;
}

/**
 * Operator <.
 *
 * @param data
 * @return
 */
bool
Null::operator<(Value &data)
{
    ERROR_LOG_ADD(ERROR_NULL_OPERATION);

    return false;
}

/**
 * Operator >=.
 *
 * @param data
 * @return
 */
bool
Null::operator>=(Value &data)
{
    ERROR_LOG_ADD(ERROR_NULL_OPERATION);

    return false;
}

/**
 * Operator <=.
 *
 * @param data
 * @return
 */
bool
Null::operator<=(Value &data)
{
    ERROR_LOG_ADD(ERROR_NULL_OPERATION);

    return false;
}

/**
 * @inherit
 */
bool
Null::scan()
{
    ERROR_LOG_ADD(ERROR_NULL_OPERATION);

    return false;
}

/**
 * @inherit
 */
bool
Null::isReference()
{
    return true;
}
