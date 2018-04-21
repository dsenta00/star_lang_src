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

#include <ORM/Object.h>
#include <ORM/Relationship.h>
#include <ErrorBundle/ErrorLog.h>
#include <sstream>
#include <utility>

/**
 * The constructor.
 *
 * @param type
 * @param id
 */
Object::Object(const uint64_t id)
{
    this->marked = false;
    this->id = std::to_string(id);
}

/**
 * The constructor.
 *
 * @param type
 * @param id
 */
Object::Object(std::string id)
{
    this->marked = false;
    this->id = std::move(id);
}

/**
 * Get master relationship.
 *
 * @param relationshipName
 * @return relationship if exists, otherwise return NULL.
 */
Relationship *
Object::masterRelationshipGet(std::string relationshipName)
{
    auto it = this->masterRelationships.find(relationshipName);

    if (it != this->masterRelationships.end())
    {
        return (*it).second.get();
    }

    return nullptr;
}

/**
 * Add master relationship.
 *
 * @param relationshipName
 * @param type
 */
void
Object::masterRelationshipAdd(std::string relationshipName,
                              eRelationshipType type)
{
    if (this->masterRelationshipGet(relationshipName))
    {
        return;
    }

    RelationshipPtr rp(new Relationship(relationshipName, type));

    this->masterRelationships[relationshipName] = rp;
}

/**
 * Remove all relationships.
 */
void
Object::masterRelationshipsClearObjects()
{
    for (auto &masterRelationship : this->masterRelationships)
    {
        Relationship *r = masterRelationship.second.get();

        while (!r->empty())
        {
            Object *e = r->front();

            r->removeObject(e);
            e->slaveRelationshipRemoveObject(r->getName(), this);
        }
    }
}

/**
 *
 * @param relationshipName
 */
void
Object::masterRelationshipsClearObjects(std::string relationshipName)
{
    Relationship *r = this->masterRelationshipGet(std::move(relationshipName));

    if (!r)
    {
        return;
    }

    while (!r->empty())
    {
        Object *e = r->front();

        r->removeObject(e);
        e->slaveRelationshipRemoveObject(r->getName(), this);
    }
}

/**
 * Get object ID.
 *
 * @return object ID.
 */
std::string
Object::getId()
{
    return this->id;
}

/**
 * Get object ID.
 *
 * @return object ID.
 */
void
Object::setId(std::string newId)
{
    this->id = std::move(newId);
}

/**
 * Get marked.
 *
 * @return marked.
 */
bool
Object::getMarked()
{
    return this->marked;
}

/**
 * Set marked.
 *
 * @param marked
 */
void
Object::setMarked(bool marked)
{
    this->marked = marked;
}

/**
 * Add object to master relationship.
 *
 * @param relationshipName - relationship name.
 * @param o - the object.
 */
void
Object::masterRelationshipAddObject(std::string relationshipName, Object *o)
{
    Relationship *r = this->masterRelationshipGet(relationshipName);

    if (!r)
    {
        ERROR_LOG_ADD(ERROR_ENTITY_UNKNOWN_RELATIONSHIP);
        return;
    }

    r->addObject(o);

    switch (r->getType())
    {
        case ONE_TO_MANY:
            o->slaveRelationshipAdd(relationshipName, ONE_TO_MANY);
            break;
        case ONE_TO_ONE:
            o->slaveRelationshipAdd(relationshipName, ONE_TO_ONE);
            break;
        default:
            return;
    }

    o->slaveRelationshipAddObject(std::move(relationshipName), this);
}

/**
 * Remove object from master relationship.
 *
 * @param relationshipName - relationship name.
 * @param o - the object.
 */
void
Object::masterRelationshipRemoveObject(std::string relationshipName, Object *o)
{
    Relationship *r = this->masterRelationshipGet(std::move(relationshipName));

    if (!r)
    {
        return;
    }

    if (!o)
    {
        return;
    }

    r->removeObject(o);
    o->slaveRelationshipRemoveObject(r->getName(), this);
}

/**
 * Check if object has slave relations.
 *
 * @return true if have, otherwise return false.
 */
bool
Object::slaveRelationshipHasRelations()
{
    for (auto &slaveRelationship : this->slaveRelationships)
    {
        Relationship *r = slaveRelationship.second.get();

        if (!r->empty())
        {
            return true;
        }
    }

    return false;
}

/**
 * Notify this object that another object is removing
 * it from relationship.
 *
 * @param relationshipName - relationship name.
 * @param o - the object.
 */
void
Object::slaveRelationshipRemoveObject(std::string relationshipName, Object *o)
{
    Relationship *r = this->slaveRelationshipGet(std::move(relationshipName));

    if (!r)
    {
        return;
    }

    r->removeObject(o);

    if (!this->slaveRelationshipHasRelations())
    {
        this->setMarked(true);
        this->masterRelationshipsClearObjects();
    }
}

/**
 * @brief object::back
 * @param relationshipName
 * @return
 */
Object *
Object::masterRelationshipBack(std::string relationshipName)
{
    Relationship *r = this->masterRelationshipGet(std::move(relationshipName));

    if (!r)
    {
        ERROR_LOG_ADD(ERROR_ENTITY_UNKNOWN_RELATIONSHIP);
        return nullptr;
    }

    return r->back();
}

/**
 * @param relationshipName
 * @return
 */
Object *
Object::slaveRelationshipBack(std::string relationshipName)
{
    Relationship *r = this->slaveRelationshipGet(std::move(relationshipName));

    if (!r)
    {
        ERROR_LOG_ADD(ERROR_ENTITY_UNKNOWN_RELATIONSHIP);
        return nullptr;
    }

    return r->back();
}

/**
 * The destructor.
 */
Object::~Object()
{
    this->slaveRelationshipNotifyDestroyed();
    this->masterRelationshipsClearObjects();
}

/**
 * Add slave relationship.
 *
 * @param relationshipName
 * @param type
 */
void
Object::slaveRelationshipAdd(std::string relationshipName, eRelationshipType type)
{
    if (this->slaveRelationshipGet(relationshipName))
    {
        return;
    }

    RelationshipPtr rp(new Relationship(relationshipName, type));
    this->slaveRelationships[relationshipName] = rp;
}

/**
 * Get slave relationship.
 *
 * @param relationshipName
 * @return
 */
Relationship *
Object::slaveRelationshipGet(std::string relationshipName)
{
    auto it = this->slaveRelationships.find(relationshipName);

    if (it != this->slaveRelationships.end())
    {
        return (*it).second.get();
    }

    return nullptr;
}

/**
 * Add object to slave relationship.
 *
 * @param relationshipName
 * @param o
 */
void
Object::slaveRelationshipAddObject(std::string relationshipName, Object *o)
{
    Relationship *r = this->slaveRelationshipGet(std::move(relationshipName));

    if (!r)
    {
        ERROR_LOG_ADD(ERROR_ENTITY_UNKNOWN_RELATIONSHIP);
        return;
    }

    r->addObject(o);
}

/**
 * Notify through slave relationship that this object is being destroyed.
 */
void
Object::slaveRelationshipNotifyDestroyed()
{
    for (auto &slaveRelationship : this->slaveRelationships)
    {
        Relationship *r = slaveRelationship.second.get();

        while (!r->empty())
        {
            /*
             * Tell master Object to remove this Object.
             */
            Object *e = r->front();
            e->masterRelationshipRemoveObject(r->getName(), this);
        }
    }
}

/**
 * Get front from master relationship.
 *
 * @param relationshipName
 * @return
 */
Object *
Object::masterRelationshipFront(std::string relationshipName)
{
    Relationship *r = this->masterRelationshipGet(std::move(relationshipName));

    if (!r)
    {
        ERROR_LOG_ADD(ERROR_ENTITY_UNKNOWN_RELATIONSHIP);
        return nullptr;
    }

    return r->front();
}