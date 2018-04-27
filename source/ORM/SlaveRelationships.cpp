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

#include <ORM/Object.h>
#include <ORM/Relationship.h>
#include <ORM/Relationships.h>
#include <ORM/SlaveRelationships.h>
#include <ORM/MasterRelationships.h>
#include <ErrorBundle/ErrorLog.h>

SlaveRelationships::SlaveRelationships(Object *self) : Relationships(self)
{}

/**
 * Add object to slave relationship.
 *
 * @param relationshipName
 * @param o
 */
void
SlaveRelationships::add(std::string relationshipName, Object *o)
{
    Relationship *r = this->get(std::move(relationshipName));

    if (!r)
    {
        ERROR_LOG_ADD(ERROR_ENTITY_UNKNOWN_RELATIONSHIP);
        return;
    }

    r->addObject(o);
}

void
SlaveRelationships::remove(std::string relationshipName, Object *o)
{
    Relationship *r = this->get(std::move(relationshipName));

    if (!r)
    {
        return;
    }

    r->removeObject(o);

    if (!this->hasRelations())
    {
        this->self->setMarked(true);
        this->self->getMaster()->clearObjects();
    }
}

/**
 * Notify through slave relationship that this object is being destroyed.
 */
void
SlaveRelationships::notifyDestroyed()
{
    for (auto &slaveRelationship : this->relationships)
    {
        Relationship *r = slaveRelationship.second.get();

        while (!r->empty())
        {
            /*
             * Tell master Object to remove this Object.
             */
            Object *e = r->front();
            e->getMaster()->remove(r->getName(), self);
        }
    }
}

SlaveRelationships::~SlaveRelationships()
{
    this->notifyDestroyed();
}
