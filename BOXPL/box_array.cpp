#include "box_array.h"
#include "box_monitor.h"
#include "box_virtual_memory.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern box_virtual_memory virtual_memory;

/**
 * The constructor.
 */
box_array::box_array()
{
  type = BOX_DATA_INVALID;

  BOX_ERROR(ERROR_BOX_ARRAY_ZERO_SIZE);
  BOX_ERROR(ERROR_BOX_ARRAY_INVALID_DATA_TYPE);
}

/**
 * The constructor.
 *
 * @param type - data type.
 * @param size - array size in members.
 */
box_array::box_array(box_data_type type, uint32_t size)
{
  this->type = type;

  if (type >= BOX_DATA_INVALID)
  {
    type = BOX_DATA_INVALID;
    BOX_ERROR(ERROR_BOX_ARRAY_INVALID_DATA_TYPE);
    return;
  }

  if (size == 0)
  {
    BOX_ERROR(ERROR_BOX_ARRAY_ZERO_SIZE);
    return;
  }

  data_array.reserve(size);

  for (uint32_t i = 0; i < size; ++i)
  {
    data_array.push_back(new box_data(type));
  }
}

/**
 * Get number of data.
 *
 * @return number of data.
 */
uint16_t
box_array::get_noof()
{
  return data_array.size();
}

/**
 * Operator [].
 *
 * @param index - element index.
 * @return data of found, otherwise return NULL.
 */
box_data *
box_array::operator [] (uint32_t index)
{
  if (type == BOX_DATA_INVALID)
  {
    BOX_ERROR(ERROR_BOX_ARRAY_INVALID_DATA_TYPE);
    return NULL;
  }

  if (get_noof() == 0)
  {
    BOX_ERROR(ERROR_BOX_ARRAY_ZERO_SIZE);
    return NULL;
  }

  if (index >= get_noof())
  {
    BOX_ERROR(ERROR_BOX_ARRAY_OUT_OF_BOUNDS);
    return NULL;
  }

  return data_array[index];
}

/**
 * Operator +=. Copy and append data to this array.
 *
 * @param data - the data.
 * @return true if operation success, otherwise return false.
 */
bool
box_array::operator += (box_data *data)
{
  if (data == NULL)
  {
    BOX_ERROR(ERROR_BOX_ARRAY_ADDING_NULL_DATA);
    return false;
  }

  if ((type == BOX_DATA_INVALID) ||
      (data->get_type() == BOX_DATA_INVALID))
  {
    BOX_ERROR(ERROR_BOX_ARRAY_INVALID_DATA_TYPE);
    return false;
  }

  box_data *new_data = new box_data(type);
  (*new_data) = (*data);
  data_array.push_back(new_data);

  return true;
}

/**
 * Operator +=. Copy and append array elements to
 * this array.
 *
 * @param array - the array.
 * @return true if operation success, otherwise return false.
 */
bool
box_array::operator += (box_array &array)
{
  if ((type == BOX_DATA_INVALID) ||
      (array.type == BOX_DATA_INVALID))
  {
    BOX_ERROR(ERROR_BOX_ARRAY_INVALID_DATA_TYPE);
    return false;
  }

  for (box_data *data : array.data_array)
  {
    if (!((*this) += data))
    {
      return false;
    }
  }

  return true;
}

/**
 * Convert array into string.
 *
 * @return the box data string.
 */
box_data
box_array::to_string()
{
  box_data str(BOX_DATA_STRING);

  if (type == BOX_DATA_INVALID)
  {
    BOX_ERROR(ERROR_BOX_ARRAY_INVALID_DATA_TYPE);
    return str;
  }

  if (get_noof() == 0)
  {
    BOX_ERROR(ERROR_BOX_ARRAY_ZERO_SIZE);
    return str;
  }

  char ch  = ' ';
  box_data separator_char(BOX_DATA_CHAR, (void *)&ch);

  for (box_data *data : data_array)
  {
    str += *data;

    if (!BOX_OK)
    {
      str.default_value();
      break;
    }

    if ((data != data_array.back()) && (type != BOX_DATA_CHAR))
    {
      str += separator_char;
    }
  }

  return str;
}
