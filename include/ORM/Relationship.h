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

#pragma once

#include "FwDecl.h"
#include "eRelationshipType.h"
#include <vector>
#include <string>
#include <functional>
#include <memory>

typedef enum {
    FOREACH_CONTINUE,
    FOREACH_IT2_REMOVED,
    FOREACH_IT1_REMOVED
} eForEachResult;

using ObjVector = std::vector<Object *>;

/**
 * The relationship class.
 * Contains and manages all objects in relationship.
 */
class Relationship : public ObjVector {
public:
    Relationship(std::string relationshipName, eRelationshipType type);

    std::string &getName();
    eRelationshipType getType();

    void sort(const std::function<bool(Object *, Object *)> &func);
    void forEach(const std::function<eForEachResult(Object *, Object *)> &func);
    void addObject(Object *o);
    void removeObject(Object *o);

    Object *find(const std::function<bool(Object *)> &func);
    Object *find(std::string id);
    Object *front();
    Object *back();
protected:
    std::string name;
    eRelationshipType type;
};
