/*
 * Copyright 2017 Duje Senta, Tomislav Radanovic
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "box_array.h"
#include "box_data.h"
#include "box_monitor.h"
#include "ORM/relationship.h"
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
  this->master_relationship_add("array", ONE_TO_MANY);

  if (array == nullptr)
  {
    return;
  }

  (*this) += (entity *) array;
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
box_array::operator[](uint32_t index)
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
box_array::operator[](std::string index)
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
box_array::operator+=(entity *e)
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
  box_data &str = *box_data::create(this->id.append(" as string"),
                                    BOX_DATA_STRING);

  relationship *r = this->master_relationship_get("array");

  if (!r)
  {
    return str;
  }

  char ch = ' ';
  box_data &separator_char = *box_data::create("<<temp_char>>",
                                               BOX_DATA_CHAR,
                                               (const void *) &ch);


  r->for_each([&](entity *e) {
    if (e->get_entity_type() == "box_data")
    {
      box_data *data = (box_data *) e;
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
      box_array *data = (box_array *) e;
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

  box_virtual_memory *vm = (box_virtual_memory *) orm::get_first("box_virtual_memory");
  vm->free(separator_char.get_memory());
  orm::destroy(&separator_char);

  return str;
}

void
box_array::clear()
{
  relationship *r = this->master_relationship_get("array");

  while (r->num_of_entities())
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

  this->master_relationship_remove_entity("array", e);
  this->array.erase(e->get_id());
}

/**
 * @brief box_array::add_data
 * @param data
 */
void
box_array::insert_data(std::string index, entity *e)
{
  if (e == nullptr)
  {
    BOX_ERROR(ERROR_BOX_ARRAY_ADDING_NULL_DATA);
  }

  if (e->get_entity_type() == "box_data")
  {
    /*
     * Data is not a reference. Create a new data.
     */
    entity *new_data = box_data::create(index, *(box_data *) e);
    e = new_data;
  }

  this->master_relationship_add_entity("array", e);
  this->array[index] = e;
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
  return (box_array *) orm::create((entity *) new box_array(id, array));
}
