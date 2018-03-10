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

#ifndef COLLECTION_H
#define COLLECTION_H

#include <variable/value.h>
#include <variable/primitive_data/data_type.h>
#include "fw_decl.h"
#include <string>

/**
 * Represents data collection.
 */
class collection : public value {
public:
    explicit collection(collection *c = nullptr);
    static collection *create(collection *c = nullptr);

    object_type get_object_type() override;
    bool is_reference() override;
    uint32_t size();

    value *operator[](uint32_t index);
    value *operator[](std::string index);
    void insert(std::string index, value *o);
    void insert(uint32_t index, value *o);
    bool operator+=(value &data) override;
    bool operator-=(value &data) override;
    void clear();

    bool operator&=(value &data) override;
    bool operator|=(value &data) override;
    bool operator^=(value &data) override;
    string_data &to_string() override;
    bool to_bool() override;
    wchar_t to_char() override;
    int32_t to_int() override;
    double to_float() override;

    bool operator=(const void *data) override;
    bool operator=(value &data) override;
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

    ~collection() override;

    /*
     * Don't call this method on purpose!
     *
     * scan() use this function.
     * Almost all scan() logic is contained in this function except
     * keyboard insertion part.
     */
    void parse_stream(std::wstring input);
protected:
    void remove_data(std::string index);
    void remove_data(value *o);
    void insert_data(std::string index, value *o);
    std::map<std::string, value *> data_cache;
};

#endif // COLLECTION_H
