#include "box_data.h"
#include "box_virtual_memory.h"
#include "memory.h"
#include "ORM/relationship.h"
#include "ORM/orm.h"
#include "box_monitor.h"
#include <iostream>

/**
 * The constructor.
 *
 * @param type - data type.
 */
box_data::box_data(std::string id, box_data_type type, const void *value) : entity::entity("box_data", id)
{
  this->add_relationship("box_data_memory", ONE_TO_MANY);
  this->vm = (box_virtual_memory *) orm::get_first("box_virtual_memory");
  this->type = type;

  if (type >= BOX_DATA_INVALID)
  {
    BOX_ERROR(ERROR_BOX_DATA_INVALID_DATA_TYPE);
    return;
  }

  if (value == nullptr)
  {
    memory *mem = this->vm->alloc(BOX_DATA_TYPE_SIZE[type]);

    if (!mem)
    {
      BOX_ERROR(ERROR_BOX_DATA_NULL_DATA);
      return;
    }

    entity::add_entity("box_data_memory", (entity *) mem);
    mem->add_entity("box_data_memory", (entity *) this);

    default_value();
  }
  else
  {
    size_t size = (type == BOX_DATA_STRING) ?
                  strlen((const char *) value) + 1 :
                  BOX_DATA_TYPE_SIZE[type];

    memory *mem = vm->alloc(size);

    if (!mem)
    {
      BOX_ERROR(ERROR_BOX_DATA_NULL_DATA);
      return;
    }

    entity::add_entity("box_data_memory", (entity *) mem);
    mem->add_entity("box_data_memory", (entity *) this);

    memcpy(mem->get_pointer<void *>(), value, size);
  }
}

/**
 * The copy constructor.
 *
 * @param data - box data.
 */
box_data::box_data(std::string id, box_data &data) : entity::entity("box_data", id)
{
  this->add_relationship("box_data_memory", ONE_TO_MANY);

  memory *data_mem = data.get_memory();

  if (!data_mem)
  {
    BOX_ERROR(ERROR_BOX_DATA_NULL_DATA);
    return;
  }

  if (data.type >= BOX_DATA_INVALID)
  {
    BOX_ERROR(ERROR_BOX_DATA_INVALID_DATA_TYPE);
    return;
  }

  this->type = data.type;
  this->vm = data.vm;

  memory *mem = this->vm->alloc(data_mem->get_size());
  memcpy(mem->get_pointer<void *>(),
         data_mem->get_pointer<void *>(),
         data_mem->get_size());

  this->add_entity("box_data_memory", (entity *) mem);
  mem->add_entity("box_data_memory", (entity *) this);
}

/**
 * @brief box_data::create
 * @param id
 * @param type
 * @param value
 * @return
 */
box_data *
box_data::create(std::string id,
                 box_data_type type,
                 const void *value)
{
  return (box_data *) orm::create((entity *) new box_data(id, type, value));
}

/**
 * @brief box_data::create
 * @param id
 * @param data
 * @return
 */
box_data *
box_data::create(std::string id, box_data &data)
{
  return (box_data *) orm::create((entity *) new box_data(id, data));
}

/**
 * Convert data to bool and return.
 *
 * @return bool value
 */
bool
box_data::to_bool()
{
  bool value = false;
  memory *mem = this->get_memory();

  if (!mem)
  {
    BOX_ERROR(ERROR_BOX_DATA_NULL_DATA);
    return value;
  }

  switch (type)
  {
    case BOX_DATA_BOOL:
      value = mem->get_element<bool>();
      break;
    case BOX_DATA_STRING:
    {
      value = (get_string() != "");
      break;
    }
    case BOX_DATA_CHAR:
      value = (bool) mem->get_element<int8_t>();
      break;
    case BOX_DATA_SHORT:
      value = (bool) mem->get_element<int16_t>();
      break;
    case BOX_DATA_INT:
      value = (bool) mem->get_element<int32_t>();
      break;
    case BOX_DATA_FLOAT:
      value = (bool) mem->get_element<float32_t>();
      break;
    case BOX_DATA_LONG:
      value = (bool) mem->get_element<int64_t>();
      break;
    case BOX_DATA_DOUBLE:
      value = (bool) mem->get_element<float64_t>();
      break;
    case BOX_DATA_INVALID:
    default:
      BOX_ERROR(ERROR_BOX_DATA_INVALID_DATA_TYPE);
      break;
  }

  return value;
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
  memory *mem = this->get_memory();

  if (!mem)
  {
    BOX_ERROR(ERROR_BOX_DATA_NULL_DATA);
    return value;
  }

  switch (type)
  {
    case BOX_DATA_BOOL:
      value = (int8_t) mem->get_element<bool>();
      break;
    case BOX_DATA_STRING:
    case BOX_DATA_CHAR:
      value = mem->get_element<int8_t>();
      break;
    case BOX_DATA_SHORT:
      value = (int8_t) mem->get_element<int16_t>();
      break;
    case BOX_DATA_INT:
      value = (int8_t) mem->get_element<int32_t>();
      break;
    case BOX_DATA_FLOAT:
      value = (int8_t) mem->get_element<float32_t>();
      break;
    case BOX_DATA_LONG:
      value = (int8_t) mem->get_element<int64_t>();
      break;
    case BOX_DATA_DOUBLE:
      value = (int8_t) mem->get_element<float64_t>();
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
  memory *mem = this->get_memory();

  if (!mem)
  {
    BOX_ERROR(ERROR_BOX_DATA_NULL_DATA);
    return value;
  }

  switch (type)
  {
    case BOX_DATA_BOOL:
      value = (int16_t) mem->get_element<bool>();
      break;
    case BOX_DATA_CHAR:
      value = (int16_t) mem->get_element<int8_t>();
      break;
    case BOX_DATA_SHORT:
      value = mem->get_element<int16_t>();
      break;
    case BOX_DATA_INT:
      value = (int16_t) mem->get_element<int32_t>();
      break;
    case BOX_DATA_FLOAT:
      value = (int16_t) mem->get_element<float32_t>();
      break;
    case BOX_DATA_LONG:
      value = (int16_t) mem->get_element<int64_t>();
      break;
    case BOX_DATA_DOUBLE:
      value = (int16_t) mem->get_element<float64_t>();
      break;
    case BOX_DATA_STRING:
      sscanf(mem->get_pointer<const char *>(),
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
  memory *mem = this->get_memory();

  if (!mem)
  {
    BOX_ERROR(ERROR_BOX_DATA_NULL_DATA);
    return value;
  }

  switch (type)
  {
    case BOX_DATA_BOOL:
      value = (int32_t) mem->get_element<bool>();
      break;
    case BOX_DATA_CHAR:
      value = (int32_t) mem->get_element<int8_t>();;
      break;
    case BOX_DATA_SHORT:
      value = (int32_t) mem->get_element<int16_t>();
      break;
    case BOX_DATA_INT:
      value = mem->get_element<int32_t>();
      break;
    case BOX_DATA_FLOAT:
      value = (int32_t) mem->get_element<float32_t>();;
      break;
    case BOX_DATA_LONG:
      value = (int32_t) mem->get_element<int64_t>();
      break;
    case BOX_DATA_DOUBLE:
      value = (int32_t) mem->get_element<float64_t>();;
      break;
    case BOX_DATA_STRING:
      sscanf(mem->get_pointer<const char *>(),
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
  memory *mem = this->get_memory();

  if (!mem)
  {
    BOX_ERROR(ERROR_BOX_DATA_NULL_DATA);
    return value;
  }

  switch (type)
  {
    case BOX_DATA_BOOL:
      value = (float32_t) mem->get_element<bool>();
      break;
    case BOX_DATA_CHAR:
      value = (float32_t) mem->get_element<int8_t>();
      break;
    case BOX_DATA_SHORT:
      value = (float32_t) mem->get_element<int16_t>();
      break;
    case BOX_DATA_INT:
      value = (float32_t) mem->get_element<int32_t>();
      break;
    case BOX_DATA_FLOAT:
      value = mem->get_element<float32_t>();
      break;
    case BOX_DATA_LONG:
      value = (float32_t) mem->get_element<int64_t>();
      break;
    case BOX_DATA_DOUBLE:
      value = (float32_t) mem->get_element<float64_t>();
      break;
    case BOX_DATA_STRING:
      sscanf(mem->get_pointer<const char *>(),
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
  memory *mem = this->get_memory();

  if (!mem)
  {
    BOX_ERROR(ERROR_BOX_DATA_NULL_DATA);
    return value;
  }

  switch (type)
  {
    case BOX_DATA_BOOL:
      value = (int64_t) mem->get_element<bool>();
      break;
    case BOX_DATA_CHAR:
      value = (int64_t) mem->get_element<int8_t>();
      break;
    case BOX_DATA_SHORT:
      value = (int64_t) mem->get_element<int16_t>();
      break;
    case BOX_DATA_INT:
      value = (int64_t) mem->get_element<int64_t>();
      break;
    case BOX_DATA_FLOAT:
      value = (int64_t) mem->get_element<float32_t>();
      break;
    case BOX_DATA_LONG:
      value = mem->get_element<int64_t>();
      break;
    case BOX_DATA_DOUBLE:
      value = (int64_t) mem->get_element<float64_t>();
      break;
    case BOX_DATA_STRING:
      sscanf(mem->get_pointer<const char *>(),
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
  memory *mem = this->get_memory();

  if (!mem)
  {
    BOX_ERROR(ERROR_BOX_DATA_NULL_DATA);
    return value;
  }

  switch (type)
  {
    case BOX_DATA_BOOL:
      value = (float64_t) mem->get_element<bool>();
      break;
    case BOX_DATA_CHAR:
      value = (float64_t) mem->get_element<int8_t>();
      break;
    case BOX_DATA_SHORT:
      value = (float64_t) mem->get_element<int16_t>();
      break;
    case BOX_DATA_INT:
      value = (float64_t) mem->get_element<int32_t>();
      break;
    case BOX_DATA_FLOAT:
      value = (float64_t) mem->get_element<float32_t>();
      break;
    case BOX_DATA_LONG:
      value = (float64_t) mem->get_element<int64_t>();
      break;
    case BOX_DATA_DOUBLE:
      value = mem->get_element<float64_t>();
      break;
    case BOX_DATA_STRING:
      sscanf(mem->get_pointer<const char *>(),
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
box_data &
box_data::to_string()
{
  std::string string = this->get_string();
  box_data &str = *(box_data *) orm::create(
    (entity *) new box_data(this->id.append(" as string"),
                            BOX_DATA_STRING,
                            string.c_str())
  );
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
  memory *mem = this->get_memory();
  return (mem != nullptr) ? (uintptr_t) mem->get_pointer<void *>() : 0;
}

/**
 * Get memory information.
 *
 * @return memory information.
 */
memory *
box_data::get_memory()
{
  return (memory *) this->get_relationship("box_data_memory")->front();
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
  memory *mem = this->get_memory();

  if (!mem)
  {
    BOX_ERROR(ERROR_BOX_DATA_NULL_DATA);
    return false;
  }

  switch (type)
  {
    case BOX_DATA_BOOL:
    case BOX_DATA_CHAR:
    case BOX_DATA_SHORT:
    case BOX_DATA_INT:
    case BOX_DATA_LONG:
    case BOX_DATA_STRING:
      memset(mem->get_pointer<void *>(), 0, mem->get_size());
      break;
    case BOX_DATA_FLOAT:
      mem->get_element<float32_t>() = 0.0f;
      break;
    case BOX_DATA_DOUBLE:
      mem->get_element<float64_t>() = 0.0;
      break;
    case BOX_DATA_INVALID:
    default:
      BOX_ERROR(ERROR_BOX_DATA_INVALID_DATA_TYPE);
      return false;
      break;
  }

  return true;
}

void
box_data::convert_itself(box_data_type new_type)
{
  memory *mem = this->get_memory();

  if (!mem)
  {
    BOX_ERROR(ERROR_BOX_DATA_NULL_DATA);
    return;
  }

  memory *new_mem;

  switch (new_type)
  {
    case BOX_DATA_BOOL:
    {
      new_mem = this->vm->alloc(BOX_DATA_TYPE_SIZE[new_type]);
      new_mem->get_element<bool>() = this->to_bool();
      break;
    }
    case BOX_DATA_CHAR:
    {
      new_mem = this->vm->alloc(BOX_DATA_TYPE_SIZE[new_type]);
      new_mem->get_element<int8_t>() = this->to_char();
      break;
    }
    case BOX_DATA_SHORT:
    {
      new_mem = this->vm->alloc(BOX_DATA_TYPE_SIZE[new_type]);
      new_mem->get_element<int16_t>() = this->to_short();
      break;
    }
    case BOX_DATA_INT:
    {
      new_mem = this->vm->alloc(BOX_DATA_TYPE_SIZE[new_type]);
      new_mem->get_element<int32_t>() = this->to_int();
      break;
    }
    case BOX_DATA_LONG:
    {
      new_mem = this->vm->alloc(BOX_DATA_TYPE_SIZE[new_type]);
      new_mem->get_element<int64_t>() = this->to_long();
      break;
    }
    case BOX_DATA_STRING:
    {
      std::string str = this->get_string();
      new_mem = this->vm->alloc(str.size() + 1);
      strncpy(new_mem->get_pointer<char *>(),
              str.c_str(),
              new_mem->get_size());
      break;
    }
    case BOX_DATA_FLOAT:
    {
      new_mem = this->vm->alloc(BOX_DATA_TYPE_SIZE[new_type]);
      new_mem->get_element<float32_t>() = this->to_float();
      break;
    }
    case BOX_DATA_DOUBLE:
    {
      new_mem = this->vm->alloc(BOX_DATA_TYPE_SIZE[new_type]);
      new_mem->get_element<float64_t>() = this->to_double();
      break;
    }
    case BOX_DATA_INVALID:
    default:
      BOX_ERROR(ERROR_BOX_DATA_INVALID_DATA_TYPE);
      return;
      break;
  }

  entity::add_entity("box_data_memory", (entity *) new_mem);
  new_mem->add_entity("box_data_memory", (entity *) this);

  entity::remove_entity("box_data_memory", (entity *) mem);
  mem->remove_entity("box_data_memory", (entity *) this);
  this->vm->free(mem);

  this->type = new_type;
}

/**
 * Operator =. Assign value from memory address.
 *
 * @param data - the data memory address.
 *
 * @return true if success, otherwise return false.
 */
bool
box_data::operator=(const void *data)
{
  memory *mem = this->get_memory();

  if ((!mem) || (!data))
  {
    BOX_ERROR(ERROR_BOX_DATA_NULL_DATA);
    return false;
  }

  switch (type)
  {
    case BOX_DATA_BOOL:
      mem->get_element<bool>() = *(bool *) data;
      break;
    case BOX_DATA_CHAR:
      mem->get_element<int8_t>() = *(int8_t *) data;
      break;
    case BOX_DATA_SHORT:
      mem->get_element<int16_t>() = *(int16_t *) data;
      break;
    case BOX_DATA_INT:
      mem->get_element<int32_t>() = *(int32_t *) data;
      break;
    case BOX_DATA_FLOAT:
      mem->get_element<float32_t>() = *(float32_t *) data;
      break;
    case BOX_DATA_LONG:
      mem->get_element<int64_t>() = *(int64_t *) data;
      break;
    case BOX_DATA_DOUBLE:
      mem->get_element<float64_t>() = *(float64_t *) data;
      break;
    case BOX_DATA_STRING:
    {
      const char *str_temp = (const char *) data;
      uint32_t str_size = strlen(str_temp) + 1;

      if (mem->get_size() < str_size)
      {
        memory *new_mem = this->vm->alloc(str_size);
        entity::add_entity("box_data_memory", (entity *) new_mem);
        new_mem->add_entity("box_data_memory", (entity *) this);

        entity::remove_entity("box_data_memory", (entity *) mem);
        mem->remove_entity("box_data_memory", (entity *) this);

        this->vm->free(mem);
        mem = new_mem;
      }

      strncpy(mem->get_pointer<char *>(), str_temp, str_size);
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
box_data::operator=(box_data &data)
{
  memory *mem = this->get_memory();

  if ((!mem) || (data.get_address() == 0))
  {
    BOX_ERROR(ERROR_BOX_DATA_NULL_DATA);
    return false;
  }

  if (type == BOX_DATA_STRING)
  {
    mem->get_element<int8_t>() = 0;
    return (*this) += (data);
  }

  if (data.type == BOX_DATA_STRING)
  {
    BOX_ERROR(ERROR_BOX_DATA_ASSIGNING_STRING);
    return false;
  }

  switch (type)
  {
    case BOX_DATA_BOOL:
      mem->get_element<bool>() = data.to_bool();
      break;
    case BOX_DATA_CHAR:
      mem->get_element<int8_t>() = data.to_char();
      break;
    case BOX_DATA_SHORT:
      mem->get_element<int16_t>() = data.to_short();
      break;
    case BOX_DATA_INT:
      mem->get_element<int32_t>() = data.to_int();
      break;
    case BOX_DATA_FLOAT:
      mem->get_element<float32_t>() = data.to_float();
      break;
    case BOX_DATA_LONG:
      mem->get_element<int64_t>() = data.to_long();
      break;
    case BOX_DATA_DOUBLE:
      mem->get_element<float64_t>() = data.to_double();
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
 * Operator &=.
 *
 * @param data - the data.
 *
 * @return true if success, otherwise return false.
 */
bool
box_data::operator&=(box_data &data)
{
  memory *mem = this->get_memory();
  if ((!mem) || (data.get_address() == 0))
  {
    BOX_ERROR(ERROR_BOX_DATA_NULL_DATA);
    return false;
  }

  if (data.type == BOX_DATA_STRING)
  {
    BOX_ERROR(ERROR_BOX_DATA_LOGICAL_OPERATION_STRING);
    return false;
  }

  switch (type)
  {
    case BOX_DATA_BOOL:
      mem->get_element<bool>() &= data.to_bool();
      break;
    case BOX_DATA_CHAR:
      mem->get_element<int8_t>() &= data.to_char();
      break;
    case BOX_DATA_SHORT:
      mem->get_element<int16_t>() &= data.to_short();
      break;
    case BOX_DATA_INT:
      mem->get_element<int32_t>() &= data.to_int();
      break;
    case BOX_DATA_LONG:
      mem->get_element<int32_t>() &= data.to_long();
      break;
    case BOX_DATA_DOUBLE:
    case BOX_DATA_FLOAT:
      BOX_ERROR(ERROR_BOX_DATA_LOGICAL_OPERATION_FLOAT);
      break;
    case BOX_DATA_STRING:
      BOX_ERROR(ERROR_BOX_DATA_LOGICAL_OPERATION_STRING);
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
 * Operator |=.
 *
 * @param data - the data.
 *
 * @return true if success, otherwise return false.
 */
bool
box_data::operator|=(box_data &data)
{
  memory *mem = this->get_memory();
  if ((!mem) || (data.get_address() == 0))
  {
    BOX_ERROR(ERROR_BOX_DATA_NULL_DATA);
    return false;
  }

  if (data.type == BOX_DATA_STRING)
  {
    BOX_ERROR(ERROR_BOX_DATA_LOGICAL_OPERATION_STRING);
    return false;
  }

  switch (type)
  {
    case BOX_DATA_BOOL:
      mem->get_element<bool>() |= data.to_bool();
      break;
    case BOX_DATA_CHAR:
      mem->get_element<int8_t>() |= data.to_char();
      break;
    case BOX_DATA_SHORT:
      mem->get_element<int16_t>() |= data.to_short();
      break;
    case BOX_DATA_INT:
      mem->get_element<int32_t>() |= data.to_int();
      break;
    case BOX_DATA_LONG:
      mem->get_element<int32_t>() |= data.to_long();
      break;
    case BOX_DATA_DOUBLE:
    case BOX_DATA_FLOAT:
      BOX_ERROR(ERROR_BOX_DATA_LOGICAL_OPERATION_FLOAT);
      break;
    case BOX_DATA_STRING:
      BOX_ERROR(ERROR_BOX_DATA_LOGICAL_OPERATION_STRING);
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
 * Operator ^=
 *
 * @param data - the data.
 *
 * @return true if success, otherwise return false.
 */
bool
box_data::operator^=(box_data &data)
{
  memory *mem = this->get_memory();
  if ((!mem) || (data.get_address() == 0))
  {
    BOX_ERROR(ERROR_BOX_DATA_NULL_DATA);
    return false;
  }

  if (data.type == BOX_DATA_STRING)
  {
    BOX_ERROR(ERROR_BOX_DATA_LOGICAL_OPERATION_STRING);
    return false;
  }

  switch (type)
  {
    case BOX_DATA_BOOL:
      mem->get_element<bool>() ^= data.to_bool();
      break;
    case BOX_DATA_CHAR:
      mem->get_element<int8_t>() ^= data.to_char();
      break;
    case BOX_DATA_SHORT:
      mem->get_element<int16_t>() ^= data.to_short();
      break;
    case BOX_DATA_INT:
      mem->get_element<int32_t>() ^= data.to_int();
      break;
    case BOX_DATA_LONG:
      mem->get_element<int32_t>() ^= data.to_long();
      break;
    case BOX_DATA_DOUBLE:
    case BOX_DATA_FLOAT:
      BOX_ERROR(ERROR_BOX_DATA_LOGICAL_OPERATION_FLOAT);
      break;
    case BOX_DATA_STRING:
      BOX_ERROR(ERROR_BOX_DATA_LOGICAL_OPERATION_STRING);
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
 * Operator +=. Do operation add if primitive data type.
 * On string data type append data.
 *
 * @param data - the data.
 *
 * @return true if success, otherwise return false.
 */
bool
box_data::operator+=(box_data &data)
{
  memory *mem = this->get_memory();

  if ((!mem) || (data.get_address() == 0))
  {
    BOX_ERROR(ERROR_BOX_DATA_NULL_DATA);
    return false;
  }

  if (this->type == BOX_DATA_STRING)
  {
    std::string string = data.get_string();
    uint32_t request_size = strlen(mem->get_pointer<const char *>()) +
                            strlen(string.c_str()) + 1;

    if (mem->get_size() < request_size)
    {
      memory *new_mem = vm->realloc(mem, request_size);

      if (new_mem->get_size() != request_size)
      {
        /* Something really bad happened. */
        return false;
      }

      if (new_mem != mem)
      {
        /*
         * new_mem is different than mem,
         * switch relations.
         */
        entity::add_entity("box_data_memory", (entity *) new_mem);
        new_mem->add_entity("box_data_memory", (entity *) this);

        entity::remove_entity("box_data_memory", (entity *) mem);
        mem->remove_entity("box_data_memory", (entity *) this);

        mem = new_mem;
      }
    }

    strcat((char *) mem->get_address(), string.c_str());
    return true;
  }

  if (data.type == BOX_DATA_STRING)
  {
    BOX_ERROR(ERROR_BOX_DATA_ADDING_STRING);
    return false;
  }

  switch (this->type)
  {
    case BOX_DATA_BOOL:
      /* Doesn't make any sense to do true += false */
      break;
    case BOX_DATA_CHAR:
      mem->get_element<int8_t>() += data.to_char();
      break;
    case BOX_DATA_SHORT:
      mem->get_element<int16_t>() += data.to_short();
      break;
    case BOX_DATA_INT:
      mem->get_element<int32_t>() += data.to_int();
      break;
    case BOX_DATA_FLOAT:
      mem->get_element<float32_t>() += data.to_float();
      break;
    case BOX_DATA_LONG:
      mem->get_element<int32_t>() += data.to_long();
      break;
    case BOX_DATA_DOUBLE:
      mem->get_element<float64_t>() += data.to_double();
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
box_data::operator-=(box_data &data)
{
  memory *mem = this->get_memory();
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
    case BOX_DATA_BOOL:
      /* Doesn't make any sense to do true -= false */
      break;
    case BOX_DATA_CHAR:
      mem->get_element<int8_t>() -= data.to_char();
      break;
    case BOX_DATA_SHORT:
      mem->get_element<int16_t>() -= data.to_short();
      break;
    case BOX_DATA_INT:
      mem->get_element<int32_t>() -= data.to_int();
      break;
    case BOX_DATA_FLOAT:
      mem->get_element<float32_t>() -= data.to_float();
      break;
    case BOX_DATA_LONG:
      mem->get_element<int32_t>() -= data.to_long();
      break;
    case BOX_DATA_DOUBLE:
      mem->get_element<float64_t>() -= data.to_double();
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
box_data::operator*=(box_data &data)
{
  memory *mem = this->get_memory();
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
    case BOX_DATA_BOOL:
      /* Doesn't make any sense to do true *= false */
      break;
    case BOX_DATA_CHAR:
      mem->get_element<int8_t>() *= data.to_char();
      break;
    case BOX_DATA_SHORT:
      mem->get_element<int16_t>() *= data.to_short();
      break;
    case BOX_DATA_INT:
      mem->get_element<int32_t>() *= data.to_int();
      break;
    case BOX_DATA_FLOAT:
      mem->get_element<float32_t>() *= data.to_float();
      break;
    case BOX_DATA_LONG:
      mem->get_element<int32_t>() *= data.to_long();
      break;
    case BOX_DATA_DOUBLE:
      mem->get_element<float64_t>() *= data.to_double();
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
box_data::operator/=(box_data &data)
{
  memory *mem = this->get_memory();
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
    case BOX_DATA_BOOL:
      /* Doesn't make any sense to do true /= false */
      break;
    case BOX_DATA_CHAR:

      if (data.to_char() == 0)
      {
        BOX_ERROR(ERROR_BOX_DATA_DIVIDING_ZERO);
        return false;
      }

      mem->get_element<int8_t>() /= data.to_char();
      break;
    case BOX_DATA_SHORT:

      if (data.to_short() == 0)
      {
        BOX_ERROR(ERROR_BOX_DATA_DIVIDING_ZERO);
        return false;
      }

      mem->get_element<int16_t>() /= data.to_short();
      break;
    case BOX_DATA_INT:

      if (data.to_int() == 0)
      {
        BOX_ERROR(ERROR_BOX_DATA_DIVIDING_ZERO);
        return false;
      }

      mem->get_element<int32_t>() /= data.to_int();
      break;
    case BOX_DATA_FLOAT:

      if (data.to_float() == 0.0f)
      {
        BOX_ERROR(ERROR_BOX_DATA_DIVIDING_ZERO);
        return false;
      }

      mem->get_element<float32_t>() /= data.to_float();
      break;
    case BOX_DATA_LONG:

      if (data.to_long() == 0)
      {
        BOX_ERROR(ERROR_BOX_DATA_DIVIDING_ZERO);
        return false;
      }

      mem->get_element<int32_t>() /= data.to_long();
      break;
    case BOX_DATA_DOUBLE:

      if (data.to_double() == 0.0)
      {
        BOX_ERROR(ERROR_BOX_DATA_DIVIDING_ZERO);
        return false;
      }

      mem->get_element<float64_t>() /= data.to_double();
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
box_data::operator%=(box_data &data)
{
  memory *mem = this->get_memory();
  if ((!mem) || (data.get_address() == 0))
  {
    BOX_ERROR(ERROR_BOX_DATA_NULL_DATA);
    return false;
  }

  if ((data.type == BOX_DATA_FLOAT) ||
      (data.type == BOX_DATA_DOUBLE) ||
      (data.type == BOX_DATA_STRING) ||
      (data.type == BOX_DATA_BOOL))
  {
    BOX_ERROR(ERROR_BOX_DATA_INVALID_MODULUS);
    return false;
  }

  switch (type)
  {
    case BOX_DATA_BOOL:
      /* Doesn't make any sense to do true %= false */
      break;
    case BOX_DATA_CHAR:
      if (data.to_char() == 0)
      {
        BOX_ERROR(ERROR_BOX_DATA_MODULUS_ZERO);
        return false;
      }

      mem->get_element<int8_t>() %= data.to_char();
      break;
    case BOX_DATA_SHORT:
      if (data.to_short() == 0)
      {
        BOX_ERROR(ERROR_BOX_DATA_MODULUS_ZERO);
        return false;
      }

      mem->get_element<int16_t>() %= data.to_short();
      break;
    case BOX_DATA_INT:

      if (data.to_int() == 0)
      {
        BOX_ERROR(ERROR_BOX_DATA_MODULUS_ZERO);
        return false;
      }

      mem->get_element<int32_t>() %= data.to_int();
      break;
    case BOX_DATA_LONG:

      if (data.to_long() == 0)
      {
        BOX_ERROR(ERROR_BOX_DATA_MODULUS_ZERO);
        return false;
      }

      mem->get_element<int32_t>() %= data.to_long();
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
box_data::operator++()
{
  memory *mem = this->get_memory();
  if (!mem)
  {
    BOX_ERROR(ERROR_BOX_DATA_NULL_DATA);
    return false;
  }

  switch (type)
  {
    case BOX_DATA_BOOL:
      /* Doesn't make any sense to do true++ */
      break;
    case BOX_DATA_CHAR:
      mem->get_element<int8_t>()++;
      break;
    case BOX_DATA_SHORT:
      mem->get_element<int16_t>()++;
      break;
    case BOX_DATA_INT:
      mem->get_element<int32_t>()++;
      break;
    case BOX_DATA_FLOAT:
      mem->get_element<float32_t>() += 1.0f;
      break;
    case BOX_DATA_LONG:
      mem->get_element<int32_t>()++;
      break;
    case BOX_DATA_DOUBLE:
      mem->get_element<float64_t>() += 1.0;
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
box_data::operator--()
{
  memory *mem = this->get_memory();
  if (!mem)
  {
    BOX_ERROR(ERROR_BOX_DATA_NULL_DATA);
    return false;
  }

  switch (type)
  {
    case BOX_DATA_BOOL:
      /* Doesn't make any sense to do false-- */
      break;
    case BOX_DATA_CHAR:
      mem->get_element<int8_t>()--;
      break;
    case BOX_DATA_SHORT:
      mem->get_element<int16_t>()--;
      break;
    case BOX_DATA_INT:
      mem->get_element<int32_t>()--;
      break;
    case BOX_DATA_FLOAT:
      mem->get_element<float32_t>() -= 1.0f;
      break;
    case BOX_DATA_LONG:
      mem->get_element<int32_t>()--;
      break;
    case BOX_DATA_DOUBLE:
      mem->get_element<float64_t>() -= 1.0;
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
box_data::operator==(box_data &data)
{
  memory *mem = this->get_memory();
  if ((!mem) || (data.get_address() == 0))
  {
    BOX_ERROR(ERROR_BOX_DATA_NULL_DATA);
    return false;
  }

  switch (type)
  {
    case BOX_DATA_BOOL:
      return to_bool() == data.to_bool();
      break;
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
        return strcmp(mem->get_pointer<const char *>(),
                      (const char *) data.get_address()) == 0;
      }
      else
      {
        std::string string = data.get_string();
        return string.compare(mem->get_pointer<const char *>()) == 0;
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
box_data::operator!=(box_data &data)
{
  memory *mem = this->get_memory();
  if ((!mem) || (data.get_address() == 0))
  {
    BOX_ERROR(ERROR_BOX_DATA_NULL_DATA);
    return false;
  }

  switch (type)
  {
    case BOX_DATA_BOOL:
      return to_bool() != data.to_bool();
      break;
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
        return strcmp(mem->get_pointer<const char *>(),
                      (const char *) data.get_address()) != 0;
      }
      else
      {
        std::string string = data.get_string();
        return string.compare(mem->get_pointer<const char *>()) != 0;
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
box_data::operator>(box_data &data)
{
  memory *mem = this->get_memory();
  if ((!mem) || (data.get_address() == 0))
  {
    BOX_ERROR(ERROR_BOX_DATA_NULL_DATA);
    return false;
  }

  switch (type)
  {
    case BOX_DATA_BOOL:
      /* doesn't make any sense "true > false" */
      break;
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
        return strcmp(mem->get_pointer<const char *>(),
                      (const char *) data.get_address()) > 0;
      }
      else
      {
        std::string string = data.get_string();
        return string.compare(mem->get_pointer<const char *>()) < 0;
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
box_data::operator<(box_data &data)
{
  memory *mem = this->get_memory();
  if ((!mem) || (data.get_address() == 0))
  {
    BOX_ERROR(ERROR_BOX_DATA_NULL_DATA);
    return false;
  }

  switch (type)
  {
    case BOX_DATA_BOOL:
      /* doesn't make any sense "true < false" */
      break;
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
        return strcmp(mem->get_pointer<const char *>(),
                      (const char *) data.get_address()) < 0;
      }
      else
      {
        std::string string = data.get_string();
        return string.compare(mem->get_pointer<const char *>()) > 0;
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
box_data::operator>=(box_data &data)
{
  memory *mem = this->get_memory();
  if ((!mem) || (data.get_address() == 0))
  {
    BOX_ERROR(ERROR_BOX_DATA_NULL_DATA);
    return false;
  }

  switch (type)
  {
    case BOX_DATA_BOOL:
      /* doesn't make any sense "true >= false" */
      break;
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
        return strcmp(mem->get_pointer<const char *>(),
                      (const char *) data.get_address()) >= 0;
      }
      else
      {
        std::string string = data.get_string();
        return string.compare(mem->get_pointer<const char *>()) <= 0;
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
box_data::operator<=(box_data &data)
{
  memory *mem = this->get_memory();
  if ((!mem) || (data.get_address() == 0))
  {
    BOX_ERROR(ERROR_BOX_DATA_NULL_DATA);
    return false;
  }

  switch (type)
  {
    case BOX_DATA_BOOL:
      /* doesn't make any sense "true <= false" */
      break;
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
        return strcmp(mem->get_pointer<const char *>(),
                      (const char *) data.get_address()) <= 0;
      }
      else
      {
        std::string string = data.get_string();
        return string.compare(mem->get_pointer<const char *>()) >= 0;
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
  memory *mem = this->get_memory();
  if (!mem)
  {
    BOX_ERROR(ERROR_BOX_DATA_NULL_DATA);
    return false;
  }

  switch (type)
  {
    case BOX_DATA_BOOL:
      std::cout << (to_bool() ? "true" : "false");
      break;
    case BOX_DATA_CHAR:
      std::cout << to_char();
      break;
    case BOX_DATA_SHORT:
      std::cout << to_short();
      break;
    case BOX_DATA_INT:
      std::cout << to_int();
      break;
    case BOX_DATA_FLOAT:
      std::cout << to_float();
      break;
    case BOX_DATA_LONG:
      std::cout << to_long();
      break;
    case BOX_DATA_DOUBLE:
      std::cout << to_double();
      break;
    case BOX_DATA_STRING:
      std::cout << mem->get_pointer<char *>();
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
    std::cout << std::endl;
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
  memory *mem = this->get_memory();
  if (!mem)
  {
    BOX_ERROR(ERROR_BOX_DATA_NULL_DATA);
    return false;
  }

  switch (type)
  {
    case BOX_DATA_BOOL:
      std::cin >> mem->get_element<bool>();
      break;
    case BOX_DATA_CHAR:
      std::cin >> mem->get_element<int8_t>();
      break;
    case BOX_DATA_SHORT:
      std::cin >> mem->get_element<int16_t>();
      break;
    case BOX_DATA_INT:
      std::cin >> mem->get_element<int32_t>();
      break;
    case BOX_DATA_FLOAT:
      std::cin >> mem->get_element<float32_t>();
      break;
    case BOX_DATA_LONG:
      std::cin >> mem->get_element<int64_t>();
      break;
    case BOX_DATA_DOUBLE:
      std::cin >> mem->get_element<float64_t>();
      break;
    case BOX_DATA_STRING:
      std::cin >> mem->get_pointer<int8_t *>();
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
 * Get string.
 *
 * @return string.
 */
std::string
box_data::get_string()
{
  std::string str;
  memory *mem = this->get_memory();

  if (!mem)
  {
    BOX_ERROR(ERROR_BOX_DATA_NULL_DATA);
    return str;
  }

  switch (type)
  {
    case BOX_DATA_BOOL:
      str = to_bool() ? "true" : "false";
      break;
    case BOX_DATA_CHAR:
      str = to_char();
      break;
    case BOX_DATA_SHORT:
      str = std::to_string(to_short());
      break;
    case BOX_DATA_LONG:
      str = std::to_string(to_long());
      break;
    case BOX_DATA_INT:
      str = std::to_string(to_int());
      break;
    case BOX_DATA_FLOAT:
      str = std::to_string(to_float());
      break;
    case BOX_DATA_DOUBLE:
      str = std::to_string(to_double());
      break;
    case BOX_DATA_STRING:
      str.assign(mem->get_pointer<const char *>());
      break;
    case BOX_DATA_INVALID:
    default:
      BOX_ERROR(ERROR_BOX_DATA_INVALID_DATA_TYPE);
      break;
  }

  return str;
}

/**
 * The destructor.
 */
box_data::~box_data()
{
  memory *mem = this->get_memory();

  if (!mem)
  {
    return;
  }

  mem->remove_entity("box_data_memory", this);
  this->vm->free(mem);
}
