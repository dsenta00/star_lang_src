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

#ifndef CHAR_DATA_H
#define CHAR_DATA_H

#include "primitive_data.h"

/**
 * Character data type.
 */
class char_data : public primitive_data {
public:
    explicit char_data(std::string id, const void *value = nullptr);
    char_data(std::string id, char_data &data, bool is_reference = false);
    static char_data *create(std::string id, const void *value = nullptr);
    static char_data *create(std::string id, char_data &data, bool is_reference = false);

    bool to_bool();
    wchar_t to_char();
    int32_t to_int();
    double to_float();
    string_data &to_string();

    object_type get_object_type();

    bool default_value();
    bool operator=(const void *data);
    bool operator=(var &data);
    bool operator&=(var &data);
    bool operator|=(var &data);
    bool operator^=(var &data);
    bool operator+=(var &data);
    bool operator-=(var &data);
    bool operator*=(var &data);
    bool operator/=(var &data);
    bool operator%=(var &data);
    bool operator++();
    bool operator--();
    bool operator==(var &data);
    bool operator!=(var &data);
    bool operator>(var &data);
    bool operator<(var &data);
    bool operator>=(var &data);
    bool operator<=(var &data);

    bool print();
    bool println();
    bool scan();

    std::wstring get_string();
};


#endif //CHAR_DATA_H
