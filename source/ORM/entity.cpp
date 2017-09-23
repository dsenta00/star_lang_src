#include "ORM/entity.h"
#include "ORM/relationship.h"
#include <sstream>
#include "box_monitor.h"

/**
 * The constructor.
 *
 * @param type
 * @param id
 */
entity::entity(std::string type, const uint64_t id)
{
  this->marked = false;
  this->type = type;
  this->id = std::to_string(id);
}

/**
 * The constructor.
 *
 * @param type
 * @param id
 */
entity::entity(std::string type, std::string id)
{
  this->marked = false;
  this->type = type;
  this->id = id;
}

/**
 * Get relationship.
 *
 * @param relationship_name
 * @return relationship if exists, otherwise return NULL.
 */
relationship *
entity::get_relationship(std::string relationship_name)
{
  auto it = this->relation.find(relationship_name);

  if (it != this->relation.end())
  {
    return (*it).second.get();
  }

  return nullptr;
}

/**
 * Add relationship.
 *
 * @param relationship_name
 * @param type
 */
void
entity::add_relationship(std::string relationship_name,
                         relationship_type type)
{
  if (this->get_relationship(relationship_name))
  {
    return;
  }

  relationship_p rp(new relationship(relationship_name, type));

  this->relation[relationship_name] = rp;
}

/**
 * Remove all relationships.
 */
void
entity::remove_all_relationships()
{
  for (auto it = this->relation.begin();
       it != this->relation.end();
       it++)
  {
    relationship *r = (*it).second.get();

    while (r->num_of_entities())
    {
      entity *e = r->front();

      r->remove_entity(e);
      e->notify_remove(r->get_name(), this);
    }
  }
}

/**
 * Get entity type.
 *
 * @return entity type.
 */
std::string
entity::get_entity_type()
{
  return this->type;
}

/**
 * Get entity ID.
 *
 * @return entity ID.
 */
std::string
entity::get_id()
{
  return this->id;
}

/**
 * Get marked.
 *
 * @return marked.
 */
bool
entity::get_marked()
{
  return this->marked;
}

/**
 * Set marked.
 *
 * @param marked
 */
void
entity::set_marked(bool marked)
{
  this->marked = marked;
}

/**
 * Add entity to relationship.
 *
 * @param relationship_name - relationship name.
 * @param e - the entity.
 */
void
entity::add_entity(std::string relationship_name, entity *e)
{
  relationship *r = this->get_relationship(relationship_name);

  if (!r)
  {
    BOX_ERROR(ERROR_BOX_ENTITY_UNKNOWN_RELATIONSHIP);
    return;
  }

  r->add_entity(e);
}

/**
 * Remove entity from relationship.
 *
 * @param relationship_name - relationship name.
 * @param e - the entity.
 */
void
entity::remove_entity(std::string relationship_name, entity *e)
{
  relationship *r = this->get_relationship(relationship_name);

  if (!r)
  {
    return;
  }

  if (!e)
  {
    return;
  }

  r->remove_entity(e);
  e->notify_remove(r->get_name(), this);

  if (!this->have_relations())
  {
    this->set_marked(true);
  }
}

/**
 * Check if entity has relations.
 *
 * @return true if have, otherwise return false.
 */
bool
entity::have_relations()
{
  for (auto it = this->relation.begin();
       it != this->relation.end();
       it++)
  {
    relationship *r = (*it).second.get();

    if (r->num_of_entities() > 0)
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
 * @param e - the entity.
 */
void
entity::notify_remove(std::string relationship_name, entity *e)
{
  relationship *r = this->get_relationship(relationship_name);

  if (!r)
  {
    return;
  }

  r->remove_entity(e);

  if (!this->have_relations())
  {
    this->set_marked(true);
  }
}

/**
 * @brief entity::back
 * @param relationship_name
 * @return
 */
entity *
entity::back(std::string relationship_name)
{
  relationship *r = this->get_relationship(relationship_name);

  if (!r)
  {
    BOX_ERROR(ERROR_BOX_ENTITY_UNKNOWN_RELATIONSHIP);
    return nullptr;
  }

  return r->back();
}

/**
 * @brief entity::~entity
 */
entity::~entity()
{
  this->remove_all_relationships();
}
