#include "box_monitor.h"
#include "box_member_collection.h"

box_member_collection::box_member_collection() { }

bool
box_member_collection::add(box_member_collection_key key,
                           box_data_member *data_member)
{
  if (!data_member)
  {
    BOX_ERROR(ERROR_BOX_MEMBER_COLLECTION_INVALID_DATA_MEMBER);
    return false;
  }

  if (this->members[key])
  {
    BOX_ERROR(ERROR_BOX_MEMBER_COLLECTION_DUPLICATE_KEY);
    return false;
  }

  this->members[key] = data_member;

  return true;
}

uint16_t
box_member_collection::get_noof()
{
  return (uint16_t)this->members.size();
}

box_data_member *
box_member_collection::operator [] (box_member_collection_key key)
{
  box_data_member *data_if = members[key];

  if (data_if == NULL)
  {
    BOX_ERROR(ERROR_BOX_MEMBER_COLLECTION_INVALID_MEMBER);
  }

  return data_if;
}

box_member_collection::~box_member_collection()
{
  for (auto it = this->members.begin();
       it != this->members.end();
       it++)
  {
    if (it->second != NULL)
    {
      it->second->clean();
      delete it->second;
    }
  }
}
