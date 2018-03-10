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

#ifndef FILE_H
#define FILE_H

#include "fw_decl.h"
#include "ORM/object.h"
#include "file_mode.h"
#include <variable/value.h>

/**
 * The file object. Handles file streams.
 */
class file : public value {
public:
    explicit file();
    file(file_mode mode, const char *file_name);
    static file *create(file_mode mode, const char *file_name);
    static file *create();

    object_type get_object_type() override;
    file_mode get_mode();
    void open(file_mode mode, const char *file_name);
    void close();
    bool is_opened();
    string_data *read_all();
    void write(value *o);

    bool to_bool() override;
    wchar_t to_char() override;
    int32_t to_int() override;
    double to_float() override;
    string_data &to_string() override;

    bool is_reference() override;
    bool default_value();
    bool operator=(const void *data) override;
    bool operator=(value &data) override;
    bool operator&=(value &data) override;
    bool operator|=(value &data) override;
    bool operator^=(value &data) override;
    bool operator+=(value &data) override;
    bool operator-=(value &data) override;
    bool operator*=(value &data) override;
    bool operator/=(value &data) override;
    bool operator%=(value &data) override;
    bool operator++() override;
    bool operator--() override;
    bool operator==(value &data) override;
    bool operator!=(value &data) override;
    bool operator>(value &data) override;
    bool operator<(value &data) override;
    bool operator>=(value &data) override;
    bool operator<=(value &data) override;

    bool print() override;
    bool println() override;
    bool scan() override;

    std::wstring get_string() override;
    ~file() override;
protected:
    void read_into_buffer();
    void write_from_buffer();

    bool is_already_read;
    std::string file_name;
    std::wstring buffer;
    file_mode mode;
};

#endif //FILE_H
