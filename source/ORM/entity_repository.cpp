#include "ORM/entity_repository.h"
#include "ORM/entity.h"

/**
 * Find entity.
 *
 * @param func - function rule to find.
 * @return entity if found, otherwise return NULL.
 */
entity *
entity_repository::find(const std::function<bool(entity *)> &func)
{
  for (entity_p &ep : this->entities)
  {
    entity *e = ep.get();

    if (func(e))
    {
      return e->get_marked() ? NULL : e;
    }
  }

  return nullptr;
}

/**
 * Add new entity.
 *
 * @param e - the entity
 */
void
entity_repository::add(entity *e)
{
  if (e->get_marked())
  {
    e->set_marked(false);
    return;
  }

  this->entities.push_back(std::unique_ptr<entity>(e));
}

/**
 * Remove entity from repository.
 *
 * @param e - the entity.
 */
void
entity_repository::remove(entity *e)
{
  e->set_marked(true);
  e->remove_all_relationships();
}

/**
 * Sweep all entity objects that are marked.
 */
void
entity_repository::sweep()
{
  while (true)
  {
    auto it = std::find_if(this->entities.begin(),
                           this->entities.end(),
                           [&](entity_p &ep) {
                             return ep.get()->get_marked();
                           });

    if (it == this->entities.end())
    {
      return;
    }

    this->entities.erase(it);
  }
}

