#include "orm.h"
#include "entity.h"
#include <string>
#include <list>
#include <algorithm>

typedef std::unique_ptr<entity_repository> entity_repository_p;
static std::list<std::unique_ptr<entity_repository>> repo;

entity_repository *
orm::findEntityRepostiory(std::string entity_type)
{
  auto it = std::find_if(repo.begin(),
                         repo.end(),
                         [&] (entity_repository_p &ptr) {
            entity_repository *er = ptr.get();
            return er->getEntityType() == entity_type;
  });

  return (it != repo.end()) ? (*it).get() : NULL;
}

void
orm::addEntityRepostiory(std::string entity_type)
{
  if (orm::findEntityRepostiory(entity_type))
  {
    return;
  }

  repo.push_back(entity_repository_p(new entity_repository(entity_type)));
}

void
orm::removeEntityRepostiory(std::string entity_type)
{
  auto it = std::find_if(repo.begin(),
                         repo.end(),
                         [&] (entity_repository_p &ptr) {
            entity_repository *er = ptr.get();
            return er->getEntityType() == entity_type;
});

  if (it == repo.end())
  {
    return;
  }

  repo.erase(it);
}

entity *orm::create(entity *e)
{
  if (e)
  {
    entity_repository *er = orm::findEntityRepostiory(e->getType());

    if (!er)
    {
      delete e;
      return NULL;
    }

    er->add(e);
  }

  return e;
}

void
orm::destroy(entity *e)
{
  entity_repository *er = orm::findEntityRepostiory(e->getType());

  if (!er)
  {
    return;
  }

  er->remove(e);
  orm::sweep();
}

void
orm::sweep()
{
  for (entity_repository_p &erp : repo)
  {
    erp.get()->sweep();
  }
}

entity *
orm::select(std::string entity_type, std::function<bool(entity *)> where)
{
  entity_repository *er = orm::findEntityRepostiory(entity_type);

  if (!er)
  {
    return NULL;
  }

  return er->find(where);
}


entity *
orm::get_first(std::string entity_type)
{
  entity_repository *er = orm::findEntityRepostiory(entity_type);

  if (!er)
  {
    return NULL;
  }

  return er->find([&] (entity *e) { (void)e; return true; });
}
