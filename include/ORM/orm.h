/*
 * Copyright 2017 Duje Senta
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

#pragma once

#include "object_repository.h"
#include "object_type.h"
#include <string>
#include <functional>

/**
 * ORM interface.
 */
namespace orm {
    object_repository *find_object_repository(object_type type);
    void add_object_repository(object_type type);
    object *create(object *o);
    void change_id(object *o, std::string new_id);
    void destroy(object *o);
    void sweep();
    object *select(object_type type, std::function<bool(object *)> where);
    object *select(object_type type, std::string id);
    object *get_first(object_type type);
    void remove_object_repository(object_type type);
    void remove_all_repositories();
}

#define ORM_DESTROY(__OBJ__) \
  orm::destroy((object *)(__OBJ__))