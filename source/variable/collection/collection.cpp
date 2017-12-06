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

#include <error_handler/error_log.h>
#include <ORM/relationship.h>
#include <ORM/orm.h>
#include <memory_handler/virtual_memory.h>
#include <variable/var.h>
#include <variable/primitive_data/primitive_data.h>
#include <variable/primitive_data/string_data.h>
#include <variable/primitive_data/char_data.h>
#include <variable/primitive_data/bool_data.h>
#include <variable/primitive_data/int_data.h>
#include <variable/primitive_data/float_data.h>
#include <variable/collection/collection.h>
#include <iostream>
#include <sstream>

#define MAX_SCAN_SIZE (8192)

/**
 * The constructor.
 *
 * @param type - data type.
 * @param size - array size in members.
 */
collection::collection(std::string id, collection *c) : var::var(std::move(id), true)
{
    this->master_relationship_add("collection", ONE_TO_MANY);

    if (c == nullptr)
    {
        return;
    }

    (*this) += (var &) *c;
}

/**
 * Get number of data.
 *
 * @return number of data.
 */
uint32_t
collection::size()
{
    return static_cast<uint32_t>(this->data_cache.size());
}

/**
 * Operator [].
 *
 * @param index - element index.
 * @return data of found, otherwise return NULL.
 */
var *
collection::operator[](uint32_t index)
{
    return (*this)[std::to_string(index)];
}

/**
 * Operator [].
 *
 * @param index - element index.
 * @return data of found, otherwise return NULL.
 */
var *
collection::operator[](std::string index)
{
    if (this->data_cache.find(index) == this->data_cache.end())
    {
        return nullptr;
    }

    return this->data_cache[index];
}

/**
 * @brief collection::insert
 * @param index
 * @param o
 */
void
collection::insert(std::string index, var *o)
{
    this->remove_data(index);
    this->insert_data(index, o);
}

/**
 * @brief collection::insert
 * @param index
 * @param o
 */
void
collection::insert(uint32_t index, var *o)
{
    this->insert(std::to_string(index), o);
}

/**
 * Operator +=. Copy and append data to this array.
 *
 * @param data - the data.
 * @return true if operation success, otherwise return false.
 */
bool
collection::operator+=(var &var)
{
    this->insert_data(std::to_string(this->size()), &var);

    return true;
}

/**
 * Convert collection into string.
 *
 * @return the data string.
 */
string_data &
collection::to_string()
{
    std::string id = this->id + " as string";
    string_data *str;

    str = (this->size() == 0) ?
          string_data::create(id) :
          string_data::create(id, this->get_string().c_str());

    return *str;
}

/**
 * Clear collection.
 */
void
collection::clear()
{
    relationship *r = this->master_relationship_get("collection");

    while (!r->empty())
    {
        this->remove_data((var *) r->front());
    }

    this->data_cache.clear();
}

/**
 * Remove data from index.
 *
 * @param index
 */
void
collection::remove_data(std::string index)
{
    this->remove_data(this->data_cache[index]);
}

/**
 * Remove data.
 *
 * @param o
 */
void
collection::remove_data(var * o)
{
    if (!o)
    {
        return;
    }

    this->data_cache.erase(o->get_id());
    this->master_relationship_remove_object("collection", o);
}

/**
 * @brief collection::insert_data
 * @param index
 * @param o
 */
void
collection::insert_data(std::string index, var *o)
{
    if (o == nullptr)
    {
        ERROR_LOG_ADD(ERROR_METHOD_ADDING_NULL_DATA);
        return;
    }

    if (!o->get_is_reference())
    {
        /*
         * Data is not a reference. Create a new data.
         */
        primitive_data *new_data;

        switch (o->get_object_type())
        {
            case OBJECT_TYPE_BOOL:
                new_data = bool_data::create(o->get_id(), *(bool_data *) o);
                break;
            case OBJECT_TYPE_CHAR:
                new_data = char_data::create(o->get_id(), *(char_data *) o);
                break;
            case OBJECT_TYPE_INT:
                new_data = int_data::create(o->get_id(), *(int_data *) o);
                break;
            case OBJECT_TYPE_FLOAT:
                new_data = float_data::create(o->get_id(), *(float_data *) o);
                break;
            case OBJECT_TYPE_STRING:
                new_data = string_data::create(o->get_id(), *(string_data *) o);
                break;
            case OBJECT_TYPE_NULL:
                ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_INVALID_DATA_TYPE);
                return;
        }

        o = new_data;
    }

    this->master_relationship_add_object("collection", o);
    this->data_cache[index] = o;
}

/**
 * @brief collection::~collection()
 */
collection::~collection()
{
    this->clear();
}

/**
 * @brief create
 * @param id
 * @param c
 * @return
 */
collection *
collection::create(std::string id, collection *c)
{
    return (collection *) orm::create((object *) new collection(std::move(id), c));
}

/**
 * @inherit
 */
object_type
collection::get_object_type()
{
    return OBJECT_TYPE_COLLECTION;
}

/**
 * @inherit
 */
bool
collection::to_bool()
{
    return true;
}

/**
 * @inherit
 */
wchar_t
collection::to_char()
{
    return (wchar_t) 0;
}

/**
 * @inherit
 */
int32_t
collection::to_int()
{
    return 0;
}

/**
 * @inherit
 */
double
collection::to_float()
{
    return (double) 0;
}

/**
 * @inherit
 */
bool
collection::operator=(const void *data)
{
    ERROR_LOG_ADD(ERROR_COLLECTION_ASSIGNING_CONSTANT);

    return false;
}

/**
 * @inherit
 */
bool
collection::print()
{
    std::wcout << this->get_string();

    return true;
}

/**
 * @inherit
 */
std::wstring
collection::get_string()
{
    std::wstring result;

    if (this->size() == 0)
    {
        return result;
    }

    for (auto &it : this->data_cache)
    {
        var *data = it.second;

        result += data->get_string();
        result += L" ";
    }

    result.pop_back();

    return result;
}

/**
 * @inherit
 */
bool
collection::println()
{
    this->print();
    std::cout << std::endl;

    return true;
}

/**
 * @inherit
 */
bool
collection::scan()
{
    wchar_t input[MAX_SCAN_SIZE];

    std::wcin.getline(input, MAX_SCAN_SIZE);

    std::wstringstream wsstream(input);
    std::wstring str;

    while (wsstream.good())
    {
        wsstream >> str;

        object_type type = data_type_detect(str);
        this->insert(
            static_cast<uint32_t>(this->data_cache.size()),
            primitive_data::create("tmp", type, str.c_str())
        );
    }

    return true;
}

/**
 * @inherit
 */
bool
collection::operator++()
{
    ERROR_LOG_ADD(ERROR_COLLECTION_ASSIGNING_CONSTANT);

    return false;
}

/**
 * @inherit
 */
bool
collection::operator--()
{
    ERROR_LOG_ADD(ERROR_COLLECTION_ASSIGNING_CONSTANT);

    return false;
}

/**
 * @inherit
 */
bool
collection::operator>(var & data)
{
    return this->size() > ((collection &) data).size();
}

/**
 * @inherit
 */
bool
collection::operator<(var & data)
{
    return this->size() < ((collection &) data).size();
}

/**
 * @inherit
 */
bool
collection::operator>=(var & data)
{
    return this->size() >= ((collection &) data).size();
}

/**
 * @inherit
 */
bool
collection::operator<=(var & data)
{
    return this->size() <= ((collection &) data).size();
}

/**
 * @inherit
 */
bool
collection::operator!=(var & data)
{
    return !this->operator==(data);
}

/**
 * @inherit
 */
bool
collection::operator==(var & data)
{
    collection &self = *this;

    if (data.get_object_type() == OBJECT_TYPE_COLLECTION)
    {
        collection &array_c = (collection &) data;

        if (self.size() != array_c.size())
        {
            return false;
        }

        for (uint32_t i = 0; i < self.size(); i++)
        {
            if (*self[i] != *array_c[i])
            {
                return false;
            }
        }
    }
    else if (primitive_data::is_primitive(&data))
    {
        if (self.size() != 1)
        {
            return false;
        }

        return *self[0] == data;
    }
    else
    {
        return false;
    }

    return true;
}

/**
 * @inherit
 */
bool
collection::operator-=(var & data)
{
    this->remove_data(&data);
}

/**
 * @inherit
 */
bool
collection::operator^=(var & data)
{
    ERROR_LOG_ADD(ERROR_COLLECTION_UNSUPPORTED_OPERATOR);

    return false;
}

/**
 * @inherit
 */
bool
collection::operator/=(var & data)
{
    ERROR_LOG_ADD(ERROR_COLLECTION_UNSUPPORTED_OPERATOR);

    return false;
}

/**
 * @inherit
 */
bool
collection::operator%=(var & data)
{
    ERROR_LOG_ADD(ERROR_COLLECTION_UNSUPPORTED_OPERATOR);

    return false;
}

/**
 * @inherit
 */
bool
collection::operator*=(var & data)
{
    ERROR_LOG_ADD(ERROR_COLLECTION_UNSUPPORTED_OPERATOR);

    return false;
}

/**
 * @inherit
 */
bool
collection::operator|=(var & data)
{
    ERROR_LOG_ADD(ERROR_COLLECTION_UNSUPPORTED_OPERATOR);

    return false;
}

/**
 * @inherit
 */
bool
collection::operator&=(var & data)
{
    ERROR_LOG_ADD(ERROR_COLLECTION_UNSUPPORTED_OPERATOR);

    return false;
}

/**
 * @inherit
 */
bool
collection::operator=(var & data)
{
    this->clear();

    return this->operator+=(data);
}