#ifndef BOX_ARRAY_H
#define BOX_ARRAY_H

#include "ORM/entity.h"
#include "box_data_type.h"
#include "box_fw.h"
#include <string>

/**
 * Represents data array.
 */
class box_array : public entity {
public:
  box_array(std::string id, box_array *array = nullptr);
  uint16_t get_noof();
  entity *operator[](uint32_t index);
  entity *operator[](std::string index);
  void insert(std::string index, entity *e);
  void insert(uint32_t index, entity *e);
  bool operator+=(entity *e);
  box_data &to_string();
  void clear();
  virtual ~box_array();
  static box_array *create(std::string id, box_array *array = nullptr);
protected:
  void remove_data(std::string index);
  void remove_data(entity *e);
  void insert_data(std::string index, entity *e);
  std::map<std::string, entity *> array;
};

#endif // BOX_ARRAY_H
