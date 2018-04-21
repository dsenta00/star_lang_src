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

#include "ErrorBundle/ErrorLog.h"
#include <VariableBundle/Primitive/String.h>
#include <VariableBundle/Primitive/Int.h>
#include <VariableBundle/Primitive/Float.h>
#include "VariableBundle/Collection/Collection.h"
#include <climits>
#include "../../../include/VariableBundle/Collection/collection_test.h"
#include "../../../test_assert.h"
#include "ORM/ORM.h"

static VirtualMemory *vm;

#define ARRAY_SIZE (10)

/**
 * Return a random name.
 *
 * @return random name.
 */
static const wchar_t *random_name(int32_t i = -1)
{
    static const wchar_t str[][64] =
        {
            L"Miljenko",
            L"Mislav",
            L"Ivan",
            L"Danijela",
            L"Antonia",
            L"Ivanka",
            L"Vesna",
            L"Matea",
            L"Ivana",
            L"Ante",
        };

    return (i == -1) ? str[rand() % 10] : str[i % 10];
}

/**
 * Test collection basics.
 */
static void
collection_test_basic()
{
    ASSERT_VIRTUAL_MEMORY(*vm, 0);
    Collection &empty_collection = *Collection::create();
    ASSERT_VIRTUAL_MEMORY(*vm, 0);

    ASSERT_EQUALS(empty_collection.size(), 0);

    for (uint16_t i = 0; i < USHRT_MAX; i++)
    {
        ASSERT_TRUE(empty_collection[i] == nullptr,
                    "Returned reference should be NULL! (0x%X)",
                    empty_collection[i]);
    }

    ASSERT_EQUALS(empty_collection.size(), 0);
    ASSERT_VIRTUAL_MEMORY(*vm, 0);

    Primitive &str = empty_collection.toString();
    ASSERT_TRUE(wcscmp((const wchar_t *) str.getAddress(), L"") == 0,
                "Returned string should be empty! (%ls)",
                (const wchar_t *) str.getAddress());
    ASSERT_VIRTUAL_MEMORY(*vm, DataType::SIZE[OBJECT_TYPE_STRING]);

    std::wstring comparision;

    for (uint32_t i = 0; i < ARRAY_SIZE; i++)
    {
        if (i % 2 == 0)
        {
            Primitive *data = String::create(random_name(i));
            empty_collection.insert(i, data);
            ASSERT_OK;
            comparision.append(random_name(i));
        }
        else if (i % 3 == 0)
        {
            Primitive *data = Int::create(&i);
            empty_collection.insert(i, data);
            ASSERT_OK;
            comparision.append(std::to_wstring(i));
        }
        else
        {
            auto fi = (double) i;
            Primitive *data = Float::create(&fi);
            empty_collection.insert(i, data);
            ASSERT_OK;
            comparision.append(std::to_wstring(fi));
        }

        if (i < (ARRAY_SIZE - 1))
        {
            comparision.append(L" ");
        }
    }

    for (uint32_t i = 0; i < ARRAY_SIZE; i++)
    {
        if (i % 2 == 0)
        {
            auto *data = (Primitive *) empty_collection[i];

            ASSERT_TRUE(data->getObjectType() == OBJECT_TYPE_STRING,
                        "data should be OBJECT_TYPE_STRING");

            ASSERT_TRUE(wcscmp((const wchar_t *) data->getAddress(),
                               random_name(i)) == 0,
                        "data should be %ls (%ls)",
                        random_name(i),
                        (const char *) data->getAddress());
            ASSERT_OK;
        }
        else if (i % 3 == 0)
        {
            auto *data = (Primitive *) empty_collection[i];
            ASSERT_TRUE(data->getObjectType() == OBJECT_TYPE_INT,
                        "data should be OBJECT_TYPE_INT");

            ASSERT_TRUE(*(uint32_t *) data->getAddress() == i,
                        "data should be %u (%u)",
                        i,
                        *(uint32_t *) data->getAddress());

            ASSERT_OK;
        }
        else
        {
            auto fi = (double) i;
            auto *data = (Primitive *) empty_collection[i];

            ASSERT_TRUE(data->getObjectType() == OBJECT_TYPE_FLOAT,
                        "data should be OBJECT_TYPE_FLOAT");

            ASSERT_TRUE(*(double *) data->getAddress() == fi,
                        "data should be %f (%f)",
                        fi,
                        *(double *) data->getAddress());

            ASSERT_OK;
        }
    }

    Primitive &str2 = empty_collection.toString();

    ASSERT_TRUE(comparision == (const wchar_t *) str2.getAddress(),
                "they should be the same: expected %ls, got %ls",
                comparision.c_str(),
                (const wchar_t *) str2.getAddress());

    Collection &c = *Collection::create();

    for (uint32_t i = 0; i < ARRAY_SIZE; i++)
    {
        if (i % 2 == 0)
        {
            Value *data = String::create(random_name(i));
            c.insert(i, data);
            ASSERT_OK;
        }
        else if (i % 3 == 0)
        {
            Value *data = Int::create(&i);
            c.insert(i, data);
            ASSERT_OK;
        }
        else
        {
            c.insert(i, (Value *) &empty_collection);
            ASSERT_OK;
        }
    }

    for (uint32_t i = 0; i < ARRAY_SIZE; i++)
    {
        if (i % 2 == 0)
        {
            auto *data = (Primitive *) c[i];
            ASSERT_OK;
            ASSERT_TRUE(data->getObjectType() == OBJECT_TYPE_STRING, "data should be OBJECT_TYPE");
            ASSERT_TRUE(wcscmp((const wchar_t *) data->getAddress(),
                               random_name(i)) == 0,
                        "data should be %ls (%ls)",
                        random_name(i),
                        (const wchar_t *) data->getAddress());
        }
        else if (i % 3 == 0)
        {
            auto *data = (Primitive *) c[i];
            ASSERT_OK;
            ASSERT_TRUE(data->getObjectType() == OBJECT_TYPE_INT, "data should be OBJECT_TYPE_INT");
            ASSERT_TRUE(*(uint32_t *) data->getAddress() == i,
                        "data should be %u (%u)",
                        i,
                        *(uint32_t *) data->getAddress());
        }
        else
        {
            auto *collection_inside = (Collection *) c[i];
            ASSERT_OK;
            ASSERT_NOT_NULL(collection_inside);
            ASSERT_EQUALS(collection_inside, &empty_collection);
            ASSERT_TRUE(collection_inside == &empty_collection, "data should be the same");
        }
    }

    ORM::destroy(&c);
}

/**
 * Test collection operators.
 */
static void
collection_test_operators()
{
    Collection &c = *Collection::create();

    c.parseStream(L"\"ante\" 19 28.0 +45 -45 -45.3 \'a\' true");
    ASSERT_OK;
    ASSERT_EQUALS(c.size(), 8);

    Value *v;

    v = c[0];
    ASSERT_EQUALS(v->getObjectType(), OBJECT_TYPE_STRING);
    ASSERT_EQUALS(v->getString(), L"ante");

    v = c[1];
    ASSERT_EQUALS(v->getObjectType(), OBJECT_TYPE_INT);
    ASSERT_EQUALS(v->toInt(), 19);

    v = c[2];
    ASSERT_EQUALS(v->getObjectType(), OBJECT_TYPE_FLOAT);
    ASSERT_EQUALS(v->toFloat(), 28.0);

    v = c[3];
    ASSERT_EQUALS(v->getObjectType(), OBJECT_TYPE_INT);
    ASSERT_EQUALS(v->toInt(), 45);

    v = c[4];
    ASSERT_EQUALS(v->getObjectType(), OBJECT_TYPE_INT);
    ASSERT_EQUALS(v->toInt(), -45);

    v = c[5];
    ASSERT_EQUALS(v->getObjectType(), OBJECT_TYPE_FLOAT);
    ASSERT_TRUE((v->toFloat() == -45.3) || (v->toFloat() < -45.29) || (v->toFloat() > -45.31), "should be around 45.3");

    v = c[6];
    ASSERT_EQUALS(v->getObjectType(), OBJECT_TYPE_CHAR);
    ASSERT_EQUALS(v->toChar(), L'a');

    v = c[7];
    ASSERT_EQUALS(v->getObjectType(), OBJECT_TYPE_BOOL);
    ASSERT_TRUE(v->toBool(), "should be true!");
}



/**
 * Test collection.
 */
void
collection_test()
{
    vm = (VirtualMemory *) ORM::getFirst(OBJECT_TYPE_VIRTUAL_MEMORY);
    RUN_TEST_VM(collection_test_basic());
    RUN_TEST_VM(collection_test_operators());
}
