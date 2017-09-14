#include "relationship.h"
#include "entity.h"
#include "stdexcept"
#include "../box_monitor.h"
#include <algorithm>

relationship::relationship(entity *parent_entity, std::string relationship_name, relationship_type type)
{
  this->parent_entity = parent_entity;
  this->relationship_name = relationship_name;
  this->type = type;
}

std::string relationship::getName()
{
  return this->relationship_name;
}

relationship *relationship::setName(std::string relationship_name)
{
  this->relationship_name = relationship_name;
  return this;
}

relationship_type relationship::getType()
{
  return this->type;
}

entity *
relationship::getEntity(std::string id)
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
    return e->getId() == id;
  });
}

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

relationship *
relationship::sort(const std::function <bool(entity *, entity *)>& func)
{
  std::sort(this->entities.begin(), this->entities.end(), func);
  return this;
}

relationship *
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
          return this;
      }
    }
  }

  return this;
}


relationship *
relationship::for_each(const std::function<void(entity *)> &func)
{
  for (entity *e : this->entities)
  {
    func(e);
  }

  return this;
}

relationship *
relationship::addEntity(entity *e)
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

  e->setMarked(false);
  this->entities.push_back(e);

  return this;
}

relationship *
relationship::addEntities(relationship *r)
{
  if (this->type != r->type)
  {
    BOX_ERROR(ERROR_BOX_RELATIONSHIP_ADDING_ENTITIES_WITH_DIFFERENT_RELATIONSHIP_TYPE);
    return this;
  }

  for (entity *e : r->entities)
  {
    this->entities.push_back(e);
  }

  return this;
}

relationship *
relationship::removeEntity(entity *e)
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

  return this;
}

std::vector<entity *> &
relationship::getEntities()
{
  return this->entities;
}

entity *relationship::front()
{
  return (this->entities.size()) ? this->entities.front() : NULL;
}

entity *relationship::back()
{
  return (this->entities.size()) ? this->entities.back() : NULL;
}

void relationship::delete_all()
{
  this->entities.clear();
}

uint32_t relationship::numOfEntities()
{
  return this->entities.size();
}
