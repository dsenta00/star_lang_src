#include "orm.h"
#include "entity.h"
#include <string>
#include <list>
#include <algorithm>

typedef std::unique_ptr<entity_repository> entity_repository_p;

/**
 * @brief repo - repository list.
 */
static std::list<std::unique_ptr<entity_repository>> repo;

/**
 * Find entity repository.
 *
 * @param entity_type - entity type.
 * @return entity_repository if found, otherwise return NULL.
 */
entity_repository *
orm::find_entity_repostiory(std::string entity_type)
{
  auto it = std::find_if(repo.begin(),
                         repo.end(),
                         [&] (entity_repository_p &ptr) {
            entity_repository *er = ptr.get();
            return er->get_entity_type() == entity_type;
  });

  return (it != repo.end()) ? (*it).get() : NULL;
}

/**
 * Add entity repository if not exists.
 *
 * @param entity_type
 */
void
orm::add_entity_repostiory(std::string entity_type)
{
  if (orm::find_entity_repostiory(entity_type))
  {
    return;
  }

  repo.push_back(entity_repository_p(new entity_repository(entity_type)));
}

/**
 * Remove entity repository.
 *
 * @param entity_type
 */
void
orm::remove_entity_repostiory(std::string entity_type)
{
  auto it = std::find_if(repo.begin(),
                         repo.end(),
                         [&] (entity_repository_p &ptr) {
            entity_repository *er = ptr.get();
            return er->get_entity_type() == entity_type;
  });

  if (it == repo.end())
  {
    return;
  }

  repo.erase(it);
}

/**
 * Add new entity to repository.
 *
 * @param e - entity.
 * @return the entity.
 */
entity *
orm::create(entity *e)
{
  if (e)
  {
    entity_repository *er = orm::find_entity_repostiory(e->get_entity_type());

    if (!er)
    {
      delete e;
      return NULL;
    }

    er->add(e);
  }

  return e;
}

/**
 * Destroy entity and all relationships.
 * If objects remains marked, sweep them also.
 *
 * @param e - The entity.
 */
void
orm::destroy(entity *e)
{
  entity_repository *er = orm::find_entity_repostiory(e->get_entity_type());

  if (!er)
  {
    return;
  }

  er->remove(e);
  orm::sweep();
}

/**
 * Sweep all entities from all repositories.
 */
void
orm::sweep()
{
  for (entity_repository_p &erp : repo)
  {
    erp.get()->sweep();
  }
}

/**
 * Select command.
 * @param entity_type
 * @param where
 * @return
 */
entity *
orm::select(std::string entity_type, std::function<bool(entity *)> where)
{
  entity_repository *er = orm::find_entity_repostiory(entity_type);

  if (!er)
  {
    return NULL;
  }

  return er->find(where);
}

/**
 * Get first object from entity repository.
 *
 * @param entity_type - entity type.
 * @return first object if exists, otherwise NULL.
 */
entity *
orm::get_first(std::string entity_type)
{
  entity_repository *er = orm::find_entity_repostiory(entity_type);

  if (!er)
  {
    return NULL;
  }

  return er->find([&] (entity *e) { (void)e; return true; });
}
