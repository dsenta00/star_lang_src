#
# Copyright 2018 Duje Senta
#
# Permission is hereby granted, free of charge, to any person obtaining a
# copy of this software and associated documentation files (the "Software"),
# to deal in the Software without restriction, including without limitation
# the rights to use, copy, modify, merge, publish, distribute, sublicense,
# and/or sell copies of the Software, and to permit persons to whom the
# Software is furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included
# in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
# THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#

cmake_minimum_required(VERSION 3.2)

project(star_programming_language)
set(CMAKE_CXX_STANDARD 11)

###################################################################
# Application Main Build
###################################################################

include_directories(include/)

set(INCLUDE_FILES
        include/VariableBundle/Collection/Collection.h
        test/test_assert.h
        include/VariableBundle/Primitive/Primitive.h
        test/include/VariableBundle/Primitive/primitive_data_test.h
        include/VariableBundle/Primitive/DataType.h
        include/ErrorBundle/ErrorInfo.h
        include/fw_decl.h
        include/MethodBundle/Instruction/Instruction.h
        include/MemoryBundle/MemoryChunk.h
        include/MemoryBundle/MemoryChunkIf.h
        include/MethodBundle/Method.h
        include/ErrorBundle/ErrorLog.h
        include/MemoryBundle/VirtualMemory.h
        include/MemoryBundle/Memory.h
        include/ORM/Object.h
        include/ORM/ObjectRepository.h
        include/ORM/ORM.h
        include/ORM/FwDecl.h
        test/include/ORM/orm_test.h
        include/ORM/Relationship.h
        include/ORM/eRelationshipType.h source/MethodBundle/Instruction/PushConstantInstruction.cpp include/MethodBundle/Instruction/PushConstantInstruction.h)

set(SOURCE_FILES
        source/VariableBundle/Collection/Collection.cpp
        test/test_assert.cpp
        source/VariableBundle/Primitive/Primitive.cpp
        source/VariableBundle/Primitive/DataType.cpp
        source/ErrorBundle/ErrorInfo.cpp
        source/MethodBundle/Instruction/Instruction.cpp
        source/MemoryBundle/MemoryChunk.cpp
        source/MemoryBundle/MemoryChunkIf.cpp
        source/MethodBundle/Method.cpp
        source/ErrorBundle/ErrorLog.cpp
        source/MemoryBundle/VirtualMemory.cpp
        source/main.cpp
        source/MemoryBundle/Memory.cpp
        source/ORM/Object.cpp
        source/ORM/ObjectRepository.cpp
        source/ORM/ORM.cpp
        test/source/ORM/orm_test.cpp
        source/ORM/Relationship.cpp source/MethodBundle/Instruction/PushConstantInstruction.cpp include/MethodBundle/Instruction/PushConstantInstruction.h)

###################################################################
# Application Tests
###################################################################

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libstdc++")

set(TEST_FILES
        test/source/VariableBundle/Collection/collection_test.cpp
        test/include/VariableBundle/Collection/collection_test.h
        test/source/VariableBundle/Primitive/primitive_data_test.cpp
        test/source/MemoryBundle/memory_chunk_test.cpp
        test/include/MemoryBundle/memory_chunk_test.h
        test/source/MemoryBundle/virtual_memory_test.cpp
        test/include/MemoryBundle/virtual_memory_test.h
        test/test.cpp
        test/test.h source/VariableBundle/File/File.cpp include/VariableBundle/File/File.h test/source/VariableBundle/File/file_test.cpp test/include/VariableBundle/File/file_test.h source/VariableBundle/File/FileMode.cpp include/VariableBundle/File/FileMode.h test/source/MethodBundle/Instruction/create_instruction_test.cpp test/include/MethodBundle/Instruction/create_instruction_test.h include/MethodBundle/Instruction/OpCode.h source/MethodBundle/Instruction/CreateInstruction.cpp include/MethodBundle/Instruction/CreateInstruction.h source/MethodBundle/Instruction/AssignInstruction.cpp include/MethodBundle/Instruction/AssignInstruction.h test/include/MethodBundle/Instruction/create_and_assign_constant_instruction_test.h source/VariableBundle/Primitive/Int.cpp include/VariableBundle/Primitive/Int.h source/VariableBundle/Primitive/String.cpp include/VariableBundle/Primitive/String.h source/VariableBundle/Primitive/Float.cpp include/VariableBundle/Primitive/Float.h source/VariableBundle/Primitive/Char.cpp include/VariableBundle/Primitive/Char.h source/VariableBundle/Primitive/Bool.cpp include/VariableBundle/Primitive/Bool.h test/source/VariableBundle/Primitive/data_type_test.cpp test/include/VariableBundle/Primitive/data_type_test.h include/ORM/eObjectType.h source/VariableBundle/Value.cpp include/VariableBundle/Value.h source/VariableBundle/Null/Null.cpp include/VariableBundle/Null/Null.h source/VariableBundle/Var.cpp include/VariableBundle/Var.h source/ThreadBundle/Thread.cpp include/ThreadBundle/Thread.h source/InterpreterBundle/Interpreter.cpp include/InterpreterBundle/Interpreter.h include/ErrorBundle/eErrorStatus.h source/ORM/Relationships.cpp include/ORM/Relationships.h source/ORM/MasterRelationships.cpp include/ORM/MasterRelationships.h source/ORM/SlaveRelationships.cpp include/ORM/SlaveRelationships.h source/MethodBundle/Instruction/PushConstantInstruction.cpp include/MethodBundle/Instruction/PushConstantInstruction.h source/ConstantBundle/Constants.cpp include/ConstantBundle/Constants.h)

add_executable(runUnitTest
        ${INCLUDE_FILES}
        ${TEST_FILES}
        ${SOURCE_FILES})
