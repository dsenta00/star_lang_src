#include "box_array.h"
#include "box_data.h"
#include "box_monitor.h"
#include "ORM/relationship.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ORM/orm.h"
#include "box_virtual_memory.h"

/**
 * The constructor.
 *
 * @param type - data type.
 * @param size - array size in members.
 */
box_array::box_array(std::string id, box_array *array) : entity::entity("box_array", id)
{
  this->add_relationship("array", ONE_TO_MANY);

  /*
   * array can be subarray of many arrays
   */
  this->add_relationship("subarray", ONE_TO_MANY);

  if (array == NULL)
  {
    return;
  }

  (*this) += (entity *)array;
}

/**
 * Get number of data.
 *
 * @return number of data.
 */
uint16_t
box_array::get_noof()
{
  return this->array.size();
}

/**
 * Operator [].
 *
 * @param index - element index.
 * @return data of found, otherwise return NULL.
 */
entity *
box_array::operator [] (uint32_t index)
{
  return (*this)[std::to_string(index)];
}

/**
 * Operator [].
 *
 * @param index - element index.
 * @return data of found, otherwise return NULL.
 */
entity *
box_array::operator [] (std::string index)
{
  return this->array[index];
}

/**
 * @brief box_array::insert_entity
 * @param index
 * @param e
 */
void
box_array::insert(std::string index, entity *e)
{
  this->remove_data(index);
  this->insert_data(index, e);
}

/**
 * @brief box_array::insert_entity
 * @param index
 * @param e
 */
void
box_array::insert(uint32_t index, entity *e)
{
  this->insert(std::to_string(index), e);
}

/**
 * Operator +=. Copy and append data to this array.
 *
 * @param data - the data.
 * @return true if operation success, otherwise return false.
 */
bool
box_array::operator += (entity *e)
{
  this->insert_data(std::to_string(this->get_noof()), e);
  return true;
}

/**
 * Convert array into string.
 *
 * @return the box data string.
 */
box_data &
box_array::to_string()
{
  box_data &str = *(box_data *)orm::create((entity *)new box_data(this->id.append(" as string"), BOX_DATA_STRING));

  relationship *r = this->get_relationship("array");

  if (!r)
  {
    return str;
  }

  char ch  = ' ';
  box_data separator_char = *(box_data *)orm::create((entity *)new box_data("<<temp_char>>",
                                                                            BOX_DATA_CHAR,
                                                                            (const void *)&ch));

  r->for_each([&] (entity *e) {
    if (e->get_entity_type() == "box_data")
    {
      box_data *data = (box_data *)e;
      str += *data;

      if (!BOX_OK)
      {
        str.default_value();
        return;
      }

      if (data != r->back())
      {
        str += separator_char;
      }
    }
    else if (e->get_entity_type() == "box_array")
    {
      box_array *data = (box_array *)e;
      str += data->to_string();

      if (!BOX_OK)
      {
        str.default_value();
        return;
      }

      if (data != r->back())
      {
        str += separator_char;
      }
    }
  });

  box_virtual_memory *vm = (box_virtual_memory *)orm::get_first("box_virtual_memory");
  vm->free(separator_char.get_memory());
  orm::destroy(&separator_char);

  return str;
}

void
box_array::clear()
{
  relationship *r = this->get_relationship("array");

  while (r->numOfEntities())
  {
    this->remove_data(r->front());
  }

  this->array.clear();
}

void
box_array::remove_data(std::string index)
{
  this->remove_data(this->array[index]);
}

void
box_array::remove_data(entity *e)
{
  if (!e)
  {
    return;
  }

  relationship *r = this->get_relationship("array");
  r->removeEntity(e);

  if (e->get_entity_type() == "box_data")
  {
    e->remove_entity("array", (entity *)this);
  }
  else if (e->get_entity_type() == "box_array")
  {
    e->remove_entity("subarray", (entity *)this);
  }

  this->array.erase(e->get_id());
}


/**
 * @brief box_array::add_data
 * @param data
 */
void
box_array::insert_data(std::string index, entity *e)
{
  if (e == NULL)
  {
    BOX_ERROR(ERROR_BOX_ARRAY_ADDING_NULL_DATA);
  }

  if (e->get_entity_type() == "box_data")
  {
    entity *new_data = orm::create((entity *)new box_data(index, *(box_data *)e));
    this->add_entity("array", new_data);

    new_data->add_relationship("array", MANY_TO_ONE);
    new_data->add_entity("array", (entity *)this);

    this->array[index] = new_data;
  }
  else if (e->get_entity_type() == "box_array")
  {
    this->add_entity("array", e);
    e->add_entity("subarray", (entity *)this);

    this->array[index] = e;
  }
}


/**
 * @brief box_array::~box_array
 */
box_array::~box_array()
{
  this->clear();
}

/**
 * @brief create
 * @param id
 * @param array
 * @return
 */
box_array *
box_array::create(std::string id, box_array *array)
{
  return (box_array *)orm::create((entity *)new box_array(id, array));
}
