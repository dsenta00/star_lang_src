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
  for (auto it = this->entity_map.begin();
       it != this->entity_map.end();
       it++)
  {
    for (const auto &ep : it->second)
    {
      entity *e = ep.get();

      if (func(e))
      {
        if (e->get_marked())
        {
          continue;
        }

        return e;
      }
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
  }

  if (this->entity_map.find(e->get_id()) == this->entity_map.end())
  {
    this->entity_map[e->get_id()] = std::vector<entity_p>();
  }

  this->entity_map[e->get_id()].push_back(entity_p(e));
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
  e->master_relationships_clear_entities();
  e->slave_relationship_notify_destroyed();
}

/**
 * Change ID.
 *
 * @param e - the entity.
 * @param new_id
 */
void
entity_repository::change_id(entity *e, std::string &new_id)
{
  if (this->entity_map.find(e->get_id()) == this->entity_map.end())
  {
    /*
     * ID doesn't exist, create it and add entity.
     */
    e->set_id(new_id);
    this->add(e);
    return;
  }

  auto &entities = this->entity_map[e->get_id()];
  auto it = std::find_if(entities.begin(), entities.end(), [&](entity_p &ep) {
    return ep.get() == e;
  });

  if (it == entities.end())
  {
    /*
     * Entity is not inserted. Add entity.
     */
    e->set_id(new_id);
    this->add(e);
    return;
  }

  if (this->entity_map.find(new_id) == this->entity_map.end())
  {
    /*
     * ID doesn't exist, create it.
     */
    this->entity_map[new_id] = std::vector<entity_p>();
  }

  this->entity_map[new_id].push_back(*it);
  e->set_id(new_id);
  entities.erase(it);
}

/**
 * Sweep all entity objects that are marked.
 */
void
entity_repository::sweep()
{
  for (auto it = this->entity_map.begin();
       it != this->entity_map.end();
       it++)
  {
    auto &entities = it->second;

    while (true)
    {
      auto it2 = std::find_if(entities.begin(), entities.end(), [&](entity_p &ep) {
        return ep->get_marked();
      });

      if (it2 == entities.end())
      {
        break;
      }

      entities.erase(it2);
    }
  }
}

/**
 * Get object.
 *
 * @param id
 * @return
 */
entity *
entity_repository::get(std::string &id)
{
  if (this->entity_map.find(id) == this->entity_map.end())
  {
    return nullptr;
  }

  auto &entities = this->entity_map[id];
  return (entities.size() > 0) ? entities[0].get() : nullptr;
}

/**
 * The destructor.
 */
entity_repository::~entity_repository()
{
  for (auto it = this->entity_map.begin();
       it != this->entity_map.end();
       it++)
  {
    auto &entities = it->second;

    while (!entities.empty())
    {
      auto &entity = entities.front();
      this->remove(entity.get());
      this->sweep();
    }
  }
}

