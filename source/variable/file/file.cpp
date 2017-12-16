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

#include <error_handler/error_log.h>
#include <variable/file/file.h>
#include <fstream>
#include <codecvt>
#include <sstream>
#include <iostream>
#include <variable/primitive_data/string_data.h>
#include "variable/collection/collection.h"
#include "ORM/orm.h"

/**
 * The constructor.
 *
 * @param id
 * @param mode
 * @param file_name
 */
file::file(const char *id, file_mode mode, const char *file_name) : object::object(id)
{
    this->mode = FILE_MODE_NOT_OPEN;
    this->is_already_read = false;

    this->open(mode, file_name);
}

/**
 * The constructor.
 *
 * @param id
 */
file::file(const char *id) : object::object(id)
{
    this->mode = FILE_MODE_NOT_OPEN;
    this->is_already_read = false;
}

/**
 * Check if file is opened.
 *
 * @return
 */
bool
file::is_opened()
{
    return file_mode_is_valid(this->mode);
}

/**
 * Read all content from file.
 *
 * @return
 */
primitive_data *
file::read_all()
{
    if (!this->is_opened())
    {
        return nullptr;
    }

    if (!file_mode_can_read(this->mode))
    {
        ERROR_LOG_ADD(ERROR_FILE_READ_ON_WRITE_MODE);
        return nullptr;
    }

    this->read_into_buffer();

    primitive_data *data = nullptr;

    if (this->buffer.empty())
    {
        data = string_data::create();
    }
    else
    {
        data = string_data::create(this->buffer.c_str());
    }

    return data;
}

/**
 * Write data to file.
 * @param o
 */
void
file::write(object *o)
{
    if (!this->is_opened())
    {
        return;
    }

    if (!file_mode_can_write(this->mode))
    {
        ERROR_LOG_ADD(ERROR_FILE_WRITE_ON_READ_MODE);
        return;
    }

    if (primitive_data::is_primitive((value *)o))
    {
        this->buffer.append(((primitive_data *) o)->get_string());
    }
    else if (o->get_object_type() == OBJECT_TYPE_COLLECTION)
    {
        this->buffer.append(((collection *) o)->to_string().get_string());
    }
}

/**
 * Close file stream.
 */
void
file::close()
{
    if (!this->is_opened())
    {
        return;
    }

    if (file_mode_can_write(this->mode))
    {
        this->write_from_buffer();
    }

    this->file_name.clear();
    this->mode = FILE_MODE_NOT_OPEN;
}

/**
 * Create file object.
 *
 * @param id
 * @param mode
 * @param file_name
 * @return
 */
file *
file::create(const char *id, file_mode mode, const char *file_name)
{
    return (file *) orm::create(new file(id, mode, file_name));
}

/**
 * Create file object.
 *
 * @param id
 * @return
 */
file *
file::create(const char *id)
{
    return (file *) orm::create(new file(id));
}

/**
 * Read file content into buffer.
 */
void
file::read_into_buffer()
{
    if (this->is_already_read)
    {
        return;
    }

    std::wifstream wif(this->file_name);

    if (!wif.good())
    {
        ERROR_LOG_ADD(ERROR_FILE_UNKNOWN_FILE);
        this->mode = FILE_MODE_NOT_OPEN;
        return;
    }

    wif.imbue(std::locale(std::locale::classic(), new std::codecvt_utf8<wchar_t>));
    std::wstringstream wss;
    wss << wif.rdbuf();
    wif.close();

    this->buffer = wss.str();
    this->is_already_read = true;
}

/**
 * Open file stream.
 *
 * @param mode
 * @param file_name
 */
void
file::open(file_mode mode, const char *file_name)
{
    if (this->is_opened())
    {
        this->close();
    }

    this->file_name = file_name;

    this->mode = mode;
    if (!file_mode_is_valid(this->mode))
    {
        ERROR_LOG_ADD(ERROR_FILE_UNKNOWN_MODE);
        return;
    }

    if (this->mode == FILE_MODE_READ)
    {
        this->read_into_buffer();
    }
}

/**
 * Write from buffer.
 */
void
file::write_from_buffer()
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    std::string str;

    if (this->buffer.empty())
    {
        return;
    }

    try
    {
        str = converter.to_bytes(this->buffer);
    }
    catch (std::exception &e)
    {
        ERROR_LOG_ADD(ERROR_FILE_WRITE_FAIL);
    }

    std::ofstream of;

    if (this->mode == FILE_MODE_WRITE)
    {
        of.open(this->file_name);
    }
    else if (this->mode == FILE_MODE_APPEND)
    {
        of.open(this->file_name, std::ios_base::app);
    }

    of << str;
    of.close();

    this->buffer.clear();
}

/**
 * The destructor.
 */
file::~file()
{
    this->close();
}

/**
 * @inherit
 */
object_type
file::get_object_type()
{
    return OBJECT_TYPE_FILE;
}




