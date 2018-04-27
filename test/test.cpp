/*
 * Copyright 2018 Duje Senta
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

#include "test.h"
#include "include/ORM/orm_test.h"
#include "include/VariableBundle/Collection/collection_test.h"
#include "include/MemoryBundle/memory_chunk_test.h"
#include "include/MemoryBundle/virtual_memory_test.h"
#include "include/VariableBundle/file/file_test.h"
#include "include/MethodBundle/Instruction/create_instruction_test.h"
#include "include/MethodBundle/Instruction/create_and_assign_constant_instruction_test.h"
#include "include/VariableBundle/Primitive/data_type_test.h"
#include "include/VariableBundle/Primitive/primitive_data_test.h"
#include <cstdio>

#define RUN_TEST_SECTION(__test__) \
    do { \
            printf(#__test__ " section:\r\n\r\n"); \
            __test__(); \
            printf("\r\r\n"); \
    } while (false);

/**
 * Run tests.
 */
void run_tests()
{
    RUN_TEST_SECTION(orm_test);
    RUN_TEST_SECTION(memory_chunk_test);
    RUN_TEST_SECTION(virtual_memory_test);
    RUN_TEST_SECTION(data_type_test);
    RUN_TEST_SECTION(primitive_data_test);
    RUN_TEST_SECTION(collection_test);
    RUN_TEST_SECTION(file_test);
    RUN_TEST_SECTION(create_instruction_test);
    //RUN_TEST_SECTION(create_and_assign_constant_instruction_test);

    printf("TESTS ARE OK!\n");
}
