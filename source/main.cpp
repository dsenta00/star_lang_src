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

#include "../tests/test.h"
#include "ORM/orm.h"
#include "box_virtual_memory.h"
#include <cstdlib>

/**
 * Main program.
 *
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char *argv[])
{
    (void) argc;
    (void) argv;

    orm::add_entity_repository("memory");
    orm::add_entity_repository("box_memory_chunk");
    orm::add_entity_repository("box_array");
    orm::add_entity_repository("box_virtual_memory");
    orm::add_entity_repository("box_data");
    orm::add_entity_repository("box_method");
    orm::add_entity_repository("instruction");

    /*
     * Create global virtual memory.
     */
    box_virtual_memory::create();

    run_tests();

    orm::remove_entity_repository("memory");
    orm::remove_entity_repository("box_memory_chunk");
    orm::remove_entity_repository("box_virtual_memory");
    orm::remove_entity_repository("box_data");
    orm::remove_entity_repository("box_array");
    orm::remove_entity_repository("box_method");
    orm::remove_entity_repository("instruction");

    return EXIT_SUCCESS;
}

