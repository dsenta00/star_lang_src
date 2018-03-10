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

#ifndef OBJECT_H
#define OBJECT_H

#include "orm_fw.h"
#include "relationship_type.h"
#include "object_type.h"
#include <cstdint>
#include <map>
#include <vector>
#include <string>

typedef std::shared_ptr<relationship> relationship_p;

/**
 * The object class.
 * Each object can have relationship with another object.
 * Usage is to extend data as object base class.
 */
class object {
public:
    explicit object(uint64_t id);
    explicit object(std::string id);

    std::string get_id();
    void set_id(std::string new_id);

    virtual object_type get_object_type() = 0;

    bool get_marked();
    void set_marked(bool marked);

    /*
     * slave relationship handler functions
     */
    relationship *slave_relationship_get(std::string relationship_name);
    void slave_relationship_add(std::string relationship_name, relationship_type type);
    void slave_relationship_add_object(std::string relationship_name, object *o);
    void slave_relationship_remove_object(std::string relationship_name, object *o);
    object *slave_relationship_back(std::string relationship_name);
    bool slave_relationship_have_relations();
    void slave_relationship_notify_destroyed();

    /*
     * master relationship handler functions
     */
    relationship *master_relationship_get(std::string relationship_name);
    void master_relationship_add(std::string relationship_name, relationship_type type);
    void master_relationship_add_object(std::string relationship_name, object *o);
    void master_relationship_remove_object(std::string relationship_name, object *o);
    object *master_relationship_back(std::string relationship_name);
    object *master_relationship_front(std::string relationship_name);
    void master_relationships_clear_objects();

    virtual ~object();
protected:
    bool marked;
    std::string id;
    std::map<std::string, relationship_p> master_relationships;
    std::map<std::string, relationship_p> slave_relationships;
};

#endif // OBJECT_H
