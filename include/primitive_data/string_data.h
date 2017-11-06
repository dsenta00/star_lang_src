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

#ifndef STAR_PROGRAMMING_LANGUAGE_STRING_DATA_H
#define STAR_PROGRAMMING_LANGUAGE_STRING_DATA_H

#include <string>
#include "primitive_data.h"

/**
 * String data type.
 */
class string_data : public primitive_data {
public:
    explicit string_data(std::string id, const void *value = nullptr);
    string_data(std::string id, string_data &data);
    static string_data *create(std::string id, const void *value = nullptr);
    static string_data *create(std::string id, string_data &data);

    bool to_bool();
    wchar_t to_char();
    int32_t to_int();
    double to_float();
    primitive_data &to_string();

    bool default_value();
    bool operator=(const void *data);
    bool operator=(primitive_data &data);
    bool operator&=(primitive_data &data);
    bool operator|=(primitive_data &data);
    bool operator^=(primitive_data &data);
    bool operator+=(primitive_data &data);
    bool operator-=(primitive_data &data);
    bool operator*=(primitive_data &data);
    bool operator/=(primitive_data &data);
    bool operator%=(primitive_data &data);
    bool operator++();
    bool operator--();
    bool operator==(primitive_data &data);
    bool operator!=(primitive_data &data);
    bool operator>(primitive_data &data);
    bool operator<(primitive_data &data);
    bool operator>=(primitive_data &data);
    bool operator<=(primitive_data &data);

    bool print();
    bool println();
    bool scan();

    std::wstring get_string();
};


#endif //STAR_PROGRAMMING_LANGUAGE_STRING_DATA_H
