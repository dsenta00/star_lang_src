#include "box_data_member.h"
#include "box_monitor.h"

/**
 * The constructor.
 */
box_data_member::box_data_member()
{
  this->reference = 0;
  this->group = BOX_DATA_GROUP_INVALID;

  BOX_ERROR(ERROR_BOX_DATA_MEMBER_INVALID_GROUP);
  BOX_ERROR(ERROR_BOX_DATA_MEMBER_NULL_REFERENCE);
}

/**
 * The constructor.
 *
 * @param data - the box data.
 */
box_data_member::box_data_member(box_data *data)
{
  if (data == NULL)
  {
    BOX_ERROR(ERROR_BOX_DATA_MEMBER_NULL_REFERENCE);
    return;
  }

  this->reference = (uintptr_t)data;
  this->group = BOX_DATA_GROUP_PRIMARY;
}

/**
 * The constructor.
 *
 * @param array - the box array.
 */
box_data_member::box_data_member(box_array *array)
{
  if (array == NULL)
  {
    BOX_ERROR(ERROR_BOX_DATA_MEMBER_NULL_REFERENCE);
    return;
  }

  this->reference = (uintptr_t)array;
  this->group = BOX_DATA_GROUP_ARRAY;
}

/**
 * Get data.
 *
 * @return box data if success otherwise return NULL.
 */
box_data *
box_data_member::get_data()
{
  box_data *data = NULL;

  if (this->reference == 0)
  {
    BOX_ERROR(ERROR_BOX_DATA_MEMBER_NULL_REFERENCE);
    return NULL;
  }

  switch (this->group)
  {
  case BOX_DATA_GROUP_ARRAY:
    BOX_ERROR(ERROR_BOX_DATA_MEMBER_ARRAY_AS_DATA);
    break;
  case BOX_DATA_GROUP_PRIMARY:
    data = (box_data *)this->reference;
    break;
  case BOX_DATA_GROUP_INVALID:
  default:
    BOX_ERROR(ERROR_BOX_DATA_MEMBER_INVALID_GROUP);
    break;
  }

  return data;
}

/**
 * Get data from array.
 *
 * @param index - array index.
 *
 * @return array data if success, otherwise return NULL.
 */
box_data *
box_data_member::get_data(uint16_t index)
{
  box_data *data = NULL;

  if (this->reference == 0)
  {
    BOX_ERROR(ERROR_BOX_DATA_MEMBER_NULL_REFERENCE);
    return NULL;
  }

  switch (this->group)
  {
  case BOX_DATA_GROUP_ARRAY:
    data = (*(box_array *)this->reference)[index];
    break;
  case BOX_DATA_GROUP_PRIMARY:
    BOX_ERROR(ERROR_BOX_DATA_MEMBER_DATA_AS_ARRAY);
    break;
  case BOX_DATA_GROUP_INVALID:
  default:
    BOX_ERROR(ERROR_BOX_DATA_MEMBER_INVALID_GROUP);
    break;
  }

  return data;
}

/**
 * Get number of elements.
 *
 * @retval 0 - invalid data member group or NULL reference.
 * @retval 1 - primary data.
 * @retval > 0 - array data.
 */
uint16_t
box_data_member::get_noof()
{
  if (this->reference == 0)
  {
    BOX_ERROR(ERROR_BOX_DATA_MEMBER_NULL_REFERENCE);
    return 0;
  }

  switch (this->group)
  {
  case BOX_DATA_GROUP_ARRAY:
    return ((box_array *)this->reference)->get_noof();
  case BOX_DATA_GROUP_PRIMARY:
    return 1;
  case BOX_DATA_GROUP_INVALID:
  default:
    BOX_ERROR(ERROR_BOX_DATA_MEMBER_INVALID_GROUP);
    return 0;
  }
}

/**
 * Get data group:
 *
 * @retval BOX_DATA_GROUP_ARRAY - the array data.
 * @retval BOX_DATA_GROUP_PRIMARY - the primary data.
 * @retval BOX_DATA_GROUP_INVALID - invalid data.
 */
box_data_group
box_data_member::get_group()
{
  return this->group;
}

/**
 * Convert to string.
 *
 * @return the string box data.
 */
box_data
box_data_member::to_string()
{
  box_data str(BOX_DATA_STRING);

  if (this->reference == 0)
  {
    BOX_ERROR(ERROR_BOX_DATA_MEMBER_NULL_REFERENCE);
    return str;
  }

  switch (this->group)
  {
  case BOX_DATA_GROUP_ARRAY:
    return ((box_array *)this->reference)->to_string();
    break;
  case BOX_DATA_GROUP_PRIMARY:
    return ((box_data *)this->reference)->to_string();
    break;
  case BOX_DATA_GROUP_INVALID:
  default:
    BOX_ERROR(ERROR_BOX_DATA_MEMBER_INVALID_GROUP);
    break;
  }

  return str;
}

/**
 * Clean everything.
 */
void
box_data_member::clean()
{
  if ((this->reference == 0) ||
      (this->group == BOX_DATA_GROUP_INVALID))
  {
    return;
  }

  if (this->group == BOX_DATA_GROUP_PRIMARY)
  {
    delete (box_data *)this->reference;
  }
  else
  {
    delete (box_array *)this->reference;
  }

  this->reference = 0;
  this->group = BOX_DATA_GROUP_INVALID;
}
