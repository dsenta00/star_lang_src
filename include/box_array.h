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

#ifndef BOX_ARRAY_H
#define BOX_ARRAY_H

#include "ORM/entity.h"
#include "box_data_type.h"
#include "box_fw.h"
#include <string>

/**
 * Represents data array.
 */
class box_array : public entity {
public:
    box_array(std::string id, box_array *array = nullptr);
    uint16_t get_noof();
    entity *operator[](uint32_t index);
    entity *operator[](std::string index);
    void insert(std::string index, entity *e);
    void insert(uint32_t index, entity *e);
    bool operator+=(entity *e);
    box_data &to_string();
    void clear();
    virtual ~box_array();
    static box_array *create(std::string id, box_array *array = nullptr);
protected:
    void remove_data(std::string index);
    void remove_data(entity *e);
    void insert_data(std::string index, entity *e);
    std::map<std::string, entity *> array;
};

#endif // BOX_ARRAY_H
