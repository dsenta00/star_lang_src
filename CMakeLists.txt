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
        include/box_array.h
        include/box_assert.h
        include/box_data.h
        include/box_data_test.h
        include/box_data_type.h
        include/box_error.h
        include/box_fw.h
        include/box_instruction.h
        include/box_memory_chunk.h
        include/box_memory_chunk_if.h
        include/box_method.h
        include/box_monitor.h
        include/box_virtual_memory.h
        include/memory.h
        include/ORM/entity.h
        include/ORM/entity_repository.h
        include/ORM/orm.h
        include/ORM/orm_fw.h
        include/ORM/orm_test.h
        include/ORM/relationship.h
        include/ORM/relationship_type.h)

set(SOURCE_FILES
        source/box_array.cpp
        source/box_assert.cpp
        source/box_data.cpp
        source/box_data_type.cpp
        source/box_error.cpp
        source/box_instruction.cpp
        source/box_memory_chunk.cpp
        source/box_memory_chunk_if.cpp
        source/box_method.cpp
        source/box_monitor.cpp
        source/box_virtual_memory.cpp
        source/main.cpp
        source/memory.cpp
        source/ORM/entity.cpp
        source/ORM/entity_repository.cpp
        source/ORM/orm.cpp
        source/ORM/orm_test.cpp
        source/ORM/relationship.cpp)

###################################################################
# Application Tests
###################################################################

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libstdc++")

set(TEST_FILES
        tests/box_array_test.cpp
        tests/box_array_test.h
        tests/box_data_test.cpp
        tests/box_memory_chunk_test.cpp
        tests/box_memory_chunk_test.h
        tests/box_virtual_memory_test.cpp
        tests/box_virtual_memory_test.h
        tests/test.cpp
        tests/test.h)

add_executable(runUnitTest
        ${INCLUDE_FILES}
        ${TEST_FILES}
        ${SOURCE_FILES})
