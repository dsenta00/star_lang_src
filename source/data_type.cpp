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

#ifndef BOX_DATA_TYPE_CPP
#define BOX_DATA_TYPE_CPP

#include "data_type.h"

data_type
get_data_type_from_token(std::string &str)
{
    if (str == "bool")
    {
        return DATA_TYPE_BOOL;
    }
    else if (str == "char")
    {
        return DATA_TYPE_CHAR;
    }
    else if (str == "int")
    {
        return DATA_TYPE_INT;
    }
    else if (str == "float")
    {
        return DATA_TYPE_FLOAT;
    }
    else if (str == "string")
    {
        return DATA_TYPE_STRING;
    }
    else
    {
        return DATA_TYPE_INVALID;
    }
}

#endif // BOX_DATA_TYPE_CPP
