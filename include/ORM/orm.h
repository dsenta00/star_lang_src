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

#ifndef ORM_H
#define ORM_H

#include "object_repository.h"
#include <string>
#include <functional>

/**
 * ORM interface.
 */
namespace orm {
    object_repository *find_object_repository(std::string object_type);
    void add_object_repository(std::string object_type);
    object *create(object *o);
    void change_id(object *o, std::string new_id);
    void destroy(object *o);
    void sweep();
    object *select(std::string object_type, std::function<bool(object *)> where);
    object *select(std::string object_type, std::string id);
    object *get_first(std::string object_type);
    void remove_object_repository(std::string object_type);
}

#define ORM_SELECT(__OBJ_TYPE__, __WHERE__) \
  (__OBJ_TYPE__ *)orm::select(#__OBJ_TYPE__, [&] (object *e) { \
  __OBJ_TYPE__ *obj = (__OBJ_TYPE__ *)e; \
  (void)obj; \
  return __WHERE__; \
})

#define ORM_DESTROY(__OBJ__) \
  orm::destroy((object *)__OBJ__)

#endif // ORM_H
