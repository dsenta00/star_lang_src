/*
 * Copyright 2018 Duje Senta
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

#include "ORM/ORM.h"
#include "ORM/Object.h"

using ObjectRepositoryPtr = std::unique_ptr<ObjectRepository>;

/**
 * @brief repo - repository map.
 */
static std::map<eObjectType, ObjectRepositoryPtr> repo;

/**
 * Find object repository.
 *
 * @param type - object type.
 * @return object repository if found, otherwise return nullptr.
 */
ObjectRepository *
ORM::findObjectRepository(eObjectType type)
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
ORM::addObjectRepository(eObjectType type)
{
    if (ORM::findObjectRepository(type))
    {
        return;
    }

    repo[type] = ObjectRepositoryPtr(new ObjectRepository());
}

/**
 * Add new object to repository.
 *
 * @param o - object.
 * @return the object.
 */
Object *
ORM::create(Object *o)
{
    if (!o)
    {
        return nullptr;
    }

    ObjectRepository *repository = ORM::findObjectRepository(o->getObjectType());

    if (!repository)
    {
        ORM::addObjectRepository(o->getObjectType());
        repository = ORM::findObjectRepository(o->getObjectType());
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
ORM::changeId(Object *o, std::string new_id)
{
    if (!o)
    {
        return;
    }

    ObjectRepository *repository = ORM::findObjectRepository(o->getObjectType());

    if (!repository)
    {
        ORM::addObjectRepository(o->getObjectType());
        repository = ORM::findObjectRepository(o->getObjectType());
    }

    repository->changeId(o, new_id);
}

/**
 * Destroy object and all relationships.
 * If objects remains marked, sweep them also.
 *
 * @param o - The object.
 */
void
ORM::destroy(Object *o)
{
    ObjectRepository *repository = ORM::findObjectRepository(o->getObjectType());

    if (!repository)
    {
        return;
    }

    repository->remove(o);
    ORM::sweep();
}

/**
 * Sweep all objects from all repositories.
 */
void
ORM::sweep()
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
Object *
ORM::select(eObjectType type, std::function<bool(Object *)> where)
{
    ObjectRepository *repository = ORM::findObjectRepository(type);

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
Object *
ORM::select(eObjectType type, std::string id)
{
    ObjectRepository *repository = ORM::findObjectRepository(type);

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
Object *
ORM::getFirst(eObjectType type)
{
    ObjectRepository *repository = ORM::findObjectRepository(type);

    if (!repository)
    {
        return nullptr;
    }

    return repository->find([&](Object *e) {
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
ORM::removeObjectRepository(eObjectType type)
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
ORM::removeAllRepositories()
{
    repo.clear();
}
