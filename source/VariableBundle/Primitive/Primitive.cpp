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

#include <ORM/ORM.h>
#include <ORM/Relationship.h>
#include <ORM/MasterRelationships.h>
#include <ErrorBundle/ErrorLog.h>
#include <MemoryBundle/Memory.h>
#include <MemoryBundle/VirtualMemory.h>
#include <VariableBundle/Primitive/Primitive.h>
#include <VariableBundle/Primitive/Bool.h>
#include <VariableBundle/Primitive/Char.h>
#include <VariableBundle/Primitive/Int.h>
#include <VariableBundle/Primitive/Float.h>
#include <VariableBundle/Primitive/String.h>
#include <iostream>
#include <cstring>

/**
 * The constructor.
 *
 * @param type
 * @param value
 */
Primitive::Primitive(eObjectType type, const void *value) : Value::Value()
{
    MasterRelationships *master = this->getMaster();
    master->init("primitive_data_memory", ONE_TO_MANY);

    if (type >= OBJECT_TYPE_NULL)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_INVALID_DATA_TYPE);
        return;
    }

    if ((value == nullptr) || ((type == OBJECT_TYPE_STRING) && (wcslen((const wchar_t *)value) == 0)))
    {
        Memory *mem = this->getVirtualMemory()->alloc(DataType::SIZE[type]);

        if (!mem)
        {
            ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
            return;
        }

        master->add("primitive_data_memory", (Object *) mem);
    }
    else
    {
        uint32_t size = (type == OBJECT_TYPE_STRING) ?
                        (uint32_t) (wcslen((const wchar_t *) value) + 1) * sizeof(wchar_t) :
                        DataType::SIZE[type];

        Memory *mem = this->getVirtualMemory()->alloc(size);

        if (!mem)
        {
            ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
            return;
        }

        master->add("primitive_data_memory", (Object *) mem);
        memcpy(mem->getPointer<void *>(), value, size);
    }
}

/**
 * The copy constructor.
 *
 * @param data
 */
Primitive::Primitive(Primitive &data) : Value::Value()
{
    MasterRelationships *master = this->getMaster();
    master->init("primitive_data_memory", ONE_TO_MANY);

    Memory *data_mem = data.getMemory();

    if (!data_mem)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_NULL_DATA);
        return;
    }

    if (data.getObjectType() == OBJECT_TYPE_NULL)
    {
        ERROR_LOG_ADD(ERROR_PRIMITIVE_DATA_INVALID_DATA_TYPE);
        return;
    }

    Memory *mem = this->getVirtualMemory()->alloc(data_mem->getSize());

    memcpy(mem->getPointer<void *>(),
           data_mem->getPointer<void *>(),
           data_mem->getSize());

    master->add("primitive_data_memory", (Object *) mem);
}

/**
 * Get memory address.
 *
 * @return memory address if success, otherwise return 0.
 */
uintptr_t
Primitive::getAddress()
{
    Memory *mem = this->getMemory();

    return (mem != nullptr) ? (uintptr_t) mem->getPointer<void *>() : 0;
}

/**
 * Get memory information.
 *
 * @return memory information.
 */
Memory *
Primitive::getMemory()
{
    return (Memory *) this->getMaster()->front("primitive_data_memory");
}

/**
 * Create data.
 *
 * @param type
 * @param value
 * @return
 */
Primitive *
Primitive::create(eObjectType type, const void *value)
{
    switch (type)
    {
        case OBJECT_TYPE_BOOL:
            return Bool::create(value);
        case OBJECT_TYPE_CHAR:
            return Char::create(value);
        case OBJECT_TYPE_INT:
            return Int::create(value);
        case OBJECT_TYPE_FLOAT:
            return Float::create(value);
        case OBJECT_TYPE_STRING:
            return String::create(value);
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
Primitive *
Primitive::create(Primitive &data)
{
    switch (data.getObjectType())
    {
        case OBJECT_TYPE_BOOL:
            return Bool::create((Bool &) data);
        case OBJECT_TYPE_CHAR:
            return Char::create((Char &) data);
        case OBJECT_TYPE_INT:
            return Int::create((Int &) data);
        case OBJECT_TYPE_FLOAT:
            return Float::create((Float &) data);
        case OBJECT_TYPE_STRING:
            return String::create((String &) data);
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
Primitive::isPrimitive(Value *data)
{
    return data->getObjectType() < OBJECT_TYPE_NULL;
}

/**
 * Get virtual memory.
 *
 * @return
 */
VirtualMemory *
Primitive::getVirtualMemory()
{
    return (VirtualMemory *) ORM::getFirst(OBJECT_TYPE_VIRTUAL_MEMORY);
}
