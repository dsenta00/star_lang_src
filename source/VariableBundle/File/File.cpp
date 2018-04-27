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

#include <ORM/ORM.h>
#include <ErrorBundle/ErrorLog.h>
#include <VariableBundle/Primitive/String.h>
#include <VariableBundle/File/File.h>
#include <fstream>
#include <codecvt>
#include <sstream>
#include <iostream>
#include <locale>

/**
 * The constructor.
 *
 * @param mode
 * @param fileName
 */
File::File(eFileMode mode, const char *fileName) : Value::Value()
{
    this->mode = FILE_MODE_NOT_OPEN;
    this->isAlreadyRead = false;

    this->open(mode, fileName);
}

/**
 * The constructor.
 */
File::File() : Value::Value()
{
    this->mode = FILE_MODE_NOT_OPEN;
    this->isAlreadyRead = false;
}

/**
 * Check if file is opened.
 *
 * @return
 */
bool
File::isOpened()
{
    return FileMode::isValid(this->mode);
}

/**
 * Read all content from file.
 *
 * @return
 */
String *
File::readAll()
{
    if (!this->isOpened())
    {
        return nullptr;
    }

    if (!FileMode::canRead(this->mode))
    {
        ERROR_LOG_ADD(ERROR_FILE_READ_ON_WRITE_MODE);
        return nullptr;
    }

    this->readIntoBuffer();

    String *data = nullptr;

    if (this->buffer.empty())
    {
        data = String::create();
    }
    else
    {
        data = String::create(this->buffer.c_str());
    }

    return data;
}

/**
 * Write data to file.
 * @param o
 */
void
File::write(Value *o)
{
    if (!this->isOpened())
    {
        return;
    }

    if (!FileMode::canWrite(this->mode))
    {
        ERROR_LOG_ADD(ERROR_FILE_WRITE_ON_READ_MODE);
        return;
    }

    this->buffer.append(o->getString());
}

/**
 * Close file stream.
 */
void
File::close()
{
    if (!this->isOpened())
    {
        return;
    }

    if (FileMode::canWrite(this->mode))
    {
        this->writeFromBuffer();
    }

    this->fileName.clear();
    this->mode = FILE_MODE_NOT_OPEN;
}

/**
 * Create file object.
 *
 * @param mode
 * @param fileName
 * @return
 */
File *
File::create(eFileMode mode, const char *fileName)
{
    return (File *) ORM::create(new File(mode, fileName));
}

/**
 * Create file object.
 *
 * @return
 */
File *
File::create()
{
    return (File *) ORM::create(new File());
}

/**
 * Read file content into buffer.
 */
void
File::readIntoBuffer()
{
    if (this->isAlreadyRead)
    {
        return;
    }

    std::wifstream wif(this->fileName);

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
    this->isAlreadyRead = true;
}

/**
 * Open file stream.
 *
 * @param mode
 * @param fileName
 */
void
File::open(eFileMode mode, const char *fileName)
{
    if (this->isOpened())
    {
        this->close();
    }

    this->fileName = fileName;

    this->mode = mode;
    if (!FileMode::isValid(this->mode))
    {
        ERROR_LOG_ADD(ERROR_FILE_UNKNOWN_MODE);
        return;
    }

    if (this->mode == FILE_MODE_READ)
    {
        this->readIntoBuffer();
    }
}

/**
 * Write from buffer.
 */
void
File::writeFromBuffer()
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
        of.open(this->fileName);
    }
    else if (this->mode == FILE_MODE_APPEND)
    {
        of.open(this->fileName, std::ios_base::app);
    }

    of << str;
    of.close();

    this->buffer.clear();
}

/**
 * The destructor.
 */
File::~File()
{
    this->close();
}

/**
 * @inherit
 */
eObjectType
File::getObjectType()
{
    return OBJECT_TYPE_FILE;
}

/**
 * @inherit
 */
bool
File::toBool()
{
    return false;
}

/**
 * @inherit
 */
wchar_t
File::toChar()
{
    return 0;
}

/**
 * @inherit
 */
int32_t
File::toInt()
{
    return 0;
}

/**
 * @inherit
 */
double
File::toFloat()
{
    return 0;
}

/**
 * @inherit
 */
String &
File::toString()
{
    return *String::create(this->buffer.c_str());
}

/**
 * @inherit
 */
bool
File::isReference()
{
    return true;
}

/**
 * @inherit
 */
bool
File::defaultValue()
{
    ERROR_LOG_ADD(ERROR_FILE_INVALID_OPERATION);

    return false;
}

/**
 * @inherit
 */
bool
File::operator=(const void *data)
{
    ERROR_LOG_ADD(ERROR_FILE_INVALID_OPERATION);

    return false;
}

/**
 * @inherit
 */
bool
File::operator=(Value &data)
{
    if (FileMode::canWrite(this->mode))
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
File::operator&=(Value &data)
{
    ERROR_LOG_ADD(ERROR_FILE_INVALID_OPERATION);

    return false;
}

/**
 * @inherit
 */
bool
File::operator|=(Value &data)
{
    ERROR_LOG_ADD(ERROR_FILE_INVALID_OPERATION);

    return false;
}

/**
 * @inherit
 */
bool
File::operator^=(Value &data)
{
    ERROR_LOG_ADD(ERROR_FILE_INVALID_OPERATION);

    return false;
}

/**
 * @inherit
 */
bool
File::operator+=(Value &data)
{
    this->write(&data);

    return true;
}

/**
 * @inherit
 */
bool
File::operator-=(Value &data)
{
    ERROR_LOG_ADD(ERROR_FILE_INVALID_OPERATION);

    return false;
}

/**
 * @inherit
 */
bool
File::operator*=(Value &data)
{
    ERROR_LOG_ADD(ERROR_FILE_INVALID_OPERATION);

    return false;
}

/**
 * @inherit
 */
bool
File::operator/=(Value &data)
{
    ERROR_LOG_ADD(ERROR_FILE_INVALID_OPERATION);

    return false;
}

/**
 * @inherit
 */
bool
File::operator%=(Value &data)
{
    ERROR_LOG_ADD(ERROR_FILE_INVALID_OPERATION);

    return false;
}

/**
 * @inherit
 */
bool
File::operator++()
{
    ERROR_LOG_ADD(ERROR_FILE_INVALID_OPERATION);

    return false;
}

/**
 * @inherit
 */
bool
File::operator--()
{
    ERROR_LOG_ADD(ERROR_FILE_INVALID_OPERATION);

    return false;
}

/**
 * @inherit
 */
bool
File::operator==(Value &data)
{
    if (FileMode::canRead(this->mode))
    {
        if (!this->isAlreadyRead)
        {
            this->readIntoBuffer();
        }

        return this->buffer == data.getString();
    }

    return false;
}

/**
 * @inherit
 */
bool
File::operator!=(Value &data)
{
    return !this->operator==(data);
}

/**
 * @inherit
 */
bool
File::operator>(Value &data)
{
    ERROR_LOG_ADD(ERROR_FILE_INVALID_OPERATION);

    return false;
}

/**
 * @inherit
 */
bool
File::operator<(Value &data)
{
    ERROR_LOG_ADD(ERROR_FILE_INVALID_OPERATION);

    return false;
}

/**
 * @inherit
 */
bool
File::operator>=(Value &data)
{
    ERROR_LOG_ADD(ERROR_FILE_INVALID_OPERATION);

    return false;
}

/**
 * @inherit
 */
bool
File::operator<=(Value &data)
{
    ERROR_LOG_ADD(ERROR_FILE_INVALID_OPERATION);

    return false;
}

/**
 * @inherit
 */
bool
File::print()
{
    if (!FileMode::canRead(this->mode))
    {
        ERROR_LOG_ADD(ERROR_FILE_READ_ON_WRITE_MODE);

        return false;
    }

    if (!this->isAlreadyRead)
    {
        this->readIntoBuffer();
    }

    std::wcout << this->buffer;

    return true;
}

/**
 * @inherit
 */
bool
File::println()
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
File::scan()
{
    ERROR_LOG_ADD(ERROR_FILE_INVALID_OPERATION);

    return false;
}

/**
 * @inherit
 */
std::wstring
File::getString()
{
    return this->buffer;
}

/**
 * Get file mode.
 *
 * @return file mode.
 */
eFileMode
File::get_mode()
{
    return this->mode;
}