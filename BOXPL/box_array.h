#ifndef BOX_ARRAY_H
#define BOX_ARRAY_H
#include "box_data.h"
#include "box_vector.h"

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
protected:
  box_data_type type;
  box_vector<box_data *> data_array;
};

#endif // BOX_ARRAY_H
