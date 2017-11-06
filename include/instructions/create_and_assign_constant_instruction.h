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

#ifndef STAR_PROGRAMMING_LANGUAGE_CREATE_AND_ASSIGN_CONSTANT_INSTRUCTION_H
#define STAR_PROGRAMMING_LANGUAGE_CREATE_AND_ASSIGN_CONSTANT_INSTRUCTION_H

#include "abstract_instruction.h"

/**
 * OP_CODE_CREATE_AND_ASSIGN_CONSTANT <name> <type> <constant>
 */
class create_and_assign_constant_instruction : public abstract_instruction {
public:
    explicit create_and_assign_constant_instruction(std::vector<std::string> &arg);
    static create_and_assign_constant_instruction *create(std::string name, std::string type, std::string constant);
    abstract_instruction *execute() override;
    bool validate() override;
};


#endif //STAR_PROGRAMMING_LANGUAGE_CREATE_AND_ASSIGN_CONSTANT_INSTRUCTION_H
