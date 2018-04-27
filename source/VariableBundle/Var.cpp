/*
 * Copyright 2018 Duje Senta
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

#include <ORM/ORM.h>
#include <ORM/MasterRelationships.h>
#include <VariableBundle/Value.h>
#include <VariableBundle/Var.h>
#include <VariableBundle/Primitive/Primitive.h>
#include <utility>

/**
 * The constructor.
 *
 * @param v
 */
Var::Var(std::string id, Value *v) : Object::Object(std::move(id))
{
    MasterRelationships *master = this->getMaster();

    master->init("val", ONE_TO_ONE);

    if (v == nullptr)
    {
        v = dynamic_cast<Value *>(ORM::getFirst(OBJECT_TYPE_NULL));
    }

    master->add("val", v);
}

/**
 * Create variable.
 *
 * @param container
 * @return
 */
Var *
Var::create(std::string id, Value *container)
{
    return dynamic_cast<Var *>(ORM::create(new Var(std::move(id), container)));
}

/**
 * Get container.
 *
 * @return
 */
Value *
Var::get()
{
    return dynamic_cast<Value *>(this->getMaster()->front("val"));
}

/**
 * Set container.
 *
 * @param v
 */
void
Var::set(Value *v)
{
    if (v == nullptr)
    {
        v = dynamic_cast<Value *>(ORM::getFirst(OBJECT_TYPE_NULL));
    }

    Value *v1 = this->get();
    Value *v2 = v;

    /*
     * +---------------------------------------+----------------------------------------------------+
     * |  Conditions:                          |  Actions:                                          |
     * +---------------------------------------+----------------------------------------------------+
     * | v1 ref?   v2 ref?   v1type == v2type  | v1Remove   v1 refs as v2    v1 create     v1 = v2 |
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
     *
     * Reference problem solved using Veitch diagram.
     */

    bool v1Remove = v1->isReference() || v2->isReference() || (v1->getObjectType() != v2->getObjectType());
    bool v1RefV2 = v2->isReference();
    bool v1Create = (v1->getObjectType() != v2->getObjectType()) && !v2->isReference();
    bool v1EqV2 = !v2->isReference();

    MasterRelationships *master = this->getMaster();

    if (v1Remove)
    {
        master->remove("val", v1);
    }

    if (v1RefV2)
    {
        master->add("val", v2);
    }

    if (v1Create)
    {
        v1 = Primitive::create(v2->getObjectType());
        master->add("val", v1);
    }

    if (v1EqV2)
    {
        *v1 = *v2;
    }
}

/**
 * @inherit
 */
eObjectType
Var::getObjectType()
{
    return OBJECT_TYPE_VARIABLE;
}