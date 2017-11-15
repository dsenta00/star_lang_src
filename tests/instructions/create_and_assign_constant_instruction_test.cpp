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
#include <virtual_memory.h>
#include <ORM/orm.h>
#include <error_log.h>
#include "create_and_assign_constant_instruction_test.h"
#include "instructions/create_and_assign_constant_instruction.h"
#include "../test_assert.h"
#include "primitive_data/primitive_data.h"
#include "collection.h"
#include <method.h>

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

    i = create_and_assign_constant_instruction::create(L"", L"int", L"");
    i->execute();
    ASSERT_ERROR(ERROR_INSTRUCTION_NO_METHOD);
    ORM_DESTROY(i);
    ERROR_LOG_CLEAR;

    i = create_and_assign_constant_instruction::create(L"", L"", L"");
    i->execute();
    ASSERT_ERROR(ERROR_INSTRUCTION_NO_METHOD);
    ORM_DESTROY(i);
    ERROR_LOG_CLEAR;

    i = create_and_assign_constant_instruction::create(L"ante", L"konjo", L"");
    i->execute();
    ASSERT_ERROR(ERROR_INSTRUCTION_NO_METHOD);
    ORM_DESTROY(i);
    ERROR_LOG_CLEAR;

    /* with method */

    method *foo;
    std::vector<abstract_instruction *> instructions;

    i = create_and_assign_constant_instruction::create(L"", L"int", L"");
    instructions.clear();
    instructions.push_back(i);
    foo = method::create("foo", instructions);
    i->execute();
    ASSERT_ERROR(ERROR_INSTRUCTION_INVALID_OBJECT_NAME);
    ORM_DESTROY(i);
    ORM_DESTROY(foo);
    ERROR_LOG_CLEAR;

    i = create_and_assign_constant_instruction::create(L"", L"", L"");
    instructions.clear();
    instructions.push_back(i);
    foo = method::create("foo", instructions);
    i->execute();
    ASSERT_ERROR(ERROR_INSTRUCTION_INVALID_OBJECT_NAME);
    ORM_DESTROY(i);
    ORM_DESTROY(foo);
    ERROR_LOG_CLEAR;

    i = create_and_assign_constant_instruction::create(L"int", L"int", L"");
    instructions.clear();
    instructions.push_back(i);
    foo = method::create("foo", instructions);
    i->execute();
    ASSERT_ERROR(ERROR_INSTRUCTION_INVALID_OBJECT_NAME);
    ORM_DESTROY(i);
    ORM_DESTROY(foo);
    ERROR_LOG_CLEAR;

    i = create_and_assign_constant_instruction::create(L"float", L"float", L"");
    instructions.clear();
    instructions.push_back(i);
    foo = method::create("foo", instructions);
    i->execute();
    ASSERT_ERROR(ERROR_INSTRUCTION_INVALID_OBJECT_NAME);
    ORM_DESTROY(i);
    ORM_DESTROY(foo);
    ERROR_LOG_CLEAR;

    i = create_and_assign_constant_instruction::create(L"char", L"char", L"");
    instructions.clear();
    instructions.push_back(i);
    foo = method::create("foo", instructions);
    i->execute();
    ASSERT_ERROR(ERROR_INSTRUCTION_INVALID_OBJECT_NAME);
    ORM_DESTROY(i);
    ORM_DESTROY(foo);
    ERROR_LOG_CLEAR;

    i = create_and_assign_constant_instruction::create(L"string", L"string", L"");
    instructions.clear();
    instructions.push_back(i);
    foo = method::create("foo", instructions);
    i->execute();
    ASSERT_ERROR(ERROR_INSTRUCTION_INVALID_OBJECT_NAME);
    ORM_DESTROY(i);
    ORM_DESTROY(foo);
    ERROR_LOG_CLEAR;

    i = create_and_assign_constant_instruction::create(L"bool", L"bool", L"");
    instructions.clear();
    instructions.push_back(i);
    foo = method::create("foo", instructions);
    i->execute();
    ASSERT_ERROR(ERROR_INSTRUCTION_INVALID_OBJECT_NAME);
    ORM_DESTROY(i);
    ORM_DESTROY(foo);
    ERROR_LOG_CLEAR;

    i = create_and_assign_constant_instruction::create(L"collection", L"collection", L"");
    instructions.clear();
    instructions.push_back(i);
    foo = method::create("foo", instructions);
    i->execute();
    ASSERT_ERROR(ERROR_INSTRUCTION_INVALID_OBJECT_NAME);
    ORM_DESTROY(i);
    ORM_DESTROY(foo);
    ERROR_LOG_CLEAR;

    i = create_and_assign_constant_instruction::create(L"ante", L"konjo", L"");
    instructions.clear();
    instructions.push_back(i);
    foo = method::create("foo", instructions);
    i->execute();
    ASSERT_ERROR(ERROR_INSTRUCTION_UNKNOWN_CONSTANT_FORMAT);
    ORM_DESTROY(i);
    ORM_DESTROY(foo);
    ERROR_LOG_CLEAR;

    i = create_and_assign_constant_instruction::create(L"ante", L"konjo", L"smajo");
    instructions.clear();
    instructions.push_back(i);
    foo = method::create("foo", instructions);
    i->execute();
    ASSERT_ERROR(ERROR_INSTRUCTION_UNKNOWN_CONSTANT_FORMAT);
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
    abstract_instruction *i = create_and_assign_constant_instruction::create(L"int_name", L"int", L"3");
    ASSERT_OK;

    std::vector<abstract_instruction *> instructions;
    instructions.push_back(i);
    method *foo = method::create("foo", instructions);
    ASSERT_OK;

    ASSERT_EQUALS(foo->execute_next(), INSTRUCTION_FINISHED);
    ASSERT_OK;
    ASSERT_VIRTUAL_MEMORY(*vm, DATA_TYPE_SIZE[DATA_TYPE_INT]);
    auto *data = ORM_SELECT(primitive_data, obj->get_id() == "int_name");
    ASSERT_NOT_NULL(data);
    ASSERT_EQUALS(data->get_type(), DATA_TYPE_INT);
    ASSERT_NOT_NULL(foo->get_local_object(L"int_name"));
    ASSERT_EQUALS(data->to_int(), 3);
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
    abstract_instruction *i = create_and_assign_constant_instruction::create(L"int_name", L"int", L"3");
    ASSERT_OK;

    abstract_instruction *f = create_and_assign_constant_instruction::create(L"float_name", L"float", L"4.2");
    ASSERT_OK;

    abstract_instruction *c = create_and_assign_constant_instruction::create(L"collection_name", L"collection", L"12.4");
    ASSERT_OK;

    std::vector<abstract_instruction *> instructions;
    instructions.push_back(i);
    instructions.push_back(f);
    instructions.push_back(c);
    method *foo = method::create("foo", instructions);
    ASSERT_OK;

    ASSERT_EQUALS(foo->execute_next(), INSTRUCTION_OK);
    ASSERT_OK;
    ASSERT_VIRTUAL_MEMORY(*vm, DATA_TYPE_SIZE[DATA_TYPE_INT]);
    auto *int_name = ORM_SELECT(primitive_data, obj->get_id() == "int_name");
    ASSERT_NOT_NULL(int_name);
    ASSERT_NOT_NULL(foo->get_local_object(L"int_name"));
    ASSERT_EQUALS(int_name->to_int(), 3);

    ASSERT_EQUALS(foo->execute_next(), INSTRUCTION_OK);
    ASSERT_OK;
    ASSERT_VIRTUAL_MEMORY(*vm, DATA_TYPE_SIZE[DATA_TYPE_INT] + DATA_TYPE_SIZE[DATA_TYPE_FLOAT]);
    auto *float_name = ORM_SELECT(primitive_data, obj->get_id() == "float_name");
    ASSERT_NOT_NULL(float_name);
    ASSERT_NOT_NULL(foo->get_local_object(L"float_name"));
    ASSERT_EQUALS(int_name->to_float(), 4.2);

    ASSERT_EQUALS(foo->execute_next(), INSTRUCTION_FINISHED);
    ASSERT_OK;
    ASSERT_VIRTUAL_MEMORY(*vm, DATA_TYPE_SIZE[DATA_TYPE_INT] + DATA_TYPE_SIZE[DATA_TYPE_FLOAT]);
    auto *collection_name = ORM_SELECT(collection, obj->get_id() == "collection_name");
    ASSERT_NOT_NULL(collection_name);
    ASSERT_NOT_NULL(foo->get_local_object(L"collection_name"));
    auto *collection_data = (primitive_data *)(*collection_name)[0];
    ASSERT_NOT_NULL(collection_data);
    ASSERT_EQUALS(collection_data->to_float(), 14.2);

    ASSERT_EQUALS(foo->execute_next(), INSTRUCTION_ERROR);
    abstract_instruction *next = i->execute();
    ASSERT_EQUALS(next, f);
    ASSERT_ERROR(ERROR_METHOD_ADD_OBJECTS_OF_SAME_NAME);
    ERROR_LOG_CLEAR;
}

/**
 * create_and_assign_constant_instruction_test
 */
void create_and_assign_constant_instruction_test()
{
    vm = (virtual_memory *) orm::get_first("virtual_memory");
    RUN_TEST_VM(instruction_test_create_negative());
    RUN_TEST_VM(instruction_test_create1());
    RUN_TEST_VM(instruction_test_create2());
}
