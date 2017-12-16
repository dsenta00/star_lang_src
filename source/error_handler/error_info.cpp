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

#include "error_handler/error_info.h"
#include <cstdio>

#define ERROR_INFO_STRING_LENGTH (128)

static const char *error_info_string[ERROR_INFO_STRING_LENGTH] =
    {
        "STATUS_OK",

        "ERROR_FILE_UNKNOWN_MODE",
        "ERROR_FILE_UNKNOWN_FILE",
        "ERROR_FILE_READ_ON_WRITE_MODE",
        "ERROR_FILE_WRITE_ON_READ_MODE",
        "ERROR_FILE_WRITE_FAIL",

        "ERROR_INSTRUCTION_OBJECT_DOES_NOT_EXIST",
        "ERROR_INSTRUCTION_INVALID_NO_OF_ARGS",
        "ERROR_INSTRUCTION_UNKNOWN_CONSTANT_FORMAT",
        "ERROR_INSTRUCTION_NO_MATCH_CONSTANT_FORMAT_WITH_DATA_TYPE",
        "ERROR_INSTRUCTION_NO_METHOD",
        "ERROR_INSTRUCTION_INVALID_OBJECT_NAME",

        "ERROR_METHOD_ADD_OBJECTS_OF_SAME_NAME",

        "ERROR_RELATIONSHIP_ADDING_MORE_THAN_ONE",

        "ERROR_ENTITY_UNKNOWN_RELATIONSHIP",

        "ERROR_PRIMITIVE_DATA_INVALID_DATA_TYPE",
        "ERROR_PRIMITIVE_DATA_NULL_DATA",
        "ERROR_PRIMITIVE_DATA_ASSIGNING_STRING",
        "ERROR_PRIMITIVE_DATA_ADDING_STRING",
        "ERROR_PRIMITIVE_DATA_SUBTRACTING_STRING",
        "ERROR_PRIMITIVE_DATA_LOGICAL_OPERATION_STRING",
        "ERROR_PRIMITIVE_DATA_LOGICAL_OPERATION_FLOAT",
        "ERROR_PRIMITIVE_DATA_MULTIPLYING_STRING",
        "ERROR_PRIMITIVE_DATA_INCREMENTING_STRING",
        "ERROR_PRIMITIVE_DATA_DECREMENTING_STRING",
        "ERROR_PRIMITIVE_DATA_DIVIDING_STRING",
        "ERROR_PRIMITIVE_DATA_DIVIDING_ZERO",
        "ERROR_PRIMITIVE_DATA_INVALID_MODULUS",
        "ERROR_PRIMITIVE_DATA_MODULUS_ZERO",
        "ERROR_PRIMITIVE_DATA_BOOL_ARITHMETIC",

        "ERROR_COLLECTION_ASSIGNING_CONSTANT",
        "ERROR_COLLECTION_UNSUPPORTED_OPERATOR",

        "ERROR_METHOD_ADDING_NULL_DATA",

        "ERROR_VIRTUAL_MEMORY_UNKNOWN_CHUNK",
        "ERROR_VIRTUAL_MEMORY_ZERO_CAPACITY",
        "ERROR_VIRTUAL_MEMORY_UNKNOWN_FAULT",

        "ERROR_NULL_OPERATION"
    };

/**
 * The constructor.
 *
 * @param status - fault status
 * @param func - function name where fault occurs.
 */
error_info::error_info(error_status status, const char *func)
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
error_info::operator==(error_info &another_error)
{
    return this->status == another_error.status;
}

/**
 * Print error information.
 */
void
error_info::print()
{
    printf("[ERROR] -> %s, %s()\r\n",
           error_info_string[this->status],
           this->func);
}

/**
 * Get status in string format.
 *
 * @return status in string format.
 */
const char *
error_info::get_status_str()
{
    return error_info_string[this->status];
}

/**
 * Get status.
 *
 * @return status.
 */
error_status
error_info::get_status()
{
    return this->status;
}
