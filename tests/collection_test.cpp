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

#include "collection_test.h"
#include "collection.h"
#include "test_assert.h"
#include "virtual_memory.h"
#include "error_log.h"
#include "primitive_data.h"
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
    collection &empty_collection = *collection::create("empty_collection");
    ASSERT_VIRTUAL_MEMORY(*vm, 0);

    for (uint16_t i = 0; i < USHRT_MAX; i++)
    {
        ASSERT_TRUE(empty_collection[i] == nullptr,
                    "Returned reference should be NULL! (0x%X)",
                    empty_collection[i]);
    }

    primitive_data &str = empty_collection.to_string();
    ASSERT_TRUE(wcscmp((const wchar_t *) str.get_address(), L"") == 0,
                "Returned string should be empty! (%ls)",
                (const wchar_t *) str.get_address());
    ASSERT_VIRTUAL_MEMORY(*vm, DATA_TYPE_SIZE[DATA_TYPE_STRING]);

    std::wstring comparision;

    for (uint32_t i = 0; i < ARRAY_SIZE; i++)
    {
        if (i % 2 == 0)
        {
            primitive_data &data = *primitive_data::create("temp_name", DATA_TYPE_STRING, random_name(i));
            empty_collection.insert(i, (object *) &data);
            ASSERT_OK;
            comparision.append(random_name(i));
        }
        else if (i % 3 == 0)
        {
            primitive_data &data = *primitive_data::create("temp_name", DATA_TYPE_INT, (const void *) &i);
            empty_collection.insert(i, (object *) &data);
            ASSERT_OK;
            comparision.append(std::to_wstring(i));
        }
        else
        {
            double fi = (double) i;
            primitive_data &data = *primitive_data::create("temp_name", DATA_TYPE_FLOAT, (const void *) &fi);
            empty_collection.insert(i, (object *) &data);
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

            ASSERT_TRUE(data->get_type() == DATA_TYPE_STRING,
                        "data should be DATA_TYPE_STRING");

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
            ASSERT_TRUE(data->get_type() == DATA_TYPE_INT,
                        "data should be DATA_TYPE_INT");

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

            ASSERT_TRUE(data->get_type() == DATA_TYPE_FLOAT,
                        "data should be DATA_TYPE_FLOAT");

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

    collection &c = *collection::create("array");

    for (uint32_t i = 0; i < ARRAY_SIZE; i++)
    {
        if (i % 2 == 0)
        {
            primitive_data &data = *primitive_data::create("temp_name", DATA_TYPE_STRING, random_name(i));
            c.insert(i, (object *) &data);
            ASSERT_OK;
        }
        else if (i % 3 == 0)
        {
            primitive_data &data = *primitive_data::create("temp_name", DATA_TYPE_INT, (const void *) &i);
            c.insert(i, (object *) &data);
            ASSERT_OK;
        }
        else
        {
            c.insert(i, (object *) &empty_collection);
            ASSERT_OK;
        }
    }

    for (uint32_t i = 0; i < ARRAY_SIZE; i++)
    {
        if (i % 2 == 0)
        {
            auto *data = (primitive_data *) c[i];
            ASSERT_OK;
            ASSERT_TRUE(data->get_type() == DATA_TYPE_STRING, "data should be DATA_TYPE_STRING");
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
            ASSERT_TRUE(data->get_type() == DATA_TYPE_INT, "data should be DATA_TYPE_INT");
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

    printf("\t-> %s()::OK \n", __FUNCTION__);
}

/**
 * Test collection.
 */
void
collection_test()
{
    printf("%s()\r\n", __FUNCTION__);

    vm = (virtual_memory *) orm::get_first("virtual_memory");
    RUN_TEST(collection_test_basic());

    printf("\r\n\r\n");
}
