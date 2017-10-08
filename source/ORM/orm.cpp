/*
 * Copyright 2017 Duje Senta, Tomislav Radanovic
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "ORM/orm.h"
#include "ORM/entity.h"

typedef std::unique_ptr<entity_repository> entity_repository_p;

/**
 * @brief repo - repository map.
 */
static std::map<std::string, entity_repository_p> repo;

/**
 * Find entity repository.
 *
 * @param entity_type - entity type.
 * @return entity_repository if found, otherwise return nullptr.
 */
entity_repository *
orm::find_entity_repository(std::string entity_type)
{
  auto it = repo.find(entity_type);

  return (it != repo.end()) ? (it->second).get() : nullptr;
}

/**
 * Add entity repository if not exists.
 *
 * @param entity_type
 */
void
orm::add_entity_repository(std::string entity_type)
{
  if (orm::find_entity_repository(entity_type))
  {
    return;
  }

  repo[entity_type] = entity_repository_p(new entity_repository());
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
  if (!e)
  {
    return nullptr;
  }

  entity_repository *er = orm::find_entity_repository(e->get_entity_type());

  if (!er)
  {
    orm::add_entity_repository(e->get_entity_type());
    er = orm::find_entity_repository(e->get_entity_type());
  }

  er->add(e);
  return e;
}


void
orm::change_id(entity *e, std::string new_id)
{
  if (!e)
  {
    return;
  }

  entity_repository *er = orm::find_entity_repository(e->get_entity_type());

  if (!er)
  {
    orm::add_entity_repository(e->get_entity_type());
    er = orm::find_entity_repository(e->get_entity_type());
  }

  er->change_id(e, new_id);
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
  entity_repository *er = orm::find_entity_repository(e->get_entity_type());

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
  for (auto it = repo.begin();
       it != repo.end();
       it++)
  {
    it->second->sweep();
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
  entity_repository *er = orm::find_entity_repository(entity_type);

  if (!er)
  {
    return nullptr;
  }

  return er->find(where);
}

/**
 * Select id.
 *
 * @param entity_type
 * @param id
 * @return
 */
entity *
orm::select(std::string entity_type, std::string id)
{
  entity_repository *er = orm::find_entity_repository(entity_type);

  if (!er)
  {
    return nullptr;
  }

  return er->get(id);
}

/**
 * Get first object from entity repository.
 *
 * @param entity_type - entity type.
 * @return first object if exists, otherwise nullptr.
 */
entity *
orm::get_first(std::string entity_type)
{
  entity_repository *er = orm::find_entity_repository(entity_type);

  if (!er)
  {
    return nullptr;
  }

  return er->find([&](entity *e) {
    (void) e;
    return true;
  });
}

/**
 * Remove entity repository.
 *
 * @param entity_type
 */
void
orm::remove_entity_repository(std::string entity_type)
{
  auto it = repo.find(entity_type);
  if (it != repo.end())
  {
    repo.erase(it);
  }
}
