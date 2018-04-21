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

#include <ORM/ORM.h>
#include <ORM/Relationship.h>
#include <ErrorBundle/ErrorLog.h>
#include <MemoryBundle/VirtualMemory.h>
#include <VariableBundle/Value.h>
#include <VariableBundle/Primitive/Primitive.h>
#include <VariableBundle/Primitive/String.h>
#include <VariableBundle/Primitive/Char.h>
#include <VariableBundle/Primitive/Bool.h>
#include <VariableBundle/Primitive/Int.h>
#include <VariableBundle/Primitive/Float.h>
#include <VariableBundle/Collection/Collection.h>
#include <VariableBundle/Null/Null.h>
#include <iostream>
#include <sstream>

#define MAX_SCAN_SIZE (8192)

/**
 * The constructor.
 *
 * @param c
 */
Collection::Collection(Collection *c) : Value::Value()
{
    this->masterRelationshipAdd("Collection", ONE_TO_MANY);

    if (c == nullptr)
    {
        return;
    }

    (*this) += (Value &) *c;
}

/**
 * Get number of data.
 *
 * @return number of data.
 */
uint32_t
Collection::size()
{
    return static_cast<uint32_t>(this->data_cache.size());
}

/**
 * Operator [].
 *
 * @param index - element index.
 * @return data of found, otherwise return NULL.
 */
Value *
Collection::operator[](uint32_t index)
{
    return (*this)[std::to_string(index)];
}

/**
 * Operator [].
 *
 * @param index - element index.
 * @return data of found, otherwise return NULL.
 */
Value *
Collection::operator[](std::string index)
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
Collection::insert(std::string index, Value *o)
{
    this->removeData(index);
    this->insertData(index, o);
}

/**
 * @brief collection::insert
 * @param index
 * @param o
 */
void
Collection::insert(uint32_t index, Value *o)
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
Collection::operator+=(Value &var)
{
    this->insertData(std::to_string(this->size()), &var);

    return true;
}

/**
 * Convert collection into string.
 *
 * @return the data string.
 */
String &
Collection::toString()
{
    String *str;

    str = (this->size() == 0) ?
          String::create() :
          String::create(this->getString().c_str());

    return *str;
}

/**
 * Clear collection.
 */
void
Collection::clear()
{
    Relationship *r = this->masterRelationshipGet("Collection");

    while (!r->empty())
    {
        this->removeData((Value *) r->front());
    }

    this->data_cache.clear();
}

/**
 * Remove data from index.
 *
 * @param index
 */
void
Collection::removeData(std::string index)
{
    this->removeData(this->data_cache[index]);
}

/**
 * Remove data.
 *
 * @param o
 */
void
Collection::removeData(Value *o)
{
    if (!o)
    {
        return;
    }

    this->data_cache.erase(o->getId());
    this->masterRelationshipRemoveObject("Collection", o);
}

/**
 * @brief collection::insert_data
 * @param index
 * @param o
 */
void
Collection::insertData(std::string index, Value *o)
{
    if (o == nullptr)
    {
        ERROR_LOG_ADD(ERROR_METHOD_ADDING_NULL_DATA);
        return;
    }

    if (!o->isReference())
    {
        /*
         * Data is not a reference. Create a new data.
         */
        Value *newData;

        switch (o->getObjectType())
        {
            case OBJECT_TYPE_BOOL:
                newData = Bool::create(*(Bool *) o);
                break;
            case OBJECT_TYPE_CHAR:
                newData = Char::create(*(Char *) o);
                break;
            case OBJECT_TYPE_INT:
                newData = Int::create(*(Int *) o);
                break;
            case OBJECT_TYPE_FLOAT:
                newData = Float::create(*(Float *) o);
                break;
            case OBJECT_TYPE_STRING:
                newData = String::create(*(String *) o);
                break;
            default:
            case OBJECT_TYPE_NULL:
                ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_INVALID_DATA_TYPE);
                newData = (Null *) ORM::getFirst(OBJECT_TYPE_NULL);
        }

        o = newData;
    }

    this->masterRelationshipAddObject("Collection", o);
    this->data_cache[index] = o;
}

/**
 * @brief collection::~collection()
 */
Collection::~Collection()
{
    this->clear();
}

/**
 * @brief create
 *
 * @param c
 * @return
 */
Collection *
Collection::create(Collection *c)
{
    return (Collection *) ORM::create((Object *) new Collection(c));
}

/**
 * @inherit
 */
eObjectType
Collection::getObjectType()
{
    return OBJECT_TYPE_COLLECTION;
}

/**
 * @inherit
 */
bool
Collection::toBool()
{
    return true;
}

/**
 * @inherit
 */
wchar_t
Collection::toChar()
{
    return (wchar_t) 0;
}

/**
 * @inherit
 */
int32_t
Collection::toInt()
{
    return 0;
}

/**
 * @inherit
 */
double
Collection::toFloat()
{
    return (double) 0;
}

/**
 * @inherit
 */
bool
Collection::operator=(const void *data)
{
    ERROR_LOG_ADD(ERROR_COLLECTION_ASSIGNING_CONSTANT);

    return false;
}

/**
 * @inherit
 */
bool
Collection::print()
{
    std::wcout << this->getString();

    return true;
}

/**
 * @inherit
 */
std::wstring
Collection::getString()
{
    std::wstring result;

    if (this->size() == 0)
    {
        return result;
    }

    for (auto &it : this->data_cache)
    {
        Value *data = it.second;

        result += data->getString();
        result += L" ";
    }

    result.pop_back();

    return result;
}

/**
 * @inherit
 */
bool
Collection::println()
{
    this->print();
    std::cout << std::endl;

    return true;
}

/**
 * @inherit
 */
bool
Collection::scan()
{
    wchar_t input[MAX_SCAN_SIZE];

    std::wcin.getline(input, MAX_SCAN_SIZE);
    this->parseStream(input);

    return true;
}

/**
 * @inherit
 */
bool
Collection::operator++()
{
    ERROR_LOG_ADD(ERROR_COLLECTION_ASSIGNING_CONSTANT);

    return false;
}

/**
 * @inherit
 */
bool
Collection::operator--()
{
    ERROR_LOG_ADD(ERROR_COLLECTION_ASSIGNING_CONSTANT);

    return false;
}

/**
 * Parse stream string.
 *
 * @param input
 */
void
Collection::parseStream(std::wstring input)
{
    std::wstringstream wsstream(input);
    std::wstring str;

    while (wsstream.good())
    {
        wsstream >> str;
        eObjectType type = DataType::detect(str);

        DataType::cleanConstantFormat(str, type);
        Value *data;

        switch (type)
        {
            case OBJECT_TYPE_BOOL:
                data = Bool::create(Bool::parse(str));
                break;
            case OBJECT_TYPE_CHAR:
                data = Char::create(str.c_str());
                break;
            case OBJECT_TYPE_INT:
                data = Int::create(Int::parse(str));
                break;
            case OBJECT_TYPE_FLOAT:
                data = Float::create(Float::parse(str));
                break;
            case OBJECT_TYPE_STRING:
                data = String::create(str.c_str());
                break;
            case OBJECT_TYPE_NULL:
                data = (Null *) ORM::getFirst(OBJECT_TYPE_NULL);
                break;
            default:
            case OBJECT_TYPE_COLLECTION:
            case OBJECT_TYPE_INSTRUCTION:
            case OBJECT_TYPE_MEMORY:
            case OBJECT_TYPE_MEMORY_CHUNK:
            case OBJECT_TYPE_VIRTUAL_MEMORY:
            case OBJECT_TYPE_METHOD:
            case OBJECT_TYPE_FILE:
                continue;
        }

        this->insert(static_cast<uint32_t>(this->data_cache.size()), data);
    }
}

/**
 * @inherit
 */
bool
Collection::isReference()
{
    return true;
}

/**
 * @inherit
 */
bool
Collection::operator>(Value & data)
{
    return this->size() > ((Collection &) data).size();
}

/**
 * @inherit
 */
bool
Collection::operator<(Value & data)
{
    return this->size() < ((Collection &) data).size();
}

/**
 * @inherit
 */
bool
Collection::operator>=(Value & data)
{
    return this->size() >= ((Collection &) data).size();
}

/**
 * @inherit
 */
bool
Collection::operator<=(Value & data)
{
    return this->size() <= ((Collection &) data).size();
}

/**
 * @inherit
 */
bool
Collection::operator!=(Value & data)
{
    return !this->operator==(data);
}

/**
 * @inherit
 */
bool
Collection::operator==(Value & data)
{
    Collection &self = *this;

    if (data.getObjectType() == OBJECT_TYPE_COLLECTION)
    {
        Collection &c = (Collection &) data;

        if (self.size() != c.size())
        {
            return false;
        }

        for (uint32_t i = 0; i < self.size(); i++)
        {
            if (*self[i] != *c[i])
            {
                return false;
            }
        }
    }
    else if (Primitive::isPrimitive(&data))
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
Collection::operator-=(Value & data)
{
    this->removeData(&data);

    return true;
}

/**
 * @inherit
 */
bool
Collection::operator^=(Value & data)
{
    ERROR_LOG_ADD(ERROR_COLLECTION_UNSUPPORTED_OPERATOR);

    return false;
}

/**
 * @inherit
 */
bool
Collection::operator/=(Value & data)
{
    ERROR_LOG_ADD(ERROR_COLLECTION_UNSUPPORTED_OPERATOR);

    return false;
}

/**
 * @inherit
 */
bool
Collection::operator%=(Value & data)
{
    ERROR_LOG_ADD(ERROR_COLLECTION_UNSUPPORTED_OPERATOR);

    return false;
}

/**
 * @inherit
 */
bool
Collection::operator*=(Value & data)
{
    ERROR_LOG_ADD(ERROR_COLLECTION_UNSUPPORTED_OPERATOR);

    return false;
}

/**
 * @inherit
 */
bool
Collection::operator|=(Value & data)
{
    ERROR_LOG_ADD(ERROR_COLLECTION_UNSUPPORTED_OPERATOR);

    return false;
}

/**
 * @inherit
 */
bool
Collection::operator&=(Value & data)
{
    ERROR_LOG_ADD(ERROR_COLLECTION_UNSUPPORTED_OPERATOR);

    return false;
}

/**
 * @inherit
 */
bool
Collection::operator=(Value & data)
{
    this->clear();

    return this->operator+=(data);
}