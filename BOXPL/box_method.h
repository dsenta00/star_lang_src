#ifndef BOX_METHOD_H
#define BOX_METHOD_H
#include "ORM/entity.h"
#include "box_data_type.h"
#include "box_fw.h"

class box_method : public entity {
public:
  box_method(std::string id);
};

#endif // BOX_METHOD_H
