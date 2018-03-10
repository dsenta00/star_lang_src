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

#include <variable/primitive_data/string_data.h>
#include <variable/primitive_data/int_data.h>
#include <variable/primitive_data/float_data.h>
#include "../../../include/variable/collection/collection_test.h"
#include "variable/collection/collection.h"
#include "../../../test_assert.h"
#include "error_handler/error_log.h"
#include "ORM/orm.h"

static virtual_memory *vm;

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
    collection &empty_collection = *collection::create();
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

    primitive_data &str = empty_collection.to_string();
    ASSERT_TRUE(wcscmp((const wchar_t *) str.get_address(), L"") == 0,
                "Returned string should be empty! (%ls)",
                (const wchar_t *) str.get_address());
    ASSERT_VIRTUAL_MEMORY(*vm, DATA_TYPE_SIZE[OBJECT_TYPE_STRING]);

    std::wstring comparision;

    for (uint32_t i = 0; i < ARRAY_SIZE; i++)
    {
        if (i % 2 == 0)
        {
            primitive_data *data = string_data::create(random_name(i));
            empty_collection.insert(i, data);
            ASSERT_OK;
            comparision.append(random_name(i));
        }
        else if (i % 3 == 0)
        {
            primitive_data *data = int_data::create(&i);
            empty_collection.insert(i, data);
            ASSERT_OK;
            comparision.append(std::to_wstring(i));
        }
        else
        {
            auto fi = (double) i;
            primitive_data *data = float_data::create(&fi);
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
            auto *data = (primitive_data *) empty_collection[i];

            ASSERT_TRUE(data->get_object_type() == OBJECT_TYPE_STRING,
                        "data should be OBJECT_TYPE_STRING");

            ASSERT_TRUE(wcscmp((const wchar_t *) data->get_address(),
                               random_name(i)) == 0,
                        "data should be %ls (%ls)",
                        random_name(i),
                        (const char *) data->get_address());
            ASSERT_OK;
        }
        else if (i % 3 == 0)
        {
            auto *data = (primitive_data *) empty_collection[i];
            ASSERT_TRUE(data->get_object_type() == OBJECT_TYPE_INT,
                        "data should be OBJECT_TYPE_INT");

            ASSERT_TRUE(*(uint32_t *) data->get_address() == i,
                        "data should be %u (%u)",
                        i,
                        *(uint32_t *) data->get_address());

            ASSERT_OK;
        }
        else
        {
            auto fi = (double) i;
            auto *data = (primitive_data *) empty_collection[i];

            ASSERT_TRUE(data->get_object_type() == OBJECT_TYPE_FLOAT,
                        "data should be OBJECT_TYPE_FLOAT");

            ASSERT_TRUE(*(double *) data->get_address() == fi,
                        "data should be %f (%f)",
                        fi,
                        *(double *) data->get_address());

            ASSERT_OK;
        }
    }

    primitive_data &str2 = empty_collection.to_string();

    ASSERT_TRUE(comparision == (const wchar_t *) str2.get_address(),
                "they should be the same: expected %ls, got %ls",
                comparision.c_str(),
                (const wchar_t *) str2.get_address());

    collection &c = *collection::create();

    for (uint32_t i = 0; i < ARRAY_SIZE; i++)
    {
        if (i % 2 == 0)
        {
            value *data = string_data::create(random_name(i));
            c.insert(i, data);
            ASSERT_OK;
        }
        else if (i % 3 == 0)
        {
            value *data = int_data::create(&i);
            c.insert(i, data);
            ASSERT_OK;
        }
        else
        {
            c.insert(i, (value *) &empty_collection);
            ASSERT_OK;
        }
    }

    for (uint32_t i = 0; i < ARRAY_SIZE; i++)
    {
        if (i % 2 == 0)
        {
            auto *data = (primitive_data *) c[i];
            ASSERT_OK;
            ASSERT_TRUE(data->get_object_type() == OBJECT_TYPE_STRING, "data should be OBJECT_TYPE");
            ASSERT_TRUE(wcscmp((const wchar_t *) data->get_address(),
                               random_name(i)) == 0,
                        "data should be %ls (%ls)",
                        random_name(i),
                        (const wchar_t *) data->get_address());
        }
        else if (i % 3 == 0)
        {
            auto *data = (primitive_data *) c[i];
            ASSERT_OK;
            ASSERT_TRUE(data->get_object_type() == OBJECT_TYPE_INT, "data should be OBJECT_TYPE_INT");
            ASSERT_TRUE(*(uint32_t *) data->get_address() == i,
                        "data should be %u (%u)",
                        i,
                        *(uint32_t *) data->get_address());
        }
        else
        {
            auto *collection_inside = (collection *) c[i];
            ASSERT_OK;
            ASSERT_NOT_NULL(collection_inside);
            ASSERT_EQUALS(collection_inside, &empty_collection);
            ASSERT_TRUE(collection_inside == &empty_collection, "data should be the same");
        }
    }

    orm::destroy(&c);
}

/**
 * Test collection operators.
 */
static void
collection_test_operators()
{
    collection &c = *collection::create();

    c.parse_stream(L"\"ante\" 19 28.0 +45 -45 -45.3 \'a\' true");
    ASSERT_OK;
    ASSERT_EQUALS(c.size(), 8);

    value *v;

    v = c[0];
    ASSERT_EQUALS(v->get_object_type(), OBJECT_TYPE_STRING);
    ASSERT_EQUALS(v->get_string(), L"ante");

    v = c[1];
    ASSERT_EQUALS(v->get_object_type(), OBJECT_TYPE_INT);
    ASSERT_EQUALS(v->to_int(), 19);

    v = c[2];
    ASSERT_EQUALS(v->get_object_type(), OBJECT_TYPE_FLOAT);
    ASSERT_EQUALS(v->to_float(), 28.0);

    v = c[3];
    ASSERT_EQUALS(v->get_object_type(), OBJECT_TYPE_INT);
    ASSERT_EQUALS(v->to_int(), 45);

    v = c[4];
    ASSERT_EQUALS(v->get_object_type(), OBJECT_TYPE_INT);
    ASSERT_EQUALS(v->to_int(), -45);

    v = c[5];
    ASSERT_EQUALS(v->get_object_type(), OBJECT_TYPE_FLOAT);
    ASSERT_TRUE((v->to_float() == -45.3) || (v->to_float() < -45.29) || (v->to_float() > -45.31), "should be around 45.3");

    v = c[6];
    ASSERT_EQUALS(v->get_object_type(), OBJECT_TYPE_CHAR);
    ASSERT_EQUALS(v->to_char(), L'a');

    v = c[7];
    ASSERT_EQUALS(v->get_object_type(), OBJECT_TYPE_BOOL);
    ASSERT_TRUE(v->to_bool(), "should be true!");
}



/**
 * Test collection.
 */
void
collection_test()
{
    vm = (virtual_memory *) orm::get_first(OBJECT_TYPE_VIRTUAL_MEMORY);
    RUN_TEST_VM(collection_test_basic());
    RUN_TEST_VM(collection_test_operators());
}
