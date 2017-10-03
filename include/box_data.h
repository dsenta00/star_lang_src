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
  float64_t to_float();
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
