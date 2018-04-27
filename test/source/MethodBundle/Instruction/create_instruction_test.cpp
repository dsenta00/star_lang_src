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
#include <ORM/ORM.h>
#include <ErrorBundle/ErrorLog.h>
#include <MemoryBundle/VirtualMemory.h>
#include <VariableBundle/Var.h>
#include <MethodBundle/Instruction/CreateInstruction.h>
#include <MethodBundle/Method.h>
#include "../../../include/MethodBundle/Instruction/create_instruction_test.h"
#include "../../../test_assert.h"

static VirtualMemory *vm;

/**
 * instruction test create negative.
 */
static void
instruction_test_create_negative()
{
    ERROR_LOG_CLEAR;

    Instruction *i;

    /* Without Method */

    i = CreateInstruction::create(L"", L"int");
    i->executeIt();
    ASSERT_ERROR(ERROR_INSTRUCTION_NO_METHOD);
    ORM_DESTROY(i);
    ERROR_LOG_CLEAR;

    i = CreateInstruction::create(L"", L"");
    i->executeIt();
    ASSERT_ERROR(ERROR_INSTRUCTION_NO_METHOD);
    ORM_DESTROY(i);
    ERROR_LOG_CLEAR;

    i = CreateInstruction::create(L"ante", L"konjo");
    i->executeIt();
    ASSERT_ERROR(ERROR_INSTRUCTION_NO_METHOD);
    ORM_DESTROY(i);
    ERROR_LOG_CLEAR;

    /* with Method */

    Method *foo;
    std::vector<Instruction *> instructions;

    i = CreateInstruction::create(L"", L"int");
    instructions.clear();
    instructions.push_back(i);
    foo = Method::create("foo", instructions);
    i->executeIt();
    ASSERT_ERROR(ERROR_INSTRUCTION_INVALID_OBJECT_NAME);
    ORM_DESTROY(i);
    ORM_DESTROY(foo);
    ERROR_LOG_CLEAR;

    i = CreateInstruction::create(L"", L"");
    instructions.clear();
    instructions.push_back(i);
    foo = Method::create("foo", instructions);
    i->executeIt();
    ASSERT_ERROR(ERROR_INSTRUCTION_INVALID_OBJECT_NAME);
    ORM_DESTROY(i);
    ORM_DESTROY(foo);
    ERROR_LOG_CLEAR;

    i = CreateInstruction::create(L"int", L"int");
    instructions.clear();
    instructions.push_back(i);
    foo = Method::create("foo", instructions);
    i->executeIt();
    ASSERT_ERROR(ERROR_INSTRUCTION_INVALID_OBJECT_NAME);
    ORM_DESTROY(i);
    ORM_DESTROY(foo);
    ERROR_LOG_CLEAR;

    i = CreateInstruction::create(L"float", L"float");
    instructions.clear();
    instructions.push_back(i);
    foo = Method::create("foo", instructions);
    i->executeIt();
    ASSERT_ERROR(ERROR_INSTRUCTION_INVALID_OBJECT_NAME);
    ORM_DESTROY(i);
    ORM_DESTROY(foo);
    ERROR_LOG_CLEAR;

    i = CreateInstruction::create(L"char", L"char");
    instructions.clear();
    instructions.push_back(i);
    foo = Method::create("foo", instructions);
    i->executeIt();
    ASSERT_ERROR(ERROR_INSTRUCTION_INVALID_OBJECT_NAME);
    ORM_DESTROY(i);
    ORM_DESTROY(foo);
    ERROR_LOG_CLEAR;

    i = CreateInstruction::create(L"string", L"string");
    instructions.clear();
    instructions.push_back(i);
    foo = Method::create("foo", instructions);
    i->executeIt();
    ASSERT_ERROR(ERROR_INSTRUCTION_INVALID_OBJECT_NAME);
    ORM_DESTROY(i);
    ORM_DESTROY(foo);
    ERROR_LOG_CLEAR;

    i = CreateInstruction::create(L"bool", L"bool");
    instructions.clear();
    instructions.push_back(i);
    foo = Method::create("foo", instructions);
    i->executeIt();
    ASSERT_ERROR(ERROR_INSTRUCTION_INVALID_OBJECT_NAME);
    ORM_DESTROY(i);
    ORM_DESTROY(foo);
    ERROR_LOG_CLEAR;

    i = CreateInstruction::create(L"Collection", L"Collection");
    instructions.clear();
    instructions.push_back(i);
    foo = Method::create("foo", instructions);
    i->executeIt();
    ASSERT_ERROR(ERROR_INSTRUCTION_INVALID_OBJECT_NAME);
    ORM_DESTROY(i);
    ORM_DESTROY(foo);
    ERROR_LOG_CLEAR;

    i = CreateInstruction::create(L"ante", L"konjo");
    instructions.clear();
    instructions.push_back(i);
    foo = Method::create("foo", instructions);
    i->executeIt();
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
    Instruction *i = CreateInstruction::create(L"int_name", L"int");
    ASSERT_OK;

    std::vector<Instruction *> instructions;
    instructions.push_back(i);
    Method *foo = Method::create("foo", instructions);
    ASSERT_OK;

    ASSERT_EQUALS(foo->step(), INSTRUCTION_FINISHED);
    ASSERT_OK;
    ASSERT_NOT_NULL(foo->getVar(L"int_name"));
    foo->getVar(L"int_name")->get()->println();
    ASSERT_VIRTUAL_MEMORY(*vm, DataType::SIZE[OBJECT_TYPE_INT]);
    ASSERT_NOT_NULL(ORM::select(OBJECT_TYPE_VARIABLE, "int_name"));

    ASSERT_EQUALS(foo->step(), INSTRUCTION_ERROR);
    Instruction *next = i->executeIt();
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
    Instruction *i = CreateInstruction::create(L"int_name", L"int");
    ASSERT_OK;

    Instruction *f = CreateInstruction::create(L"float_name", L"float");
    ASSERT_OK;

    Instruction *c = CreateInstruction::create(L"collection_name", L"Collection");
    ASSERT_OK;

    std::vector<Instruction *> instructions;
    instructions.push_back(i);
    instructions.push_back(f);
    instructions.push_back(c);
    Method *foo = Method::create("foo", instructions);
    ASSERT_OK;

    ASSERT_EQUALS(foo->step(), INSTRUCTION_OK);
    ASSERT_OK;
    ASSERT_VIRTUAL_MEMORY(*vm, DataType::SIZE[OBJECT_TYPE_INT]);
    ASSERT_NOT_NULL(ORM::select(OBJECT_TYPE_VARIABLE, "int_name"));
    ASSERT_NOT_NULL(foo->getVar(L"int_name"));

    ASSERT_EQUALS(foo->step(), INSTRUCTION_OK);
    ASSERT_OK;
    ASSERT_VIRTUAL_MEMORY(*vm, DataType::SIZE[OBJECT_TYPE_INT] + DataType::SIZE[OBJECT_TYPE_FLOAT]);
    ASSERT_NOT_NULL(ORM::select(OBJECT_TYPE_VARIABLE, "float_name"));
    ASSERT_NOT_NULL(foo->getVar(L"float_name"));

    ASSERT_EQUALS(foo->step(), INSTRUCTION_FINISHED);
    ASSERT_OK;
    ASSERT_VIRTUAL_MEMORY(*vm, DataType::SIZE[OBJECT_TYPE_INT] + DataType::SIZE[OBJECT_TYPE_FLOAT]);
    ASSERT_NOT_NULL(ORM::select(OBJECT_TYPE_VARIABLE, "collection_name"));
    ASSERT_NOT_NULL(foo->getVar(L"collection_name"));

    ASSERT_EQUALS(foo->step(), INSTRUCTION_ERROR);
    Instruction *next = i->executeIt();
    ASSERT_EQUALS(next, f);
    ASSERT_ERROR(ERROR_METHOD_ADD_OBJECTS_OF_SAME_NAME);
    ERROR_LOG_CLEAR;
}

/**
 * instruction test.
 */
void create_instruction_test()
{
    vm = (VirtualMemory *) ORM::getFirst(OBJECT_TYPE_VIRTUAL_MEMORY);
    RUN_TEST_VM(instruction_test_create_negative());
    RUN_TEST_VM(instruction_test_create1());
    RUN_TEST_VM(instruction_test_create2());
}
