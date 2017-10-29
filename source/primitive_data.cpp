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

#include "primitive_data.h"
#include "virtual_memory.h"
#include "memory.h"
#include "ORM/relationship.h"
#include "ORM/orm.h"
#include "error_log.h"
#include <iostream>

/**
 * The constructor.
 *
 * @param type - data type.
 */
primitive_data::primitive_data(std::string id, data_type type, const void *value) : entity::entity("primitive_data",
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

        entity::master_relationship_add_entity("primitive_data_memory", (entity *) mem);
        default_value();
    }
    else
    {
        uint32_t size = (type == DATA_TYPE_STRING) ?
                        (uint32_t) strlen((const char *) value) + 1 :
                        DATA_TYPE_SIZE[type];

        memory *mem = vm->alloc(size);

        if (!mem)
        {
            ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
            return;
        }

        entity::master_relationship_add_entity("primitive_data_memory", (entity *) mem);
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
                               bool is_reference) : entity::entity("primitive_data", std::move(id))
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
        this->master_relationship_add_entity("primitive_data_memory", (entity *) data_mem);
        return;
    }

    memory *mem = this->vm->alloc(data_mem->get_size());
    memcpy(mem->get_pointer<void *>(),
           data_mem->get_pointer<void *>(),
           data_mem->get_size());
    this->master_relationship_add_entity("primitive_data_memory", (entity *) mem);

}

/**
 * @brief primitive_data::create
 * @param id
 * @param type
 * @param value
 * @return
 */
primitive_data *
primitive_data::create(std::string id,
                       data_type type,
                       const void *value)
{
    return (primitive_data *) orm::create((entity *) new primitive_data(std::move(id), type, value));
}

/**
 * @brief primitive_data::create
 * @param id
 * @param data
 * @param is_reference
 * @return
 */
primitive_data *
primitive_data::create(std::string id,
                       primitive_data &data,
                       bool is_reference)
{
    return (primitive_data *) orm::create((entity *) new primitive_data(std::move(id), data, is_reference));
}

/**
 * Convert data to bool and return.
 *
 * @return bool value
 */
bool
primitive_data::to_bool()
{
    bool value = false;
    memory *mem = this->get_memory();

    if (!mem)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return value;
    }

    switch (type)
    {
        case DATA_TYPE_BOOL:
            value = mem->get_element<bool>();
            break;
        case DATA_TYPE_STRING:
            value = !this->get_string().empty();
            break;
        case DATA_TYPE_CHAR:
            value = (bool) mem->get_element<int8_t>();
            break;
        case DATA_TYPE_INT:
            value = (bool) mem->get_element<int32_t>();
            break;
        case DATA_TYPE_FLOAT:
            value = (bool) mem->get_element<double>();
            break;
        case DATA_TYPE_INVALID:
        default:
            ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_INVALID_DATA_TYPE);
            break;
    }

    return value;
}

/**
 * Convert data to char and return.
 *
 * @return char value if success, otherwise return null terminating character.
 */
int8_t
primitive_data::to_char()
{
    int8_t value = 0;
    memory *mem = this->get_memory();

    if (!mem)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return value;
    }

    switch (type)
    {
        case DATA_TYPE_BOOL:
            value = (int8_t) mem->get_element<bool>();
            break;
        case DATA_TYPE_STRING:
        case DATA_TYPE_CHAR:
            value = mem->get_element<int8_t>();
            break;
        case DATA_TYPE_INT:
            value = (int8_t) mem->get_element<int32_t>();
            break;
        case DATA_TYPE_FLOAT:
            value = (int8_t) mem->get_element<double>();
            break;
        case DATA_TYPE_INVALID:
        default:
            ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_INVALID_DATA_TYPE);
            break;
    }

    return value;
}

/**
 * Convert data to int and return.
 *
 * @return int value if success, otherwise return 0.
 */
int32_t
primitive_data::to_int()
{
    int32_t value = 0;
    memory *mem = this->get_memory();

    if (!mem)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return value;
    }

    switch (type)
    {
        case DATA_TYPE_BOOL:
            value = (int32_t) mem->get_element<bool>();
            break;
        case DATA_TYPE_CHAR:
            value = (int32_t) mem->get_element<int8_t>();
            break;
        case DATA_TYPE_INT:
            value = mem->get_element<int32_t>();
            break;
        case DATA_TYPE_FLOAT:
            value = (int32_t) mem->get_element<double>();
            break;
        case DATA_TYPE_STRING:
            sscanf(mem->get_pointer<const char *>(),
                   DATA_TYPE_FORMAT[DATA_TYPE_INT],
                   &value);
            break;
        case DATA_TYPE_INVALID:
        default:
            ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_INVALID_DATA_TYPE);
            break;
    }

    return value;
}

/**
 * Convert data to float and return.
 *
 * @return float value if success, otherwise return 0.0.
 */
double
primitive_data::to_float()
{
    double value = 0.0;
    memory *mem = this->get_memory();

    if (!mem)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return value;
    }

    switch (type)
    {
        case DATA_TYPE_BOOL:
            value = (double) mem->get_element<bool>();
            break;
        case DATA_TYPE_CHAR:
            value = (double) mem->get_element<int8_t>();
            break;
        case DATA_TYPE_INT:
            value = (double) mem->get_element<int32_t>();
            break;
        case DATA_TYPE_FLOAT:
            value = mem->get_element<double>();
            break;
        case DATA_TYPE_STRING:
            sscanf(mem->get_pointer<const char *>(),
                   DATA_TYPE_FORMAT[DATA_TYPE_FLOAT],
                   &value);
            break;
        case DATA_TYPE_INVALID:
        default:
            ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_INVALID_DATA_TYPE);
            break;
    }

    return value;
}

/**
 * Convert data to string and return.
 *
 * @return the data string
 */
primitive_data &
primitive_data::to_string()
{
    std::string string = this->get_string();

    return *primitive_data::create(this->id.append(" as string"),
                                   DATA_TYPE_STRING,
                                   string.c_str());
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
 * Set data to default value.
 *
 * @return true if success, otherwise return false.
 */
bool
primitive_data::default_value()
{
    memory *mem = this->get_memory();

    if (!mem)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return false;
    }

    switch (this->type)
    {
        case DATA_TYPE_BOOL:
        case DATA_TYPE_CHAR:
        case DATA_TYPE_INT:
        case DATA_TYPE_STRING:
            memset(mem->get_pointer<void *>(), 0, mem->get_size());
            break;
        case DATA_TYPE_FLOAT:
            mem->get_element<double>() = 0.0;
            break;
        case DATA_TYPE_INVALID:
        default:
            ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_INVALID_DATA_TYPE);
            return false;
    }

    return true;
}

void
primitive_data::convert_itself(data_type new_type)
{
    memory *mem = this->get_memory();

    if (!mem)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return;
    }

    if (this->type == new_type)
    {
        /* It really makes no sense */
        return;
    }

    memory *new_mem;

    switch (new_type)
    {
        case DATA_TYPE_BOOL:
        {
            new_mem = this->vm->alloc(DATA_TYPE_SIZE[new_type]);
            new_mem->get_element<bool>() = this->to_bool();
            break;
        }
        case DATA_TYPE_CHAR:
        {
            new_mem = this->vm->alloc(DATA_TYPE_SIZE[new_type]);
            new_mem->get_element<int8_t>() = this->to_char();
            break;
        }
        case DATA_TYPE_INT:
        {
            new_mem = this->vm->alloc(DATA_TYPE_SIZE[new_type]);
            new_mem->get_element<int32_t>() = this->to_int();
            break;
        }
        case DATA_TYPE_STRING:
        {
            std::string str = this->get_string();
            new_mem = this->vm->alloc(static_cast<uint32_t>(str.size() + 1));
            strncpy(new_mem->get_pointer<char *>(),
                    str.c_str(),
                    new_mem->get_size());
            break;
        }
        case DATA_TYPE_FLOAT:
        {
            new_mem = this->vm->alloc(DATA_TYPE_SIZE[new_type]);
            new_mem->get_element<double>() = this->to_float();
            break;
        }
        case DATA_TYPE_INVALID:
        default:
            ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_INVALID_DATA_TYPE);
            return;
    }

    entity::master_relationship_add_entity("primitive_data_memory", (entity *) new_mem);
    entity::master_relationship_remove_entity("primitive_data_memory", (entity *) mem);

    this->vm->free(mem);
    this->type = new_type;
    this->is_reference = (this->type == DATA_TYPE_STRING);
}

/**
 * Operator =. Assign value from memory address.
 *
 * @param data - the data memory address.
 *
 * @return true if success, otherwise return false.
 */
bool
primitive_data::operator=(const void *data)
{
    memory *mem = this->get_memory();

    if ((!mem) || (!data))
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return false;
    }

    switch (type)
    {
        case DATA_TYPE_BOOL:
            mem->get_element<bool>() = *(bool *) data;
            break;
        case DATA_TYPE_CHAR:
            mem->get_element<int8_t>() = *(int8_t *) data;
            break;
        case DATA_TYPE_INT:
            mem->get_element<int32_t>() = *(int32_t *) data;
            break;
        case DATA_TYPE_FLOAT:
            mem->get_element<double>() = *(double *) data;
            break;
        case DATA_TYPE_STRING:
        {
            const char *str_temp = (const char *) data;
            uint32_t str_size = static_cast<uint32_t>(strlen(str_temp) + 1);

            if (mem->get_size() < str_size)
            {
                memory *new_mem = this->vm->alloc(str_size);

                entity::master_relationship_remove_entity("primitive_data_memory", (entity *) mem);
                entity::master_relationship_add_entity("primitive_data_memory", (entity *) new_mem);

                this->vm->free(mem);
                mem = new_mem;
            }

            strncpy(mem->get_pointer<char *>(), str_temp, str_size);
            break;
        }
        case DATA_TYPE_INVALID:
        default:
            ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_INVALID_DATA_TYPE);
            return false;
            break;
    }

    return true;
}

/**
 * Operator =. Assign value from another data.
 *
 * @param data - the data.
 *
 * @return true if success, otherwise return false.
 */
bool
primitive_data::operator=(primitive_data &data)
{
    memory *mem = this->get_memory();

    if ((!mem) || (data.get_address() == 0))
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return false;
    }

    if (type == DATA_TYPE_STRING)
    {
        mem->get_element<int8_t>() = 0;
        return (*this) += (data);
    }

    if (data.type == DATA_TYPE_STRING)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_ASSIGNING_STRING);
        return false;
    }

    switch (type)
    {
        case DATA_TYPE_BOOL:
            mem->get_element<bool>() = data.to_bool();
            break;
        case DATA_TYPE_CHAR:
            mem->get_element<int8_t>() = data.to_char();
            break;
        case DATA_TYPE_INT:
            mem->get_element<int32_t>() = data.to_int();
            break;
        case DATA_TYPE_FLOAT:
            mem->get_element<double>() = data.to_float();
            break;
        case DATA_TYPE_STRING:
        case DATA_TYPE_INVALID:
        default:
            ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_INVALID_DATA_TYPE);
            return false;
    }

    return true;
}

/**
 * Operator &=.
 *
 * @param data - the data.
 *
 * @return true if success, otherwise return false.
 */
bool
primitive_data::operator&=(primitive_data &data)
{
    memory *mem = this->get_memory();
    if ((!mem) || (data.get_address() == 0))
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return false;
    }

    if (data.type == DATA_TYPE_STRING)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_LOGICAL_OPERATION_STRING);
        return false;
    }

    switch (type)
    {
        case DATA_TYPE_BOOL:
            mem->get_element<bool>() &= data.to_bool();
            break;
        case DATA_TYPE_CHAR:
            mem->get_element<int8_t>() &= data.to_char();
            break;
        case DATA_TYPE_INT:
            mem->get_element<int32_t>() &= data.to_int();
            break;
        case DATA_TYPE_FLOAT:
            ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_LOGICAL_OPERATION_FLOAT);
            break;
        case DATA_TYPE_STRING:
            ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_LOGICAL_OPERATION_STRING);
            return false;
        case DATA_TYPE_INVALID:
        default:
            ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_INVALID_DATA_TYPE);
            return false;
    }

    return true;
}

/**
 * Operator |=.
 *
 * @param data - the data.
 *
 * @return true if success, otherwise return false.
 */
bool
primitive_data::operator|=(primitive_data &data)
{
    memory *mem = this->get_memory();
    if ((!mem) || (data.get_address() == 0))
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return false;
    }

    if (data.type == DATA_TYPE_STRING)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_LOGICAL_OPERATION_STRING);
        return false;
    }

    switch (this->type)
    {
        case DATA_TYPE_BOOL:
            mem->get_element<bool>() |= data.to_bool();
            break;
        case DATA_TYPE_CHAR:
            mem->get_element<int8_t>() |= data.to_char();
            break;
        case DATA_TYPE_INT:
            mem->get_element<int32_t>() |= data.to_int();
            break;
        case DATA_TYPE_FLOAT:
            ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_LOGICAL_OPERATION_FLOAT);
            break;
        case DATA_TYPE_STRING:
            ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_LOGICAL_OPERATION_STRING);
            return false;
        case DATA_TYPE_INVALID:
        default:
            ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_INVALID_DATA_TYPE);
            return false;
    }

    return true;
}

/**
 * Operator ^=
 *
 * @param data - the data.
 *
 * @return true if success, otherwise return false.
 */
bool
primitive_data::operator^=(primitive_data &data)
{
    memory *mem = this->get_memory();
    if ((!mem) || (data.get_address() == 0))
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return false;
    }

    if (data.type == DATA_TYPE_STRING)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_LOGICAL_OPERATION_STRING);
        return false;
    }

    switch (this->type)
    {
        case DATA_TYPE_BOOL:
            mem->get_element<bool>() ^= data.to_bool();
            break;
        case DATA_TYPE_CHAR:
            mem->get_element<int8_t>() ^= data.to_char();
            break;
        case DATA_TYPE_INT:
            mem->get_element<int32_t>() ^= data.to_int();
            break;
        case DATA_TYPE_FLOAT:
            ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_LOGICAL_OPERATION_FLOAT);
            break;
        case DATA_TYPE_STRING:
            ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_LOGICAL_OPERATION_STRING);
            return false;
        case DATA_TYPE_INVALID:
        default:
            ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_INVALID_DATA_TYPE);
            return false;
    }

    return true;
}

/**
 * Operator +=. Do operation add if primitive data type.
 * On string data type append data.
 *
 * @param data - the data.
 *
 * @return true if success, otherwise return false.
 */
bool
primitive_data::operator+=(primitive_data &data)
{
    memory *mem = this->get_memory();

    if ((!mem) || (data.get_address() == 0))
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return false;
    }

    if (this->type == DATA_TYPE_STRING)
    {
        std::string string = data.get_string();
        uint32_t request_size = static_cast<uint32_t>(strlen(mem->get_pointer<const char *>()) +
                                                      strlen(string.c_str()) + 1);

        if (mem->get_size() < request_size)
        {
            memory *new_mem = vm->realloc(mem, request_size);

            if (new_mem->get_size() != request_size)
            {
                /* Something really bad happened. */
                return false;
            }

            if (new_mem != mem)
            {
                /*
                 * new_mem is different than mem,
                 * switch relations.
                 */
                entity::master_relationship_remove_entity("primitive_data_memory", (entity *) mem);
                entity::master_relationship_add_entity("primitive_data_memory", (entity *) new_mem);

                mem = new_mem;
            }
        }

        strcat((char *) mem->get_address(), string.c_str());
        return true;
    }

    if (data.type == DATA_TYPE_STRING)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_ADDING_STRING);
        return false;
    }

    switch (this->type)
    {
        case DATA_TYPE_BOOL:
            /* Doesn't make any sense to do true += false */
            break;
        case DATA_TYPE_CHAR:
            mem->get_element<int8_t>() += data.to_char();
            break;
        case DATA_TYPE_INT:
            mem->get_element<int32_t>() += data.to_int();
            break;
        case DATA_TYPE_FLOAT:
            mem->get_element<double>() += data.to_float();
            break;
        case DATA_TYPE_INVALID:
        default:
            ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_INVALID_DATA_TYPE);
            return false;
    }

    return true;
}

/**
 * Operator -=. Do subtract operation.
 *
 * @param data - the data.
 *
 * @return true if success, otherwise return false.
 */
bool
primitive_data::operator-=(primitive_data &data)
{
    memory *mem = this->get_memory();
    if ((!mem) || (data.get_address() == 0))
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return false;
    }

    if (data.type == DATA_TYPE_STRING)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_SUBTRACTING_STRING);
        return false;
    }

    switch (type)
    {
        case DATA_TYPE_BOOL:
            /* Doesn't make any sense to do true -= false */
            break;
        case DATA_TYPE_CHAR:
            mem->get_element<int8_t>() -= data.to_char();
            break;
        case DATA_TYPE_INT:
            mem->get_element<int32_t>() -= data.to_int();
            break;
        case DATA_TYPE_FLOAT:
            mem->get_element<double>() -= data.to_float();
            break;
        case DATA_TYPE_STRING:
            ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_SUBTRACTING_STRING);
            return false;
        case DATA_TYPE_INVALID:
        default:
            ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_INVALID_DATA_TYPE);
            return false;
    }

    return true;
}

/**
 * Operator *=. Multiply values.
 *
 * @param data - the data.
 *
 * @return true if success, otherwise return false.
 */
bool
primitive_data::operator*=(primitive_data &data)
{
    memory *mem = this->get_memory();
    if ((!mem) || (data.get_address() == 0))
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return false;
    }

    if (data.type == DATA_TYPE_STRING)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_MULTIPLYING_STRING);
        return false;
    }

    switch (type)
    {
        case DATA_TYPE_BOOL:
            /* Doesn't make any sense to do true *= false */
            break;
        case DATA_TYPE_CHAR:
            mem->get_element<int8_t>() *= data.to_char();
            break;
        case DATA_TYPE_INT:
            mem->get_element<int32_t>() *= data.to_int();
            break;
        case DATA_TYPE_FLOAT:
            mem->get_element<double>() *= data.to_float();
            break;
        case DATA_TYPE_STRING:
            ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_MULTIPLYING_STRING);
            return false;
        case DATA_TYPE_INVALID:
        default:
            ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_INVALID_DATA_TYPE);
            return false;
    }

    return true;
}

/**
 * Operator /=. Divide values.
 *
 * @param data - the data.
 *
 * @return true if success, otherwise return false.
 */
bool
primitive_data::operator/=(primitive_data &data)
{
    memory *mem = this->get_memory();
    if ((!mem) || (data.get_address() == 0))
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return false;
    }

    if (data.type == DATA_TYPE_STRING)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_DIVIDING_STRING);
        return false;
    }

    switch (type)
    {
        case DATA_TYPE_BOOL:
            /* Doesn't make any sense to do true /= false */
            break;
        case DATA_TYPE_CHAR:

            if (data.to_char() == 0)
            {
                ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_DIVIDING_ZERO);
                return false;
            }

            mem->get_element<int8_t>() /= data.to_char();
            break;
        case DATA_TYPE_INT:

            if (data.to_int() == 0)
            {
                ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_DIVIDING_ZERO);
                return false;
            }

            mem->get_element<int32_t>() /= data.to_int();
            break;
        case DATA_TYPE_FLOAT:

            if (data.to_float() == 0.0)
            {
                ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_DIVIDING_ZERO);
                return false;
            }

            mem->get_element<double>() /= data.to_float();
            break;
        case DATA_TYPE_STRING:
            ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_DIVIDING_STRING);
            return false;
        case DATA_TYPE_INVALID:
        default:
            ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_INVALID_DATA_TYPE);
            return false;
    }

    return true;
}

/**
 * Operator %=. Do modulus between values.
 *
 * @param data - the data.
 *
 * @return true if success, otherwise return false.
 */
bool
primitive_data::operator%=(primitive_data &data)
{
    memory *mem = this->get_memory();
    if ((!mem) || (data.get_address() == 0))
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return false;
    }

    if ((data.type == DATA_TYPE_FLOAT) ||
        (data.type == DATA_TYPE_STRING) ||
        (data.type == DATA_TYPE_BOOL))
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_INVALID_MODULUS);
        return false;
    }

    switch (this->type)
    {
        case DATA_TYPE_BOOL:
            /* Doesn't make any sense to do true %= false */
            break;
        case DATA_TYPE_CHAR:
            if (data.to_char() == 0)
            {
                ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_MODULUS_ZERO);
                return false;
            }

            mem->get_element<int8_t>() %= data.to_char();
            break;
        case DATA_TYPE_INT:

            if (data.to_int() == 0)
            {
                ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_MODULUS_ZERO);
                return false;
            }

            mem->get_element<int32_t>() %= data.to_int();
            break;
        case DATA_TYPE_FLOAT:
        case DATA_TYPE_STRING:
            ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_INVALID_MODULUS);
            return false;
        case DATA_TYPE_INVALID:
        default:
            ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_INVALID_DATA_TYPE);
            return false;
    }

    return true;
}

/**
 * Operator ++. Increment value.
 *
 * @return true if success, otherwise return false.
 */
bool
primitive_data::operator++()
{
    memory *mem = this->get_memory();
    if (!mem)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return false;
    }

    switch (type)
    {
        case DATA_TYPE_BOOL:
            /* Doesn't make any sense to do true++ */
            break;
        case DATA_TYPE_CHAR:
            mem->get_element<int8_t>()++;
            break;
        case DATA_TYPE_INT:
            mem->get_element<int32_t>()++;
            break;
        case DATA_TYPE_FLOAT:
            mem->get_element<double>() += 1.0;
            break;
        case DATA_TYPE_STRING:
            ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_INCREMENTING_STRING);
            return false;
        case DATA_TYPE_INVALID:
        default:
            ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_INVALID_DATA_TYPE);
            return false;
    }

    return true;
}

/**
 * Operator --. Decrement value.
 *
 * @return true if success, otherwise return false.
 */
bool
primitive_data::operator--()
{
    memory *mem = this->get_memory();
    if (!mem)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return false;
    }

    switch (type)
    {
        case DATA_TYPE_BOOL:
            /* Doesn't make any sense to do false-- */
            break;
        case DATA_TYPE_CHAR:
            mem->get_element<int8_t>()--;
            break;
        case DATA_TYPE_INT:
            mem->get_element<int32_t>()--;
            break;
        case DATA_TYPE_FLOAT:
            mem->get_element<double>() -= 1.0;
            break;
        case DATA_TYPE_STRING:
            ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_DECREMENTING_STRING);
            return false;
        case DATA_TYPE_INVALID:
        default:
            ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_INVALID_DATA_TYPE);
            return false;
    }

    return true;
}

/**
 * Operator ==. Check values equality.
 *
 * @param data - the data.
 *
 * @return true if equal, otherwise return false.
 */
bool
primitive_data::operator==(primitive_data &data)
{
    memory *mem = this->get_memory();
    if ((!mem) || (data.get_address() == 0))
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return false;
    }

    switch (type)
    {
        case DATA_TYPE_BOOL:
            return to_bool() == data.to_bool();
        case DATA_TYPE_CHAR:
            return to_char() == data.to_char();
        case DATA_TYPE_INT:
            return to_int() == data.to_int();
        case DATA_TYPE_FLOAT:
            return to_float() == data.to_float();
        case DATA_TYPE_STRING:
        {
            if (data.type == DATA_TYPE_STRING)
            {
                return strcmp(mem->get_pointer<const char *>(),
                              (const char *) data.get_address()) == 0;
            }
            else
            {
                std::string string = data.get_string();
                return string.compare(mem->get_pointer<const char *>()) == 0;
            }
        }
        case DATA_TYPE_INVALID:
        default:
            ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_INVALID_DATA_TYPE);
            return false;
    }

    return true;
}

/**
 * Operator ==. Check values non equality.
 *
 * @param data - the data.
 *
 * @return true if not equal, otherwise return false.
 */
bool
primitive_data::operator!=(primitive_data &data)
{
    memory *mem = this->get_memory();
    if ((!mem) || (data.get_address() == 0))
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return false;
    }

    switch (type)
    {
        case DATA_TYPE_BOOL:
            return to_bool() != data.to_bool();
        case DATA_TYPE_CHAR:
            return to_char() != data.to_char();
        case DATA_TYPE_INT:
            return to_int() != data.to_int();
        case DATA_TYPE_FLOAT:
            return to_float() != data.to_float();
        case DATA_TYPE_STRING:
            if (data.type == DATA_TYPE_STRING)
            {
                return strcmp(mem->get_pointer<const char *>(),
                              (const char *) data.get_address()) != 0;
            }
            else
            {
                std::string string = data.get_string();
                return string != mem->get_pointer<const char *>();
            }
        case DATA_TYPE_INVALID:
        default:
            ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_INVALID_DATA_TYPE);
            return false;
    }

    return true;
}

/**
 * Operator >. Check if this value is bigger than another.
 *
 * @param data - the data.
 *
 * @return true if this value is bigger, otherwise return false.
 */
bool
primitive_data::operator>(primitive_data &data)
{
    memory *mem = this->get_memory();
    if ((!mem) || (data.get_address() == 0))
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return false;
    }

    switch (this->type)
    {
        case DATA_TYPE_BOOL:
            /* doesn't make any sense "true > false" */
            break;
        case DATA_TYPE_CHAR:
            return to_char() > data.to_char();
        case DATA_TYPE_INT:
            return to_int() > data.to_int();
        case DATA_TYPE_FLOAT:
            return to_float() > data.to_float();
        case DATA_TYPE_STRING:
        {
            if (data.type == DATA_TYPE_STRING)
            {
                return strcmp(mem->get_pointer<const char *>(),
                              (const char *) data.get_address()) > 0;
            }
            else
            {
                std::string string = data.get_string();
                return string.compare(mem->get_pointer<const char *>()) < 0;
            }
            break;
        }
        case DATA_TYPE_INVALID:
        default:
            ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_INVALID_DATA_TYPE);
            return false;
            break;
    }

    return true;
}

/**
 * Operator <. Check if this value is lesser than another.
 *
 * @param data - the data.
 *
 * @return true if this value is lesser, otherwise return false.
 */
bool
primitive_data::operator<(primitive_data &data)
{
    memory *mem = this->get_memory();
    if ((!mem) || (data.get_address() == 0))
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return false;
    }

    switch (type)
    {
        case DATA_TYPE_BOOL:
            /* doesn't make any sense "true < false" */
            break;
        case DATA_TYPE_CHAR:
            return to_char() < data.to_char();
        case DATA_TYPE_INT:
            return to_int() < data.to_int();
        case DATA_TYPE_FLOAT:
            return to_float() < data.to_float();
        case DATA_TYPE_STRING:
            if (data.type == DATA_TYPE_STRING)
            {
                return strcmp(mem->get_pointer<const char *>(),
                              (const char *) data.get_address()) < 0;
            }
            else
            {
                std::string string = data.get_string();
                return string.compare(mem->get_pointer<const char *>()) > 0;
            }
        case DATA_TYPE_INVALID:
        default:
            ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_INVALID_DATA_TYPE);
            return false;
    }

    return true;
}

/**
 * Operator >=. Check if this value is bigger or equal than another.
 *
 * @param data - the data.
 *
 * @return true if this value is bigger or equal, otherwise return false.
 */
bool
primitive_data::operator>=(primitive_data &data)
{
    memory *mem = this->get_memory();
    if ((!mem) || (data.get_address() == 0))
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return false;
    }

    switch (type)
    {
        case DATA_TYPE_BOOL:
            /* doesn't make any sense "true >= false" */
            break;
        case DATA_TYPE_CHAR:
            return to_char() >= data.to_char();
        case DATA_TYPE_INT:
            return to_int() >= data.to_int();
        case DATA_TYPE_FLOAT:
            return to_float() >= data.to_float();
        case DATA_TYPE_STRING:
            if (data.type == DATA_TYPE_STRING)
            {
                return strcmp(mem->get_pointer<const char *>(),
                              (const char *) data.get_address()) >= 0;
            }
            else
            {
                std::string string = data.get_string();
                return string.compare(mem->get_pointer<const char *>()) <= 0;
            }
        case DATA_TYPE_INVALID:
        default:
            ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_INVALID_DATA_TYPE);
            return false;
    }

    return true;
}

/**
 * Operator <=. Check if this value is smaller than another.
 *
 * @param data - the data.
 *
 * @return true if this value is smaller, otherwise return false.
 */
bool
primitive_data::operator<=(primitive_data &data)
{
    memory *mem = this->get_memory();
    if ((!mem) || (data.get_address() == 0))
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return false;
    }

    switch (type)
    {
        case DATA_TYPE_BOOL:
            /* doesn't make any sense "true <= false" */
            break;
        case DATA_TYPE_CHAR:
            return to_char() <= data.to_char();
        case DATA_TYPE_INT:
            return to_int() <= data.to_int();
        case DATA_TYPE_FLOAT:
            return to_float() <= data.to_float();
        case DATA_TYPE_STRING:
            if (data.type == DATA_TYPE_STRING)
            {
                return strcmp(mem->get_pointer<const char *>(),
                              (const char *) data.get_address()) <= 0;
            }
            else
            {
                std::string string = data.get_string();
                return string.compare(mem->get_pointer<const char *>()) >= 0;
            }
        case DATA_TYPE_INVALID:
        default:
            ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_INVALID_DATA_TYPE);
            return false;
    }

    return true;
}

/**
 * Print value on console.
 *
 * @return true if success, otherwise return false.
 */
bool
primitive_data::print()
{
    memory *mem = this->get_memory();
    if (!mem)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return false;
    }

    switch (type)
    {
        case DATA_TYPE_BOOL:
            std::cout << (to_bool() ? "true" : "false");
            break;
        case DATA_TYPE_CHAR:
            std::cout << to_char();
            break;
        case DATA_TYPE_INT:
            std::cout << to_int();
            break;
        case DATA_TYPE_FLOAT:
            std::cout << to_float();
            break;
        case DATA_TYPE_STRING:
            std::cout << mem->get_pointer<char *>();
            break;
        case DATA_TYPE_INVALID:
        default:
            ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_INVALID_DATA_TYPE);
            return false;
    }

    return true;
}

/**
 * Print value on screen together with new line.
 *
 * @return true if success, otherwise return false.
 */
bool primitive_data::println()
{
    if (print())
    {
        std::cout << std::endl;
        return true;
    }

    return false;
}

/**
 * Insert value from console terminal into data.
 *
 * @return true if success, otherwise return false.
 */
bool
primitive_data::scan()
{
    memory *mem = this->get_memory();
    if (!mem)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return false;
    }

    switch (type)
    {
        case DATA_TYPE_BOOL:
            std::cin >> mem->get_element<bool>();
            break;
        case DATA_TYPE_CHAR:
            std::cin >> mem->get_element<int8_t>();
            break;
        case DATA_TYPE_INT:
            std::cin >> mem->get_element<int32_t>();
            break;
        case DATA_TYPE_FLOAT:
            std::cin >> mem->get_element<double>();
            break;
        case DATA_TYPE_STRING:
            std::cin >> mem->get_pointer<int8_t *>();
            break;
        case DATA_TYPE_INVALID:
        default:
            ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_INVALID_DATA_TYPE);
            return false;
    }

    return true;
}

/**
 * Get string.
 *
 * @return string.
 */
std::string
primitive_data::get_string()
{
    std::string str;
    memory *mem = this->get_memory();

    if (!mem)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return str;
    }

    switch (type)
    {
        case DATA_TYPE_BOOL:
            str = to_bool() ? "true" : "false";
            break;
        case DATA_TYPE_CHAR:
            str = to_char();
            break;
        case DATA_TYPE_INT:
            str = std::to_string(to_int());
            break;
        case DATA_TYPE_FLOAT:
            str = std::to_string(to_float());
            break;
        case DATA_TYPE_STRING:
            str.assign(mem->get_pointer<const char *>());
            break;
        case DATA_TYPE_INVALID:
        default:
            ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_INVALID_DATA_TYPE);
            break;
    }

    return str;
}
