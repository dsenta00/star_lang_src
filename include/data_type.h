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

#ifndef DATA_TYPE_H
#define DATA_TYPE_H

#include <cstdint>
#include <string>

/**
 * Data type enumeration.
 *
 * @value DATA_TYPE_BOOL    - represents 8 bit boolean.
 * @value DATA_TYPE_CHAR    - represents 8 bit character type.
 * @value DATA_TYPE_INT     - represents 32 bit signed type.
 * @value DATA_TYPE_FLOAT   - represents 32 bit floating point type.
 * @value DATA_TYPE_STRING  - represents string type (the character array
 *                            that ends with value of 0)
 * @value DATA_TYPE_INVALID - invalid type.
 */
typedef enum {
    DATA_TYPE_BOOL,
    DATA_TYPE_CHAR,
    DATA_TYPE_INT,
    DATA_TYPE_FLOAT,
    DATA_TYPE_STRING,
    DATA_TYPE_INVALID
} data_type;

const wchar_t DATA_TYPE_FORMAT[][8] =
    {
        L"%d",  // DATA_TYPE_BOOL,
        L"%c",  // DATA_TYPE_CHAR,
        L"%d",  // DATA_TYPE_INT,
        L"%lf", // DATA_TYPE_FLOAT,
        L"%ls", // DATA_TYPE_STRING,
        L""     // DATA_TYPE_INVALID
    };

const uint8_t DATA_TYPE_SIZE[] =
    {
        sizeof(bool),        // DATA_TYPE_BOOL,
        sizeof(wchar_t),     // DATA_TYPE_CHAR,
        sizeof(int32_t),     // DATA_TYPE_INT,
        sizeof(double),      // DATA_TYPE_FLOAT,
        sizeof(wchar_t) * 8, // DATA_TYPE_STRING, default size
        0                    // DATA_TYPE_INVALID
    };

data_type get_from_token(std::string &str);

#endif // DATA_TYPE_H
