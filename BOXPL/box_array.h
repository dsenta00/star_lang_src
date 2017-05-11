#ifndef BOX_ARRAY_H
#define BOX_ARRAY_H
#include "box_data.h"
#include <vector>

/**
 * Represents data array.
 */
class box_array {
public:
  box_array();
  box_array(box_data_type type, uint32_t size);

  uint16_t get_noof();
  box_data * operator [] (uint32_t index);
  bool operator += (box_data *data);
  bool operator += (box_array &array);
  box_data to_string();

  ~box_array();
protected:
  box_data_type type;
  std::vector<box_data *> box_data_vector;
};

#endif // BOX_ARRAY_H
