#
# Copyright 2017 Duje Senta, Tomislav Radanovic
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
        include/variable/collection/collection.h
        test/test_assert.h
        include/variable/primitive_data/primitive_data.h
        test/include/variable/primitive_data/primitive_data_test.h
        include/variable/primitive_data/data_type.h
        include/error_handler/error_info.h
        include/fw_decl.h
        include/method/instructions/abstract_instruction.h
        include/memory_handler/memory_chunk.h
        include/memory_handler/memory_chunk_if.h
        include/method/method.h
        include/error_handler/error_log.h
        include/memory_handler/virtual_memory.h
        include/memory_handler/memory.h
        include/ORM/object.h
        include/ORM/object_repository.h
        include/ORM/orm.h
        include/ORM/orm_fw.h
        test/include/ORM/orm_test.h
        include/ORM/relationship.h
        include/ORM/relationship_type.h)

set(SOURCE_FILES
        source/variable/collection/collection.cpp
        test/test_assert.cpp
        source/variable/primitive_data/primitive_data.cpp
        source/variable/primitive_data/data_type.cpp
        source/error_handler/error_info.cpp
        source/method/instructions/abstract_instruction.cpp
        source/memory_handler/memory_chunk.cpp
        source/memory_handler/memory_chunk_if.cpp
        source/method/method.cpp
        source/error_handler/error_log.cpp
        source/memory_handler/virtual_memory.cpp
        source/main.cpp
        source/memory_handler/memory.cpp
        source/ORM/object.cpp
        source/ORM/object_repository.cpp
        source/ORM/orm.cpp
        test/source/ORM/orm_test.cpp
        source/ORM/relationship.cpp)

###################################################################
# Application Tests
###################################################################

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libstdc++")

set(TEST_FILES
        test/source/variable/collection/collection_test.cpp
        test/include/variable/collection/collection_test.h
        test/source/variable/primitive_data/primitive_data_test.cpp
        test/source/memory_handler/memory_chunk_test.cpp
        test/include/memory_handler/memory_chunk_test.h
        test/source/memory_handler/virtual_memory_test.cpp
        test/include/memory_handler/virtual_memory_test.h
        test/test.cpp
        test/test.h source/variable/file/file.cpp include/variable/file/file.h test/source/variable/file/file_test.cpp test/include/variable/file/file_test.h source/variable/file/file_mode.cpp include/variable/file/file_mode.h test/source/method/instructions/create_instruction_test.cpp test/include/method/instructions/create_instruction_test.h include/method/instructions/op_code.h source/method/instructions/create_instruction.cpp include/method/instructions/create_instruction.h source/method/instructions/create_and_assign_constant_instruction.cpp include/method/instructions/create_and_assign_constant_instruction.h source/method/instructions/create_and_assign_object_instruction.cpp include/method/instructions/create_and_assign_object_instruction.h source/method/instructions/pop_and_store_instruction.cpp include/method/instructions/pop_and_store_instruction.h test/source/method/instructions/create_and_assign_constant_instruction_test.cpp test/include/method/instructions/create_and_assign_constant_instruction_test.h source/variable/primitive_data/int_data.cpp include/variable/primitive_data/int_data.h source/variable/primitive_data/string_data.cpp include/variable/primitive_data/string_data.h source/variable/primitive_data/float_data.cpp include/variable/primitive_data/float_data.h source/variable/primitive_data/char_data.cpp include/variable/primitive_data/char_data.h source/variable/primitive_data/bool_data.cpp include/variable/primitive_data/bool_data.h test/source/variable/primitive_data/data_type_test.cpp test/include/variable/primitive_data/data_type_test.h include/ORM/object_type.h source/variable/var.cpp include/variable/var.h)

add_executable(runUnitTest
        ${INCLUDE_FILES}
        ${TEST_FILES}
        ${SOURCE_FILES})
