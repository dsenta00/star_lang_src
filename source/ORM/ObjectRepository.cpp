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

#include <algorithm>
#include <ORM/ObjectRepository.h>
#include <ORM/Object.h>
#include <ORM/MasterRelationships.h>
#include <ORM/SlaveRelationships.h>

/**
 * Find object.
 *
 * @param func - function rule to find.
 * @return object if found, otherwise return NULL.
 */
Object *
ObjectRepository::find(const std::function<bool(Object *)> &func)
{
    if (this->objectMap.empty())
    {
        return nullptr;
    }

    for (auto &it : this->objectMap)
    {
        if (it.second.empty())
        {
            continue;
        }

        for (const auto &op : it.second)
        {
            Object *o = op.get();

            if (func(o))
            {
                if (o->getMarked())
                {
                    continue;
                }

                return o;
            }
        }
    }

    return nullptr;
}

/**
 * Add new object.
 *
 * @param o - the object
 */
void
ObjectRepository::add(Object *o)
{
    if (this->find([&](Object *a) { return a == o; }) != nullptr)
    {
        return;
    }

    if (o->getMarked())
    {
        o->setMarked(false);
    }

    if (this->objectMap.find(o->getId()) == this->objectMap.end())
    {
        this->objectMap[o->getId()] = std::vector<ObjectPtr>();
    }

    this->objectMap[o->getId()].push_back(ObjectPtr(o));
}

/**
 * Remove object from repository.
 *
 * @param o - the object.
 */
void
ObjectRepository::remove(Object *o)
{
    o->setMarked(true);

    o->getMaster()->clearObjects();
    o->getSlave()->notifyDestroyed();
}

/**
 * Change ID.
 *
 * @param o - the object.
 * @param newId
 */
void
ObjectRepository::changeId(Object *o, std::string &newId)
{
    if (this->objectMap.find(o->getId()) == this->objectMap.end())
    {
        /*
         * ID doesn't exist, create it and init Object.
         */
        o->setId(newId);
        this->add(o);
        return;
    }

    auto &objects = this->objectMap[o->getId()];
    auto it = std::find_if(objects.begin(), objects.end(), [&](ObjectPtr &op) {
        return op.get() == o;
    });

    if (it == objects.end())
    {
        /*
         * Object is not inserted. Add Object.
         */
        o->setId(newId);
        this->add(o);
        return;
    }

    if (this->objectMap.find(newId) == this->objectMap.end())
    {
        /*
         * ID doesn't exist, create it.
         */
        this->objectMap[newId] = std::vector<ObjectPtr>();
    }

    this->objectMap[newId].push_back(*it);
    o->setId(newId);
    objects.erase(it);
}

/**
 * Sweep all objects that are marked.
 */
void
ObjectRepository::sweep()
{
    for (auto &it : this->objectMap)
    {
        auto &objects = it.second;

        while (true)
        {
            auto it2 = std::find_if(objects.begin(), objects.end(), [&](ObjectPtr &op) {
                return op->getMarked();
            });

            if (it2 == objects.end())
            {
                break;
            }

            objects.erase(it2);
        }
    }
}

/**
 * Get object.
 *
 * @param id
 * @return
 */
Object *
ObjectRepository::get(std::string &id)
{
    if (this->objectMap.find(id) == this->objectMap.end())
    {
        return nullptr;
    }

    auto &objects = this->objectMap[id];

    return !objects.empty() ? objects[0].get() : nullptr;
}

/**
 * The destructor.
 */
ObjectRepository::~ObjectRepository()
{
    for (auto &it : this->objectMap)
    {
        auto &objects = it.second;

        while (!objects.empty())
        {
            auto &object = objects.front();
            this->remove(object.get());
            this->sweep();
        }
    }
}

