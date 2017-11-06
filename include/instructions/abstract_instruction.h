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

#ifndef BOX_INSTRUCTION_H
#define BOX_INSTRUCTION_H

#include "ORM/object.h"
#include "ORM/orm_fw.h"
#include "fw_decl.h"
#include "data_type.h"
#include "op_code.h"

/**
 * @brief The instruction class
 */
class abstract_instruction : public object {
public:
    explicit abstract_instruction(op_code op, std::vector<std::string> arg);
    op_code &get_op_code();
    virtual abstract_instruction *execute() = 0;
    virtual bool validate() = 0;
protected:
    op_code op;
    std::vector<std::string> arg;
    bool validated;

    /*
     * Helper methods
     */
    method *get_method();
    data_type detect_data_type(std::string &sample);
    std::string clean_constant_format(std::string &sample, data_type type);
    bool object_name_is_valid(std::string &sample);
};

#endif // BOX_INSTRUCTION_H
