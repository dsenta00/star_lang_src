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

#pragma once

#include <ORM/object.h>
#include <fw_decl.h>

/**
 * The value object.
 * Value can be array, null, integer...
 */
class value : public object {
public:
    explicit value();

    virtual bool to_bool() = 0;
    virtual wchar_t to_char() = 0;
    virtual int32_t to_int() = 0;
    virtual double to_float() = 0;
    virtual string_data &to_string() = 0;

    virtual bool is_reference() = 0;

    virtual bool operator=(const void *data) = 0;
    virtual bool operator=(value &data) = 0;
    virtual bool operator&=(value &data) = 0;
    virtual bool operator|=(value &data) = 0;
    virtual bool operator^=(value &data) = 0;
    virtual bool operator+=(value &var) = 0;
    virtual bool operator-=(value &data) = 0;
    virtual bool operator*=(value &data) = 0;
    virtual bool operator/=(value &data) = 0;
    virtual bool operator%=(value &data) = 0;
    virtual bool operator++() = 0;
    virtual bool operator--() = 0;
    virtual bool operator==(value &data) = 0;
    virtual bool operator!=(value &data) = 0;
    virtual bool operator>(value &data) = 0;
    virtual bool operator<(value &data) = 0;
    virtual bool operator>=(value &data) = 0;
    virtual bool operator<=(value &data) = 0;

    virtual bool print() = 0;
    virtual bool println() = 0;
    virtual bool scan() = 0;
    virtual std::wstring get_string() = 0;
};