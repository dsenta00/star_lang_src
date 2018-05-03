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

#pragma once

#include <VariableBundle/Value.h>
#include <VariableBundle/Primitive/DataType.h>
#include "ForwardDeclarations.h"
#include <string>

/**
 * Represents data collection.
 */
class Collection : public Value {
public:
    explicit Collection(Collection *c = nullptr);
    static Collection *create(Collection *c = nullptr);

    eObjectType getObjectType() override;
    bool isReference() override;
    uint32_t size();

    Value *operator[](uint32_t index);
    Value *operator[](std::string index);
    void insert(std::string index, Value *o);
    void insert(uint32_t index, Value *o);
    bool operator+=(Value &data) override;
    bool operator-=(Value &data) override;
    void clear();

    bool operator&=(Value &data) override;
    bool operator|=(Value &data) override;
    bool operator^=(Value &data) override;
    String &toString() override;
    bool toBool() override;
    wchar_t toChar() override;
    int32_t toInt() override;
    double toFloat() override;

    bool operator=(const void *data) override;
    bool operator=(Value &data) override;
    bool operator*=(Value &data) override;
    bool operator/=(Value &data) override;
    bool operator%=(Value &data) override;
    bool operator++() override;
    bool operator--() override;
    
    bool operator==(Value &data) override;
    bool operator!=(Value &data) override;
    bool operator>(Value &data) override;
    bool operator<(Value &data) override;
    bool operator>=(Value &data) override;
    bool operator<=(Value &data) override;
    
    bool print() override;
    bool println() override;
    bool scan() override;

    std::wstring getString() override;

    ~Collection();

    /*
     * Don't call this Method on purpose!
     *
     * scan() use this function.
     * Almost all scan() logic is contained in this function except
     * keyboard insertion part.
     */
    void parseStream(std::wstring input);
protected:
    void removeData(std::string index);
    void removeData(Value *o);
    void insertData(std::string index, Value *o);
    std::map<std::string, Value *> data_cache;
};