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

#ifndef STAR_PROGRAMMING_LANGUAGE_FILE_H
#define STAR_PROGRAMMING_LANGUAGE_FILE_H

#include "ORM/entity.h"
#include "fw_decl.h"
#include "file_mode.h"

/**
 * The file entity. Handles file streams.
 */
class file : public entity {
public:
    explicit file(const char *id);
    file(const char *id, file_mode mode, const char *file_name);
    void open(file_mode mode, const char *file_name);
    void close();
    bool is_opened();
    primitive_data *read_all();
    int64_t get_size();
    void write(entity *e);
    static file *create(const char *id, file_mode mode, const char *file_name);
    static file *create(const char *id);
    ~file() override;
protected:
    void read_into_buffer();
    bool is_already_read;
    FILE *file_pointer;
    std::string buffer;
    file_mode mode;
};


#endif //STAR_PROGRAMMING_LANGUAGE_FILE_H
