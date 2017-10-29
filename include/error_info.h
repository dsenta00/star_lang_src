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

#ifndef ERROR_INFO_H
#define ERROR_INFO_H

typedef enum {
    STATUS_OK,

    ERROR_FILE_UNKNOWN_MODE,
    ERROR_FILE_UNKNOWN_FILE,
    ERROR_FILE_READ_ON_WRITE_MODE,
    ERROR_FILE_WRITE_ON_READ_MODE,
    ERROR_FILE_DID_NOT_OPEN,

    ERROR_INSTRUCTION_OBJECT_DOES_NOT_EXIST,
    ERROR_INSTRUCTION_UNKNOWN,
    ERROR_INSTRUCTION_INVALID_NO_OF_ARGS,
    ERROR_INSTRUCTION_UNKNOWN_CONSTANT_FORMAT,
    ERROR_INSTRUCTION_NO_MATCH_CONSTANT_FORMAT_WITH_DATA_TYPE,

    ERROR_METHOD_ADD_OBJECTS_OF_SAME_NAME,

    ERROR_RELATIONSHIP_ADDING_MORE_THAN_ONE,

    ERROR_ENTITY_UNKNOWN_RELATIONSHIP,

    ERROR_PRIMITIVE_DATA_INVALID_DATA_TYPE,
    ERROR_PRIMITIVE_DATA_NULL_DATA,
    ERROR_PRIMITIVE_DATA_ASSIGNING_STRING,
    ERROR_PRIMITIVE_DATA_ADDING_STRING,
    ERROR_PRIMITIVE_DATA_SUBTRACTING_STRING,
    ERROR_PRIMITIVE_DATA_LOGICAL_OPERATION_STRING,
    ERROR_PRIMITIVE_DATA_LOGICAL_OPERATION_FLOAT,
    ERROR_PRIMITIVE_DATA_MULTIPLYING_STRING,
    ERROR_PRIMITIVE_DATA_INCREMENTING_STRING,
    ERROR_PRIMITIVE_DATA_DECREMENTING_STRING,
    ERROR_PRIMITIVE_DATA_DIVIDING_STRING,
    ERROR_PRIMITIVE_DATA_DIVIDING_ZERO,
    ERROR_PRIMITIVE_DATA_INVALID_MODULUS,
    ERROR_PRIMITIVE_DATA_MODULUS_ZERO,

    ERROR_METHOD_ADDING_NULL_DATA,

    ERROR_VIRTUAL_MEMORY_UNKNOWN_CHUNK,
    ERROR_VIRTUAL_MEMORY_ZERO_CAPACITY,
    ERROR_VIRTUAL_MEMORY_UNKNOWN_FAULT
} error_status;

class error_info {
public:
    error_info(error_status status, const char *func);
    bool operator==(error_info &another_error);
    void print();
    error_status get_status();
    const char *get_status_str();
protected:
    error_status status;
    const char *func;
};

#endif // ERROR_INFO_H
