#include "box_array_test.h"
#include "box_array.h"
#include "box_assert.h"
#include "box_virtual_memory.h"
#include "box_monitor.h"
#include "box_data.h"
#include "ORM/orm.h"

static box_virtual_memory *virtual_memory;

#define ARRAY_SIZE (10)

/**
 * Return a random name.
 *
 * @return random name.
 */
static const char *random_name(int32_t i = -1)
{
  static const char str[][64] =
    {
      "Miljenko",
      "Mislav",
      "Ivan",
      "Danijela",
      "Antonia",
      "Ivanka",
      "Vesna",
      "Matea",
      "Ivana",
      "Ante",
    };

  return (i == -1) ? str[rand() % 10] : str[i % 10];
}

/**
 * Test array basics.
 */
static void
box_array_test_basic()
{
  ASSERT_VIRTUAL_MEMORY(*virtual_memory, 0);
  box_array &empty_array = *box_array::create("empty_array");
  ASSERT_VIRTUAL_MEMORY(*virtual_memory, 0);

  for (uint16_t i = 0; i < USHRT_MAX; i++)
  {
    ASSERT_TRUE(empty_array[i] == nullptr,
                "Returned reference should be NULL! (0x%X)",
                empty_array[i]);
  }

  box_data &str = empty_array.to_string();
  ASSERT_TRUE(strcmp((const char *) str.get_address(),
                     "") == 0,
              "Returned string should be empty! (%s)",
              (const char *) str.get_address());
  ASSERT_VIRTUAL_MEMORY(*virtual_memory, BOX_DATA_TYPE_SIZE[BOX_DATA_STRING]);

  std::string comparision;

  for (uint32_t i = 0; i < ARRAY_SIZE; i++)
  {
    if (i % 2 == 0)
    {
      box_data &data = *box_data::create("temp_name", BOX_DATA_STRING, random_name(i));
      empty_array.insert(i, (entity *) &data);
      ASSERT_OK;
      comparision.append(random_name(i));
    }
    else if (i % 3 == 0)
    {
      box_data &data = *box_data::create("temp_name", BOX_DATA_INT, (const void *) &i);
      empty_array.insert(i, (entity *) &data);
      ASSERT_OK;
      comparision.append(std::to_string(i));
    }
    else
    {
      float32_t fi = (float32_t) i;
      box_data &data = *box_data::create("temp_name", BOX_DATA_FLOAT, (const void *) &fi);
      empty_array.insert(i, (entity *) &data);
      ASSERT_OK;
      comparision.append(std::to_string(fi));
    }

    if (i < (ARRAY_SIZE - 1))
    {
      comparision.append(" ");
    }
  }

  for (uint32_t i = 0; i < ARRAY_SIZE; i++)
  {
    if (i % 2 == 0)
    {
      box_data *data = (box_data *) empty_array[i];

      ASSERT_TRUE(data->get_type() == BOX_DATA_STRING,
                  "data should be BOX_DATA_STRING");

      ASSERT_TRUE(strcmp((const char *) data->get_address(),
                         random_name(i)) == 0,
                  "data should be %s (%s)",
                  random_name(i),
                  (const char *) data->get_address());
      ASSERT_OK;
    }
    else if (i % 3 == 0)
    {
      box_data *data = (box_data *) empty_array[i];
      ASSERT_TRUE(data->get_type() == BOX_DATA_INT,
                  "data should be BOX_DATA_INT");

      ASSERT_TRUE(*(uint32_t *) data->get_address() == i,
                  "data should be %u (%u)",
                  i,
                  *(uint32_t *) data->get_address());

      ASSERT_OK;
    }
    else
    {
      float32_t fi = (float32_t) i;
      box_data *data = (box_data *) empty_array[i];

      ASSERT_TRUE(data->get_type() == BOX_DATA_FLOAT,
                  "data should be BOX_DATA_FLOAT");

      ASSERT_TRUE(*(float32_t *) data->get_address() == fi,
                  "data should be %f (%f)",
                  fi,
                  *(float32_t *) data->get_address());

      ASSERT_OK;
    }
  }

  box_data &str2 = empty_array.to_string();

  ASSERT_TRUE(comparision.compare((const char *) str2.get_address()) == 0,
              "they should be the same: expected %s, got %s",
              comparision.c_str(),
              (const char *) str2.get_address());

  box_array &array = *box_array::create("array");

  for (uint32_t i = 0; i < ARRAY_SIZE; i++)
  {
    if (i % 2 == 0)
    {
      box_data &data = *box_data::create("temp_name", BOX_DATA_STRING, random_name(i));
      array.insert(i, (entity *) &data);
      ASSERT_OK;
    }
    else if (i % 3 == 0)
    {
      box_data &data = *box_data::create("temp_name", BOX_DATA_INT, (const void *) &i);
      array.insert(i, (entity *) &data);
      ASSERT_OK;
    }
    else
    {
      array.insert(i, (entity *) &empty_array);
      ASSERT_OK;
    }
  }

  for (uint32_t i = 0; i < ARRAY_SIZE; i++)
  {
    if (i % 2 == 0)
    {
      box_data *data = (box_data *) array[i];
      ASSERT_OK;
      ASSERT_TRUE(data->get_type() == BOX_DATA_STRING, "data should be BOX_DATA_STRING");
      ASSERT_TRUE(strcmp((const char *) data->get_address(),
                         random_name(i)) == 0,
                  "data should be %s (%s)",
                  random_name(i),
                  (const char *) data->get_address());
    }
    else if (i % 3 == 0)
    {
      box_data *data = (box_data *) array[i];
      ASSERT_OK;
      ASSERT_TRUE(data->get_type() == BOX_DATA_INT, "data should be BOX_DATA_INT");
      ASSERT_TRUE(*(uint32_t *) data->get_address() == i,
                  "data should be %u (%u)",
                  i,
                  *(uint32_t *) data->get_address());
    }
    else
    {
      box_array *array_inside = (box_array *) array[i];
      ASSERT_OK;
      ASSERT_NOT_NULL(array_inside);
      ASSERT_EQUALS(array_inside, &empty_array);
      ASSERT_TRUE(array_inside == &empty_array, "data should be the same");
    }
  }

  orm::destroy(&array);

  printf("\t-> %s()::OK \n", __FUNCTION__);
}

/**
 * Test BOX array.
 */
void
box_array_test()
{
  printf("%s()\r\n", __FUNCTION__);

  virtual_memory = (box_virtual_memory *) orm::get_first("box_virtual_memory");
  BOX_TEST(box_array_test_basic());

  printf("\r\n\r\n");
}
