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

#include <ORM/Relationships.h>
#include <ORM/Relationship.h>
#include <ErrorBundle/ErrorLog.h>

Relationships::Relationships(Object *self)
{
    this->self = self;
}


Relationship *
Relationships::get(std::string relationshipName)
{
    auto it = this->relationships.find(relationshipName);

    if (it != this->relationships.end())
    {
        return (*it).second.get();
    }

    return nullptr;
}

void
Relationships::init(std::string relationshipName, eRelationshipType type)
{
    if (this->get(relationshipName))
    {
        return;
    }

    RelationshipPtr rp(new Relationship(relationshipName, type));

    this->relationships[relationshipName] = rp;
}

Object *
Relationships::front(std::string relationshipName)
{
    Relationship *r = this->get(std::move(relationshipName));

    if (!r)
    {
        ERROR_LOG_ADD(ERROR_ENTITY_UNKNOWN_RELATIONSHIP);
        return nullptr;
    }

    return r->front();
}

Object *
Relationships::back(std::string relationshipName)
{
    Relationship *r = this->get(std::move(relationshipName));

    if (!r)
    {
        ERROR_LOG_ADD(ERROR_ENTITY_UNKNOWN_RELATIONSHIP);
        return nullptr;
    }

    return r->back();
}

bool
Relationships::hasRelations()
{
    for (auto &relationship : this->relationships)
    {
        Relationship *r = relationship.second.get();

        if (!r->empty())
        {
            return true;
        }
    }

    return false;
}
