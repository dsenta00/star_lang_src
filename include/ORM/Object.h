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

#pragma once

#include "FwDecl.h"
#include "eRelationshipType.h"
#include "eObjectType.h"
#include <cstdint>
#include <map>
#include <memory>
#include <vector>
#include <string>

using RelationshipPtr = std::shared_ptr<Relationship>;

/**
 * The object class.
 * Each object can have relationship with another object.
 * Usage is to extend data as object base class.
 */
class Object {
public:
    explicit Object(uint64_t id);
    explicit Object(std::string id);

    std::string getId();
    void setId(std::string newId);

    virtual eObjectType getObjectType() = 0;

    bool getMarked();
    void setMarked(bool marked);

    /*
     * slave Relationship handler functions
     */
    Relationship *slaveRelationshipGet(std::string relationshipName);
    void slaveRelationshipAdd(std::string relationshipName, eRelationshipType type);
    void slaveRelationshipAddObject(std::string relationshipName, Object *o);
    void slaveRelationshipRemoveObject(std::string relationshipName, Object *o);
    Object *slaveRelationshipBack(std::string relationshipName);
    bool slaveRelationshipHasRelations();
    void slaveRelationshipNotifyDestroyed();

    /*
     * master Relationship handler functions
     */
    Relationship *masterRelationshipGet(std::string relationshipName);
    void masterRelationshipAdd(std::string relationshipName, eRelationshipType type);
    void masterRelationshipAddObject(std::string relationshipName, Object *o);
    void masterRelationshipRemoveObject(std::string relationshipName, Object *o);
    Object *masterRelationshipBack(std::string relationshipName);
    Object *masterRelationshipFront(std::string relationshipName);
    void masterRelationshipsClearObjects();
    void masterRelationshipsClearObjects(std::string relationshipName);

    virtual ~Object();
protected:
    bool marked;
    std::string id;
    std::map<std::string, RelationshipPtr> masterRelationships;
    std::map<std::string, RelationshipPtr> slaveRelationships;
};