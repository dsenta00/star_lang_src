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

#pragma once

#include <cstdint>
#include <string>
#include <ORM/object_type.h>

const wchar_t DATA_TYPE_FORMAT[][8] =
    {
        L"%d",  // OBJECT_TYPE_BOOL,
        L"%c",  // OBJECT_TYPE_CHAR,
        L"%d",  // OBJECT_TYPE_INT,
        L"%lf", // OBJECT_TYPE_FLOAT,
        L"%ls", // OBJECT_TYPE_STRING,
        L""     // OBJECT_TYPE_NULL
    };

const uint8_t DATA_TYPE_SIZE[] =
    {
        sizeof(bool),        // OBJECT_TYPE_BOOL,
        sizeof(wchar_t),     // OBJECT_TYPE_CHAR,
        sizeof(int32_t),     // OBJECT_TYPE_INT,
        sizeof(double),      // OBJECT_TYPE_FLOAT,
        sizeof(wchar_t) * 8, // OBJECT_TYPE_SIZE, default size
        0                    // OBJECT_TYPE_NULL
    };

object_type data_type_get_from_token(std::wstring str);
object_type data_type_detect(std::wstring sample);
std::wstring data_type_clean_constant_format(std::wstring &sample, object_type type);