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

#include <variable/value.h>
#include <ORM/orm.h>
#include <variable/primitive_data/primitive_data.h>

#include <utility>
#include "variable/var.h"

/**
 * The constructor.
 *
 * @param v
 */
var::var(std::string id, value *v) : object::object(std::move(id))
{
    this->master_relationship_add("val", ONE_TO_ONE);

    if (v == nullptr)
    {
        v = dynamic_cast<value *>(orm::get_first(OBJECT_TYPE_NULL));
    }

    this->master_relationship_add_object("val", v);
}

/**
 * Create variable.
 *
 * @param container
 * @return
 */
var *
var::create(std::string id, value *container)
{
    return dynamic_cast<var *>(orm::create(new var(std::move(id), container)));
}

/**
 * Get container.
 *
 * @return
 */
value *
var::get()
{
    return dynamic_cast<value *>(this->master_relationship_back("val"));
}

/**
 * Set container.
 *
 * @param v
 */
void
var::set(value *v)
{
    if (v == nullptr)
    {
        v = dynamic_cast<value *>(orm::get_first(OBJECT_TYPE_NULL));
    }

    value *v1 = this->get();
    value *v2 = v;

    /*
     * +---------------------------------------+----------------------------------------------------+
     * |  Conditions:                          |  Actions:                                          |
     * +---------------------------------------+----------------------------------------------------+
     * | v1 ref?   v2 ref?   v1type == v2type  | v1_remove   v1 refs as v2    v1 create     v1 = v2 |
     * +---------------------------------------+----------------------------------------------------+
     * |   0         0               0         |     1              0              1           1    |
     * |   0         0               1         |     0              0              0           1    |
     * |   0         1               0         |     1              1              0           0    |
     * |   0         1               1         |     R              R              R           R    |
     * |   1         0               0         |     1              0              1           1    |
     * |   1         0               1         |     R              R              R           R    |
     * |   1         1               0         |     1              1              0           0    |
     * |   1         1               1         |     1              1              0           0    |
     * +---------------------------------------+----------------------------------------------------+
     *    Reference problem solved using Veitch diagram.
     */

    bool v1_remove = v1->is_reference() || v2->is_reference() || (v1->get_object_type() != v2->get_object_type());
    bool v1_ref_v2 = v2->is_reference();
    bool v1_create = (v1->get_object_type() != v2->get_object_type()) && !v2->is_reference();
    bool v1_eq_v2 = !v2->is_reference();

    if (v1_remove)
    {
        this->master_relationship_remove_object("val", v1);
    }

    if (v1_ref_v2)
    {
        this->master_relationship_add_object("val", v2);
    }

    if (v1_create)
    {
        v1 = primitive_data::create(v2->get_object_type());
        this->master_relationship_add_object("val", v1);
    }

    if (v1_eq_v2)
    {
        *v1 = *v2;
    }
}

/**
 * @inherit
 */
object_type
var::get_object_type()
{
    return OBJECT_TYPE_VARIABLE;
}


