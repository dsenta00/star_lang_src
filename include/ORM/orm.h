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

#include "entity_repository.h"
#include <string>
#include <functional>

/**
 * ORM interface.
 */
namespace orm {
    entity_repository *find_entity_repository(std::string entity_type);
    void add_entity_repository(std::string entity_type);
    entity *create(entity *data);
    void change_id(entity *e, std::string new_id);
    void destroy(entity *e);
    void sweep();
    entity *select(std::string entity_type, std::function<bool(entity *)> where);
    entity *select(std::string entity_type, std::string id);
    entity *get_first(std::string entity_type);
    void remove_entity_repository(std::string entity_type);
}

#define ORM_SELECT(__ENTITY_TYPE__, __WHERE__) \
  (__ENTITY_TYPE__ *)orm::select(#__ENTITY_TYPE__, [&] (entity *e) { \
  __ENTITY_TYPE__ *obj = (__ENTITY_TYPE__ *)e; \
  (void)obj; \
  return __WHERE__; \
})

#define ORM_DESTROY(__ENTITY__) \
  orm::destroy((entity *)__ENTITY__)

#endif // ORM_H
