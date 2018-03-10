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

#include <error_handler/error_log.h>
#include <ORM/relationship.h>
#include <ORM/orm.h>
#include <memory_handler/memory.h>
#include <memory_handler/virtual_memory.h>
#include <variable/primitive_data/primitive_data.h>
#include <variable/primitive_data/bool_data.h>
#include <variable/primitive_data/char_data.h>
#include <variable/primitive_data/int_data.h>
#include <variable/primitive_data/float_data.h>
#include <variable/primitive_data/string_data.h>
#include <iostream>

/**
 * The constructor.
 *
 * @param type
 * @param value
 */
primitive_data::primitive_data(object_type type, const void *value) : value::value()
{
    this->master_relationship_add("primitive_data_memory", ONE_TO_MANY);
    this->vm = (virtual_memory *) orm::get_first(OBJECT_TYPE_VIRTUAL_MEMORY);

    if (type >= OBJECT_TYPE_NULL)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_INVALID_DATA_TYPE);
        return;
    }

    if ((value == nullptr) || ((type == OBJECT_TYPE_STRING) && (wcslen((const wchar_t *)value) == 0)))
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
        uint32_t size = (type == OBJECT_TYPE_STRING) ?
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
 * @param data
 */
primitive_data::primitive_data(primitive_data &data) : value::value()
{
    this->master_relationship_add("primitive_data_memory", ONE_TO_MANY);

    memory *data_mem = data.get_memory();

    if (!data_mem)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return;
    }

    if (data.get_object_type() == OBJECT_TYPE_NULL)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_INVALID_DATA_TYPE);
        return;
    }

    this->vm = data.vm;

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
 * Create data.
 *
 * @param type
 * @param value
 * @return
 */
primitive_data *
primitive_data::create(object_type type, const void *value)
{
    switch (type)
    {
        case OBJECT_TYPE_BOOL:
            return bool_data::create(value);
        case OBJECT_TYPE_CHAR:
            return char_data::create(value);
        case OBJECT_TYPE_INT:
            return int_data::create(value);
        case OBJECT_TYPE_FLOAT:
            return float_data::create(value);
        case OBJECT_TYPE_STRING:
            return string_data::create(value);
        default:
        case OBJECT_TYPE_NULL:
            ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_INVALID_DATA_TYPE);
            return nullptr;
    }
}

/**
 * Create data
 *
 * @param data
 * @return
 */
primitive_data *
primitive_data::create(primitive_data &data)
{
    switch (data.get_object_type())
    {
        case OBJECT_TYPE_BOOL:
            return bool_data::create((bool_data &) data);
        case OBJECT_TYPE_CHAR:
            return char_data::create((char_data &) data);
        case OBJECT_TYPE_INT:
            return int_data::create((int_data &) data);
        case OBJECT_TYPE_FLOAT:
            return float_data::create((float_data &) data);
        case OBJECT_TYPE_STRING:
            return string_data::create((string_data &) data);
        default:
        case OBJECT_TYPE_NULL:
            ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_INVALID_DATA_TYPE);
            return nullptr;
    }
}

/**
 * Is primitive data.
 *
 * @param data
 * @return true if primitive, otherwise return false.
 */
bool
primitive_data::is_primitive(value *data)
{
    return data->get_object_type() < OBJECT_TYPE_NULL;
}
