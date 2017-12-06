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

#include <utility>

#include "ORM/orm.h"
#include "ORM/object.h"

typedef std::unique_ptr<object_repository> object_repository_p;

/**
 * @brief repo - repository map.
 */
static std::map<object_type, object_repository_p> repo;

/**
 * Find object repository.
 *
 * @param type - object type.
 * @return object repository if found, otherwise return nullptr.
 */
object_repository *
orm::find_object_repository(object_type type)
{
    auto it = repo.find(type);

    return (it != repo.end()) ? (it->second).get() : nullptr;
}

/**
 * Add object repository if not exists.
 *
 * @param type
 */
void
orm::add_object_repository(object_type type)
{
    if (orm::find_object_repository(type))
    {
        return;
    }

    repo[type] = object_repository_p(new object_repository());
}

/**
 * Add new object to repository.
 *
 * @param o - object.
 * @return the object.
 */
object *
orm::create(object *o)
{
    if (!o)
    {
        return nullptr;
    }

    object_repository *repository = orm::find_object_repository(o->get_object_type());

    if (!repository)
    {
        orm::add_object_repository(o->get_object_type());
        repository = orm::find_object_repository(o->get_object_type());
    }

    repository->add(o);
    return o;
}

/**
 * Change object ID.
 *
 * @param o
 * @param new_id
 */
void
orm::change_id(object *o, std::string new_id)
{
    if (!o)
    {
        return;
    }

    object_repository *repository = orm::find_object_repository(o->get_object_type());

    if (!repository)
    {
        orm::add_object_repository(o->get_object_type());
        repository = orm::find_object_repository(o->get_object_type());
    }

    repository->change_id(o, new_id);
}

/**
 * Destroy object and all relationships.
 * If objects remains marked, sweep them also.
 *
 * @param o - The object.
 */
void
orm::destroy(object *o)
{
    object_repository *repository = orm::find_object_repository(o->get_object_type());

    if (!repository)
    {
        return;
    }

    repository->remove(o);
    orm::sweep();
}

/**
 * Sweep all objects from all repositories.
 */
void
orm::sweep()
{
    for (auto &it : repo)
    {
        it.second->sweep();
    }
}

/**
 * Select command.
 *
 * @param type
 * @param where
 * @return
 */
object *
orm::select(object_type type, std::function<bool(object *)> where)
{
    object_repository *repository = orm::find_object_repository(type);

    if (!repository)
    {
        return nullptr;
    }

    return repository->find(where);
}

/**
 * Select id.
 *
 * @param type
 * @param id
 * @return
 */
object *
orm::select(object_type type, std::string id)
{
    object_repository *repository = orm::find_object_repository(type);

    if (!repository)
    {
        return nullptr;
    }

    return repository->get(id);
}

/**
 * Get first object from object repository.
 *
 * @param type - object type.
 * @return first object if exists, otherwise nullptr.
 */
object *
orm::get_first(object_type type)
{
    object_repository *repository = orm::find_object_repository(type);

    if (!repository)
    {
        return nullptr;
    }

    return repository->find([&](object *e) {
        (void) e;
        return true;
    });
}

/**
 * Remove object repository.
 *
 * @param type
 */
void
orm::remove_object_repository(object_type type)
{
    auto it = repo.find(type);

    if (it != repo.end())
    {
        repo.erase(it);
    }
}

/**
 * Remove all repositories
 */
void
orm::remove_all_repositories()
{
    repo.clear();
}
