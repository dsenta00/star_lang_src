#include "box_array_test.h"
#include "box_array.h"
#include "box_assert.h"
#include "box_virtual_memory.h"
#include <limits.h>

extern box_virtual_memory virtual_memory;

#define TEST_ARRAY_SIZE (10)

#define ASSERT_VIRTUAL_MEMORY(__BYTES__) \
  ASSERT_TRUE(virtual_memory.get_allocated_total() == __BYTES__, \
              "Total allocated should be %u (%u)", \
              __BYTES__, \
              virtual_memory.get_allocated_total())

/**
 * Test char array.
 */
static void
box_array_test_char()
{
  ASSERT_VIRTUAL_MEMORY(0);
  box_array char_array(BOX_DATA_CHAR, TEST_ARRAY_SIZE);
  ASSERT_VIRTUAL_MEMORY(BOX_DATA_TYPE_SIZE[BOX_DATA_CHAR] * TEST_ARRAY_SIZE);
  ASSERT_OK;

  for (uint16_t i = 0; i < TEST_ARRAY_SIZE; i++)
  {
    ASSERT_TRUE(char_array[i] != NULL,
                "Returned reference shouldn't be NULL! (0x%X)",
                char_array[i]);
    ASSERT_OK;
    ASSERT_TRUE(*(int8_t *)(char_array[i]->get_address()) == 0,
                "Default element value should be 0! (%c)",
                *(int8_t *)(char_array[i]->get_address()));
  }

  for (uint16_t i = TEST_ARRAY_SIZE; i < USHRT_MAX; i++)
  {
    ASSERT_TRUE(char_array[i] == NULL,
                "Returned reference should be NULL! (0x%X)",
                char_array[i]);
    ASSERT_ERROR(ERROR_BOX_ARRAY_OUT_OF_BOUNDS);
    BOX_ERROR_CLEAR;
  }

  for (uint16_t i = 0; i < TEST_ARRAY_SIZE; i++)
  {
    *(int8_t *)(char_array[i]->get_address()) = (float32_t)(i + '0');
  }

  for (uint16_t i = 0; i < TEST_ARRAY_SIZE; i++)
  {
    ASSERT_TRUE(*(int8_t *)(char_array[i]->get_address()) == (int8_t)(i + '0'),
                "Element value should be %c! (%c)",
                (int8_t)(i + '0'),
                *(int8_t *)(char_array[i]->get_address()));
  }

  *(int8_t *)(char_array[TEST_ARRAY_SIZE - 1]->get_address()) = 0;

  box_data string_data = char_array.to_string();

  ASSERT_TRUE(strcmp((const char *)string_data.get_address(), "012345678") == 0,
              "Strings should be same! (%s)",
              (const char *)string_data.get_address());
  ASSERT_OK;

  ASSERT_VIRTUAL_MEMORY(BOX_DATA_TYPE_SIZE[BOX_DATA_CHAR] * TEST_ARRAY_SIZE +
                        sizeof("012345678"));
  printf("\t-> %s()::OK\n", __FUNCTION__);
}

/**
 * Test float array.
 */
static void
box_array_test_float()
{
  ASSERT_VIRTUAL_MEMORY(0);
  box_array float_array(BOX_DATA_FLOAT, TEST_ARRAY_SIZE);
  ASSERT_VIRTUAL_MEMORY(BOX_DATA_TYPE_SIZE[BOX_DATA_FLOAT] * TEST_ARRAY_SIZE);
  ASSERT_OK;

  for (uint16_t i = 0; i < TEST_ARRAY_SIZE; i++)
  {
    ASSERT_TRUE(float_array[i] != NULL,
                "Returned reference shouldn't be NULL! (0x%X)",
                float_array[i]);
    ASSERT_OK;

    ASSERT_TRUE(*(float32_t *)(float_array[i]->get_address()) == 0.0,
                "Default element value should be 0.0! (%f)",
                *(float32_t *)(float_array[i]->get_address()));
  }

  for (uint16_t i = TEST_ARRAY_SIZE; i < USHRT_MAX; i++)
  {
    ASSERT_TRUE(float_array[i] == NULL,
                "Returned reference should be NULL! (0x%X)",
                float_array[i]);
    ASSERT_ERROR(ERROR_BOX_ARRAY_OUT_OF_BOUNDS);
    BOX_ERROR_CLEAR;
  }

  for (uint16_t i = 0; i < TEST_ARRAY_SIZE; i++)
  {
    *(float32_t *)(float_array[i]->get_address()) = (float32_t)i;
  }

  for (uint16_t i = 0; i < TEST_ARRAY_SIZE; i++)
  {
    ASSERT_TRUE(*(float32_t *)(float_array[i]->get_address()) == (float32_t)i,
                "Element value should be %f! (%f)", (float32_t)i,
                *(float32_t *)(float_array[i]->get_address()));
  }

  box_data string_data = float_array.to_string();

  ASSERT_TRUE(strcmp((const char *)string_data.get_address(),
                     "0.000000 1.000000 2.000000 3.000000 4.000000 "
                     "5.000000 6.000000 7.000000 8.000000 9.000000") == 0,
              "Strings should be same! (%s)",
              (const char *)string_data.get_address());
  ASSERT_OK;

  ASSERT_VIRTUAL_MEMORY(BOX_DATA_TYPE_SIZE[BOX_DATA_FLOAT] * TEST_ARRAY_SIZE +
                        sizeof("0.000000 1.000000 2.000000 3.000000 4.000000 "
                               "5.000000 6.000000 7.000000 8.000000 9.000000"));
  printf("\t-> %s()::OK\n", __FUNCTION__);
}

/**
 * Test array of double.
 */
static void
box_array_test_double()
{
  ASSERT_VIRTUAL_MEMORY(0);
  box_array float_array(BOX_DATA_DOUBLE, TEST_ARRAY_SIZE);
  ASSERT_VIRTUAL_MEMORY(BOX_DATA_TYPE_SIZE[BOX_DATA_DOUBLE] * TEST_ARRAY_SIZE);

  ASSERT_OK;

  for (uint16_t i = 0; i < TEST_ARRAY_SIZE; i++)
  {
    ASSERT_TRUE(float_array[i] != NULL,
                "Returned reference shouldn't be NULL! (0x%X)",
                float_array[i]);

    ASSERT_OK;

    ASSERT_TRUE(*(float64_t *)(float_array[i]->get_address()) == 0.0,
                "Default element value should be 0.0! (%f)",
                *(float64_t *)(float_array[i]->get_address()));
  }

  for (uint16_t i = TEST_ARRAY_SIZE; i < USHRT_MAX; i++)
  {
    ASSERT_TRUE(float_array[i] == NULL,
                "Returned reference should be NULL! (0x%X)",
                float_array[i]);
    ASSERT_ERROR(ERROR_BOX_ARRAY_OUT_OF_BOUNDS);
    BOX_ERROR_CLEAR;
  }

  for (uint16_t i = 0; i < TEST_ARRAY_SIZE; i++)
  {
    *(float64_t *)(float_array[i]->get_address()) = (float32_t)i;
  }

  for (uint16_t i = 0; i < TEST_ARRAY_SIZE; i++)
  {
    ASSERT_TRUE(*(float64_t *)(float_array[i]->get_address()) == (float64_t)i,
                "Element value should be %f! (%f)",
                (float64_t)i,
                *(float64_t *)(float_array[i]->get_address()));
  }

  box_data string_data = float_array.to_string();

  ASSERT_TRUE(strcmp((const char *)string_data.get_address(),
                     "0.000000 1.000000 2.000000 3.000000 4.000000 5.000000 "
                     "6.000000 7.000000 8.000000 9.000000") == 0,
              "Strings should be the same! (%s)",
              (const char *)string_data.get_address());
  ASSERT_OK;

  ASSERT_VIRTUAL_MEMORY(BOX_DATA_TYPE_SIZE[BOX_DATA_DOUBLE] * TEST_ARRAY_SIZE +
                        sizeof("0.000000 1.000000 2.000000 3.000000 4.000000 "
                               "5.000000 6.000000 7.000000 8.000000 9.000000"));
  printf("\t-> %s()::OK\n", __FUNCTION__);
}

/**
 * Test int array. (16, 32, 64 bit)
 */
template <typename T>
static void
box_array_test_int(box_data_type int_type)
{
  ASSERT_VIRTUAL_MEMORY(0);
  box_array int_array(int_type, TEST_ARRAY_SIZE);
  ASSERT_VIRTUAL_MEMORY(BOX_DATA_TYPE_SIZE[int_type] * TEST_ARRAY_SIZE);

  ASSERT_OK;

  for (uint16_t i = 0; i < TEST_ARRAY_SIZE; i++)
  {
    ASSERT_TRUE(int_array[i] != NULL,
                "Returned reference shouldn't be NULL! (0x%X)",
                int_array[i]);
    ASSERT_OK;

    ASSERT_TRUE(*(T *)(int_array[i]->get_address()) == 0,
                "Default element value should be 0! (%d)",
                *(T *)(int_array[i]->get_address()));
  }

  for (uint16_t i = TEST_ARRAY_SIZE; i < USHRT_MAX; i++)
  {
    ASSERT_TRUE(int_array[i] == NULL,
                "Returned reference should be NULL! (0x%X)",
                int_array[i]);
    ASSERT_ERROR(ERROR_BOX_ARRAY_OUT_OF_BOUNDS);
    BOX_ERROR_CLEAR;
  }

  for (uint16_t i = 0; i < TEST_ARRAY_SIZE; i++)
  {
    *(T *)(int_array[i]->get_address()) = i;
  }

  for (uint16_t i = 0; i < TEST_ARRAY_SIZE; i++)
  {
    ASSERT_TRUE(*(T *)(int_array[i]->get_address()) == i,
                "Element value should be %d! (%d)",
                i,
                *(T *)(int_array[i]->get_address()));
  }

  box_data string_data = int_array.to_string();

  ASSERT_TRUE(strcmp((const char *)string_data.get_address(),
                     "0 1 2 3 4 5 6 7 8 9") == 0,
              "Strings should be same! (%s)",
              (const char *)string_data.get_address());
  ASSERT_VIRTUAL_MEMORY(BOX_DATA_TYPE_SIZE[int_type] * TEST_ARRAY_SIZE +
                        sizeof("0 1 2 3 4 5 6 7 8 9"));
  ASSERT_OK;

  box_data int_data(int_type);
  *(T *)int_data.get_address() = 10;

  ASSERT_VIRTUAL_MEMORY(BOX_DATA_TYPE_SIZE[int_type] * TEST_ARRAY_SIZE +
                        sizeof("0 1 2 3 4 5 6 7 8 9") +
                        BOX_DATA_TYPE_SIZE[int_type]);

  ASSERT_TRUE(int_array += &int_data, "int_array and int_data should add!");

  ASSERT_VIRTUAL_MEMORY(BOX_DATA_TYPE_SIZE[int_type] * (TEST_ARRAY_SIZE + 1) +
                        sizeof("0 1 2 3 4 5 6 7 8 9") +
                        BOX_DATA_TYPE_SIZE[int_type]);

  ASSERT_TRUE(int_array.get_noof() == (TEST_ARRAY_SIZE + 1),
              "int_array should have %u members (%u)",
              (TEST_ARRAY_SIZE + 1),
              int_array.get_noof());

  ASSERT_TRUE(*(T *)int_array[TEST_ARRAY_SIZE]->get_address() == 10,
              "int_array[%u] should be %u (%u)",
              TEST_ARRAY_SIZE,
              10,
              *(int32_t *)int_array[TEST_ARRAY_SIZE]->get_address());

  box_array second_int_array(int_type, 3);

  ASSERT_VIRTUAL_MEMORY(BOX_DATA_TYPE_SIZE[int_type] * (TEST_ARRAY_SIZE + 1) +
                        sizeof("0 1 2 3 4 5 6 7 8 9") +
                        BOX_DATA_TYPE_SIZE[int_type] +
                        BOX_DATA_TYPE_SIZE[int_type] * 3);

  for (uint16_t i = 0; i < 3; i++)
  {
    *(T *)(second_int_array[i]->get_address()) = i + 11;
  }

  ASSERT_TRUE(int_array += second_int_array,
              "int_array and second_int_array should add!");

  ASSERT_VIRTUAL_MEMORY(BOX_DATA_TYPE_SIZE[int_type] * (TEST_ARRAY_SIZE + 4) +
                        sizeof("0 1 2 3 4 5 6 7 8 9") +
                        BOX_DATA_TYPE_SIZE[int_type] +
                        BOX_DATA_TYPE_SIZE[int_type] * 3);

  ASSERT_TRUE(int_array.get_noof() == (TEST_ARRAY_SIZE + 4),
              "int_array should have %u members (%u)",
              (TEST_ARRAY_SIZE + 4),
              int_array.get_noof());

  for (uint16_t i = 0; i < (TEST_ARRAY_SIZE + 4); i++)
  {
    ASSERT_TRUE(*(T *)(int_array[i]->get_address()) == i,
                "int_array[%u] should be %u",
                i,
                *(int32_t *)(int_array[i]->get_address()));
  }


  printf("\t-> %s(%s)::OK\n", __FUNCTION__, int_data.get_type_str());
}

/**
 * Test array basics.
 */
static void
box_array_test_basic()
{
  ASSERT_VIRTUAL_MEMORY(0);
  box_array invalid_array;
  ASSERT_VIRTUAL_MEMORY(0);

  ASSERT_ERROR(ERROR_BOX_ARRAY_INVALID_DATA_TYPE);
  BOX_ERROR_CLEAR;

  for (uint16_t i = 0; i < USHRT_MAX; i++)
  {
    ASSERT_TRUE(invalid_array[i] == NULL,
                "Returned reference should be NULL! (0x%X)",
                invalid_array[i]);
    ASSERT_ERROR(ERROR_BOX_ARRAY_INVALID_DATA_TYPE);
    BOX_ERROR_CLEAR;
  }

  box_data str = invalid_array.to_string();
  ASSERT_TRUE(strcmp((const char *)str.get_address(),
                     "") == 0,
              "Returned string should be empty! (%s)",
              (const char *)str.get_address());
  ASSERT_ERROR(ERROR_BOX_ARRAY_INVALID_DATA_TYPE);
  BOX_ERROR_CLEAR;

  ASSERT_VIRTUAL_MEMORY(BOX_DATA_TYPE_SIZE[BOX_DATA_STRING]);

  box_array invalid_array2(BOX_DATA_INVALID, 0);
  ASSERT_ERROR(ERROR_BOX_ARRAY_INVALID_DATA_TYPE);
  BOX_ERROR_CLEAR;
  ASSERT_VIRTUAL_MEMORY(BOX_DATA_TYPE_SIZE[BOX_DATA_STRING]);

  for (uint16_t i = 0; i < USHRT_MAX; i++)
  {
    ASSERT_TRUE(invalid_array2[i] == NULL,
                "Returned reference should be NULL! (0x%X)",
                invalid_array2[i]);
    ASSERT_ERROR(ERROR_BOX_ARRAY_INVALID_DATA_TYPE);
    BOX_ERROR_CLEAR;
  }

  ASSERT_TRUE(strcmp((const char *)invalid_array2.to_string().get_address(),
                     "") == 0,
              "Returned string should be empty! (%s)",
              (const char *)invalid_array2.to_string().get_address());
  ASSERT_ERROR(ERROR_BOX_ARRAY_INVALID_DATA_TYPE);
  BOX_ERROR_CLEAR;

  box_array invalid_array3(BOX_DATA_INVALID, 16);
  ASSERT_ERROR(ERROR_BOX_ARRAY_INVALID_DATA_TYPE);
  BOX_ERROR_CLEAR;

  for (uint16_t i = 0; i < USHRT_MAX; i++)
  {
    ASSERT_TRUE(invalid_array3[i] == NULL,
                "Returned reference should be NULL! (0x%X)",
                invalid_array3[i]);
    ASSERT_ERROR(ERROR_BOX_ARRAY_INVALID_DATA_TYPE);
    BOX_ERROR_CLEAR;
  }

  ASSERT_TRUE(strcmp((const char *)invalid_array3.to_string().get_address(),
                     "") == 0,
              "Returned string should be empty! (%s)",
              (const char *)invalid_array3.to_string().get_address());
  ASSERT_ERROR(ERROR_BOX_ARRAY_INVALID_DATA_TYPE);
  BOX_ERROR_CLEAR;

  box_array invalid_array4(BOX_DATA_INT, 0);
  ASSERT_ERROR(ERROR_BOX_ARRAY_ZERO_SIZE);
  BOX_ERROR_CLEAR;

  for (uint16_t i = 0; i < USHRT_MAX; i++)
  {
    ASSERT_TRUE(invalid_array4[i] == NULL,
                "Returned reference should be NULL! (0x%X)",
                invalid_array4[i]);
    ASSERT_ERROR(ERROR_BOX_ARRAY_ZERO_SIZE);
    BOX_ERROR_CLEAR;
  }

  ASSERT_TRUE(strcmp((const char *)invalid_array4.to_string().get_address(),
                     "") == 0,
              "Returned string should be empty! (%s)",
              (const char *)invalid_array4.to_string().get_address());
  ASSERT_ERROR(ERROR_BOX_ARRAY_ZERO_SIZE);
  BOX_ERROR_CLEAR;

  printf("\t-> %s()::OK \n", __FUNCTION__);
}

/**
 * Return a random name.
 *
 * @return random name.
 */
static const char *random_name()
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

  return str[rand() % 10];
}

/**
 * Test string array.
 */
void
box_array_test_string()
{
#define STRING_ARRAY_SIZE (10)
  box_data str(BOX_DATA_STRING);
  box_array str_array(BOX_DATA_STRING, STRING_ARRAY_SIZE);

  for (uint32_t i = 0; i < STRING_ARRAY_SIZE; i++)
  {
    const char *random_name_str = random_name();
    (*str_array[i]) = (const void *)random_name_str;

    ASSERT_TRUE(strcmp((const char *)str_array[i]->get_address(),
                       random_name_str) == 0,
                "Should be same (%s) and (%s)",
                (const char *)str_array[i]->get_address(),
                random_name_str);
    ASSERT_OK;

    if (i != (STRING_ARRAY_SIZE - 1))
    {
      str += (*str_array[i]);
      char separator = ' ';
      box_data box_char(BOX_DATA_CHAR, (void *)&separator);
      str += box_char;
    }
    else
    {
      str += (*str_array[i]);
    }
    ASSERT_OK;
  }

  box_data str_array_str = str_array.to_string();

  ASSERT_TRUE(str == str_array_str,
              "str (%s) and str_array.to_string() (%s) should be the same",
              (const char *)str.get_address(),
              (const char *)str_array_str.get_address());

  printf("\t-> %s()::OK\n", __FUNCTION__);
#undef STRING_ARRAY_SIZE
}

/**
 * Test BOX array.
 */
void
box_array_test()
{
  printf("%s()\r\n", __FUNCTION__);
  box_array_test_basic();
  box_array_test_int<int16_t>(BOX_DATA_SHORT);
  box_array_test_int<int32_t>(BOX_DATA_INT);
  box_array_test_int<int64_t>(BOX_DATA_LONG);
  box_array_test_float();
  box_array_test_double();
  box_array_test_char();
  box_array_test_string();
  printf("\r\n\r\n");
}
