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

#ifndef BOX_METHOD_H
#define BOX_METHOD_H

#include "ORM/entity.h"
#include "data_type.h"
#include "fw_decl.h"

typedef enum {
    INSTRUCTION_OK,
    INSTRUCTION_ERROR,
    INSTRUCTION_FINISHED
} instruction_result;

/**
 * @brief The method class
 */
class method : public entity {
protected:
    entity *result;
    std::map<std::string, entity *> local_objects;
    std::vector<entity *> stack;
    instruction *current_instruction;
public:
    method(std::string id, std::vector<instruction *> &instructions);
    instruction_result execute_next();
    void add_local_object(entity *e);
    entity *get_local_object(std::string id);
    void push_stack(entity *e);
    entity *pop_stack();
};

#endif // BOX_METHOD_H
