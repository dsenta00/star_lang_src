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

#include "ORM/object.h"
#include "ORM/relationship.h"
#include <sstream>
#include "error_log.h"

/**
 * The constructor.
 *
 * @param type
 * @param id
 */
object::object(std::string type, const uint64_t id)
{
    this->marked = false;
    this->type = std::move(type);
    this->id = std::to_string(id);
}

/**
 * The constructor.
 *
 * @param type
 * @param id
 */
object::object(std::string type, std::string id)
{
    this->marked = false;
    this->type = std::move(type);
    this->id = std::move(id);
}

/**
 * Get master relationship.
 *
 * @param relationship_name
 * @return relationship if exists, otherwise return NULL.
 */
relationship *
object::master_relationship_get(std::string relationship_name)
{
    auto it = this->master_relationships.find(relationship_name);

    if (it != this->master_relationships.end())
    {
        return (*it).second.get();
    }

    return nullptr;
}

/**
 * Add master relationship.
 *
 * @param relationship_name
 * @param type
 */
void
object::master_relationship_add(std::string relationship_name,
                                relationship_type type)
{
    if (this->master_relationship_get(relationship_name))
    {
        return;
    }

    relationship_p rp(new relationship(relationship_name, type));

    this->master_relationships[relationship_name] = rp;
}

/**
 * Remove all relationships.
 */
void
object::master_relationships_clear_objects()
{
    for (auto &master_relationship : this->master_relationships)
    {
        relationship *r = master_relationship.second.get();

        while (r->size())
        {
            object *e = r->front();

            r->remove_object(e);
            e->slave_relationship_remove_object(r->get_name(), this);
        }
    }
}

/**
 * Get entity type.
 *
 * @return entity type.
 */
std::string
object::get_object_type()
{
    return this->type;
}

/**
 * Get entity ID.
 *
 * @return entity ID.
 */
std::string
object::get_id()
{
    return this->id;
}

/**
 * Get entity ID.
 *
 * @return entity ID.
 */
void
object::set_id(std::string new_id)
{
    this->id = std::move(new_id);
}

/**
 * Get marked.
 *
 * @return marked.
 */
bool
object::get_marked()
{
    return this->marked;
}

/**
 * Set marked.
 *
 * @param marked
 */
void
object::set_marked(bool marked)
{
    this->marked = marked;
}

/**
 * Add entity to master relationship.
 *
 * @param relationship_name - relationship name.
 * @param o - the entity.
 */
void
object::master_relationship_add_object(std::string relationship_name,
                                       object *o)
{
    relationship *r = this->master_relationship_get(relationship_name);

    if (!r)
    {
        ERROR_LOG_ADD(ERROR_ENTITY_UNKNOWN_RELATIONSHIP);
        return;
    }

    r->add_object(o);

    switch (r->get_type())
    {
        case ONE_TO_MANY:
            o->slave_relationship_add(relationship_name, ONE_TO_MANY);
            break;
        case ONE_TO_ONE:
            o->slave_relationship_add(relationship_name, ONE_TO_ONE);
            break;
        default:
            return;
    }

    o->slave_relationship_add_entity(relationship_name, this);
}

/**
 * Remove entity from master relationship.
 *
 * @param relationship_name - relationship name.
 * @param o - the entity.
 */
void
object::master_relationship_remove_object(std::string relationship_name,
                                          object *o)
{
    relationship *r = this->master_relationship_get(std::move(relationship_name));

    if (!r)
    {
        return;
    }

    if (!o)
    {
        return;
    }

    r->remove_object(o);
    o->slave_relationship_remove_object(r->get_name(), this);
}

/**
 * Check if entity has relations.
 *
 * @return true if have, otherwise return false.
 */
bool
object::slave_relationship_have_relations()
{
    for (auto &slave_relationship : this->slave_relationships)
    {
        relationship *r = slave_relationship.second.get();

        if (r->size() > 0)
        {
            return true;
        }
    }

    return false;
}

/**
 * Notify this entity that another entity is removing
 * it from relationship.
 *
 * @param relationship_name - relationship name.
 * @param o - the entity.
 */
void
object::slave_relationship_remove_object(std::string relationship_name,
                                         object *o)
{
    relationship *r = this->slave_relationship_get(std::move(relationship_name));

    if (!r)
    {
        return;
    }

    r->remove_object(o);

    if (!this->slave_relationship_have_relations())
    {
        this->set_marked(true);
        this->master_relationships_clear_objects();
    }
}

/**
 * @brief entity::back
 * @param relationship_name
 * @return
 */
object *
object::master_relationship_back(std::string relationship_name)
{
    relationship *r = this->master_relationship_get(std::move(relationship_name));

    if (!r)
    {
        ERROR_LOG_ADD(ERROR_ENTITY_UNKNOWN_RELATIONSHIP);
        return nullptr;
    }

    return r->back();
}

/**
 * @brief entity::back
 * @param relationship_name
 * @return
 */
object *
object::slave_relationship_back(std::string relationship_name)
{
    relationship *r = this->slave_relationship_get(std::move(relationship_name));

    if (!r)
    {
        ERROR_LOG_ADD(ERROR_ENTITY_UNKNOWN_RELATIONSHIP);
        return nullptr;
    }

    return r->back();
}

/**
 * @brief entity::~entity
 */
object::~object()
{
    this->master_relationships_clear_objects();
}

void
object::slave_relationship_add(std::string relationship_name, relationship_type type)
{
    if (this->slave_relationship_get(relationship_name))
    {
        return;
    }

    relationship_p rp(new relationship(relationship_name, type));
    this->slave_relationships[relationship_name] = rp;
}

relationship *
object::slave_relationship_get(std::string relationship_name)
{
    auto it = this->slave_relationships.find(relationship_name);

    if (it != this->slave_relationships.end())
    {
        return (*it).second.get();
    }

    return nullptr;
}

void
object::slave_relationship_add_entity(std::string relationship_name, object *o)
{
    relationship *r = this->slave_relationship_get(std::move(relationship_name));

    if (!r)
    {
        ERROR_LOG_ADD(ERROR_ENTITY_UNKNOWN_RELATIONSHIP);
        return;
    }

    r->add_object(o);
}

void
object::slave_relationship_notify_destroyed()
{
    for (auto &slave_relationship : this->slave_relationships)
    {
        relationship *r = slave_relationship.second.get();

        while (r->size())
        {
            /*
             * Tell master object to remove this object.
             */
            object *e = r->front();
            e->master_relationship_remove_object(r->get_name(), this);
        }
    }
}