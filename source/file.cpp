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

#include <error_log.h>
#include <file.h>
#include "collection.h"
#include "primitive_data.h"
#include "ORM/orm.h"

/**
 * The constructor.
 *
 * @param id
 * @param mode
 * @param file_name
 */
file::file(const char *id, file_mode mode, const char *file_name) : entity::entity("file", id)
{
    this->file_pointer = nullptr;
    this->mode = FILE_MODE_NOT_OPEN;
    this->is_already_read = false;

    this->open(mode, file_name);
}

/**
 * The constructor.
 *
 * @param id
 */
file::file(const char *id) : entity::entity("file", id)
{
    this->file_pointer = nullptr;
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
    return (this->file_pointer != nullptr) && file_mode_is_valid(this->mode);
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

    if (!this->is_already_read)
    {
        this->read_into_buffer();
    }

    primitive_data *data = nullptr;

    if (this->buffer.empty())
    {
        data = primitive_data::create(
            std::string(this->id).append(":content"),
            DATA_TYPE_STRING
        );
    }
    else
    {
        data = primitive_data::create(
            std::string(this->id).append(":content"),
            DATA_TYPE_STRING,
            this->buffer.c_str()
        );
    }

    this->is_already_read = true;
    return data;
}

/**
 * Get file size in bytes.
 *
 * @return
 */
int64_t
file::get_size()
{
    int64_t size;

    if (!this->is_opened())
    {
        return 0;
    }

    rewind(this->file_pointer);
    fseek(this->file_pointer, 0L, SEEK_END);
    size = ftell(this->file_pointer);
    rewind(this->file_pointer);

    return size;
}

/**
 * Write data to file.
 * @param e
 */
void
file::write(entity *e)
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

    if (e->get_entity_type() == "primitive_data")
    {
        this->buffer.append(((primitive_data *) e)->get_string());
    }
    else if (e->get_entity_type() == "collection")
    {
        this->buffer.append(((collection *) e)->to_string().get_string());
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
        fwrite(buffer.c_str(), sizeof(int8_t), buffer.size(), this->file_pointer);
        this->buffer.clear();
    }

    fclose(this->file_pointer);
    this->file_pointer = nullptr;
    this->mode = FILE_MODE_NOT_OPEN;
}

/**
 * The destructor.
 */
file::~file()
{
    this->close();
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
    int64_t size = this->get_size();

    if (size == 0)
    {
        this->buffer.clear();
    }
    else
    {
        this->buffer.resize(static_cast<unsigned long>(size + 1));

        fread((void *) this->buffer.c_str(),
              sizeof(int8_t),
              static_cast<size_t>(size + 1),
              this->file_pointer
        );
    }
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

    this->mode = mode;
    if (!file_mode_is_valid(this->mode))
    {
        ERROR_LOG_ADD(ERROR_FILE_UNKNOWN_MODE);
        return;
    }

    this->file_pointer = fopen(file_name, file_mode_get_format(mode));
    if (!this->file_pointer)
    {
        if (this->mode == FILE_MODE_READ)
        {
            ERROR_LOG_ADD(ERROR_FILE_UNKNOWN_FILE);
        }
        else
        {
            ERROR_LOG_ADD(ERROR_FILE_DID_NOT_OPEN);
        }

        this->mode = FILE_MODE_NOT_OPEN;
    }
}




