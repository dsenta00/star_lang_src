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

#ifndef RELATIONSHIP_H
#define RELATIONSHIP_H

#include "orm_fw.h"
#include "relationship_type.h"
#include <vector>
#include <string>
#include <functional>
#include <memory>

typedef enum {
    FOREACH_CONTINUE,
    FOREACH_IT2_REMOVED,
    FOREACH_IT1_REMOVED
} foreach_result;

typedef std::vector<object *> obj_vector;

/**
 * The relationship class.
 * Contains and manages all objects in relationship.
 */
class relationship : public obj_vector {
public:
    relationship(std::string relationship_name, relationship_type type);
    std::string &get_name();
    relationship_type get_type();
    object *find(const std::function<bool(object *)> &func);
    void sort(const std::function<bool(object *, object *)> &func);
    void for_each(const std::function<foreach_result(object *, object *)> &func);
    void add_object(object *o);
    void remove_object(object *o);
    object *front();
    object *back();
protected:
    std::string relationship_name;
    relationship_type type;
};

#endif // RELATIONSHIP_H
