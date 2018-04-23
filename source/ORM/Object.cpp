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

#include <ORM/Object.h>
#include <ORM/Relationship.h>
#include <ErrorBundle/ErrorLog.h>
#include <sstream>
#include <utility>

/**
 * The constructor.
 *
 * @param type
 * @param id
 */
Object::Object(const uint64_t id)
{
    this->marked = false;
    this->id = std::to_string(id);

    this->masterRelationshipsPtr = MasterRelationshipsPtr(new MasterRelationships(this));
    this->slaveRelationshipsPtr = SlaveRelationshipsPtr(new SlaveRelationships(this));
}

/**
 * The constructor.
 *
 * @param type
 * @param id
 */
Object::Object(std::string id)
{
    this->marked = false;
    this->id = std::move(id);

    this->masterRelationshipsPtr = MasterRelationshipsPtr(new MasterRelationships(this));
    this->slaveRelationshipsPtr = SlaveRelationshipsPtr(new SlaveRelationships(this));
}

/**
 * Get object ID.
 *
 * @return object ID.
 */
std::string
Object::getId()
{
    return this->id;
}

/**
 * Get object ID.
 *
 * @return object ID.
 */
void
Object::setId(std::string newId)
{
    this->id = std::move(newId);
}

/**
 * Get marked.
 *
 * @return marked.
 */
bool
Object::getMarked()
{
    return this->marked;
}

/**
 * Set marked.
 *
 * @param marked
 */
void
Object::setMarked(bool marked)
{
    this->marked = marked;
}

/**
 * Get master relationships.
 *
 * @return
 */
MasterRelationships *
Object::getMaster()
{
    return this->masterRelationshipsPtr.get();
}

/**
 * Get slave relationships.
 *
 * @return
 */
SlaveRelationships *
Object::getSlave()
{
    return this->slaveRelationshipsPtr.get();
}