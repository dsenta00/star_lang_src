#include "relationship.h"
#include "entity.h"
#include "stdexcept"
#include "../box_monitor.h"
#include <algorithm>

/**
 * The constructor.
 *
 * @param parent_entity - relationship owner.
 * @param relationship_name - relationship name.
 * @param type - relationship type.
 */
relationship::relationship(entity *parent_entity,
                           std::string relationship_name,
                           relationship_type type)
{
  this->parent_entity = parent_entity;
  this->relationship_name = relationship_name;
  this->type = type;
}

/**
 * Get relationsjip name.
 *
 * @return relationship name.
 */
std::string &
relationship::get_name()
{
  return this->relationship_name;
}

/**
 * Set relationship name.
 *
 * @param relationship_name
 */
void
relationship::set_name(std::string relationship_name)
{
  this->relationship_name = relationship_name;
}

/**
 * Get relationship type.
 *
 * @return relationship type.
 */
relationship_type
relationship::get_type()
{
  return this->type;
}

/**
 * Get entity by ID.
 *
 * @param id - entity ID.
 * @return entity if found, otherwise return NULL.
 */
entity *
relationship::get_entity(std::string id)
{
  if (id == "single")
  {
    if (this->entities.size() == 0)
    {
      return NULL;
    }

    return this->entities.front();
  }

  return this->find([&] (entity *e) {
    return e->get_id() == id;
  });
}

/**
 * Find entity.
 *
 * @param func - function rule.
 * @return entity if found, otherwise return NULL.
 */
entity *
relationship::find(const std::function<bool(entity *)> &func)
{
  for (entity *e : this->entities)
  {
    if (func(e))
    {
      return e;
    }
  }

  return NULL;
}

/**
 * Sort entities.
 *
 * @param func - function to sort.
 */
void
relationship::sort(const std::function <bool(entity *, entity *)>& func)
{
  std::sort(this->entities.begin(), this->entities.end(), func);
}

/**
 * foreach two neighbour iterators.
 *
 * @param func - function to handle current interators.
 */
void
relationship::for_each(const std::function<foreach_result(entity *, entity *)> &func)
{
  for (auto it1 = this->entities.begin();
       it1 != this->entities.end();
       it1++)
  {
    auto it2 = it1;
    it2++;

    while (it2 != this->entities.end())
    {
      foreach_result result = func(*it1, *it2);

      switch (result)
      {
        case FOREACH_CONTINUE:
          it1++;
          it2++;
          break;
        case FOREACH_IT2_REMOVED:
          it2 = it1;
          it2++;
          break;
        case FOREACH_IT1_REMOVED:
          it1 = it2;
          it2++;
          break;
        default:
          return;
      }
    }
  }
}

/**
 * foreach one iterator.
 *
 * @param func - function to handle iterator.
 */
void
relationship::for_each(const std::function<void(entity *)> &func)
{
  for (entity *e : this->entities)
  {
    func(e);
  }
}

/**
 * Add entity to this relationship.
 *
 * @param e - the entity.
 */
void
relationship::add_entity(entity *e)
{
  switch (this->type)
  {
    case ONE_TO_MANY:
    {
      break;
    }
    default:
    case ONE_TO_ONE:
    case MANY_TO_ONE:
    {
      if (this->entities.size() != 0)
      {
        BOX_ERROR(ERROR_BOX_RELATIONSHIP_ADDING_MORE_THAN_ONE);
      }
      break;
    }
  }

  e->set_marked(false);
  this->entities.push_back(e);
}

/**
 * Add entities.
 *
 * @param r - relationship that contains entities.
 */
void
relationship::add_entities(relationship *r)
{
  if (this->type != r->type)
  {
    BOX_ERROR(ERROR_BOX_RELATIONSHIP_ADDING_ENTITIES_WITH_DIFFERENT_RELATIONSHIP_TYPE);
    return;
  }

  for (entity *e : r->entities)
  {
    this->entities.push_back(e);
  }
}

/**
 * Remove entity from this relationship.
 *
 * @param e - the entity.
 */
void
relationship::remove_entity(entity *e)
{
  for (auto it = this->entities.begin();
       it != this->entities.end();
       it++)
  {
    if (*it == e)
    {
      this->entities.erase(it);
      break;
    }
  }
}

/**
 * Get entites
 * @return entity vector
 */
std::vector<entity *> &
relationship::get_entities()
{
  return this->entities;
}

/**
 * Get front entity from this relationship.
 *
 * @return entity if exists, otherwise return NULL.
 */
entity *
relationship::front()
{
  return (this->entities.size()) ? this->entities.front() : NULL;
}

/**
 * Get back entity from this relationship.
 *
 * @return entity if exists, otherwise return NULL.
 */
entity *
relationship::back()
{
  return (this->entities.size()) ? this->entities.back() : NULL;
}

/**
 * Delete all entites from relationship.
 */
void
relationship::delete_all()
{
  this->entities.clear();
}

/**
 * Get number of entities.
 *
 * @return number of entities.
 */
uint32_t
relationship::num_of_entities()
{
  return this->entities.size();
}
