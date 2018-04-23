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

#include <ORM/MasterRelationships.h>
#include <ORM/SlaveRelationships.h>
#include <ORM/Relationship.h>
#include <ORM/Object.h>
#include <ErrorBundle/ErrorLog.h>

MasterRelationships::MasterRelationships(Object *self) : Relationships(self)
{
}

void
MasterRelationships::clearObjects()
{
    for (auto &relationship : this->relationships)
    {
        Relationship *r = relationship.second.get();

        while (!r->empty())
        {
            Object *e = r->front();

            r->removeObject(e);
            e->getSlave()->remove(r->getName(), self);
        }
    }
}

void
MasterRelationships::clearObjects(std::string relationshipName)
{
    Relationship *r = this->get(std::move(relationshipName));

    if (!r)
    {
        return;
    }

    while (!r->empty())
    {
        Object *e = r->front();

        r->removeObject(e);
        e->getSlave()->remove(r->getName(), self);
    }
}

void
MasterRelationships::add(std::string relationshipName, Object *o)
{
    Relationship *r = this->get(relationshipName);

    if (!r)
    {
        ERROR_LOG_ADD(ERROR_ENTITY_UNKNOWN_RELATIONSHIP);
        return;
    }

    r->addObject(o);

    switch (r->getType())
    {
        case ONE_TO_MANY:
            o->getSlave()->init(relationshipName, ONE_TO_MANY);
            break;
        case ONE_TO_ONE:
            o->getSlave()->init(relationshipName, ONE_TO_ONE);
            break;
        default:
            return;
    }

    o->getSlave()->add(std::move(relationshipName), self);
}

void
MasterRelationships::remove(std::string relationshipName, Object *o)
{
    Relationship *r = this->get(std::move(relationshipName));

    if (!r)
    {
        return;
    }

    if (!o)
    {
        return;
    }

    r->removeObject(o);
    o->getSlave()->remove(r->getName(), self);
}

MasterRelationships::~MasterRelationships()
{
    this->clearObjects();
}
