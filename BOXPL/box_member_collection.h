#ifndef BOX_MEMBER_COLLECTION_H
#define BOX_MEMBER_COLLECTION_H
#include <map>
#include "box_data.h"
#include "box_data_member.h"

typedef uint16_t box_member_collection_key;

class box_member_collection {
public:
  box_member_collection();
  bool add(box_member_collection_key key,
           box_data_member *data_if);
  box_data *to_string();
  uint16_t get_noof();
  box_data_member *operator [] (box_member_collection_key key);
  ~box_member_collection();
protected:
  std::map<box_member_collection_key, box_data_member *> members;
};

#endif // BOX_MEMBER_COLLECTION_H
