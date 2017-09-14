#include "entity_repository.h"
#include "entity.h"
#include <algorithm>

entity_repository::entity_repository(std::string entity_type)
{
  this->entity_type = entity_type;
}

std::string &
entity_repository::getEntityType()
{
  return this->entity_type;
}

entity *
entity_repository::find(const std::function <bool(entity *)>& func)
{
  for (entity_p &ep : this->entities)
  {
    entity *e = ep.get();

    if (func(e))
    {
      return e->getMarked() ? NULL : e;
    }
  }

  return NULL;
}

entity_repository *
entity_repository::add(entity *e)
{
  if (e->getMarked())
  {
    e->setMarked(false);
    return this;
  }

  this->entities.push_back(std::unique_ptr<entity>(e));
  return this;
}

entity_repository *
entity_repository::remove(entity *e)
{
  e->setMarked(true);
  e->removeAllRelationships();

  return this;
}

void
entity_repository::sweep()
{
  while (true)
  {
    auto it = std::find_if(this->entities.begin(),
                           this->entities.end(),
                           [&] (entity_p &ep) {
              return ep.get()->getMarked();
    });

    if (it == this->entities.end())
    {
      return;
    }

    this->entities.erase(it);
  }
}

void
entity_repository::clear()
{
  this->entities.clear();
}
