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

#include "primitive_data/primitive_data.h"
#include "virtual_memory.h"
#include "memory.h"
#include "ORM/relationship.h"
#include "ORM/orm.h"
#include "error_log.h"
#include <iostream>
#include <primitive_data/bool_data.h>
#include <primitive_data/char_data.h>
#include <primitive_data/int_data.h>
#include <primitive_data/float_data.h>
#include <primitive_data/string_data.h>

/**
 * The constructor.
 *
 * @param type - data type.
 */
primitive_data::primitive_data(std::string id, data_type type, const void *value) : object::object("primitive_data",
                                                                                                   std::move(id))
{
    this->master_relationship_add("primitive_data_memory", ONE_TO_MANY);
    this->vm = (virtual_memory *) orm::get_first("virtual_memory");
    this->type = type;

    if (this->type >= DATA_TYPE_INVALID)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_INVALID_DATA_TYPE);
        return;
    }

    this->is_reference = (this->type == DATA_TYPE_STRING);

    if (value == nullptr)
    {
        memory *mem = this->vm->alloc(DATA_TYPE_SIZE[type]);

        if (!mem)
        {
            ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
            return;
        }

        object::master_relationship_add_object("primitive_data_memory", (object *) mem);
    }
    else
    {
        uint32_t size = (type == DATA_TYPE_STRING) ?
                        (uint32_t) (wcslen((const wchar_t *) value) + 1) * sizeof(wchar_t) :
                        DATA_TYPE_SIZE[type];

        memory *mem = vm->alloc(size);

        if (!mem)
        {
            ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
            return;
        }

        object::master_relationship_add_object("primitive_data_memory", (object *) mem);
        memcpy(mem->get_pointer<void *>(), value, size);
    }
}

/**
 * The copy constructor.
 *
 * @param id
 * @param data
 */
primitive_data::primitive_data(std::string id,
                               primitive_data &data,
                               bool is_reference) : object::object("primitive_data", std::move(id))
{
    this->master_relationship_add("primitive_data_memory", ONE_TO_MANY);

    memory *data_mem = data.get_memory();

    if (!data_mem)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return;
    }

    if (data.type >= DATA_TYPE_INVALID)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_INVALID_DATA_TYPE);
        return;
    }

    this->type = data.type;
    this->vm = data.vm;
    this->is_reference = (this->type == DATA_TYPE_STRING) || is_reference;

    if (this->is_reference)
    {
        this->master_relationship_add_object("primitive_data_memory", (object *) data_mem);
        return;
    }

    memory *mem = this->vm->alloc(data_mem->get_size());
    memcpy(mem->get_pointer<void *>(),
           data_mem->get_pointer<void *>(),
           data_mem->get_size());
    this->master_relationship_add_object("primitive_data_memory", (object *) mem);

}

/**
 * Get memory address.
 *
 * @return memory address if success, otherwise return 0.
 */
uintptr_t
primitive_data::get_address()
{
    memory *mem = this->get_memory();
    return (mem != nullptr) ? (uintptr_t) mem->get_pointer<void *>() : 0;
}

/**
 * Get memory information.
 *
 * @return memory information.
 */
memory *
primitive_data::get_memory()
{
    return (memory *) this->master_relationship_get("primitive_data_memory")->front();
}

/**
 * Get data type.
 *
 * @return data type.
 */
data_type
primitive_data::get_type()
{
    return this->type;
}

/**
 * Get is_reference.
 *
 * @return
 */
bool
primitive_data::get_is_reference()
{
    return this->is_reference;
}

/**
 * Create data.
 *
 * @param id
 * @param type
 * @param value
 * @return
 */
primitive_data *
primitive_data::create(std::string id, data_type type, const void *value)
{
    switch (type)
    {
        case DATA_TYPE_BOOL:
            return bool_data::create(std::move(id), value);
        case DATA_TYPE_CHAR:
            return char_data::create(std::move(id), value);
        case DATA_TYPE_INT:
            return int_data::create(std::move(id), value);
        case DATA_TYPE_FLOAT:
            return float_data::create(std::move(id), value);
        case DATA_TYPE_STRING:
            return string_data::create(std::move(id), value);
        default:
        case DATA_TYPE_INVALID:
            ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_INVALID_DATA_TYPE);
            return nullptr;
    }
}

/**
 * Create data
 *
 * @param id
 * @param data
 * @param is_reference
 * @return
 */
primitive_data *
primitive_data::create(std::string id, primitive_data &data, bool is_reference)
{
    switch (data.get_type())
    {
        case DATA_TYPE_BOOL:
            return bool_data::create(std::move(id), (bool_data &)data, is_reference);
        case DATA_TYPE_CHAR:
            return char_data::create(std::move(id), (char_data &)data, is_reference);
        case DATA_TYPE_INT:
            return int_data::create(std::move(id), (int_data &)data, is_reference);
        case DATA_TYPE_FLOAT:
            return float_data::create(std::move(id), (float_data &)data, is_reference);
        case DATA_TYPE_STRING:
            return string_data::create(std::move(id), (string_data &)data);
        default:
        case DATA_TYPE_INVALID:
            ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_INVALID_DATA_TYPE);
            return nullptr;
    }
}
