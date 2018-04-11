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
 * @param mode
 * @param file_name
 */
file::file(file_mode mode, const char *file_name) : value::value()
{
    this->mode = FILE_MODE_NOT_OPEN;
    this->is_already_read = false;

    this->open(mode, file_name);
}

/**
 * The constructor.
 */
file::file() : value::value()
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
string_data *
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

    string_data *data = nullptr;

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
file::write(value *o)
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

    this->buffer.append(o->get_string());
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
 * @param mode
 * @param file_name
 * @return
 */
file *
file::create(file_mode mode, const char *file_name)
{
    return (file *) orm::create(new file(mode, file_name));
}

/**
 * Create file object.
 *
 * @return
 */
file *
file::create()
{
    return (file *) orm::create(new file());
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

/**
 * @inherit
 */
bool
file::to_bool()
{
    return false;
}

/**
 * @inherit
 */
wchar_t
file::to_char()
{
    return 0;
}

/**
 * @inherit
 */
int32_t
file::to_int()
{
    return 0;
}

/**
 * @inherit
 */
double
file::to_float()
{
    return 0;
}

/**
 * @inherit
 */
string_data &
file::to_string()
{
    return *string_data::create(this->buffer.c_str());
}

/**
 * @inherit
 */
bool
file::is_reference()
{
    return true;
}

/**
 * @inherit
 */
bool
file::default_value()
{
    ERROR_LOG_ADD(ERROR_FILE_INVALID_OPERATION);

    return false;
}

/**
 * @inherit
 */
bool
file::operator=(const void *data)
{
    ERROR_LOG_ADD(ERROR_FILE_INVALID_OPERATION);

    return false;
}

/**
 * @inherit
 */
bool
file::operator=(value &data)
{
    if (file_mode_can_write(this->mode))
    {
        ERROR_LOG_ADD(ERROR_FILE_WRITE_ON_READ_MODE);
        return false;
    }

    this->buffer.clear();
    this->write(&data);

    return true;
}

/**
 * @inherit
 */
bool
file::operator&=(value &data)
{
    ERROR_LOG_ADD(ERROR_FILE_INVALID_OPERATION);

    return false;
}

/**
 * @inherit
 */
bool
file::operator|=(value &data)
{
    ERROR_LOG_ADD(ERROR_FILE_INVALID_OPERATION);

    return false;
}

/**
 * @inherit
 */
bool
file::operator^=(value &data)
{
    ERROR_LOG_ADD(ERROR_FILE_INVALID_OPERATION);

    return false;
}

/**
 * @inherit
 */
bool
file::operator+=(value &data)
{
    this->write(&data);

    return true;
}

/**
 * @inherit
 */
bool
file::operator-=(value &data)
{
    ERROR_LOG_ADD(ERROR_FILE_INVALID_OPERATION);

    return false;
}

/**
 * @inherit
 */
bool
file::operator*=(value &data)
{
    ERROR_LOG_ADD(ERROR_FILE_INVALID_OPERATION);

    return false;
}

/**
 * @inherit
 */
bool
file::operator/=(value &data)
{
    ERROR_LOG_ADD(ERROR_FILE_INVALID_OPERATION);

    return false;
}

/**
 * @inherit
 */
bool
file::operator%=(value &data)
{
    ERROR_LOG_ADD(ERROR_FILE_INVALID_OPERATION);

    return false;
}

/**
 * @inherit
 */
bool
file::operator++()
{
    ERROR_LOG_ADD(ERROR_FILE_INVALID_OPERATION);

    return false;
}

/**
 * @inherit
 */
bool
file::operator--()
{
    ERROR_LOG_ADD(ERROR_FILE_INVALID_OPERATION);

    return false;
}

/**
 * @inherit
 */
bool
file::operator==(value &data)
{
    if (file_mode_can_read(this->mode))
    {
        if (!this->is_already_read)
        {
            this->read_into_buffer();
        }

        return this->buffer == data.get_string();
    }

    return false;
}

/**
 * @inherit
 */
bool
file::operator!=(value &data)
{
    return !this->operator==(data);
}

/**
 * @inherit
 */
bool
file::operator>(value &data)
{
    ERROR_LOG_ADD(ERROR_FILE_INVALID_OPERATION);

    return false;
}

/**
 * @inherit
 */
bool
file::operator<(value &data)
{
    ERROR_LOG_ADD(ERROR_FILE_INVALID_OPERATION);

    return false;
}

/**
 * @inherit
 */
bool
file::operator>=(value &data)
{
    ERROR_LOG_ADD(ERROR_FILE_INVALID_OPERATION);

    return false;
}

/**
 * @inherit
 */
bool
file::operator<=(value &data)
{
    ERROR_LOG_ADD(ERROR_FILE_INVALID_OPERATION);

    return false;
}

/**
 * @inherit
 */
bool
file::print()
{
    if (!file_mode_can_read(this->mode))
    {
        ERROR_LOG_ADD(ERROR_FILE_READ_ON_WRITE_MODE);

        return false;
    }

    if (!this->is_already_read)
    {
        this->read_into_buffer();
    }

    std::wcout << this->buffer;

    return true;
}

/**
 * @inherit
 */
bool
file::println()
{
    if (this->print())
    {
        std::cout << std::endl;

        return true;
    }

    return false;
}

/**
 * @inherit
 */
bool
file::scan()
{
    ERROR_LOG_ADD(ERROR_FILE_INVALID_OPERATION);

    return false;
}

/**
 * @inherit
 */
std::wstring
file::get_string()
{
    return this->buffer;
}

/**
 * Get file mode.
 *
 * @return file mode.
 */
file_mode
file::get_mode()
{
    return this->mode;
}