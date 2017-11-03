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

typedef enum {
    CREATE,
    CREATE_AND_ASSIGN_CONSTANT,
    CREATE_AND_ASSIGN_OBJECT,
    POP_AND_STORE
} op_code;

/**
 * @brief The instruction class
 */
class instruction : public object {
protected:
    op_code op;
    std::vector<std::string> arg;

    /*
     * instructions logic
     */
    void create();
    void create_and_assign_constant();
    void create_and_assign_object();
    void pop_and_store();

public:
    explicit instruction(op_code op,
                         instruction *next_instruction = nullptr,
                         instruction *branch_result_false = nullptr);

    op_code &get_op_code();

    /*
     * helpers
     */
    data_type detect_data_type(std::string &sample);
    std::string clean_constant_format(std::string &sample, data_type type);


    instruction *execute();
};

#endif // BOX_INSTRUCTION_H
