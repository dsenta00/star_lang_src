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
#include "variable/primitive_data/data_type.h"
#include "fw_decl.h"

typedef enum {
    INSTRUCTION_OK,
    INSTRUCTION_ERROR,
    INSTRUCTION_FINISHED
} instruction_result;

/**
 * @brief The method class
 */
class method : public object {
public:
    method(std::string id, std::vector<abstract_instruction *> &instructions);
    static method *create(std::string id, std::vector<abstract_instruction *> &instructions);

    object_type get_object_type() override;
    instruction_result execute_next();

    void push_stack(value *v);
    value *pop_stack();

    void add_var(var *v);
    var *get_var(std::wstring id);

    void clear();
protected:
    abstract_instruction *current_instruction;
};