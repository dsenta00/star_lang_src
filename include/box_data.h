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

#ifndef BOX_DATA_H
#define BOX_DATA_H

#include "ORM/entity.h"
#include "box_data_type.h"
#include "box_fw.h"
#include <string>

/**
 * The box_data class. Represents data in BOX PL and defines
 * all operations in that scope.
 */
class box_data : public entity {
public:
  box_data(std::string id,
           box_data_type type = BOX_DATA_INVALID,
           const void *value = nullptr);
  box_data(std::string id, box_data &data);
  static box_data *create(std::string id,
                          box_data_type type = BOX_DATA_INVALID,
                          const void *value = nullptr);
  static box_data *create(std::string id,
                          box_data &data);
  bool to_bool();
  int8_t to_char();
  int32_t to_int();
  double to_float();
  box_data &to_string();

  void convert_itself(box_data_type new_type = BOX_DATA_INVALID);
  bool default_value();
  bool operator=(const void *data);
  bool operator=(box_data &data);
  bool operator&=(box_data &data);
  bool operator|=(box_data &data);
  bool operator^=(box_data &data);
  bool operator+=(box_data &data);
  bool operator-=(box_data &data);
  bool operator*=(box_data &data);
  bool operator/=(box_data &data);
  bool operator%=(box_data &data);
  bool operator++();
  bool operator--();
  bool operator==(box_data &data);
  bool operator!=(box_data &data);
  bool operator>(box_data &data);
  bool operator<(box_data &data);
  bool operator>=(box_data &data);
  bool operator<=(box_data &data);

  memory *get_memory();
  uintptr_t get_address();
  box_data_type get_type();

  bool print();
  bool println();
  bool scan();

protected:
  box_virtual_memory *vm;
  std::string get_string();
  box_data_type type;
};

#endif // BOX_DATA_H
