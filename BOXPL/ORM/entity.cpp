#include "entity.h"
#include "relationship.h"
#include <algorithm>
#include <sstream>
#include <string>
#include "../box_monitor.h"

/**
 * @brief entity::entity
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
 * @brief entity::entity
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
 * @brief entity::getRelationship
 * @param relationship_name
 * @return
 */
relationship *
entity::getRelationship(std::string relationship_name)
{
  auto it = this->relation.find(relationship_name);

  if (it != this->relation.end())
  {
    return (*it).second.get();
  }

  return NULL;
}

/**
 * @brief entity::addRelationship
 * @param relationship_name
 * @param type
 */
void
entity::addRelationship(std::string relationship_name, relationship_type type)
{
  if (this->getRelationship(relationship_name))
  {
    return;
  }

  relationship_p rp(new relationship((entity *)this, relationship_name, type));

  this->relation[relationship_name] = rp;
}

/**
 * @brief entity::removeRelationship
 * @param relationship_name
 */
void
entity::removeRelationship(std::string relationship_name)
{
  auto it = this->relation.find(relationship_name);

  if (it != this->relation.end())
  {
    this->relation.erase(it);
  }
}

/**
 * @brief entity::removeAllRelationships
 */
void
entity::removeAllRelationships()
{
  for (auto it = this->relation.begin();
       it != this->relation.end();
       it++)
  {
    relationship *r = (*it).second.get();

    while (r->numOfEntities())
    {
      entity *e = r->front();

      r->removeEntity(e);
      e->notifyRemove(r->getName(), this);
    }
  }
}

/**
 * @brief entity::getType
 * @return
 */
std::string
entity::getType()
{
  return this->type;
}

/**
 * @brief entity::getId
 * @return
 */
std::string
entity::getId()
{
  return this->id;
}

/**
 * @brief entity::getMarked
 * @return
 */
bool
entity::getMarked()
{
  return this->marked;
}

/**
 * @brief entity::setMarked
 * @param marked
 */
void
entity::setMarked(bool marked)
{
  this->marked = marked;
}

/**
 * @brief entity::addEntity
 * @param relationship_name
 * @param e
 * @return
 */
entity *
entity::addEntity(std::string relationship_name, entity *e)
{
  relationship *r = this->getRelationship(relationship_name);

  if (!r)
  {
    BOX_ERROR(ERROR_BOX_ENTITY_UNKNOWN_RELATIONSHIP);
    return this;
  }

  r->addEntity(e);

  return this;
}

/**
 * @brief entity::addEntities
 * @param relationship_name
 * @param er
 * @return
 */
entity*
entity::addEntities(std::string relationship_name, relationship *er)
{
  relationship *r = this->getRelationship(relationship_name);

  if (!r)
  {
    BOX_ERROR(ERROR_BOX_ENTITY_UNKNOWN_RELATIONSHIP);
    return this;
  }

  r->addEntities(er);

  return this;
}

/**
 * @brief entity::removeEntity
 * @param relationship_name
 * @param e
 * @return
 */
entity *
entity::removeEntity(std::string relationship_name, entity *e)
{
  relationship *r = this->getRelationship(relationship_name);

  if (!r)
  {
    return this;
  }

  if (!e)
  {
    return this;
  }

  r->removeEntity(e);
  e->notifyRemove(r->getName(), this);

  if (!this->hasRelations())
  {
    this->setMarked(true);
  }

  return this;
}

/**
 * @brief entity::hasRelations
 * @return
 */
bool
entity::hasRelations()
{
  for (auto it = this->relation.begin();
       it != this->relation.end();
       it++)
  {
    relationship *r = (*it).second.get();

    if(r->numOfEntities() > 0)
    {
      return true;
    }
  }

  return false;
}

/**
 * Remove entity from all relationships.
 *
 * @param e
 * @return this
 */
entity *
entity::removeEntity(entity *e)
{
  for (auto it = this->relation.begin();
       it != this->relation.end();
       it++)
  {
    relationship *r = (*it).second.get();
    r->removeEntity(e);
    e->notifyRemove(r->getName(), this);
  }

  if (!this->hasRelations())
  {
    this->setMarked(true);
  }

  return this;
}

/**
 * @brief entity::notifyRemove
 * @param relationship_name
 * @param e
 */
void
entity::notifyRemove(std::string relationship_name, entity *e)
{
  relationship *r = this->getRelationship(relationship_name);

  if (!r)
  {
    BOX_ERROR(ERROR_BOX_ENTITY_UNKNOWN_RELATIONSHIP);
    return;
  }

  r->removeEntity(e);

  if (!this->hasRelations())
  {
    this->setMarked(true);
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
  relationship *r = this->getRelationship(relationship_name);

  if (!r)
  {
    BOX_ERROR(ERROR_BOX_ENTITY_UNKNOWN_RELATIONSHIP);
    return this;
  }

  return r->back();

  return NULL;
}

/**
 * @brief entity::~entity
 */
entity::~entity()
{
  this->removeAllRelationships();
}
