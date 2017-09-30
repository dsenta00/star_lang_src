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
 * Get master relationship.
 *
 * @param relationship_name
 * @return relationship if exists, otherwise return NULL.
 */
relationship *
entity::master_relationship_get(std::string relationship_name)
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
entity::master_relationship_add(std::string relationship_name,
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
entity::master_relationships_clear_entities()
{
  for (auto it = this->master_relationships.begin();
       it != this->master_relationships.end();
       it++)
  {
    relationship *r = (*it).second.get();

    while (r->num_of_entities())
    {
      entity *e = r->front();

      r->remove_entity(e);
      e->slave_relationship_remove_entity(r->get_name(), this);
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
 * Get entity ID.
 *
 * @return entity ID.
 */
void
entity::set_id(std::string new_id)
{
  this->id = new_id;
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
 * Add entity to master relationship.
 *
 * @param relationship_name - relationship name.
 * @param e - the entity.
 */
void
entity::master_relationship_add_entity(std::string relationship_name,
                                       entity *e)
{
  relationship *r = this->master_relationship_get(relationship_name);

  if (!r)
  {
    BOX_ERROR(ERROR_BOX_ENTITY_UNKNOWN_RELATIONSHIP);
    return;
  }

  r->add_entity(e);

  switch (r->get_type())
  {
    case ONE_TO_MANY:
      e->slave_relationship_add(relationship_name, ONE_TO_MANY);
      break;
    case ONE_TO_ONE:
      e->slave_relationship_add(relationship_name, ONE_TO_ONE);
      break;
    default:
      return;
  }

  e->slave_relationship_add_entity(relationship_name, this);
}

/**
 * Remove entity from master relationship.
 *
 * @param relationship_name - relationship name.
 * @param e - the entity.
 */
void
entity::master_relationship_remove_entity(std::string relationship_name,
                                          entity *e)
{
  relationship *r = this->master_relationship_get(relationship_name);

  if (!r)
  {
    return;
  }

  if (!e)
  {
    return;
  }

  r->remove_entity(e);
  e->slave_relationship_remove_entity(r->get_name(), this);
}

/**
 * Check if entity has relations.
 *
 * @return true if have, otherwise return false.
 */
bool
entity::slave_relationship_have_relations()
{
  for (auto it = this->slave_relationships.begin();
       it != this->slave_relationships.end();
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
entity::slave_relationship_remove_entity(std::string relationship_name,
                                         entity *e)
{
  relationship *r = this->slave_relationship_get(relationship_name);

  if (!r)
  {
    return;
  }

  r->remove_entity(e);

  if (!this->slave_relationship_have_relations())
  {
    this->set_marked(true);
    this->master_relationships_clear_entities();
  }
}

/**
 * @brief entity::back
 * @param relationship_name
 * @return
 */
entity *
entity::master_relationship_back(std::string relationship_name)
{
  relationship *r = this->master_relationship_get(relationship_name);

  if (!r)
  {
    BOX_ERROR(ERROR_BOX_ENTITY_UNKNOWN_RELATIONSHIP);
    return nullptr;
  }

  return r->back();
}

/**
 * @brief entity::back
 * @param relationship_name
 * @return
 */
entity *
entity::slave_relationship_back(std::string relationship_name)
{
  relationship *r = this->slave_relationship_get(relationship_name);

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
  this->master_relationships_clear_entities();
}

void
entity::slave_relationship_add(std::string relationship_name, relationship_type type)
{
  if (this->slave_relationship_get(relationship_name))
  {
    return;
  }

  relationship_p rp(new relationship(relationship_name, type));
  this->slave_relationships[relationship_name] = rp;
}

relationship *
entity::slave_relationship_get(std::string relationship_name)
{
  auto it = this->slave_relationships.find(relationship_name);

  if (it != this->slave_relationships.end())
  {
    return (*it).second.get();
  }

  return nullptr;
}

void
entity::slave_relationship_add_entity(std::string relationship_name, entity *e)
{
  relationship *r = this->slave_relationship_get(relationship_name);

  if (!r)
  {
    BOX_ERROR(ERROR_BOX_ENTITY_UNKNOWN_RELATIONSHIP);
    return;
  }

  r->add_entity(e);
}

void
entity::slave_relationship_notify_destroyed()
{
  for (auto it = this->slave_relationships.begin();
       it != this->slave_relationships.end();
       it++)
  {
    relationship *r = (*it).second.get();

    while (r->num_of_entities())
    {
      /*
       * Tell master entity to remove this entity.
       */
      entity *e = r->front();
      e->master_relationship_remove_entity(r->get_name(), this);
    }
  }
}
