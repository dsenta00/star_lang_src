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
        include/collection.h
        tests/test_assert.h
        include/primitive_data.h
        tests/primitive_data_test.h
        include/data_type.h
        include/error_info.h
        include/fw_decl.h
        include/instruction.h
        include/memory_chunk.h
        include/memory_chunk_if.h
        include/method.h
        include/error_log.h
        include/virtual_memory.h
        include/memory.h
        include/ORM/object.h
        include/ORM/object_repository.h
        include/ORM/orm.h
        include/ORM/orm_fw.h
        tests/orm_test.h
        include/ORM/relationship.h
        include/ORM/relationship_type.h)

set(SOURCE_FILES
        source/collection.cpp
        tests/test_assert.cpp
        source/primitive_data.cpp
        source/data_type.cpp
        source/error_info.cpp
        source/instruction.cpp
        source/memory_chunk.cpp
        source/memory_chunk_if.cpp
        source/method.cpp
        source/error_log.cpp
        source/virtual_memory.cpp
        source/main.cpp
        source/memory.cpp
        source/ORM/object.cpp
        source/ORM/object_repository.cpp
        source/ORM/orm.cpp
        tests/orm_test.cpp
        source/ORM/relationship.cpp)

###################################################################
# Application Tests
###################################################################

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libstdc++")

set(TEST_FILES
        tests/collection_test.cpp
        tests/collection_test.h
        tests/primitive_data_test.cpp
        tests/memory_chunk_test.cpp
        tests/memory_chunk_test.h
        tests/virtual_memory_test.cpp
        tests/virtual_memory_test.h
        tests/test.cpp
        tests/test.h source/file.cpp include/file.h tests/file_test.cpp tests/file_test.h source/file_mode.cpp include/file_mode.h)

add_executable(runUnitTest
        ${INCLUDE_FILES}
        ${TEST_FILES}
        ${SOURCE_FILES})
