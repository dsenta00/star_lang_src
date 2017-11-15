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

#include <regex>
#include "data_type.h"

/**
 * Get data type from token.
 *
 * @param str
 * @return
 */
data_type
data_type_get_from_token(const std::wstring str)
{
    if (str == L"bool")
    {
        return DATA_TYPE_BOOL;
    }
    else if (str == L"char")
    {
        return DATA_TYPE_CHAR;
    }
    else if (str == L"int")
    {
        return DATA_TYPE_INT;
    }
    else if (str == L"float")
    {
        return DATA_TYPE_FLOAT;
    }
    else if (str == L"string")
    {
        return DATA_TYPE_STRING;
    }
    else
    {
        return DATA_TYPE_INVALID;
    }
}

/**
 * Detect data_type.
 *
 * @param sample
 * @return
 */
data_type
data_type_detect(const std::wstring sample)
{
    if (sample.empty())
    {
        return DATA_TYPE_INVALID;
    }

    if (std::regex_match(sample, std::wregex(L"([-+]?[0-9]+)")))
    {
        return DATA_TYPE_INT;
    }
    else if (std::regex_match(sample, std::wregex(L"[-+]?[0-9]*\\.?[0-9]*")))
    {
        return DATA_TYPE_FLOAT;
    }
    else if (std::regex_match(sample, std::wregex(L"\'?.\'")))
    {
        return DATA_TYPE_CHAR;
    }
    else if (std::regex_match(sample, std::wregex(L"\"(.*)\"")) or std::regex_match(sample, std::wregex(L"\'(.*)+\'")))
    {
        return DATA_TYPE_STRING;
    }
    else if (std::regex_match(sample, std::wregex(L"true|false")))
    {
        return DATA_TYPE_BOOL;
    }
    else
    {
        return DATA_TYPE_INVALID;
    }
}

/**
 * Clean constant format.
 *
 * @param sample
 * @param type
 * @return
 */
std::wstring
clean_constant_format(std::wstring &sample, data_type type)
{
    switch (type)
    {
        case DATA_TYPE_INT:
        case DATA_TYPE_FLOAT:
            /*
             * nothing to do.
             */
            break;
        case DATA_TYPE_CHAR:
        case DATA_TYPE_STRING:
        {
            sample.erase(sample.begin());
            sample.erase(sample.end());
            break;
        }
        case DATA_TYPE_BOOL:
        {
            sample.assign(1, (wchar_t) (sample == L"true"));
            break;
        }
        default:
            break;
    }

    return sample;
}

#endif // BOX_DATA_TYPE_CPP
