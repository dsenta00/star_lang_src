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
  this->master_relationship_add("box_data_memory", ONE_TO_MANY);
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

    entity::master_relationship_add_entity("box_data_memory", (entity *) mem);
    default_value();
  }
  else
  {
    uint32_t size = (type == BOX_DATA_STRING) ?
                    (uint32_t) strlen((const char *) value) + 1 :
                    BOX_DATA_TYPE_SIZE[type];

    memory *mem = vm->alloc(size);

    if (!mem)
    {
      BOX_ERROR(ERROR_BOX_DATA_NULL_DATA);
      return;
    }

    entity::master_relationship_add_entity("box_data_memory", (entity *) mem);
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
  this->master_relationship_add("box_data_memory", ONE_TO_MANY);

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

  this->master_relationship_add_entity("box_data_memory", (entity *) mem);
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
      value = !this->get_string().empty();
      break;
    case BOX_DATA_CHAR:
      value = (bool) mem->get_element<int8_t>();
      break;
    case BOX_DATA_INT:
      value = (bool) mem->get_element<int32_t>();
      break;
    case BOX_DATA_FLOAT:
      value = (bool) mem->get_element<double>();
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
    case BOX_DATA_INT:
      value = (int8_t) mem->get_element<int32_t>();
      break;
    case BOX_DATA_FLOAT:
      value = (int8_t) mem->get_element<double>();
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
      value = (int32_t) mem->get_element<int8_t>();
      break;
    case BOX_DATA_INT:
      value = mem->get_element<int32_t>();
      break;
    case BOX_DATA_FLOAT:
      value = (int32_t) mem->get_element<double>();
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
 * @return float value if success, otherwise return 0.0.
 */
double
box_data::to_float()
{
  double value = 0.0;
  memory *mem = this->get_memory();

  if (!mem)
  {
    BOX_ERROR(ERROR_BOX_DATA_NULL_DATA);
    return value;
  }

  switch (type)
  {
    case BOX_DATA_BOOL:
      value = (double) mem->get_element<bool>();
      break;
    case BOX_DATA_CHAR:
      value = (double) mem->get_element<int8_t>();
      break;
    case BOX_DATA_INT:
      value = (double) mem->get_element<int32_t>();
      break;
    case BOX_DATA_FLOAT:
      value = mem->get_element<double>();
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
 * Convert data to string and return.
 *
 * @return the box data string
 */
box_data &
box_data::to_string()
{
  std::string string = this->get_string();

  return *box_data::create(this->id.append(" as string"),
                           BOX_DATA_STRING,
                           string.c_str());
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
  return (memory *) this->master_relationship_get("box_data_memory")->front();
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

  switch (this->type)
  {
    case BOX_DATA_BOOL:
    case BOX_DATA_CHAR:
    case BOX_DATA_INT:
    case BOX_DATA_STRING:
      memset(mem->get_pointer<void *>(), 0, mem->get_size());
      break;
    case BOX_DATA_FLOAT:
      mem->get_element<double>() = 0.0;
      break;
    case BOX_DATA_INVALID:
    default:
      BOX_ERROR(ERROR_BOX_DATA_INVALID_DATA_TYPE);
      return false;
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
    case BOX_DATA_INT:
    {
      new_mem = this->vm->alloc(BOX_DATA_TYPE_SIZE[new_type]);
      new_mem->get_element<int32_t>() = this->to_int();
      break;
    }
    case BOX_DATA_STRING:
    {
      std::string str = this->get_string();
      new_mem = this->vm->alloc(static_cast<uint32_t>(str.size() + 1));
      strncpy(new_mem->get_pointer<char *>(),
              str.c_str(),
              new_mem->get_size());
      break;
    }
    case BOX_DATA_FLOAT:
    {
      new_mem = this->vm->alloc(BOX_DATA_TYPE_SIZE[new_type]);
      new_mem->get_element<double>() = this->to_float();
      break;
    }
    case BOX_DATA_INVALID:
    default:
      BOX_ERROR(ERROR_BOX_DATA_INVALID_DATA_TYPE);
      return;
  }

  entity::master_relationship_add_entity("box_data_memory", (entity *) new_mem);
  entity::master_relationship_remove_entity("box_data_memory", (entity *) mem);

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
    case BOX_DATA_INT:
      mem->get_element<int32_t>() = *(int32_t *) data;
      break;
    case BOX_DATA_FLOAT:
      mem->get_element<double>() = *(double *) data;
      break;
    case BOX_DATA_STRING:
    {
      const char *str_temp = (const char *) data;
      uint32_t str_size = static_cast<uint32_t>(strlen(str_temp) + 1);

      if (mem->get_size() < str_size)
      {
        memory *new_mem = this->vm->alloc(str_size);

        entity::master_relationship_remove_entity("box_data_memory", (entity *) mem);
        entity::master_relationship_add_entity("box_data_memory", (entity *) new_mem);

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
    case BOX_DATA_INT:
      mem->get_element<int32_t>() = data.to_int();
      break;
    case BOX_DATA_FLOAT:
      mem->get_element<double>() = data.to_float();
      break;
    case BOX_DATA_STRING:
    case BOX_DATA_INVALID:
    default:
      BOX_ERROR(ERROR_BOX_DATA_INVALID_DATA_TYPE);
      return false;
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
    case BOX_DATA_INT:
      mem->get_element<int32_t>() &= data.to_int();
      break;
    case BOX_DATA_FLOAT:
      BOX_ERROR(ERROR_BOX_DATA_LOGICAL_OPERATION_FLOAT);
      break;
    case BOX_DATA_STRING:
      BOX_ERROR(ERROR_BOX_DATA_LOGICAL_OPERATION_STRING);
      return false;
    case BOX_DATA_INVALID:
    default:
      BOX_ERROR(ERROR_BOX_DATA_INVALID_DATA_TYPE);
      return false;
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

  switch (this->type)
  {
    case BOX_DATA_BOOL:
      mem->get_element<bool>() |= data.to_bool();
      break;
    case BOX_DATA_CHAR:
      mem->get_element<int8_t>() |= data.to_char();
      break;
    case BOX_DATA_INT:
      mem->get_element<int32_t>() |= data.to_int();
      break;
    case BOX_DATA_FLOAT:
      BOX_ERROR(ERROR_BOX_DATA_LOGICAL_OPERATION_FLOAT);
      break;
    case BOX_DATA_STRING:
      BOX_ERROR(ERROR_BOX_DATA_LOGICAL_OPERATION_STRING);
      return false;
    case BOX_DATA_INVALID:
    default:
      BOX_ERROR(ERROR_BOX_DATA_INVALID_DATA_TYPE);
      return false;
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
    case BOX_DATA_INT:
      mem->get_element<int32_t>() ^= data.to_int();
      break;
    case BOX_DATA_FLOAT:
      BOX_ERROR(ERROR_BOX_DATA_LOGICAL_OPERATION_FLOAT);
      break;
    case BOX_DATA_STRING:
      BOX_ERROR(ERROR_BOX_DATA_LOGICAL_OPERATION_STRING);
      return false;
    case BOX_DATA_INVALID:
    default:
      BOX_ERROR(ERROR_BOX_DATA_INVALID_DATA_TYPE);
      return false;
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
    uint32_t request_size = static_cast<uint32_t>(strlen(mem->get_pointer<const char *>()) +
                                                  strlen(string.c_str()) + 1);

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
        entity::master_relationship_remove_entity("box_data_memory", (entity *) mem);
        entity::master_relationship_add_entity("box_data_memory", (entity *) new_mem);

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
    case BOX_DATA_INT:
      mem->get_element<int32_t>() += data.to_int();
      break;
    case BOX_DATA_FLOAT:
      mem->get_element<double>() += data.to_float();
      break;
    case BOX_DATA_INVALID:
    default:
      BOX_ERROR(ERROR_BOX_DATA_INVALID_DATA_TYPE);
      return false;
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
    case BOX_DATA_INT:
      mem->get_element<int32_t>() -= data.to_int();
      break;
    case BOX_DATA_FLOAT:
      mem->get_element<double>() -= data.to_float();
      break;
    case BOX_DATA_STRING:
      BOX_ERROR(ERROR_BOX_DATA_SUBTRACTING_STRING);
      return false;
    case BOX_DATA_INVALID:
    default:
      BOX_ERROR(ERROR_BOX_DATA_INVALID_DATA_TYPE);
      return false;
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
    case BOX_DATA_INT:
      mem->get_element<int32_t>() *= data.to_int();
      break;
    case BOX_DATA_FLOAT:
      mem->get_element<double>() *= data.to_float();
      break;
    case BOX_DATA_STRING:
      BOX_ERROR(ERROR_BOX_DATA_MULTIPLYING_STRING);
      return false;
    case BOX_DATA_INVALID:
    default:
      BOX_ERROR(ERROR_BOX_DATA_INVALID_DATA_TYPE);
      return false;
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
    case BOX_DATA_INT:

      if (data.to_int() == 0)
      {
        BOX_ERROR(ERROR_BOX_DATA_DIVIDING_ZERO);
        return false;
      }

      mem->get_element<int32_t>() /= data.to_int();
      break;
    case BOX_DATA_FLOAT:

      if (data.to_float() == 0.0)
      {
        BOX_ERROR(ERROR_BOX_DATA_DIVIDING_ZERO);
        return false;
      }

      mem->get_element<double>() /= data.to_float();
      break;
    case BOX_DATA_STRING:
      BOX_ERROR(ERROR_BOX_DATA_DIVIDING_STRING);
      return false;
    case BOX_DATA_INVALID:
    default:
      BOX_ERROR(ERROR_BOX_DATA_INVALID_DATA_TYPE);
      return false;
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
    case BOX_DATA_INT:

      if (data.to_int() == 0)
      {
        BOX_ERROR(ERROR_BOX_DATA_MODULUS_ZERO);
        return false;
      }

      mem->get_element<int32_t>() %= data.to_int();
      break;
    case BOX_DATA_FLOAT:
    case BOX_DATA_STRING:
      BOX_ERROR(ERROR_BOX_DATA_INVALID_MODULUS);
      return false;
    case BOX_DATA_INVALID:
    default:
      BOX_ERROR(ERROR_BOX_DATA_INVALID_DATA_TYPE);
      return false;
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
    case BOX_DATA_INT:
      mem->get_element<int32_t>()++;
      break;
    case BOX_DATA_FLOAT:
      mem->get_element<double>() += 1.0;
      break;
    case BOX_DATA_STRING:
      BOX_ERROR(ERROR_BOX_DATA_INCREMENTING_STRING);
      return false;
    case BOX_DATA_INVALID:
    default:
      BOX_ERROR(ERROR_BOX_DATA_INVALID_DATA_TYPE);
      return false;
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
    case BOX_DATA_INT:
      mem->get_element<int32_t>()--;
      break;
    case BOX_DATA_FLOAT:
      mem->get_element<double>() -= 1.0;
      break;
    case BOX_DATA_STRING:
      BOX_ERROR(ERROR_BOX_DATA_DECREMENTING_STRING);
      return false;
    case BOX_DATA_INVALID:
    default:
      BOX_ERROR(ERROR_BOX_DATA_INVALID_DATA_TYPE);
      return false;
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
    case BOX_DATA_CHAR:
      return to_char() == data.to_char();
    case BOX_DATA_INT:
      return to_int() == data.to_int();
    case BOX_DATA_FLOAT:
      return to_float() == data.to_float();
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
    }
    case BOX_DATA_INVALID:
    default:
      BOX_ERROR(ERROR_BOX_DATA_INVALID_DATA_TYPE);
      return false;
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
    case BOX_DATA_CHAR:
      return to_char() != data.to_char();
    case BOX_DATA_INT:
      return to_int() != data.to_int();
    case BOX_DATA_FLOAT:
      return to_float() != data.to_float();
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
    case BOX_DATA_INVALID:
    default:
      BOX_ERROR(ERROR_BOX_DATA_INVALID_DATA_TYPE);
      return false;
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
    case BOX_DATA_INT:
      return to_int() > data.to_int();
    case BOX_DATA_FLOAT:
      return to_float() > data.to_float();
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
    case BOX_DATA_INT:
      return to_int() < data.to_int();
    case BOX_DATA_FLOAT:
      return to_float() < data.to_float();
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
    case BOX_DATA_INVALID:
    default:
      BOX_ERROR(ERROR_BOX_DATA_INVALID_DATA_TYPE);
      return false;
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
    case BOX_DATA_INT:
      return to_int() >= data.to_int();
    case BOX_DATA_FLOAT:
      return to_float() >= data.to_float();
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
    case BOX_DATA_INT:
      return to_int() <= data.to_int();
    case BOX_DATA_FLOAT:
      return to_float() <= data.to_float();
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
    case BOX_DATA_INVALID:
    default:
      BOX_ERROR(ERROR_BOX_DATA_INVALID_DATA_TYPE);
      return false;
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
    case BOX_DATA_INT:
      std::cout << to_int();
      break;
    case BOX_DATA_FLOAT:
      std::cout << to_float();
      break;
    case BOX_DATA_STRING:
      std::cout << mem->get_pointer<char *>();
      break;
    case BOX_DATA_INVALID:
    default:
      BOX_ERROR(ERROR_BOX_DATA_INVALID_DATA_TYPE);
      return false;
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
    case BOX_DATA_INT:
      std::cin >> mem->get_element<int32_t>();
      break;
    case BOX_DATA_FLOAT:
      std::cin >> mem->get_element<double>();
      break;
    case BOX_DATA_STRING:
      std::cin >> mem->get_pointer<int8_t *>();
      break;
    case BOX_DATA_INVALID:
    default:
      BOX_ERROR(ERROR_BOX_DATA_INVALID_DATA_TYPE);
      return false;
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
    case BOX_DATA_INT:
      str = std::to_string(to_int());
      break;
    case BOX_DATA_FLOAT:
      str = std::to_string(to_float());
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
