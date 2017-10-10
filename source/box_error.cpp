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

#include "box_error.h"
#include <cstdio>

#define BOX_DATA_STRING_LENGTH (128)

static const char *box_status_string[BOX_DATA_STRING_LENGTH] =
    {
        "BOX_STATUS_OK",

        "ERROR_BOX_INSTRUCTION_OBJECT_DOES_NOT_EXIST",
        "ERROR_BOX_INSTRUCTION_UNKNOWN",
        "ERROR_BOX_INSTRUCTION_INVALID_NO_OF_ARGS",
        "ERROR_BOX_INSTRUCTION_UNKNOWN_CONSTANT_FORMAT",
        "ERROR_BOX_INSTRUCTION_NO_MATCH_CONSTANT_FORMAT_WITH_DATA_TYPE",

        "ERROR_BOX_METHOD_ADD_OBJECTS_OF_SAME_NAME",

        "ERROR_BOX_RELATIONSHIP_ADDING_MORE_THAN_ONE",

        "ERROR_BOX_ENTITY_UNKNOWN_RELATIONSHIP",

        "ERROR_BOX_DATA_INVALID_DATA_TYPE",
        "ERROR_BOX_DATA_NULL_DATA",
        "ERROR_BOX_DATA_ASSIGNING_STRING",
        "ERROR_BOX_DATA_ADDING_STRING",
        "ERROR_BOX_DATA_SUBTRACTING_STRING",
        "ERROR_BOX_DATA_LOGICAL_OPERATION_STRING",
        "ERROR_BOX_DATA_LOGICAL_OPERATION_FLOAT",
        "ERROR_BOX_DATA_MULTIPLYING_STRING",
        "ERROR_BOX_DATA_INCREMENTING_STRING",
        "ERROR_BOX_DATA_DECREMENTING_STRING",
        "ERROR_BOX_DATA_DIVIDING_STRING",
        "ERROR_BOX_DATA_DIVIDING_ZERO",
        "ERROR_BOX_DATA_INVALID_MODULUS",
        "ERROR_BOX_DATA_MODULUS_ZERO",

        "ERROR_BOX_ARRAY_ADDING_NULL_DATA",

        "ERROR_BOX_VIRTUAL_MEMORY_UNKNOWN_CHUNK",
        "ERROR_BOX_VIRTUAL_MEMORY_ZERO_CAPACITY",
        "ERROR_BOX_VIRTUAL_MEMORY_UNKNOWN_FAULT"
    };

/**
 * The constructor.
 *
 * @param status - fault status
 * @param func - function name where fault occurs.
 */
box_error::box_error(box_status status, const char *func)
{
    this->status = status;
    this->func = func;
}

/**
 * Operator =. It compares this class instance
 * with another class instance same type.
 *
 * @param another_error - another error.
 * @return true if same, otherwise return false.
 */
bool
box_error::operator==(box_error &another_error)
{
    return this->status == another_error.status;
}

/**
 * Print error information.
 */
void
box_error::print()
{
    printf("[BOX_ERROR] -> %s, %s()\r\n",
           box_status_string[this->status],
           this->func);
}

/**
 * Get status in string format.
 *
 * @return status in string format.
 */
const char *
box_error::get_status_str()
{
    return box_status_string[this->status];
}

/**
 * Get status.
 *
 * @return status.
 */
box_status
box_error::get_status()
{
    return this->status;
}
