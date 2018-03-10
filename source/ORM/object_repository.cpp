/*
 * Copyright 2017 Duje Senta
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

#include "ORM/object_repository.h"
#include "ORM/object.h"

/**
 * Find object.
 *
 * @param func - function rule to find.
 * @return object if found, otherwise return NULL.
 */
object *
object_repository::find(const std::function<bool(object *)> &func)
{
    if (this->object_map.empty())
    {
        return nullptr;
    }

    for (auto &it : this->object_map)
    {
        if (it.second.empty())
        {
            continue;
        }

        for (const auto &op : it.second)
        {
            object *o = op.get();

            if (func(o))
            {
                if (o->get_marked())
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
object_repository::add(object *o)
{
    if (this->find([&](object *a) { return a == o; }) != nullptr)
    {
        return;
    }

    if (o->get_marked())
    {
        o->set_marked(false);
    }

    if (this->object_map.find(o->get_id()) == this->object_map.end())
    {
        this->object_map[o->get_id()] = std::vector<object_p>();
    }

    this->object_map[o->get_id()].push_back(object_p(o));
}

/**
 * Remove object from repository.
 *
 * @param o - the object.
 */
void
object_repository::remove(object *o)
{
    o->set_marked(true);
    o->master_relationships_clear_objects();
    o->slave_relationship_notify_destroyed();
}

/**
 * Change ID.
 *
 * @param o - the object.
 * @param new_id
 */
void
object_repository::change_id(object *o, std::string &new_id)
{
    if (this->object_map.find(o->get_id()) == this->object_map.end())
    {
        /*
         * ID doesn't exist, create it and add object.
         */
        o->set_id(new_id);
        this->add(o);
        return;
    }

    auto &objects = this->object_map[o->get_id()];
    auto it = std::find_if(objects.begin(), objects.end(), [&](object_p &op) {
        return op.get() == o;
    });

    if (it == objects.end())
    {
        /*
         * object is not inserted. Add object.
         */
        o->set_id(new_id);
        this->add(o);
        return;
    }

    if (this->object_map.find(new_id) == this->object_map.end())
    {
        /*
         * ID doesn't exist, create it.
         */
        this->object_map[new_id] = std::vector<object_p>();
    }

    this->object_map[new_id].push_back(*it);
    o->set_id(new_id);
    objects.erase(it);
}

/**
 * Sweep all objects that are marked.
 */
void
object_repository::sweep()
{
    for (auto &it : this->object_map)
    {
        auto &objects = it.second;

        while (true)
        {
            auto it2 = std::find_if(objects.begin(), objects.end(), [&](object_p &op) {
                return op->get_marked();
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
object *
object_repository::get(std::string &id)
{
    if (this->object_map.find(id) == this->object_map.end())
    {
        return nullptr;
    }

    auto &objects = this->object_map[id];

    return !objects.empty() ? objects[0].get() : nullptr;
}

/**
 * The destructor.
 */
object_repository::~object_repository()
{
    for (auto &it : this->object_map)
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

