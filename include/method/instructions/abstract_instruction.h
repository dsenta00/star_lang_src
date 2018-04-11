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
#include "ORM/orm_fw.h"
#include "fw_decl.h"
#include "variable/primitive_data/data_type.h"
#include "op_code.h"

/**
 * @brief The instruction class
 */
class abstract_instruction : public object {
public:
    explicit abstract_instruction(op_code op, std::vector<std::wstring> &arg);

    object_type get_object_type() override;

    op_code &get_op_code();
    virtual abstract_instruction *execute() = 0;
    virtual bool validate() = 0;
protected:
    op_code op;
    std::vector<std::wstring> arg;
    bool validated;

    /*
     * Helper methods
     */
    method *get_method();
    bool object_name_is_valid(std::wstring &sample);
};