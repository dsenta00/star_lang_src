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


#include <ConstantBundle/Constants.h>
#include <VariableBundle/Value.h>
#include <ORM/ORM.h>
#include <ErrorBundle/ErrorLog.h>

/**
 * The constructor.
 */
Constants::Constants() : Object::Object(0)
{
    this->getMaster()->init("Constants", ONE_TO_MANY);
}

/**
 * Add value.
 *
 * @param val
 */
void
Constants::add(Value *val)
{
    this->getMaster()->add("Constants", val);
    this->values.push_back(val);
}

/**
 * Get value.
 *
 * @param i
 * @return
 */
Value *
Constants::get(uint32_t i)
{
    if (i < this->values.size())
    {
        return this->values[i];
    }

    ERROR_LOG_ADD(ERROR_CONSTANT_UNDEFINED);
    return (Value *)ORM::getFirst(OBJECT_TYPE_NULL);
}

/**
 * Create constants.
 * @return
 */
Constants *
Constants::create()
{
    return (Constants *)ORM::create(new Constants());
}

eObjectType
Constants::getObjectType()
{
    return OBJECT_TYPE_CONSTANTS;
}
