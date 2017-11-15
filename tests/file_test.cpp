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
#include <cstdio>
#include <collection.h>
#include <codecvt>
#include <locale>
#include <primitive_data/string_data.h>
#include <primitive_data/float_data.h>
#include <primitive_data/int_data.h>
#include "error_log.h"
#include "ORM/orm.h"
#include "file_test.h"
#include "file.h"
#include "test_assert.h"

#define TEST_TEXT (L"1234 miljenko _+_")

std::string convert_utf8_to_ascii(std::wstring ws)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    return converter.to_bytes(ws);
}

/**
 * create_file
 * @param file_name
 * @param text
 */
static void
create_file(const char *file_name, const wchar_t *text)
{
    FILE *fp = nullptr;

    fp = fopen(file_name, "w+");

    if (!fp)
    {
        perror("Problem!... skipping test");
        return;
    }

    if (wcslen(text) > 0)
    {
        std::string str = convert_utf8_to_ascii(text);
        fwrite(str.c_str(),
               sizeof(int8_t),
               str.size(),
               fp);
    }

    fclose(fp);
}

/**
 * compare_file_content
 * @param file_name
 * @param text
 * @return
 */
static bool
compare_file_content(const char *file_name, const char *text)
{
    FILE *fp = nullptr;

    fp = fopen(file_name, "r");

    if (!fp)
    {
        perror("Problem!");
        return false;
    }

    uint64_t size = strlen(text);
    auto *str = (char *) calloc(size + 1, sizeof(int8_t));

    if (!str)
    {
        perror("Problem!");
        return false;
    }

    fread(str, sizeof(int8_t), size + 1, fp);
    fclose(fp);

    bool result = (strcmp(str, text) == 0);
    free(str);
    return result;
}

/**
 * file_test_read
 */
static void
file_test_read()
{
    /*
     * Negative test case
     */
    file *f = file::create("fr", FILE_MODE_READ, "miljenkoooooo");

    ASSERT_ERROR(ERROR_FILE_UNKNOWN_FILE);
    ASSERT_NOT_NULL(f);
    ASSERT_FALSE(f->is_opened(), "file should not be opened!");
    ERROR_LOG_CLEAR;
    ORM_DESTROY(f);

    /*
     * Positive test case
     */
    const char *file_name = "test_star.txt";
    create_file("test_star.txt", TEST_TEXT);
    f = file::create("fr", FILE_MODE_READ, file_name);

    ASSERT_OK;
    ASSERT_NOT_NULL(f);
    ASSERT_TRUE(f->is_opened(), "file should be opened!");

    auto *str = f->read_all();
    ASSERT_NOT_NULL(str);
    ASSERT_EQUALS(str->get_id(), "fr:content");
    ASSERT_TRUE(wcscmp((const wchar_t *) str->get_address(), TEST_TEXT) == 0,
                "Should be equal! \"%ls\"",
                (const wchar_t *) str->get_address());
    ORM_DESTROY(str);
    /* try again */
    str = f->read_all();
    ASSERT_NOT_NULL(str);
    ASSERT_EQUALS(str->get_id(), "fr:content");
    ASSERT_TRUE(wcscmp((const wchar_t *) str->get_address(), TEST_TEXT) == 0,
                "Should be equal! \"%ls\"",
                (const wchar_t *) str->get_address());
    ORM_DESTROY(str);

    f->close();

    ASSERT_OK;
    ASSERT_NOT_NULL(f);
    ASSERT_FALSE(f->is_opened(), "file should not be opened!");

    ORM_DESTROY(f);
    remove(file_name);
}

/**
 * file_test_write1
 */
static void
file_test_write1()
{
    const char *file_name = "test_star_write.txt";
    file *f = file::create("fw1", FILE_MODE_WRITE, file_name);

    ASSERT_OK;
    ASSERT_NOT_NULL(f);
    ASSERT_TRUE(f->is_opened(), "file should be opened!");

    primitive_data *str = string_data::create("str", L"Miljenko 123");
    f->write(str);
    ASSERT_OK;

    f->close();
    ASSERT_OK;
    ASSERT_TRUE(compare_file_content(file_name, "Miljenko 123"), "Should be equal!");

    ORM_DESTROY(f);
    ORM_DESTROY(str);
    remove(file_name);
}

/**
 * file_test_write2
 */
static void
file_test_write2()
{
    const char *file_name = "test_star_write.txt";
    file *f = file::create("fw2", FILE_MODE_WRITE, file_name);

    ASSERT_OK;
    ASSERT_NOT_NULL(f);
    ASSERT_TRUE(f->is_opened(), "file should be opened!");

    /* fill collection */
    collection *c = collection::create("c");
    int var1 = 42;
    *c += int_data::create("var1", &var1);
    double var2 = 41.0f;
    *c += float_data::create("var2", &var2);
    const wchar_t *var3 = L"40";
    *c += string_data::create("var3", var3);
    /* fill collection end */
    f->write(c);
    ASSERT_OK;

    ASSERT_OK;
    ORM_DESTROY(f);
    ORM_DESTROY(c);
    ASSERT_OK;
    ASSERT_TRUE(compare_file_content(file_name, "42 41.000000 40"), "Should be equal!");

    remove(file_name);
}

/**
 * file_test_read_empty_file
 */
static void
file_test_read_empty_file()
{
    const char *file_name = "test_star.txt";
    create_file("test_star.txt", L"");
    file *f = file::create("fr", FILE_MODE_READ, file_name);

    ASSERT_OK;
    ASSERT_NOT_NULL(f);
    ASSERT_TRUE(f->is_opened(), "file should be opened!");

    auto *str = f->read_all();
    ASSERT_NOT_NULL(str);
    ASSERT_EQUALS(str->get_id(), "fr:content");

    ASSERT_TRUE(strcmp((const char *) str->get_address(), "") == 0,
                "Should be equal! %s",
                (const char *) str->get_address());
    ORM_DESTROY(str);
    /* try again */
    str = f->read_all();
    ASSERT_NOT_NULL(str);
    ASSERT_EQUALS(str->get_id(), "fr:content");
    ASSERT_TRUE(strcmp((const char *) str->get_address(), "") == 0,
                "Should be equal! %s",
                (const char *) str->get_address());
    ORM_DESTROY(str);

    f->close();

    ASSERT_OK;
    ASSERT_NOT_NULL(f);
    ASSERT_FALSE(f->is_opened(), "file should not be opened!");

    ORM_DESTROY(f);

    remove(file_name);
}

/**
 * file_test_append
 */
static void
file_test_append()
{
    const char *file_name = "test_star_write.txt";
    file *f = file::create("fw1", FILE_MODE_APPEND, file_name);

    ASSERT_OK;
    ASSERT_NOT_NULL(f);
    ASSERT_TRUE(f->is_opened(), "file should be opened!");

    auto *str = string_data::create("str", L"Miljenko 123");
    f->write(str);
    ASSERT_OK;

    f->close();
    ASSERT_OK;
    ASSERT_TRUE(compare_file_content(file_name, "Miljenko 123"), "Should be equal!");

    ORM_DESTROY(f);
    ORM_DESTROY(str);

    f = file::create("fw1", FILE_MODE_APPEND, file_name);

    ASSERT_OK;
    ASSERT_NOT_NULL(f);
    ASSERT_TRUE(f->is_opened(), "file should be opened!");

    str = string_data::create("str", L"74, 345");
    f->write(str);
    ASSERT_OK;

    f->close();
    ASSERT_OK;
    ASSERT_TRUE(compare_file_content(file_name, "Miljenko 12374, 345"), "Should be equal!");
    remove(file_name);
}

/**
 * Test reopen.
 */
static void
file_test_reopen()
{
    const char *file_name = "test_star_write.txt";
    file *f = file::create("f");
    ASSERT_OK;
    ASSERT_FALSE(f->is_opened(), "file should not be open!");

    f->open(FILE_MODE_WRITE, file_name);
    ASSERT_OK;
    ASSERT_TRUE(f->is_opened(), "file should be open!");

    auto *str = string_data::create("str", L"Miljenko 123");
    f->write(str);
    ASSERT_OK;
    ORM_DESTROY(str);

    f->open(FILE_MODE_APPEND, file_name);
    str = string_data::create("str", L"74, 345");
    f->write(str);
    ASSERT_OK;
    ORM_DESTROY(str);

    f->open(FILE_MODE_READ, file_name);
    str = dynamic_cast<string_data *>(f->read_all());
    ASSERT_TRUE(wcscmp((const wchar_t *) str->get_address(),
                       L"Miljenko 12374, 345") == 0, "should be equal");

    f->close();
    ASSERT_OK;
    ASSERT_TRUE(compare_file_content(file_name, "Miljenko 12374, 345"), "Should be equal!");
    remove(file_name);
}

/**
 * test file.
 */
void
file_test()
{
    RUN_TEST(file_test_read());
    RUN_TEST(file_test_read_empty_file());
    RUN_TEST(file_test_write1());
    RUN_TEST(file_test_write2());
    RUN_TEST(file_test_append());
    RUN_TEST(file_test_reopen());
}
