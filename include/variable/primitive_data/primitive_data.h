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

#include "ORM/object.h"
#include "data_type.h"
#include "fw_decl.h"
#include <string>
#include <variable/value.h>

/**
 * The primitive_data class. Represents primitive data type in
 * this programming language and defines all operations in that scope.
 */
class primitive_data : public value {
public:
    explicit primitive_data(object_type type = OBJECT_TYPE_NULL, const void *value = nullptr);
    primitive_data(primitive_data &data);

    static primitive_data *create(object_type type = OBJECT_TYPE_NULL, const void *value = nullptr);
    static primitive_data *create(primitive_data &data);

    virtual bool default_value() = 0;

    memory *get_memory();
    uintptr_t get_address();

    static bool is_primitive(value *data);
protected:
    virtual_memory *vm;
};