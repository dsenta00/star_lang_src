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

#ifndef OBJECT_TYPE_H
#define OBJECT_TYPE_H

typedef enum {
    OBJECT_TYPE_BOOL,
    OBJECT_TYPE_CHAR,
    OBJECT_TYPE_INT,
    OBJECT_TYPE_FLOAT,
    OBJECT_TYPE_STRING,
    OBJECT_TYPE_NULL,
    OBJECT_TYPE_COLLECTION,
    OBJECT_TYPE_VARIABLE,
    OBJECT_TYPE_INSTRUCTION,
    OBJECT_TYPE_MEMORY,
    OBJECT_TYPE_MEMORY_CHUNK,
    OBJECT_TYPE_VIRTUAL_MEMORY,
    OBJECT_TYPE_METHOD,
    OBJECT_TYPE_FILE,
} object_type;

#endif //OBJECT_TYPE_H
