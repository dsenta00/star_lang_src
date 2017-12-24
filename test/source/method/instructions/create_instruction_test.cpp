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
#include <memory_handler/virtual_memory.h>
#include <ORM/orm.h>
#include <error_handler/error_log.h>
#include "../../../include/method/instructions/create_instruction_test.h"
#include "../../../test_assert.h"
#include <variable/var.h>
#include <method/instructions/create_instruction.h>
#include <method/method.h>

static virtual_memory *vm;

/**
 * instruction test create negative.
 */
static void
instruction_test_create_negative()
{
    ERROR_LOG_CLEAR;

    abstract_instruction *i;

    /* Without method */

    i = create_instruction::create(L"", L"int");
    i->execute();
    ASSERT_ERROR(ERROR_INSTRUCTION_NO_METHOD);
    ORM_DESTROY(i);
    ERROR_LOG_CLEAR;

    i = create_instruction::create(L"", L"");
    i->execute();
    ASSERT_ERROR(ERROR_INSTRUCTION_NO_METHOD);
    ORM_DESTROY(i);
    ERROR_LOG_CLEAR;

    i = create_instruction::create(L"ante", L"konjo");
    i->execute();
    ASSERT_ERROR(ERROR_INSTRUCTION_NO_METHOD);
    ORM_DESTROY(i);
    ERROR_LOG_CLEAR;

    /* with method */

    method *foo;
    std::vector<abstract_instruction *> instructions;

    i = create_instruction::create(L"", L"int");
    instructions.clear();
    instructions.push_back(i);
    foo = method::create("foo", instructions);
    i->execute();
    ASSERT_ERROR(ERROR_INSTRUCTION_INVALID_OBJECT_NAME);
    ORM_DESTROY(i);
    ORM_DESTROY(foo);
    ERROR_LOG_CLEAR;

    i = create_instruction::create(L"", L"");
    instructions.clear();
    instructions.push_back(i);
    foo = method::create("foo", instructions);
    i->execute();
    ASSERT_ERROR(ERROR_INSTRUCTION_INVALID_OBJECT_NAME);
    ORM_DESTROY(i);
    ORM_DESTROY(foo);
    ERROR_LOG_CLEAR;

    i = create_instruction::create(L"int", L"int");
    instructions.clear();
    instructions.push_back(i);
    foo = method::create("foo", instructions);
    i->execute();
    ASSERT_ERROR(ERROR_INSTRUCTION_INVALID_OBJECT_NAME);
    ORM_DESTROY(i);
    ORM_DESTROY(foo);
    ERROR_LOG_CLEAR;

    i = create_instruction::create(L"float", L"float");
    instructions.clear();
    instructions.push_back(i);
    foo = method::create("foo", instructions);
    i->execute();
    ASSERT_ERROR(ERROR_INSTRUCTION_INVALID_OBJECT_NAME);
    ORM_DESTROY(i);
    ORM_DESTROY(foo);
    ERROR_LOG_CLEAR;

    i = create_instruction::create(L"char", L"char");
    instructions.clear();
    instructions.push_back(i);
    foo = method::create("foo", instructions);
    i->execute();
    ASSERT_ERROR(ERROR_INSTRUCTION_INVALID_OBJECT_NAME);
    ORM_DESTROY(i);
    ORM_DESTROY(foo);
    ERROR_LOG_CLEAR;

    i = create_instruction::create(L"string", L"string");
    instructions.clear();
    instructions.push_back(i);
    foo = method::create("foo", instructions);
    i->execute();
    ASSERT_ERROR(ERROR_INSTRUCTION_INVALID_OBJECT_NAME);
    ORM_DESTROY(i);
    ORM_DESTROY(foo);
    ERROR_LOG_CLEAR;

    i = create_instruction::create(L"bool", L"bool");
    instructions.clear();
    instructions.push_back(i);
    foo = method::create("foo", instructions);
    i->execute();
    ASSERT_ERROR(ERROR_INSTRUCTION_INVALID_OBJECT_NAME);
    ORM_DESTROY(i);
    ORM_DESTROY(foo);
    ERROR_LOG_CLEAR;

    i = create_instruction::create(L"collection", L"collection");
    instructions.clear();
    instructions.push_back(i);
    foo = method::create("foo", instructions);
    i->execute();
    ASSERT_ERROR(ERROR_INSTRUCTION_INVALID_OBJECT_NAME);
    ORM_DESTROY(i);
    ORM_DESTROY(foo);
    ERROR_LOG_CLEAR;

    i = create_instruction::create(L"ante", L"konjo");
    instructions.clear();
    instructions.push_back(i);
    foo = method::create("foo", instructions);
    i->execute();
    ASSERT_ERROR(ERROR_PRIMITIVE_DATA_INVALID_DATA_TYPE);
    ORM_DESTROY(i);
    ORM_DESTROY(foo);
    ERROR_LOG_CLEAR;
}

/**
 * instruction test create 1.
 */
static void
instruction_test_create1()
{
    /*
     * create <name> <type>
     */
    abstract_instruction *i = create_instruction::create(L"int_name", L"int");
    ASSERT_OK;

    std::vector<abstract_instruction *> instructions;
    instructions.push_back(i);
    method *foo = method::create("foo", instructions);
    ASSERT_OK;

    ASSERT_EQUALS(foo->execute_next(), INSTRUCTION_FINISHED);
    ASSERT_OK;
    ASSERT_NOT_NULL(foo->get_variable(L"int_name"));
    foo->get_variable(L"int_name")->get()->println();
    ASSERT_VIRTUAL_MEMORY(*vm, DATA_TYPE_SIZE[OBJECT_TYPE_INT]);
    ASSERT_NOT_NULL(orm::select(OBJECT_TYPE_VARIABLE, "int_name"));

    ASSERT_EQUALS(foo->execute_next(), INSTRUCTION_ERROR);
    abstract_instruction *next = i->execute();
    ASSERT_NULL(next);
    ASSERT_ERROR(ERROR_METHOD_ADD_OBJECTS_OF_SAME_NAME);
    ERROR_LOG_CLEAR;
}

/**
 * instruction test create 2.
 */
static void
instruction_test_create2()
{
    /*
     * create <name> <type>
     */
    abstract_instruction *i = create_instruction::create(L"int_name", L"int");
    ASSERT_OK;

    abstract_instruction *f = create_instruction::create(L"float_name", L"float");
    ASSERT_OK;

    abstract_instruction *c = create_instruction::create(L"collection_name", L"collection");
    ASSERT_OK;

    std::vector<abstract_instruction *> instructions;
    instructions.push_back(i);
    instructions.push_back(f);
    instructions.push_back(c);
    method *foo = method::create("foo", instructions);
    ASSERT_OK;

    ASSERT_EQUALS(foo->execute_next(), INSTRUCTION_OK);
    ASSERT_OK;
    ASSERT_VIRTUAL_MEMORY(*vm, DATA_TYPE_SIZE[OBJECT_TYPE_INT]);
    ASSERT_NOT_NULL(orm::select(OBJECT_TYPE_VARIABLE, "int_name"));
    ASSERT_NOT_NULL(foo->get_variable(L"int_name"));

    ASSERT_EQUALS(foo->execute_next(), INSTRUCTION_OK);
    ASSERT_OK;
    ASSERT_VIRTUAL_MEMORY(*vm, DATA_TYPE_SIZE[OBJECT_TYPE_INT] + DATA_TYPE_SIZE[OBJECT_TYPE_FLOAT]);
    ASSERT_NOT_NULL(orm::select(OBJECT_TYPE_VARIABLE, "float_name"));
    ASSERT_NOT_NULL(foo->get_variable(L"float_name"));

    ASSERT_EQUALS(foo->execute_next(), INSTRUCTION_FINISHED);
    ASSERT_OK;
    ASSERT_VIRTUAL_MEMORY(*vm, DATA_TYPE_SIZE[OBJECT_TYPE_INT] + DATA_TYPE_SIZE[OBJECT_TYPE_FLOAT]);
    ASSERT_NOT_NULL(orm::select(OBJECT_TYPE_VARIABLE, "collection_name"));
    ASSERT_NOT_NULL(foo->get_variable(L"collection_name"));

    ASSERT_EQUALS(foo->execute_next(), INSTRUCTION_ERROR);
    abstract_instruction *next = i->execute();
    ASSERT_EQUALS(next, f);
    ASSERT_ERROR(ERROR_METHOD_ADD_OBJECTS_OF_SAME_NAME);
    ERROR_LOG_CLEAR;
}

/**
 * instruction test.
 */
void create_instruction_test()
{
    vm = (virtual_memory *) orm::get_first(OBJECT_TYPE_VIRTUAL_MEMORY);
    RUN_TEST_VM(instruction_test_create_negative());
    RUN_TEST_VM(instruction_test_create1());
    RUN_TEST_VM(instruction_test_create2());
}
