#include "box_data.h"
#include "box_virtual_memory.h"
#include <string.h>
#include <stdio.h>
#include <stddef.h>

extern box_virtual_memory virtual_memory;

/**
 * Default constructor.
 */
box_data::box_data()
{
  type = BOX_DATA_INVALID;
  mem = NULL;

  BOX_ERROR(ERROR_BOX_DATA_NULL_DATA);
  BOX_ERROR(ERROR_BOX_DATA_INVALID_DATA_TYPE);
}

/**
 * The constructor.
 *
 * @param type - data type.
 */
box_data::box_data(box_data_type type)
{
  this->type = type;

  if (type >= BOX_DATA_INVALID)
  {
    BOX_ERROR(ERROR_BOX_DATA_INVALID_DATA_TYPE);
    return;
  }

  mem = virtual_memory.alloc(BOX_DATA_TYPE_SIZE[type]);

  if (!mem)
  {
    BOX_ERROR(ERROR_BOX_DATA_NULL_DATA);
    return;
  }

  default_value();
}

/**
 * The constructor.
 *
 * @param type - data type.
 * @param value - initial value.
 */
box_data::box_data(box_data_type type, const void *value)
{
  if (!value)
  {
    mem = NULL;
    BOX_ERROR(ERROR_BOX_DATA_NULL_DATA);
    return;
  }

  this->type = type;

  if (type >= BOX_DATA_INVALID)
  {
    BOX_ERROR(ERROR_BOX_DATA_INVALID_DATA_TYPE);
    return;
  }

  size_t size = (type == BOX_DATA_STRING) ?
        strlen((const char *)value) + 1 :
        BOX_DATA_TYPE_SIZE[type];

  mem = virtual_memory.alloc(size);

  if (!mem)
  {
    BOX_ERROR(ERROR_BOX_DATA_NULL_DATA);
    return;
  }

  memcpy((void *)mem->address, value, size);
}

/**
 * The copy constructor.
 *
 * @param data - box data.
 */
box_data::box_data(const box_data &data)
{
  if (!data.mem)
  {
    BOX_ERROR(ERROR_BOX_DATA_NULL_DATA);
    return;
  }

  if (data.type >= BOX_DATA_INVALID)
  {
    BOX_ERROR(ERROR_BOX_DATA_INVALID_DATA_TYPE);
    return;
  }

  mem = data.mem;
  type = data.type;

  mem->increase_ref();
}

/**
 * Get type string.
 *
 * @return type to string.
 */
const char *
box_data::get_type_str()
{
  switch (type)
  {
  case BOX_DATA_CHAR:
    return "BOX_DATA_CHAR";
    break;
  case BOX_DATA_SHORT:
    return "BOX_DATA_SHORT";
    break;
  case BOX_DATA_INT:
    return "BOX_DATA_INT";
    break;
  case BOX_DATA_FLOAT:
    return "BOX_DATA_FLOAT";
    break;
  case BOX_DATA_LONG:
    return "BOX_DATA_LONG";
    break;
  case BOX_DATA_DOUBLE:
    return "BOX_DATA_DOUBLE";
    break;
  case BOX_DATA_STRING:
    return "BOX_DATA_STRING";
    break;
  case BOX_DATA_INVALID:
  default:
    return "BOX_DATA_INVALID";
    break;
  }
}

/**
 * Convert data to char and return.
 *
 * @return char value if success, otherwise return null terminating character.
 */
int8_t
box_data::to_char()
{
  int8_t value = 0;

  if (!mem)
  {
    BOX_ERROR(ERROR_BOX_DATA_NULL_DATA);
    return value;
  }

  switch (type)
  {
  case BOX_DATA_STRING:
  case BOX_DATA_CHAR:
    value = *(int8_t *)mem->address;
    break;
  case BOX_DATA_SHORT:
    value = (int8_t)*(int16_t *)mem->address;
    break;
  case BOX_DATA_INT:
    value = (int8_t)*(int32_t *)mem->address;
    break;
  case BOX_DATA_FLOAT:
    value = (int8_t)*(float32_t *)mem->address;
    break;
  case BOX_DATA_LONG:
    value = (int8_t)*(int64_t *)mem->address;
    break;
  case BOX_DATA_DOUBLE:
    value = (int8_t)*(float64_t *)mem->address;
    break;
  case BOX_DATA_INVALID:
  default:
    BOX_ERROR(ERROR_BOX_DATA_INVALID_DATA_TYPE);
    break;
  }

  return value;
}

/**
 * Convert data to short and return.
 *
 * @return short value if success, otherwise return 0.
 */
int16_t
box_data::to_short()
{
  int16_t value = 0;

  if (!mem)
  {
    BOX_ERROR(ERROR_BOX_DATA_NULL_DATA);
    return value;
  }

  switch (type)
  {
  case BOX_DATA_CHAR:
    value = (int16_t)*(int8_t *)mem->address;
    break;
  case BOX_DATA_SHORT:
    value = *(int16_t *)mem->address;
    break;
  case BOX_DATA_INT:
    value = (int16_t)*(int32_t *)mem->address;
    break;
  case BOX_DATA_FLOAT:
    value = (int16_t)*(float32_t *)mem->address;
    break;
  case BOX_DATA_LONG:
    value = (int16_t)*(int64_t *)mem->address;
    break;
  case BOX_DATA_DOUBLE:
    value = (int16_t)*(float64_t *)mem->address;
    break;
  case BOX_DATA_STRING:
    sscanf((const char *)mem->address,
           BOX_DATA_TYPE_FORMAT[BOX_DATA_SHORT],
           &value);
    break;
  case BOX_DATA_INVALID:
  default:
    BOX_ERROR(ERROR_BOX_DATA_INVALID_DATA_TYPE);
    break;
  }

  return value;
}

/**
 * Convert data to int and return.
 *
 * @return int value if success, otherwise return 0.
 */
int32_t
box_data::to_int()
{
  int32_t value = 0;

  if (!mem)
  {
    BOX_ERROR(ERROR_BOX_DATA_NULL_DATA);
    return value;
  }

  switch (type)
  {
  case BOX_DATA_CHAR:
    value = (int32_t)*(int8_t *)mem->address;
    break;
  case BOX_DATA_SHORT:
    value = (int32_t)*(int16_t *)mem->address;
    break;
  case BOX_DATA_INT:
    value = *(int32_t *)mem->address;
    break;
  case BOX_DATA_FLOAT:
    value = (int32_t)*(float32_t *)mem->address;
    break;
  case BOX_DATA_LONG:
    value = (int32_t)*(int64_t *)mem->address;
    break;
  case BOX_DATA_DOUBLE:
    value = (int32_t)*(float64_t *)mem->address;
    break;
  case BOX_DATA_STRING:
    sscanf((const char *)mem->address,
           BOX_DATA_TYPE_FORMAT[BOX_DATA_INT],
           &value);
    break;
  case BOX_DATA_INVALID:
  default:
    BOX_ERROR(ERROR_BOX_DATA_INVALID_DATA_TYPE);
    break;
  }

  return value;
}

/**
 * Convert data to float and return.
 *
 * @return float value if success, otherwise return 0.0f.
 */
float32_t
box_data::to_float()
{
  float32_t value = 0.0f;

  if (!mem)
  {
    BOX_ERROR(ERROR_BOX_DATA_NULL_DATA);
    return value;
  }

  switch (type)
  {
  case BOX_DATA_CHAR:
    value = (float32_t)*(int8_t *)mem->address;
    break;
  case BOX_DATA_SHORT:
    value = (float32_t)*(int16_t *)mem->address;
    break;
  case BOX_DATA_INT:
    value = (float32_t)*(int32_t *)mem->address;
    break;
  case BOX_DATA_FLOAT:
    value = *(float32_t *)mem->address;
    break;
  case BOX_DATA_LONG:
    value = (float32_t)*(int64_t *)mem->address;
    break;
  case BOX_DATA_DOUBLE:
    value = (float32_t)*(float64_t *)mem->address;
    break;
  case BOX_DATA_STRING:
    sscanf((const char *)mem->address,
           BOX_DATA_TYPE_FORMAT[BOX_DATA_FLOAT],
           &value);
    break;
  case BOX_DATA_INVALID:
  default:
    BOX_ERROR(ERROR_BOX_DATA_INVALID_DATA_TYPE);
    break;
  }

  return value;
}

/**
 * Convert data to long and return.
 *
 * @return long value if success, otherwise return 0.
 */
int64_t
box_data::to_long()
{
  int64_t value = 0;

  if (!mem)
  {
    BOX_ERROR(ERROR_BOX_DATA_NULL_DATA);
    return value;
  }

  switch (type)
  {
  case BOX_DATA_CHAR:
    value = (int64_t)*(int8_t *)mem->address;
    break;
  case BOX_DATA_SHORT:
    value = (int64_t)*(int16_t *)mem->address;
    break;
  case BOX_DATA_INT:
    value = (int64_t)*(int32_t *)mem->address;
    break;
  case BOX_DATA_FLOAT:
    value = (int64_t)*(float32_t *)mem->address;
    break;
  case BOX_DATA_LONG:
    value = *(int64_t *)mem->address;
    break;
  case BOX_DATA_DOUBLE:
    value = (int64_t)*(float64_t *)mem->address;
    break;
  case BOX_DATA_STRING:
    sscanf((const char *)mem->address,
           BOX_DATA_TYPE_FORMAT[BOX_DATA_LONG],
           &value);
    break;
  case BOX_DATA_INVALID:
  default:
    BOX_ERROR(ERROR_BOX_DATA_INVALID_DATA_TYPE);
    break;
  }

  return value;
}

/**
 * Convert data to double and return.
 *
 * @return double value if success, otherwise return 0.0.
 */
float64_t
box_data::to_double()
{
  float64_t value = 0.0;

  if (!mem)
  {
    BOX_ERROR(ERROR_BOX_DATA_NULL_DATA);
    return value;
  }

  switch (type)
  {
  case BOX_DATA_CHAR:
    value = (float64_t)*(int8_t *)mem->address;
    break;
  case BOX_DATA_SHORT:
    value = (float64_t)*(int16_t *)mem->address;
    break;
  case BOX_DATA_INT:
    value = (float64_t)*(int32_t *)mem->address;
    break;
  case BOX_DATA_FLOAT:
    value = (float64_t)*(float32_t *)mem->address;
    break;
  case BOX_DATA_LONG:
    value = (float64_t)*(int64_t *)mem->address;
    break;
  case BOX_DATA_DOUBLE:
    value = *(float64_t *)mem->address;
    break;
  case BOX_DATA_STRING:
    sscanf((const char *)mem->address,
           BOX_DATA_TYPE_FORMAT[BOX_DATA_DOUBLE],
           &value);
    break;
  case BOX_DATA_INVALID:
  default:
    BOX_ERROR(ERROR_BOX_DATA_INVALID_DATA_TYPE);
    break;
  }

  return value;
}

/**
 * Convert data to string and return.
 *
 * @return the box data string
 */
box_data
box_data::to_string()
{
  const char *buffer = get_string();
  box_data str(BOX_DATA_STRING, buffer);
  return str;
}

/**
 * Get memory address.
 *
 * @return memory address if success, otherwise return 0.
 */
uintptr_t
box_data::get_address()
{
  return (mem != NULL) ? mem->address : 0;
}

/**
 * Get memory information.
 *
 * @return memory information.
 */
memory *
box_data::get_memory()
{
  return mem;
}

/**
 * Get data type.
 *
 * @return data type.
 */
box_data_type
box_data::get_type()
{
  return type;
}

/**
 * Set data to default value.
 *
 * @return true if success, otherwise return false.
 */
bool
box_data::default_value()
{
  if (!mem)
  {
    BOX_ERROR(ERROR_BOX_DATA_NULL_DATA);
    return false;
  }

  switch(type)
  {
  case BOX_DATA_CHAR:
  case BOX_DATA_SHORT:
  case BOX_DATA_INT:
  case BOX_DATA_LONG:
  case BOX_DATA_STRING:
    memset((void *)mem->address, 0, mem->size);
    break;
  case BOX_DATA_FLOAT:
    *(float32_t *)mem->address = 0.0f;
    break;
  case BOX_DATA_DOUBLE:
    *(float64_t *)mem->address = 0.0;
    break;
  case BOX_DATA_INVALID:
  default:
    BOX_ERROR(ERROR_BOX_DATA_INVALID_DATA_TYPE);
    return false;
    break;
  }

  return true;
}

/**
 * Operator =. Assign value from memory address.
 *
 * @param data - the data memory address.
 *
 * @return true if success, otherwise return false.
 */
bool
box_data::operator = (const void *data)
{
  if ((!mem) || (!data))
  {
    BOX_ERROR(ERROR_BOX_DATA_NULL_DATA);
    return false;
  }

  switch (type)
  {
  case BOX_DATA_CHAR:
    *(int8_t *)mem->address = *(int8_t *)data;
    break;
  case BOX_DATA_SHORT:
    *(int16_t *)mem->address = *(int16_t *)data;
    break;
  case BOX_DATA_INT:
    *(int32_t *)mem->address = *(int32_t *)data;
    break;
  case BOX_DATA_FLOAT:
    *(float32_t *)mem->address = *(float32_t *)data;
    break;
  case BOX_DATA_LONG:
    *(int64_t *)mem->address = *(int64_t *)data;
    break;
  case BOX_DATA_DOUBLE:
    *(float64_t *)mem->address = *(float64_t *)data;
    break;
  case BOX_DATA_STRING:
  {
    box_data data_string(BOX_DATA_STRING, data);
    memset((void *)mem->address, 0, mem->size);
    return (*this += data_string);
    break;
  }
  case BOX_DATA_INVALID:
  default:
    BOX_ERROR(ERROR_BOX_DATA_INVALID_DATA_TYPE);
    return false;
    break;
  }

  return true;
}

/**
 * Operator =. Assign value from another data.
 *
 * @param data - the data.
 *
 * @return true if success, otherwise return false.
 */
bool
box_data::operator = (box_data &data)
{
  if ((!mem) || (data.get_address() == 0))
  {
    BOX_ERROR(ERROR_BOX_DATA_NULL_DATA);
    return false;
  }

  if (type == BOX_DATA_STRING)
  {
    memset((void *)mem->address, 0, mem->size);
    return (*this) += (data);
  }

  if (data.type == BOX_DATA_STRING)
  {
    BOX_ERROR(ERROR_BOX_DATA_ASSIGNING_STRING);
    return false;
  }

  switch (type)
  {
  case BOX_DATA_CHAR:
    *(int8_t *)mem->address = data.to_char();
    break;
  case BOX_DATA_SHORT:
    *(int16_t *)mem->address = data.to_short();
    break;
  case BOX_DATA_INT:
    *(int32_t *)mem->address = data.to_int();
    break;
  case BOX_DATA_FLOAT:
    *(float32_t *)mem->address = data.to_float();
    break;
  case BOX_DATA_LONG:
    *(int64_t *)mem->address = data.to_long();
    break;
  case BOX_DATA_DOUBLE:
    *(float64_t *)mem->address = data.to_double();
    break;
  case BOX_DATA_STRING:
  case BOX_DATA_INVALID:
  default:
    BOX_ERROR(ERROR_BOX_DATA_INVALID_DATA_TYPE);
    return false;
    break;
  }

  return true;
}

/**
 * Operator +=. Do operation add if primitive data type.
 * On string data type append data.
 *
 * @param data - the data.
 *
 * @return true if success, otherwise return false.
 */
bool
box_data::operator += (box_data &data)
{
  if ((!mem) || (data.get_address() == 0))
  {
    BOX_ERROR(ERROR_BOX_DATA_NULL_DATA);
    return false;
  }

  if (type == BOX_DATA_STRING)
  {
    const char *buffer = data.get_string();

    uint32_t request_size = strlen((const char *)mem->address) +
                            strlen(buffer) + 1;

    if (mem->size < request_size)
    {
      mem = virtual_memory.realloc(mem, request_size);

      if (mem->size != request_size)
      {
        /* Something really bad happened. */
        return false;
      }
    }

    strcat((char *)mem->address, buffer);
    return true;
  }

  if (data.type == BOX_DATA_STRING)
  {
    BOX_ERROR(ERROR_BOX_DATA_ADDING_STRING);
    return false;
  }

  switch (type)
  {
  case BOX_DATA_CHAR:
    *(int8_t *)mem->address += data.to_char();
    break;
  case BOX_DATA_SHORT:
    *(int16_t *)mem->address += data.to_short();
    break;
  case BOX_DATA_INT:
    *(int32_t *)mem->address += data.to_int();
    break;
  case BOX_DATA_FLOAT:
    *(float32_t *)mem->address += data.to_float();
    break;
  case BOX_DATA_LONG:
    *(int64_t *)mem->address += data.to_long();
    break;
  case BOX_DATA_DOUBLE:
    *(float64_t *)mem->address += data.to_double();
    break;
  case BOX_DATA_INVALID:
  default:
    BOX_ERROR(ERROR_BOX_DATA_INVALID_DATA_TYPE);
    return false;
    break;
  }

  return true;
}

/**
 * Operator -=. Do subtract operation.
 *
 * @param data - the data.
 *
 * @return true if success, otherwise return false.
 */
bool
box_data::operator -= (box_data &data)
{
  if ((!mem) || (data.get_address() == 0))
  {
    BOX_ERROR(ERROR_BOX_DATA_NULL_DATA);
    return false;
  }

  if (data.type == BOX_DATA_STRING)
  {
    BOX_ERROR(ERROR_BOX_DATA_SUBTRACTING_STRING);
    return false;
  }

  switch (type)
  {
  case BOX_DATA_CHAR:
    *(int8_t *)mem->address -= data.to_char();
    break;
  case BOX_DATA_SHORT:
    *(int16_t *)mem->address -= data.to_short();
    break;
  case BOX_DATA_INT:
    *(int32_t *)mem->address -= data.to_int();
    break;
  case BOX_DATA_FLOAT:
    *(float32_t *)mem->address -= data.to_float();
    break;
  case BOX_DATA_LONG:
    *(int64_t *)mem->address -= data.to_long();
    break;
  case BOX_DATA_DOUBLE:
    *(float64_t *)mem->address -= data.to_double();
    break;
  case BOX_DATA_STRING:
    BOX_ERROR(ERROR_BOX_DATA_SUBTRACTING_STRING);
    return false;
    break;
  case BOX_DATA_INVALID:
  default:
    BOX_ERROR(ERROR_BOX_DATA_INVALID_DATA_TYPE);
    return false;
    break;
  }

  return true;
}

/**
 * Operator *=. Multiply values.
 *
 * @param data - the data.
 *
 * @return true if success, otherwise return false.
 */
bool
box_data::operator *= (box_data &data)
{
  if ((!mem) || (data.get_address() == 0))
  {
    BOX_ERROR(ERROR_BOX_DATA_NULL_DATA);
    return false;
  }

  if (data.type == BOX_DATA_STRING)
  {
    BOX_ERROR(ERROR_BOX_DATA_MULTIPLYING_STRING);
    return false;
  }

  switch (type)
  {
  case BOX_DATA_CHAR:
    *(int8_t *)mem->address *= data.to_char();
    break;
  case BOX_DATA_SHORT:
    *(int16_t *)mem->address *= data.to_short();
    break;
  case BOX_DATA_INT:
    *(int32_t *)mem->address *= data.to_int();
    break;
  case BOX_DATA_FLOAT:
    *(float32_t *)mem->address *= data.to_float();
    break;
  case BOX_DATA_LONG:
    *(int64_t *)mem->address *= data.to_long();
    break;
  case BOX_DATA_DOUBLE:
    *(float64_t *)mem->address *= data.to_double();
    break;
  case BOX_DATA_STRING:
    BOX_ERROR(ERROR_BOX_DATA_MULTIPLYING_STRING);
    return false;
    break;
  case BOX_DATA_INVALID:
  default:
    BOX_ERROR(ERROR_BOX_DATA_INVALID_DATA_TYPE);
    return false;
    break;
  }

  return true;
}

/**
 * Operator /=. Divide values.
 *
 * @param data - the data.
 *
 * @return true if success, otherwise return false.
 */
bool
box_data::operator /= (box_data &data)
{
  if ((!mem) || (data.get_address() == 0))
  {
    BOX_ERROR(ERROR_BOX_DATA_NULL_DATA);
    return false;
  }

  if (data.type == BOX_DATA_STRING)
  {
    BOX_ERROR(ERROR_BOX_DATA_DIVIDING_STRING);
    return false;
  }

  switch (type)
  {
  case BOX_DATA_CHAR:

    if (data.to_char() == 0)
    {
      BOX_ERROR(ERROR_BOX_DATA_DIVIDING_ZERO);
      return false;
    }

    *(int8_t *)mem->address /= data.to_char();
    break;
  case BOX_DATA_SHORT:

    if (data.to_short() == 0)
    {
      BOX_ERROR(ERROR_BOX_DATA_DIVIDING_ZERO);
      return false;
    }

    *(int16_t *)mem->address /= data.to_short();
    break;
  case BOX_DATA_INT:

    if (data.to_int() == 0)
    {
      BOX_ERROR(ERROR_BOX_DATA_DIVIDING_ZERO);
      return false;
    }

    *(int32_t *)mem->address /= data.to_int();
    break;
  case BOX_DATA_FLOAT:

    if (data.to_float() == 0.0f)
    {
      BOX_ERROR(ERROR_BOX_DATA_DIVIDING_ZERO);
      return false;
    }

    *(float32_t *)mem->address /= data.to_float();
    break;
  case BOX_DATA_LONG:

    if (data.to_long() == 0)
    {
      BOX_ERROR(ERROR_BOX_DATA_DIVIDING_ZERO);
      return false;
    }

    *(int64_t *)mem->address /= data.to_long();
    break;
  case BOX_DATA_DOUBLE:

    if (data.to_double() == 0.0)
    {
      BOX_ERROR(ERROR_BOX_DATA_DIVIDING_ZERO);
      return false;
    }

    *(float64_t *)mem->address /= data.to_double();
    break;
  case BOX_DATA_STRING:
    BOX_ERROR(ERROR_BOX_DATA_DIVIDING_STRING);
    return false;
    break;
  case BOX_DATA_INVALID:
  default:
    BOX_ERROR(ERROR_BOX_DATA_INVALID_DATA_TYPE);
    return false;
    break;
  }

  return true;
}

/**
 * Operator %=. Do modulus between values.
 *
 * @param data - the data.
 *
 * @return true if success, otherwise return false.
 */
bool
box_data::operator %= (box_data &data)
{
  if ((!mem) || (data.get_address() == 0))
  {
    BOX_ERROR(ERROR_BOX_DATA_NULL_DATA);
    return false;
  }

  if ((data.type == BOX_DATA_FLOAT) ||
      (data.type == BOX_DATA_DOUBLE) ||
      (data.type == BOX_DATA_STRING))
  {
    BOX_ERROR(ERROR_BOX_DATA_INVALID_MODULUS);
    return false;
  }

  switch (type)
  {
  case BOX_DATA_CHAR:
    if (data.to_char() == 0)
    {
      BOX_ERROR(ERROR_BOX_DATA_MODULUS_ZERO);
      return false;
    }

    *(int8_t *)mem->address %= data.to_char();
    break;
  case BOX_DATA_SHORT:
    if (data.to_short() == 0)
    {
      BOX_ERROR(ERROR_BOX_DATA_MODULUS_ZERO);
      return false;
    }

    *(int16_t *)mem->address %= data.to_short();
    break;
  case BOX_DATA_INT:

    if (data.to_int() == 0)
    {
      BOX_ERROR(ERROR_BOX_DATA_MODULUS_ZERO);
      return false;
    }

    *(int32_t *)mem->address %= data.to_int();
    break;
  case BOX_DATA_LONG:

    if (data.to_long() == 0)
    {
      BOX_ERROR(ERROR_BOX_DATA_MODULUS_ZERO);
      return false;
    }

    *(int64_t *)mem->address %= data.to_long();
    break;
  case BOX_DATA_FLOAT:
  case BOX_DATA_DOUBLE:
  case BOX_DATA_STRING:
    BOX_ERROR(ERROR_BOX_DATA_INVALID_MODULUS);
    return false;
    break;
  case BOX_DATA_INVALID:
  default:
    BOX_ERROR(ERROR_BOX_DATA_INVALID_DATA_TYPE);
    return false;
    break;
  }

  return true;
}

/**
 * Operator ++. Increment value.
 *
 * @return true if success, otherwise return false.
 */
bool
box_data::operator ++ ()
{
  if (!mem)
  {
    BOX_ERROR(ERROR_BOX_DATA_NULL_DATA);
    return false;
  }

  switch (type)
  {
  case BOX_DATA_CHAR:
    (*(int8_t *)mem->address)++;
    break;
  case BOX_DATA_SHORT:
    (*(int16_t *)mem->address)++;
    break;
  case BOX_DATA_INT:
    (*(int32_t *)mem->address)++;
    break;
  case BOX_DATA_FLOAT:
    (*(float32_t *)mem->address) += 1.0f;
    break;
  case BOX_DATA_LONG:
    (*(int64_t *)mem->address)++;
    break;
  case BOX_DATA_DOUBLE:
    (*(float64_t *)mem->address) += 1.0;
    break;
  case BOX_DATA_STRING:
    BOX_ERROR(ERROR_BOX_DATA_INCREMENTING_STRING);
    return false;
    break;
  case BOX_DATA_INVALID:
  default:
    BOX_ERROR(ERROR_BOX_DATA_INVALID_DATA_TYPE);
    return false;
    break;
  }

  return true;
}

/**
 * Operator --. Decrement value.
 *
 * @return true if success, otherwise return false.
 */
bool
box_data::operator -- ()
{
  if (!mem)
  {
    BOX_ERROR(ERROR_BOX_DATA_NULL_DATA);
    return false;
  }

  switch (type)
  {
  case BOX_DATA_CHAR:
    (*(int8_t *)mem->address)--;
    break;
  case BOX_DATA_SHORT:
    (*(int16_t *)mem->address)--;
    break;
  case BOX_DATA_INT:
    (*(int32_t *)mem->address)--;
    break;
  case BOX_DATA_FLOAT:
    (*(float32_t *)mem->address) -= 1.0f;
    break;
  case BOX_DATA_LONG:
    (*(int64_t *)mem->address)--;
    break;
  case BOX_DATA_DOUBLE:
    (*(float64_t *)mem->address) -= 1.0;
    break;
  case BOX_DATA_STRING:
    BOX_ERROR(ERROR_BOX_DATA_DECREMENTING_STRING);
    return false;
    break;
  case BOX_DATA_INVALID:
  default:
    BOX_ERROR(ERROR_BOX_DATA_INVALID_DATA_TYPE);
    return false;
    break;
  }

  return true;
}

/**
 * Operator ==. Check values equality.
 *
 * @param data - the data.
 *
 * @return true if equal, otherwise return false.
 */
bool
box_data::operator == (box_data &data)
{
  if ((!mem) || (data.get_address() == 0))
  {
    BOX_ERROR(ERROR_BOX_DATA_NULL_DATA);
    return false;
  }

  switch (type)
  {
  case BOX_DATA_CHAR:
    return to_char() == data.to_char();
    break;
  case BOX_DATA_SHORT:
    return to_short() == data.to_short();
    break;
  case BOX_DATA_INT:
    return to_int() == data.to_int();
    break;
  case BOX_DATA_FLOAT:
    return to_float() == data.to_float();
    break;
  case BOX_DATA_LONG:
    return to_long() == data.to_long();
    break;
  case BOX_DATA_DOUBLE:
    return to_double() == data.to_double();
    break;
  case BOX_DATA_STRING:
  {
    if (data.type == BOX_DATA_STRING)
    {
      return strcmp((const char *)mem->address,
                    (const char *)data.get_address()) == 0;
    }
    else
    {
      return strcmp((const char *)mem->address,
                    (const char *)data.get_string()) == 0;
    }
    break;
  }
  case BOX_DATA_INVALID:
  default:
    BOX_ERROR(ERROR_BOX_DATA_INVALID_DATA_TYPE);
    return false;
    break;
  }

  return true;
}

/**
 * Operator ==. Check values non equality.
 *
 * @param data - the data.
 *
 * @return true if not equal, otherwise return false.
 */
bool
box_data::operator != (box_data &data)
{
  if ((!mem) || (data.get_address() == 0))
  {
    BOX_ERROR(ERROR_BOX_DATA_NULL_DATA);
    return false;
  }

  switch (type)
  {
  case BOX_DATA_CHAR:
    return to_char() != data.to_char();
    break;
  case BOX_DATA_SHORT:
    return to_short() != data.to_short();
    break;
  case BOX_DATA_INT:
    return to_int() != data.to_int();
    break;
  case BOX_DATA_FLOAT:
    return to_float() != data.to_float();
    break;
  case BOX_DATA_LONG:
    return to_long() != data.to_long();
    break;
  case BOX_DATA_DOUBLE:
    return to_double() != data.to_double();
    break;
  case BOX_DATA_STRING:
    if (data.type == BOX_DATA_STRING)
    {
      return strcmp((const char *)mem->address,
                    (const char *)data.get_address()) != 0;
    }
    else
    {
      return strcmp((const char *)mem->address,
                    (const char *)data.get_string()) != 0;
    }
    break;
  case BOX_DATA_INVALID:
  default:
    BOX_ERROR(ERROR_BOX_DATA_INVALID_DATA_TYPE);
    return false;
    break;
  }

  return true;
}

/**
 * Operator >. Check if this value is bigger than another.
 *
 * @param data - the data.
 *
 * @return true if this value is bigger, otherwise return false.
 */
bool
box_data::operator > (box_data &data)
{
  if ((!mem) || (data.get_address() == 0))
  {
    BOX_ERROR(ERROR_BOX_DATA_NULL_DATA);
    return false;
  }

  switch (type)
  {
  case BOX_DATA_CHAR:
    return to_char() > data.to_char();
    break;
  case BOX_DATA_SHORT:
    return to_short() > data.to_short();
    break;
  case BOX_DATA_INT:
    return to_int() > data.to_int();
    break;
  case BOX_DATA_FLOAT:
    return to_float() > data.to_float();
    break;
  case BOX_DATA_LONG:
    return to_long() > data.to_long();
    break;
  case BOX_DATA_DOUBLE:
    return to_double() > data.to_double();
    break;
  case BOX_DATA_STRING:
    if (data.type == BOX_DATA_STRING)
    {
      return strcmp((const char *)mem->address,
                    (const char *)data.get_address()) > 0;
    }
    else
    {
      return strcmp((const char *)mem->address,
                    (const char *)data.get_string()) > 0;
    }
    break;
  case BOX_DATA_INVALID:
  default:
    BOX_ERROR(ERROR_BOX_DATA_INVALID_DATA_TYPE);
    return false;
    break;
  }

  return true;
}

/**
 * Operator <. Check if this value is lesser than another.
 *
 * @param data - the data.
 *
 * @return true if this value is lesser, otherwise return false.
 */
bool
box_data::operator < (box_data &data)
{
  if ((!mem) || (data.get_address() == 0))
  {
    BOX_ERROR(ERROR_BOX_DATA_NULL_DATA);
    return false;
  }

  switch (type)
  {
  case BOX_DATA_CHAR:
    return to_char() < data.to_char();
    break;
  case BOX_DATA_SHORT:
    return to_short() < data.to_short();
    break;
  case BOX_DATA_INT:
    return to_int() < data.to_int();
    break;
  case BOX_DATA_FLOAT:
    return to_float() < data.to_float();
    break;
  case BOX_DATA_LONG:
    return to_long() < data.to_long();
    break;
  case BOX_DATA_DOUBLE:
    return to_double() < data.to_double();
    break;
  case BOX_DATA_STRING:
    if (data.type == BOX_DATA_STRING)
    {
      return strcmp((const char *)mem->address,
                    (const char *)data.get_address()) < 0;
    }
    else
    {
      return strcmp((const char *)mem->address,
                    (const char *)data.get_string()) < 0;
    }
    break;
  case BOX_DATA_INVALID:
  default:
    BOX_ERROR(ERROR_BOX_DATA_INVALID_DATA_TYPE);
    return false;
    break;
  }

  return true;
}

/**
 * Operator >=. Check if this value is bigger or equal than another.
 *
 * @param data - the data.
 *
 * @return true if this value is bigger or equal, otherwise return false.
 */
bool
box_data::operator >= (box_data &data)
{
  if ((!mem) || (data.get_address() == 0))
  {
    BOX_ERROR(ERROR_BOX_DATA_NULL_DATA);
    return false;
  }

  switch (type)
  {
  case BOX_DATA_CHAR:
    return to_char() >= data.to_char();
    break;
  case BOX_DATA_SHORT:
    return to_short() >= data.to_short();
    break;
  case BOX_DATA_INT:
    return to_int() >= data.to_int();
    break;
  case BOX_DATA_FLOAT:
    return to_float() >= data.to_float();
    break;
  case BOX_DATA_LONG:
    return to_long() >= data.to_long();
    break;
  case BOX_DATA_DOUBLE:
    return to_double() >= data.to_double();
    break;
  case BOX_DATA_STRING:
    if (data.type == BOX_DATA_STRING)
    {
      return strcmp((const char *)mem->address,
                    (const char *)data.get_address()) >= 0;
    }
    else
    {
      return strcmp((const char *)mem->address,
                    (const char *)data.get_string()) >= 0;
    }
    break;
  case BOX_DATA_INVALID:
  default:
    BOX_ERROR(ERROR_BOX_DATA_INVALID_DATA_TYPE);
    return false;
    break;
  }

  return true;
}

/**
 * Operator <=. Check if this value is smaller than another.
 *
 * @param data - the data.
 *
 * @return true if this value is smaller, otherwise return false.
 */
bool
box_data::operator <= (box_data &data)
{
  if ((!mem) || (data.get_address() == 0))
  {
    BOX_ERROR(ERROR_BOX_DATA_NULL_DATA);
    return false;
  }

  switch (type)
  {
  case BOX_DATA_CHAR:
    return to_char() <= data.to_char();
    break;
  case BOX_DATA_SHORT:
    return to_short() <= data.to_short();
    break;
  case BOX_DATA_INT:
    return to_int() <= data.to_int();
    break;
  case BOX_DATA_FLOAT:
    return to_float() <= data.to_float();
    break;
  case BOX_DATA_LONG:
    return to_long() <= data.to_long();
    break;
  case BOX_DATA_DOUBLE:
    return to_double() <= data.to_double();
    break;
  case BOX_DATA_STRING:
    if (data.type == BOX_DATA_STRING)
    {
      return strcmp((const char *)mem->address,
                    (const char *)data.get_address()) <= 0;
    }
    else
    {
      return strcmp((const char *)mem->address,
                    (const char *)data.get_string()) <= 0;
    }
    break;
  case BOX_DATA_INVALID:
  default:
    BOX_ERROR(ERROR_BOX_DATA_INVALID_DATA_TYPE);
    return false;
    break;
  }

  return true;
}

/**
 * Print value on console.
 *
 * @return true if success, otherwise return false.
 */
bool
box_data::print()
{
  if (!mem)
  {
    BOX_ERROR(ERROR_BOX_DATA_NULL_DATA);
    return false;
  }

  switch (type)
  {
  case BOX_DATA_CHAR:
    printf(BOX_DATA_TYPE_FORMAT[type],
        to_char());
    break;
  case BOX_DATA_SHORT:
    printf(BOX_DATA_TYPE_FORMAT[type],
        to_short());
    break;
  case BOX_DATA_INT:
    printf(BOX_DATA_TYPE_FORMAT[type],
        to_int());
    break;
  case BOX_DATA_FLOAT:
    printf(BOX_DATA_TYPE_FORMAT[type],
        to_float());
    break;
  case BOX_DATA_LONG:
    printf(BOX_DATA_TYPE_FORMAT[type],
        to_long());
    break;
  case BOX_DATA_DOUBLE:
    printf(BOX_DATA_TYPE_FORMAT[type],
        to_double());
    break;
  case BOX_DATA_STRING:
    printf(BOX_DATA_TYPE_FORMAT[type],
        (char *)mem->address);
    break;
  case BOX_DATA_INVALID:
  default:
    BOX_ERROR(ERROR_BOX_DATA_INVALID_DATA_TYPE);
    return false;
    break;
  }

  return true;
}

/**
 * Print value on screen together with new line.
 *
 * @return true if success, otherwise return false.
 */
bool box_data::println()
{
  if (print())
  {
    printf("\r\n");
    return true;
  }

  return false;
}

/**
 * Insert value from console terminal into data.
 *
 * @return true if success, otherwise return false.
 */
bool
box_data::scan()
{
  if (!mem)
  {
    BOX_ERROR(ERROR_BOX_DATA_NULL_DATA);
    return false;
  }

  switch (type)
  {
  case BOX_DATA_CHAR:
    scanf(BOX_DATA_TYPE_FORMAT[type],
        (int8_t *)mem->address);
    break;
  case BOX_DATA_SHORT:
    scanf(BOX_DATA_TYPE_FORMAT[type],
        (int16_t *)mem->address);
    break;
  case BOX_DATA_INT:
    scanf(BOX_DATA_TYPE_FORMAT[type],
        (int32_t *)mem->address);
    break;
  case BOX_DATA_FLOAT:
    scanf(BOX_DATA_TYPE_FORMAT[type],
        (float32_t *)mem->address);
    break;
  case BOX_DATA_LONG:
    scanf(BOX_DATA_TYPE_FORMAT[type],
        (int64_t *)mem->address);
    break;
  case BOX_DATA_DOUBLE:
    scanf(BOX_DATA_TYPE_FORMAT[type],
        (float64_t *)mem->address);
    break;
  case BOX_DATA_STRING:
    if (scanf(BOX_DATA_TYPE_FORMAT[type],
              (int8_t *)mem->address))
    {
      return false;
    }
    break;
  case BOX_DATA_INVALID:
  default:
    BOX_ERROR(ERROR_BOX_DATA_INVALID_DATA_TYPE);
    return false;
    break;
  }

  return true;
}

const char *
box_data::get_string()
{
  static char buffer[32768];

  buffer[0] = 0;

  if (!mem)
  {
    BOX_ERROR(ERROR_BOX_DATA_NULL_DATA);
    return buffer;
  }

  switch (type)
  {
  case BOX_DATA_CHAR:
    sprintf(buffer,
            BOX_DATA_TYPE_FORMAT[type],
            this->to_char());
    break;
  case BOX_DATA_SHORT:
    sprintf(buffer,
            BOX_DATA_TYPE_FORMAT[type],
            this->to_short());
    break;
  case BOX_DATA_LONG:
    sprintf(buffer,
            BOX_DATA_TYPE_FORMAT[type],
            this->to_long());
    break;
  case BOX_DATA_INT:
    sprintf(buffer,
            BOX_DATA_TYPE_FORMAT[type],
            this->to_int());
    break;
  case BOX_DATA_FLOAT:
    sprintf(buffer,
            BOX_DATA_TYPE_FORMAT[type],
            this->to_float());
    break;
  case BOX_DATA_DOUBLE:
    sprintf(buffer,
            BOX_DATA_TYPE_FORMAT[type],
            this->to_double());
    break;
  case BOX_DATA_STRING:
    sprintf(buffer,
            BOX_DATA_TYPE_FORMAT[type],
            (const char *)mem->address);
    break;
  case BOX_DATA_INVALID:
  default:
    BOX_ERROR(ERROR_BOX_DATA_INVALID_DATA_TYPE);
    break;
  }

  return buffer;
}

/**
 * The destructor.
 */
box_data::~box_data()
{
  if (!mem)
  {
    return;
  }

  mem->decrease_ref();

  if (mem->ready_to_remove())
  {
    virtual_memory.free(mem);
  }
}
