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

#include "ORM/relationship.h"
#include "ORM/entity.h"
#include "error_log.h"

/**
 * The constructor.
 *
 * @param parent_entity - relationship owner.
 * @param relationship_name - relationship name.
 * @param type - relationship type.
 */
relationship::relationship(std::string relationship_name, relationship_type type)
{
    this->relationship_name = std::move(relationship_name);
    this->type = type;
}

/**
 * Get relationship name.
 *
 * @return relationship name.
 */
std::string &
relationship::get_name()
{
    return this->relationship_name;
}

/**
 * Get relationship type.
 *
 * @return relationship type.
 */
relationship_type
relationship::get_type()
{
    return this->type;
}

/**
 * Find entity.
 *
 * @param func - function rule.
 * @return entity if found, otherwise return NULL.
 */
entity *
relationship::find(const std::function<bool(entity *)> &func)
{
    auto it = std::find_if(this->entities.begin(), this->entities.end(), func);

    return (it != this->entities.end()) ? *it : nullptr;
}

/**
 * Sort entities.
 *
 * @param func - function to sort.
 */
void
relationship::sort(const std::function<bool(entity *, entity *)> &func)
{
    std::sort(this->entities.begin(), this->entities.end(), func);
}

/**
 * foreach two neighbour iterators.
 *
 * @param func - function to handle current iterators.
 */
void
relationship::for_each(const std::function<foreach_result(entity *, entity *)> &func)
{
    for (auto it1 = this->entities.begin();
         it1 != this->entities.end();
         it1++)
    {
        auto it2 = it1;
        it2++;

        while (it2 != this->entities.end())
        {
            foreach_result result = func(*it1, *it2);

            switch (result)
            {
                case FOREACH_CONTINUE:
                    it1++;
                    it2++;
                    break;
                case FOREACH_IT2_REMOVED:
                    it2 = it1;
                    it2++;
                    break;
                case FOREACH_IT1_REMOVED:
                    it1 = it2;
                    it2++;
                    break;
                default:
                    return;
            }
        }
    }
}

/**
 * foreach one iterator.
 *
 * @param func - function to handle iterator.
 */
void
relationship::for_each(const std::function<void(entity *)> &func)
{
    for (entity *e : this->entities)
    {
        func(e);
    }
}

/**
 * Add entity to this relationship.
 *
 * @param e - the entity.
 */
void
relationship::add_entity(entity *e)
{
    if (this->type == ONE_TO_ONE)
    {
        if (!this->entities.empty())
        {
            ERROR_LOG_ADD(ERROR_RELATIONSHIP_ADDING_MORE_THAN_ONE);
            return;
        }
    }

    e->set_marked(false);
    this->entities.push_back(e);
}

/**
 * Remove entity from this relationship.
 *
 * @param e - the entity.
 */
void
relationship::remove_entity(entity *e)
{
    for (auto it = this->entities.begin();
         it != this->entities.end();
         it++)
    {
        if (*it == e)
        {
            this->entities.erase(it);
            break;
        }
    }
}

/**
 * Get entites
 * @return entity vector
 */
std::vector<entity *> &
relationship::get_entities()
{
    return this->entities;
}

/**
 * Get front entity from this relationship.
 *
 * @return entity if exists, otherwise return NULL.
 */
entity *
relationship::front()
{
    return (this->entities.size()) ? this->entities.front() : nullptr;
}

/**
 * Get back entity from this relationship.
 *
 * @return entity if exists, otherwise return NULL.
 */
entity *
relationship::back()
{
    return (this->entities.size()) ? this->entities.back() : nullptr;
}

/**
 * Get number of entities.
 *
 * @return number of entities.
 */
uint32_t
relationship::num_of_entities()
{
    return static_cast<uint32_t>(this->entities.size());
}
