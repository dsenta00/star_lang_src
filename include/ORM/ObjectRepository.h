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
#include <map>
#include <vector>
#include <string>
#include <functional>
#include <memory>

using ObjectPtr = std::shared_ptr<Object>;

/**
 * The object_repository class.
 *
 * All created objects of same type are stored right here.
 * Each object type has its own object_repository.
 */
class ObjectRepository {
public:
    Object *find(const std::function<bool(Object *)> &func);
    Object *get(std::string &id);
    void add(Object *o);
    void remove(Object *o);
    void changeId(Object *o, std::string &newId);
    void sweep();
    ~ObjectRepository();
protected:
    /*
     * key    -> Object ID
     * values -> Object array
     */
    std::map<std::string, std::vector<ObjectPtr>> objectMap;
};