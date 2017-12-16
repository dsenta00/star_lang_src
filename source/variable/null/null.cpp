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

#include <iostream>
#include <variable/primitive_data/string_data.h>
#include <ORM/orm.h>
#include <error_handler/error_log.h>
#include "variable/null/null.h"

/**
 * The constructor.
 */
null::null() : value()
{}

/**
 * To string.
 *
 * @return
 */
string_data &
null::to_string()
{
    return *string_data::create( L"(null)");
}

/**
 * Put to default value.
 *
 * @return
 */
bool
null::default_value()
{
    return true;
}

/**
 * To bool.
 *
 * @return
 */
bool
null::to_bool()
{
    return false;
}

/**
 * To char.
 *
 * @return
 */
wchar_t
null::to_char()
{
    return 0;
}

/**
 * To integer.
 *
 * @return
 */
int32_t
null::to_int()
{
    return 0;
}

/**
 * To float.
 *
 * @return
 */
double
null::to_float()
{
    return 0;
}

/**
 * @inherit
 */
object_type
null::get_object_type()
{
    return OBJECT_TYPE_NULL;
}

/**
 * @inherit
 */
bool
null::print()
{
    std::cout << "(null)";
    return true;
}

/**
 * @inherit
 */
bool
null::println()
{
    this->print();
    std::cout << std::endl;

    return true;
}

/**
 * @inherit
 */
std::wstring
null::get_string()
{
    return std::wstring(L"(null)");
}

/**
 * Create null.
 */
null *
null::create()
{
    return (null *) orm::create(new null());
}

/**
 * Operator =.
 *
 * @param data
 * @return
 */
bool
null::operator=(const void *data)
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
null::operator=(value &data)
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
null::operator&=(value &data)
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
null::operator|=(value &data)
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
null::operator^=(value &data)
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
null::operator+=(value &data)
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
null::operator-=(value &data)
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
null::operator*=(value &data)
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
null::operator/=(value &data)
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
null::operator%=(value &data)
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
null::operator++()
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
null::operator--()
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
null::operator==(value &data)
{
    return data.get_object_type() == OBJECT_TYPE_NULL;
}

/**
 * Operator !=.
 *
 * @param data
 * @return
 */
bool
null::operator!=(value &data)
{
    return data.get_object_type() != OBJECT_TYPE_NULL;
}

/**
 * Operator >.
 *
 * @param data
 * @return
 */
bool
null::operator>(value &data)
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
null::operator<(value &data)
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
null::operator>=(value &data)
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
null::operator<=(value &data)
{
    ERROR_LOG_ADD(ERROR_NULL_OPERATION);

    return false;
}

/**
 * @inherit
 */
bool
null::scan()
{
    ERROR_LOG_ADD(ERROR_NULL_OPERATION);

    return false;
}

/**
 * @inherit
 */
bool
null::is_reference()
{
    return true;
}
