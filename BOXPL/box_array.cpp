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
  this->type = BOX_DATA_INVALID;

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
  if (type >= BOX_DATA_INVALID)
  {
    this->type = BOX_DATA_INVALID;
    BOX_ERROR(ERROR_BOX_ARRAY_INVALID_DATA_TYPE);
    return;
  }

  this->type = type;

  if (size == 0)
  {
    BOX_ERROR(ERROR_BOX_ARRAY_ZERO_SIZE);
    return;
  }

  this->box_data_vector.reserve(size);

  for (uint32_t i = 0; i < size; ++i)
  {
    this->box_data_vector.push_back(new box_data(this->type));
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
  return this->box_data_vector.size();
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
  if (this->type == BOX_DATA_INVALID)
  {
    BOX_ERROR(ERROR_BOX_ARRAY_INVALID_DATA_TYPE);
    return NULL;
  }

  if (this->get_noof() == 0)
  {
    BOX_ERROR(ERROR_BOX_ARRAY_ZERO_SIZE);
    return NULL;
  }

  if (index >= this->get_noof())
  {
    BOX_ERROR(ERROR_BOX_ARRAY_OUT_OF_BOUNDS);
    return NULL;
  }

  return this->box_data_vector[index];
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

  if ((this->type == BOX_DATA_INVALID) ||
      (data->get_type() == BOX_DATA_INVALID))
  {
    BOX_ERROR(ERROR_BOX_ARRAY_INVALID_DATA_TYPE);
    return false;
  }

  box_data *new_data = new box_data(this->type);
  (*new_data) = (*data);
  this->box_data_vector.push_back(new_data);

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
  if ((this->type == BOX_DATA_INVALID) ||
      (array.type == BOX_DATA_INVALID))
  {
    BOX_ERROR(ERROR_BOX_ARRAY_INVALID_DATA_TYPE);
    return false;
  }

  for (box_data *data : array.box_data_vector)
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

  if (this->type == BOX_DATA_INVALID)
  {
    BOX_ERROR(ERROR_BOX_ARRAY_INVALID_DATA_TYPE);
    return str;
  }

  if (this->get_noof() == 0)
  {
    BOX_ERROR(ERROR_BOX_ARRAY_ZERO_SIZE);
    return str;
  }

  char ch  = ' ';
  box_data separator_char(BOX_DATA_CHAR, (void *)&ch);

  for (box_data *data : this->box_data_vector)
  {
    str += *data;

    if (!BOX_OK)
    {
      str.default_value();
      break;
    }

    if ((data != box_data_vector.back()) && (this->type != BOX_DATA_CHAR))
    {
      str += separator_char;
    }
  }

  return str;
}

/**
 * The destructor.
 */
box_array::~box_array()
{
  for (box_data *data : box_data_vector)
  {
    delete data;
  }

  this->box_data_vector.clear();
}
