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

#ifndef PRIMITIVE_TYPE_H
#define PRIMITIVE_TYPE_H

#include "ORM/object.h"
#include "data_type.h"
#include "fw_decl.h"
#include <string>

/**
 * The primitive_data class. Represents primitive data type in
 * this programming language and defines all operations in that scope.
 */
class primitive_data : public object {
public:
    explicit primitive_data(std::string id, data_type type = DATA_TYPE_INVALID, const void *value = nullptr);
    primitive_data(std::string id, primitive_data &data, bool is_reference = false);

    static primitive_data *create(std::string id, data_type type = DATA_TYPE_INVALID, const void *value = nullptr);
    static primitive_data *create(std::string id, primitive_data &data, bool is_reference = false);

    memory *get_memory();
    uintptr_t get_address();
    data_type get_type();
    bool get_is_reference();

    virtual bool to_bool() = 0;
    virtual wchar_t to_char() = 0;
    virtual int32_t to_int() = 0;
    virtual double to_float() = 0;

    virtual primitive_data &to_string() = 0;
    virtual bool default_value() = 0;
    virtual bool operator=(const void *data) = 0;
    virtual bool operator=(primitive_data &data) = 0;
    virtual bool operator&=(primitive_data &data) = 0;
    virtual bool operator|=(primitive_data &data) = 0;
    virtual bool operator^=(primitive_data &data) = 0;
    virtual bool operator+=(primitive_data &data) = 0;
    virtual bool operator-=(primitive_data &data) = 0;
    virtual bool operator*=(primitive_data &data) = 0;
    virtual bool operator/=(primitive_data &data) = 0;
    virtual bool operator%=(primitive_data &data) = 0;
    virtual bool operator++() = 0;
    virtual bool operator--() = 0;
    virtual bool operator==(primitive_data &data) = 0;
    virtual bool operator!=(primitive_data &data) = 0;
    virtual bool operator>(primitive_data &data) = 0;
    virtual bool operator<(primitive_data &data) = 0;
    virtual bool operator>=(primitive_data &data) = 0;
    virtual bool operator<=(primitive_data &data) = 0;

    virtual bool print() = 0;
    virtual bool println() = 0;
    virtual bool scan() = 0;
    virtual std::wstring get_string() = 0;
protected:
    bool is_reference;
    virtual_memory *vm;
    data_type type;
};

#endif // PRIMITIVE_TYPE_H
