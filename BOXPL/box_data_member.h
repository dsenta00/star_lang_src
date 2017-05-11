#ifndef BOX_DATA_MEMBER_H
#define BOX_DATA_MEMBER_H
#include "box_data.h"
#include "box_array.h"
#include <stdint.h>

typedef enum _box_data_group {
  BOX_DATA_GROUP_INVALID,
  BOX_DATA_GROUP_PRIMARY,
  BOX_DATA_GROUP_ARRAY
} box_data_group;

class box_data_member {
public:
  box_data_member();
  box_data_member(box_data *data);
  box_data_member(box_array *array);

  box_data_group get_group();
  box_data to_string();
  box_data *get_data();
  box_data *get_data(uint16_t index);
  uint16_t get_noof();

  void clean();
protected:
  box_data_group group;
  uintptr_t reference;
};

#endif // BOX_DATA_MEMBER_H
