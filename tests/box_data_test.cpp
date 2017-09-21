#include "box_data.h"
#include "box_data_test.h"
#include "box_assert.h"
#include "box_monitor.h"
#include "box_virtual_memory.h"
#include "ORM/orm.h"
#include "memory.h"

static box_virtual_memory *virtual_memory;

#define ASSERT_VIRTUAL_MEMORY(__BYTES__) \
  ASSERT_TRUE(virtual_memory->get_allocated_total() == (__BYTES__), \
  "Total allocated should be %u (%u)", \
  (__BYTES__), \
  virtual_memory->get_allocated_total())

#define BOX_TEST(__test__) \
  printf("\t-> " #__test__ "()::Start\r\n"); \
  clear_vm(); \
  __test__(); \
  BOX_OK;

static box_data &
alloc_box_data(std::string id, box_data_type type = BOX_DATA_INVALID, const void *value = NULL)
{
  return *(box_data *)orm::create((entity *)new box_data(id, type, value));
}

static void clear_vm()
{
  if (virtual_memory)
  {
    orm::destroy(virtual_memory);
  }

  virtual_memory =
      (box_virtual_memory *)orm::create((entity *)new box_virtual_memory(CHUNK_MINIMUM_CAPACITY));
}


/**
 * Test data as float.
 */
static void
box_data_test_float()
{
  BOX_ERROR_CLEAR;
  ASSERT_VIRTUAL_MEMORY(0);

  float fnum = 65.0f;
  box_data &float_data = alloc_box_data("float_data", BOX_DATA_FLOAT, &fnum);
  ASSERT_VIRTUAL_MEMORY(sizeof(float));

  ASSERT_OK;

  ASSERT_TRUE(++float_data, "float_data should be incremented!");
  ASSERT_OK;
  ASSERT_TRUE(float_data.to_float() == 66.0f, "float_data should be 66.0f!");

  ASSERT_TRUE(--float_data, "int_data should be incremented!");
  ASSERT_OK;
  ASSERT_TRUE(float_data.to_float() == 65.0f, "int_data should be 65.0f!");

  ASSERT_TRUE(float_data.to_char() == 'A',
              "float_data to char() -> %c instead of %c",
              float_data.to_char(), 'A');
  ASSERT_OK;

  ASSERT_TRUE(float_data.to_short() == 65,
              "float_data to short() -> %hd instead of %hd",
              float_data.to_short(), 65);
  ASSERT_OK;

  ASSERT_TRUE(float_data.to_int() == 65,
              "float_data to float() -> %d instead of %d",
              float_data.to_int(), 65);
  ASSERT_OK;

  ASSERT_TRUE(float_data.to_long() == 65,
              "float_data to long() -> %ld instead of %ld",
              float_data.to_int(), 65);
  ASSERT_OK;

  ASSERT_TRUE(float_data.to_double() == 65.0,
              "float_data to double() -> %lf instead of %lf",
              float_data.to_double(), 65.0);
  ASSERT_OK;

  box_data &str = float_data.to_string();
  ASSERT_TRUE(strcmp((const char *)str.get_address(), "65.000000") == 0,
              "string should be 65.000000! (%s)",
              (const char *)str.get_address());
  ASSERT_OK;

  ASSERT_VIRTUAL_MEMORY(sizeof(float) +
                        sizeof("65.000000"));

  int num = 35;
  box_data &int_data = alloc_box_data("int_data", BOX_DATA_INT, &num);

  ASSERT_VIRTUAL_MEMORY(sizeof(float) +
                        sizeof("65.000000") +
                        sizeof(int));

  ASSERT_FALSE(float_data == int_data,
               "float_data and int_data should not be equal");
  ASSERT_OK;

  ASSERT_TRUE(float_data != int_data,
              "float_data and int_data should not be equal");
  ASSERT_OK;

  ASSERT_TRUE(float_data > int_data,
              "float_data should be bigger than int_data");
  ASSERT_OK;

  ASSERT_TRUE(float_data >= int_data,
              "float_data should be bigger or equal than int_data");
  ASSERT_OK;

  ASSERT_FALSE(float_data < int_data,
               "float_data should be bigger than int_data");
  ASSERT_OK;

  ASSERT_FALSE(float_data <= int_data,
               "float_data should be bigger or equal than int_data");
  ASSERT_OK;

  ASSERT_TRUE(float_data += int_data,
              "float_data and int_data should add");
  ASSERT_TRUE(float_data.to_float() == 100.0f, "int_data should be 100.0f!");

  ASSERT_TRUE(float_data -= int_data, "float_data and int_data should substract");
  ASSERT_TRUE(float_data.to_float() == 65.0f, "int_data should be 65.0f!");

  ASSERT_TRUE(float_data *= int_data, "float_data and int_data should multiply");
  ASSERT_TRUE(float_data.to_float() == 2275.0f, "int_data should be 2275.0f!");

  ASSERT_TRUE(float_data /= int_data, "float_data and int_data should divide");
  ASSERT_TRUE(float_data.to_float() == 65.0f, "int_data should be 65.0f!");

  ASSERT_FALSE(float_data %= int_data, "float_data and int_data shouldn't mod");
  ASSERT_TRUE(float_data.to_float() == 65.0f, "int_data should be 65.0f!");
  ASSERT_ERROR(ERROR_BOX_DATA_INVALID_MODULUS);
  BOX_ERROR_CLEAR;

  /*
   * Putting int_data2 to value of zero.
   */
  num = 0;
  int_data = &num;

  ASSERT_FALSE(float_data /= int_data, "int_data and int_data2 shouldn't divide");
  ASSERT_TRUE(float_data.to_float() == 65.0f, "int_data should be 65.0f");
  ASSERT_ERROR(ERROR_BOX_DATA_DIVIDING_ZERO);
  BOX_ERROR_CLEAR;

  ASSERT_FALSE(float_data %= int_data, "int_data and int_data2 shouldn't mod");
  ASSERT_TRUE(float_data.to_int() == 65.0f, "int_data should be 65.0f!");
  ASSERT_ERROR(ERROR_BOX_DATA_INVALID_MODULUS);
  BOX_ERROR_CLEAR;

  float fnum2 = 35.0f;
  box_data &float_data2 = alloc_box_data("float_data2", BOX_DATA_FLOAT, &fnum2);

  ASSERT_VIRTUAL_MEMORY(sizeof(float) +
                        sizeof("65.000000") +
                        sizeof(int) +
                        sizeof(float));

  ASSERT_FALSE(float_data == float_data2,
               "float_data and float_data2 should not be equal");
  ASSERT_OK;

  ASSERT_TRUE(float_data != float_data2,
              "float_data and float_data2 should not be equal");
  ASSERT_OK;

  ASSERT_TRUE(float_data > float_data2,
              "float_data should be bigger than float_data2");
  ASSERT_OK;

  ASSERT_TRUE(float_data >= float_data2,
              "float_data should be bigger or equal than float_data2");
  ASSERT_OK;

  ASSERT_FALSE(float_data < float_data2,
               "float_data should be bigger than float_data2");
  ASSERT_OK;

  ASSERT_FALSE(float_data <= float_data2,
               "float_data should be bigger or equal than float_data2");
  ASSERT_OK;

  ASSERT_TRUE(float_data += float_data2,
              "float_data and float_data2 should add");
  ASSERT_TRUE(float_data.to_float() == 100.0f, "float_data should be 100.0f!");

  ASSERT_TRUE(float_data -= float_data2,
              "float_data and float_data2 should substract");
  ASSERT_TRUE(float_data.to_float() == 65.0f, "float_data should be 65.0f!");

  ASSERT_TRUE(float_data *= float_data2,
              "float_data and float_data2 should multiply");
  ASSERT_TRUE(float_data.to_float() == 2275.0f, "float_data should be 2275.0f!");

  ASSERT_TRUE(float_data /= float_data2,
              "float_data and float_data2 should divide");
  ASSERT_TRUE(float_data.to_float() == 65.0f, "float_data should be 65.0f!");

  /*
   * setting float_data to zero value
   */
  fnum2 = 0.0;
  float_data2 = &fnum2;

  ASSERT_FALSE(float_data /= float_data2,
               "int_data and float_data shouldn't divide");
  ASSERT_TRUE(float_data.to_float() == 65.0f, "int_data should be 65.0f");
  ASSERT_ERROR(ERROR_BOX_DATA_DIVIDING_ZERO);
  BOX_ERROR_CLEAR;

  ASSERT_FALSE(float_data %= float_data, "int_data and float_data shouldn't mod");
  ASSERT_TRUE(float_data.to_float() == 65.0f, "int_data should be 65.0f");
  ASSERT_ERROR(ERROR_BOX_DATA_INVALID_MODULUS);
  BOX_ERROR_CLEAR;

  /* interaction with string */

  box_data &string_data = alloc_box_data("string_data", BOX_DATA_STRING, "35");

  ASSERT_VIRTUAL_MEMORY(sizeof(float) +
                        sizeof("65.000000") +
                        sizeof(int) +
                        sizeof(float) +
                        sizeof("35"));

  ASSERT_FALSE(float_data == string_data,
               "float_data and string_data should not be equal");
  ASSERT_OK;

  ASSERT_TRUE(float_data != string_data,
              "float_data and string_data should not be equal");
  ASSERT_OK;

  ASSERT_TRUE(float_data > string_data,
              "float_data should be bigger than string_data");
  ASSERT_OK;

  ASSERT_TRUE(float_data >= string_data,
              "float_data should be bigger or equal than string_data");
  ASSERT_OK;

  ASSERT_FALSE(float_data < string_data,
               "float_data should be bigger than string_data");
  ASSERT_OK;

  ASSERT_FALSE(float_data <= string_data,
               "float_data should be bigger or equal than string_data");
  ASSERT_OK;

  ASSERT_FALSE(float_data += string_data,
               "float_data and string_data shouldn't add");
  ASSERT_TRUE(float_data.to_float() == 65.0f, "float_data should be 65.0f!");
  ASSERT_ERROR(ERROR_BOX_DATA_ADDING_STRING);
  BOX_ERROR_CLEAR;

  ASSERT_FALSE(float_data -= string_data,
               "float_data and string_data shouldn't subtract");
  ASSERT_TRUE(float_data.to_float() == 65.0f, "float_data should be 65.0f!");
  ASSERT_ERROR(ERROR_BOX_DATA_SUBTRACTING_STRING);
  BOX_ERROR_CLEAR;

  ASSERT_FALSE(float_data *= string_data,
               "float_data and string_data shouldn't multiply");
  ASSERT_TRUE(float_data.to_float() == 65.0f, "float_data should be 65.0f!");
  ASSERT_ERROR(ERROR_BOX_DATA_MULTIPLYING_STRING);
  BOX_ERROR_CLEAR;

  ASSERT_FALSE(float_data /= string_data,
               "float_data and string_data shouldn't divide");
  ASSERT_TRUE(float_data.to_float() == 65.0f, "float_data should be 65.0f!");
  ASSERT_ERROR(ERROR_BOX_DATA_DIVIDING_STRING);
  BOX_ERROR_CLEAR;

  string_data = "35 Grupa Zana rules!";
  ASSERT_TRUE(strcmp((const char *)string_data.get_address(),
                     "35 Grupa Zana rules!") == 0,
              "string should be \"35 Grupa Zana rules!\" (%s)",
              (const char *)string_data.get_address());

  ASSERT_VIRTUAL_MEMORY(sizeof(float) +
                        sizeof("65.000000") +
                        sizeof(int) +
                        sizeof(float) +
                        sizeof("35 Grupa Zana rules!"));

  ASSERT_FALSE(float_data == string_data,
               "float_data and string_data should be equal");
  ASSERT_OK;

  ASSERT_TRUE(float_data != string_data,
              "float_data and string_data should not be equal");
  ASSERT_OK;

  ASSERT_TRUE(float_data > string_data,
              "float_data should be bigger than string_data");
  ASSERT_OK;

  ASSERT_TRUE(float_data >= string_data,
              "float_data should be bigger or equal than string_data");
  ASSERT_OK;

  ASSERT_FALSE(float_data < string_data,
               "float_data should be bigger than string_data");
  ASSERT_OK;

  ASSERT_FALSE(float_data <= string_data,
               "float_data should be bigger or equal than string_data");
  ASSERT_OK;

  ASSERT_FALSE(float_data += string_data,
               "float_data and string_data shouldn't add");
  ASSERT_TRUE(float_data.to_float() == 65.0f, "string_data should be 65.0f!");
  ASSERT_ERROR(ERROR_BOX_DATA_ADDING_STRING);
  BOX_ERROR_CLEAR;

  ASSERT_FALSE(float_data -= string_data,
               "float_data and string_data shouldn't substract");
  ASSERT_TRUE(float_data.to_float() == 65.0f, "float_data should be 65!.0f");
  ASSERT_ERROR(ERROR_BOX_DATA_SUBTRACTING_STRING);
  BOX_ERROR_CLEAR;

  ASSERT_FALSE(float_data *= string_data,
               "float_data and string_data shouldn't multiply");
  ASSERT_TRUE(float_data.to_float() == 65.0f, "float_data should be 65.0f!");
  ASSERT_ERROR(ERROR_BOX_DATA_MULTIPLYING_STRING);
  BOX_ERROR_CLEAR;

  ASSERT_FALSE(float_data /= string_data,
               "float_data and string_data shouldn't divide");
  ASSERT_TRUE(float_data.to_float() == 65.0f, "float_data should be 65.0f!");
  ASSERT_ERROR(ERROR_BOX_DATA_DIVIDING_STRING);
  BOX_ERROR_CLEAR;

  string_data = "0";

  ASSERT_FALSE(float_data /= string_data,
               "float_data and float_data shouldn't divide");
  ASSERT_TRUE(float_data.to_float() == 65.0f, "float_data should be 65.0f");
  ASSERT_ERROR(ERROR_BOX_DATA_DIVIDING_STRING);
  BOX_ERROR_CLEAR;

  ASSERT_FALSE(float_data %= string_data,
               "float_data and string_data shouldn't mod");
  ASSERT_TRUE(float_data.to_float() == 65.0f, "float_data should be 65.0f");
  ASSERT_ERROR(ERROR_BOX_DATA_INVALID_MODULUS);
  BOX_ERROR_CLEAR;

  ASSERT_FALSE(float_data = string_data,
               "string_data shouldn't assign to float_data");
  ASSERT_TRUE(float_data.to_float() == 65.0f, "float_data should be 65.0f");
  ASSERT_ERROR(ERROR_BOX_DATA_ASSIGNING_STRING);
  BOX_ERROR_CLEAR;

  ASSERT_TRUE(float_data = int_data, "int_data should assign to float_data");
  ASSERT_OK;

  ASSERT_TRUE(float_data.to_float() == 0.0f, "float_data should be 0.0f");
  ASSERT_OK;

  printf("\t-> %s()::OK\n", __FUNCTION__);
}

/**
 * Test data as string.
 */
static void
box_data_test_string()
{
  BOX_ERROR_CLEAR;
  ASSERT_VIRTUAL_MEMORY(0);

  box_data &empty_string = alloc_box_data("empty_string", BOX_DATA_STRING);
  ASSERT_OK;
  ASSERT_VIRTUAL_MEMORY(BOX_DATA_TYPE_SIZE[BOX_DATA_STRING]);
  ASSERT_TRUE(strcmp((const char *)empty_string.get_address(), "") == 0,
              "string_data should be empty!");

  box_data &string_data = alloc_box_data("string_data", BOX_DATA_STRING, "32");
  ASSERT_OK;
  ASSERT_VIRTUAL_MEMORY(BOX_DATA_TYPE_SIZE[BOX_DATA_STRING] +
                        sizeof("32"));

  ASSERT_FALSE(++string_data, "string_data shouldn't' be incremented!");
  BOX_ERROR(ERROR_BOX_DATA_INCREMENTING_STRING);
  BOX_ERROR_CLEAR;

  ASSERT_FALSE(--string_data, "string_data shouldn't' be decremented!");
  BOX_ERROR(ERROR_BOX_DATA_DECREMENTING_STRING);
  BOX_ERROR_CLEAR;

  ASSERT_TRUE(string_data.to_char() == '3',
              "int_data to char() -> %c instead of %c",
              string_data.to_char(),
              '3');
  ASSERT_OK;

  ASSERT_TRUE(string_data.to_short() == 32,
              "int_data to short() -> %hd instead of %hd",
              string_data.to_short(), 32);
  ASSERT_OK;

  ASSERT_TRUE(string_data.to_float() == 32.0f,
              "int_data to float() -> %f instead of %f",
              string_data.to_float(),
              32.0f);
  ASSERT_OK;

  ASSERT_TRUE(string_data.to_long() == 32,
              "int_data to long() -> %ld instead of %ld",
              string_data.to_long(), 32);
  ASSERT_OK;

  ASSERT_TRUE(string_data.to_double() == 32.0,
              "int_data to double() -> %lf instead of %lf",
              string_data.to_double(),
              32.0);
  ASSERT_OK;

  box_data &str = string_data.to_string();
  ASSERT_TRUE(strcmp((const char *)str.get_address(), "32") == 0,
              "string should be 32! (%s)",
              (const char *)str.get_address());
  ASSERT_OK;
  ASSERT_VIRTUAL_MEMORY(BOX_DATA_TYPE_SIZE[BOX_DATA_STRING] +
                        sizeof("32") +
                        sizeof("32"));

  box_data &string_data2 = alloc_box_data("string_data2", BOX_DATA_STRING, "31");
  ASSERT_VIRTUAL_MEMORY(BOX_DATA_TYPE_SIZE[BOX_DATA_STRING] +
                        sizeof("32") +
                        sizeof("32") +
                        sizeof("31"));

  ASSERT_FALSE(string_data == string_data2,
               "string_data and string_data2 should not be equal");
  ASSERT_OK;

  ASSERT_TRUE(string_data != string_data2,
              "string_data and string_data2 should not be equal");
  ASSERT_OK;

  ASSERT_TRUE(string_data > string_data2,
              "string_data should be bigger than string_data2");
  ASSERT_OK;

  ASSERT_TRUE(string_data >= string_data2,
              "string_data should be bigger or equal than string_data2");
  ASSERT_OK;

  ASSERT_FALSE(string_data < string_data2,
               "string_data should be bigger than string_data2");
  ASSERT_OK;

  ASSERT_FALSE(string_data <= string_data2,
               "string_data should be bigger or equal than string_data2");
  ASSERT_OK;

  ASSERT_VIRTUAL_MEMORY(BOX_DATA_TYPE_SIZE[BOX_DATA_STRING] +
                        sizeof("32") +
                        sizeof("32") +
                        sizeof("31"));

  ASSERT_TRUE(string_data += string_data2,
              "string_data and string_data2 should add");
  ASSERT_OK;
  ASSERT_TRUE(strcmp((const char *)string_data.get_address(), "3231") == 0,
              "string_data should be 3231! (%s)",
              (const char *)string_data.get_address());
  ASSERT_OK;

  ASSERT_VIRTUAL_MEMORY(BOX_DATA_TYPE_SIZE[BOX_DATA_STRING] +
                        sizeof("3231") +
                        sizeof("32") +
                        sizeof("31"));

  ASSERT_FALSE(string_data -= string_data2,
               "string_data and string_data2 shouldn't substract");
  ASSERT_TRUE(strcmp((const char *)string_data.get_address(), "3231") == 0,
              "string_data should be 3231!");
  BOX_ERROR(ERROR_BOX_DATA_SUBTRACTING_STRING);
  BOX_ERROR_CLEAR;

  ASSERT_FALSE(string_data *= string_data2,
               "string_data and string_data2 shouldn't multiply");
  ASSERT_TRUE(strcmp((const char *)string_data.get_address(), "3231") == 0,
              "string_data should be 3231!");
  BOX_ERROR(ERROR_BOX_DATA_MULTIPLYING_STRING);
  BOX_ERROR_CLEAR;

  ASSERT_FALSE(string_data /= string_data2,
               "string_data and string_data2 shouldn't divide");
  ASSERT_TRUE(strcmp((const char *)string_data.get_address(), "3231") == 0,
              "string_data should be 3231!");
  BOX_ERROR(ERROR_BOX_DATA_DIVIDING_STRING);
  BOX_ERROR_CLEAR;

  ASSERT_FALSE(string_data %= string_data2,
               "string_data and string_data2 shouldn't mod");
  ASSERT_TRUE(strcmp((const char *)string_data.get_address(), "3231") == 0,
              "string_data should be 3231!");
  BOX_ERROR(ERROR_BOX_DATA_INVALID_MODULUS);
  BOX_ERROR_CLEAR;

  int num = 31;
  box_data &int_data = alloc_box_data("int_data", BOX_DATA_INT, &num);

  ASSERT_VIRTUAL_MEMORY(BOX_DATA_TYPE_SIZE[BOX_DATA_STRING] +
                        sizeof("3231") +
                        sizeof("32") +
                        sizeof("31") +
                        BOX_DATA_TYPE_SIZE[BOX_DATA_INT]);

  ASSERT_FALSE(string_data == int_data,
               "string_data and int_data should not be equal");
  ASSERT_OK;

  ASSERT_TRUE(string_data != int_data,
              "string_data and int_data should not be equal");
  ASSERT_OK;

  ASSERT_TRUE(string_data > int_data,
              "string_data should be bigger than int_data");
  ASSERT_OK;

  ASSERT_TRUE(string_data >= int_data,
              "string_data should be bigger or equal than int_data");
  ASSERT_OK;

  ASSERT_FALSE(string_data < int_data,
               "string_data should be bigger than int_data");
  ASSERT_OK;

  ASSERT_FALSE(string_data <= int_data,
               "string_data should be bigger or equal than int_data");
  ASSERT_OK;

  ASSERT_TRUE(string_data += int_data, "string_data and int_data should add");
  ASSERT_TRUE(strcmp((const char *)string_data.get_address(), "323131") == 0,
              "string_data should be 323131! (%s)",
              (const char *)string_data.get_address());
  ASSERT_OK;

  ASSERT_VIRTUAL_MEMORY(BOX_DATA_TYPE_SIZE[BOX_DATA_STRING] +
                        sizeof("323131") +
                        sizeof("32") +
                        sizeof("31") +
                        BOX_DATA_TYPE_SIZE[BOX_DATA_INT]);

  ASSERT_FALSE(string_data -= int_data,
               "string_data and int_data shouldn't substract");
  ASSERT_TRUE(strcmp((const char *)string_data.get_address(), "323131") == 0,
              "string_data should be 323131!");
  BOX_ERROR(ERROR_BOX_DATA_SUBTRACTING_STRING);
  BOX_ERROR_CLEAR;

  ASSERT_FALSE(string_data *= int_data,
               "string_data and int_data shouldn't multiply");
  ASSERT_TRUE(strcmp((const char *)string_data.get_address(), "323131") == 0,
              "string_data should be 323131!");
  BOX_ERROR(ERROR_BOX_DATA_MULTIPLYING_STRING);
  BOX_ERROR_CLEAR;

  ASSERT_FALSE(string_data /= int_data, "string_data and int_data shouldn't divide");
  ASSERT_TRUE(strcmp((const char *)string_data.get_address(), "323131") == 0,
              "string_data should be 323131!");
  BOX_ERROR(ERROR_BOX_DATA_DIVIDING_STRING);
  BOX_ERROR_CLEAR;

  ASSERT_FALSE(string_data %= int_data, "string_data and int_data shouldn't mod");
  ASSERT_TRUE(strcmp((const char *)string_data.get_address(), "323131") == 0,
              "string_data should be 323131!");
  BOX_ERROR(ERROR_BOX_DATA_INVALID_MODULUS);
  BOX_ERROR_CLEAR;

  float fnum = 31.0f;
  box_data &float_data = alloc_box_data("float_data", BOX_DATA_FLOAT, &fnum);

  ASSERT_VIRTUAL_MEMORY(BOX_DATA_TYPE_SIZE[BOX_DATA_STRING] +
                        sizeof("323131") +
                        sizeof("32") +
                        sizeof("31") +
                        BOX_DATA_TYPE_SIZE[BOX_DATA_INT] +
                        BOX_DATA_TYPE_SIZE[BOX_DATA_FLOAT]);

  ASSERT_FALSE(string_data == float_data,
               "string_data and float_data should not be equal");
  ASSERT_OK;

  ASSERT_TRUE(string_data != float_data,
              "string_data and float_data should not be equal");
  ASSERT_OK;

  ASSERT_TRUE(string_data > float_data,
              "string_data should be bigger than float_data");
  ASSERT_OK;

  ASSERT_TRUE(string_data >= float_data,
              "string_data should be bigger or equal than float_data");
  ASSERT_OK;

  ASSERT_FALSE(string_data < float_data,
               "string_data should be bigger than float_data");
  ASSERT_OK;

  ASSERT_FALSE(string_data <= float_data,
               "string_data should be bigger or equal than float_data");
  ASSERT_OK;

  ASSERT_TRUE(string_data += float_data,
              "string_data and float_data should add");
  ASSERT_TRUE(strcmp((const char *)string_data.get_address(),
                     "32313131.000000") == 0,
              "string_data should be 32313131.000000! (%s)",
              (const char *)string_data.get_address());
  ASSERT_OK;

  ASSERT_VIRTUAL_MEMORY(BOX_DATA_TYPE_SIZE[BOX_DATA_STRING] +
                        sizeof("32313131.000000") +
                        sizeof("32") +
                        sizeof("31") +
                        BOX_DATA_TYPE_SIZE[BOX_DATA_INT] +
                        BOX_DATA_TYPE_SIZE[BOX_DATA_FLOAT]);

  ASSERT_FALSE(string_data -= float_data,
               "string_data and float_data shouldn't substract");
  ASSERT_TRUE(strcmp((const char *)string_data.get_address(), "32313131.000000") == 0,
              "string_data should be 32313131.000000!");
  BOX_ERROR(ERROR_BOX_DATA_SUBTRACTING_STRING);
  BOX_ERROR_CLEAR;

  ASSERT_FALSE(string_data *= float_data,
               "string_data and float_data shouldn't multiply");
  ASSERT_TRUE(strcmp((const char *)string_data.get_address(), "32313131.000000") == 0,
              "string_data should be 32313131.000000!");
  BOX_ERROR(ERROR_BOX_DATA_MULTIPLYING_STRING);
  BOX_ERROR_CLEAR;

  ASSERT_FALSE(string_data /= float_data,
               "string_data and float_data shouldn't divide");
  ASSERT_TRUE(strcmp((const char *)string_data.get_address(),
                     "32313131.000000") == 0,
              "string_data should be 32313131.000000!");
  BOX_ERROR(ERROR_BOX_DATA_DIVIDING_STRING);
  BOX_ERROR_CLEAR;

  ASSERT_FALSE(string_data %= float_data,
               "string_data and float_data shouldn't mod");
  ASSERT_TRUE(strcmp((const char *)string_data.get_address(),
                     "32313131.000000") == 0,
              "string_data should be 32313131.000000!");
  BOX_ERROR(ERROR_BOX_DATA_INVALID_MODULUS);
  BOX_ERROR_CLEAR;

  ASSERT_TRUE(string_data = float_data,
              "float_data should be assigned to string_data");
  ASSERT_TRUE(strcmp((const char *)string_data.get_address(), "31.000000") == 0,
              "string_data should be 31.000000! (%s)",
              (const char *)string_data.get_address());
  ASSERT_OK;

  ASSERT_TRUE(string_data = int_data, "int_data should be assigned to string");
  ASSERT_TRUE(strcmp((const char *)string_data.get_address(), "31") == 0,
              "string_data should be 31! (%s)",
              (const char *)string_data.get_address());
  ASSERT_OK;

  ASSERT_VIRTUAL_MEMORY(BOX_DATA_TYPE_SIZE[BOX_DATA_STRING] +
                        sizeof("32313131.000000") + // still reserved
                        sizeof("32") +
                        sizeof("31") +
                        BOX_DATA_TYPE_SIZE[BOX_DATA_INT] +
                        BOX_DATA_TYPE_SIZE[BOX_DATA_FLOAT]);

  printf("\t-> %s()::OK\n", __FUNCTION__);
}

/**
 * Test integer data.
 */
static void
box_data_test_int()
{
  BOX_ERROR_CLEAR;
  int num = 65;
  box_data &int_data = alloc_box_data("int_data", BOX_DATA_INT, &num);

  ASSERT_OK;

  ASSERT_TRUE(++int_data, "int_data should be incremented!");
  ASSERT_OK;
  ASSERT_TRUE(int_data.to_int() == 66, "int_data should be 66!");

  ASSERT_TRUE(--int_data, "int_data should be incremented!");
  ASSERT_OK;
  ASSERT_TRUE(int_data.to_int() == 65, "int_data should be 65!");

  printf("debug -> ");
  ASSERT_TRUE(int_data.print(), "int_data should be printed!");
  printf("\n");
  ASSERT_OK;

  printf("debug -> ");
  ASSERT_TRUE(int_data.println(), "int_data should be printed!");
  ASSERT_OK;

  ASSERT_TRUE(int_data.to_char() == 'A',
              "int_data to char() -> %c instead of %c",
              int_data.to_char(),
              'A');
  ASSERT_OK;

  ASSERT_TRUE(int_data.to_short() == 65,
              "int_data to short() -> %hd instead of %hd",
              int_data.to_short(),
              65);
  ASSERT_OK;

  ASSERT_TRUE(int_data.to_float() == 65.0f,
              "int_data to float() -> %f instead of %f",
              int_data.to_float(),
              65.0f);
  ASSERT_OK;

  ASSERT_TRUE(int_data.to_long() == 65,
              "int_data to long() -> %ld instead of %ld",
              int_data.to_long(),
              65);
  ASSERT_OK;

  ASSERT_TRUE(int_data.to_double() == 65.0,
              "int_data to double() -> %lf instead of %lf",
              int_data.to_double(),
              65.0);
  ASSERT_OK;

  box_data &str = int_data.to_string();
  ASSERT_OK;
  ASSERT_TRUE(strcmp((const char *)str.get_address(), "65") == 0,
              "string should be 65! (%s)",
              (const char *)str.get_address());

  int num2 = 35;
  box_data &int_data2 = alloc_box_data("int_data", BOX_DATA_INT, &num2);

  ASSERT_FALSE(int_data == int_data2,
               "int_data and int_data2 should not be equal");
  ASSERT_OK;

  ASSERT_TRUE(int_data != int_data2,
              "int_data and int_data2 should not be equal");
  ASSERT_OK;

  ASSERT_TRUE(int_data > int_data2,
              "int_data should be bigger than int_data2");
  ASSERT_OK;

  ASSERT_TRUE(int_data >= int_data2,
              "int_data should be bigger or equal than int_data2");
  ASSERT_OK;

  ASSERT_FALSE(int_data < int_data2,
               "int_data should be bigger than int_data2");
  ASSERT_OK;

  ASSERT_FALSE(int_data <= int_data2,
               "int_data should be bigger or equal than int_data2");
  ASSERT_OK;

  ASSERT_TRUE(int_data += int_data2, "int_data and int_data2 should add");
  ASSERT_TRUE(int_data.to_int() == 100, "int_data should be 100!");

  ASSERT_TRUE(int_data -= int_data2, "int_data and int_data2 should substract");
  ASSERT_TRUE(int_data.to_int() == 65, "int_data should be 65!");

  ASSERT_TRUE(int_data *= int_data2, "int_data and int_data2 should multiply");
  ASSERT_TRUE(int_data.to_int() == 2275, "int_data should be 2275!");

  ASSERT_TRUE(int_data /= int_data2, "int_data and int_data2 should divide");
  ASSERT_TRUE(int_data.to_int() == 65, "int_data should be 65!");

  ASSERT_TRUE(int_data %= int_data2, "int_data and int_data2 should mod");
  ASSERT_TRUE(int_data.to_int() == 30, "int_data should be 30!");

  /*
   * Putting int_data2 to value of zero.
   */
  int_data2.default_value();

  ASSERT_FALSE(int_data /= int_data2,
               "int_data and int_data2 shouldn't divide");
  ASSERT_TRUE(int_data.to_int() == 30,
              "int_data should be 30",
              int_data.to_int());
  BOX_ERROR(ERROR_BOX_DATA_DIVIDING_ZERO);
  BOX_ERROR_CLEAR;

  ASSERT_FALSE(int_data %= int_data2, "int_data and int_data2 shouldn't mod");
  ASSERT_TRUE(int_data.to_int() == 30, "int_data should be 30!");
  BOX_ERROR(ERROR_BOX_DATA_MODULUS_ZERO);
  BOX_ERROR_CLEAR;

  /*
   * Putting int_data to value of 65.
   */
  *(int *)int_data.get_address() = 65;

  float fnum = 35.0f;
  box_data &float_data = alloc_box_data("float_data", BOX_DATA_FLOAT, &fnum);

  ASSERT_FALSE(int_data == float_data,
               "int_data and float_data should not be equal");
  ASSERT_OK;

  ASSERT_TRUE(int_data != float_data,
              "int_data and float_data should not be equal");
  ASSERT_OK;

  ASSERT_TRUE(int_data > float_data,
              "int_data should be bigger than float_data");
  ASSERT_OK;

  ASSERT_TRUE(int_data >= float_data,
              "int_data should be bigger or equal than float_data");
  ASSERT_OK;

  ASSERT_FALSE(int_data < float_data,
               "int_data should be bigger than float_data");
  ASSERT_OK;

  ASSERT_FALSE(int_data <= float_data,
               "int_data should be bigger or equal than float_data");
  ASSERT_OK;

  ASSERT_TRUE(int_data += float_data, "int_data and float_data should add");
  ASSERT_TRUE(int_data.to_int() == 100, "int_data should be 100!");

  ASSERT_TRUE(int_data -= float_data, "int_data and float_data should substract");
  ASSERT_TRUE(int_data.to_int() == 65, "int_data should be 65!");

  ASSERT_TRUE(int_data *= float_data, "int_data and float_data should multiply");
  ASSERT_TRUE(int_data.to_int() == 2275, "int_data should be 2275!");

  ASSERT_TRUE(int_data /= float_data, "int_data and float_data should divide");
  ASSERT_TRUE(int_data.to_int() == 65, "int_data should be 65!");

  /*
   * setting float_data to zero value
   */
  *(float *)float_data.get_address() = 0.0f;

  ASSERT_FALSE(int_data /= float_data,
               "int_data and float_data shouldn't divide");
  ASSERT_TRUE(int_data.to_int() == 65,
              "int_data should be 65",
              int_data.to_int());
  BOX_ERROR(ERROR_BOX_DATA_DIVIDING_ZERO);
  BOX_ERROR_CLEAR;

  ASSERT_FALSE(int_data %= float_data, "int_data and float_data shouldn't mod");
  ASSERT_TRUE(int_data.to_int() == 65, "int_data should be 65");
  BOX_ERROR(ERROR_BOX_DATA_INVALID_MODULUS);
  BOX_ERROR_CLEAR;

  /* interaction with string */

  box_data &string_data = alloc_box_data("string_data", BOX_DATA_STRING, "35");

  ASSERT_FALSE(int_data == string_data,
               "int_data and string_data should not be equal");
  ASSERT_OK;

  ASSERT_TRUE(int_data != string_data,
              "int_data and string_data should not be equal");
  ASSERT_OK;

  ASSERT_TRUE(int_data > string_data,
              "int_data should be bigger than string_data");
  ASSERT_OK;

  ASSERT_TRUE(int_data >= string_data,
              "int_data should be bigger or equal than string_data");
  ASSERT_OK;

  ASSERT_FALSE(int_data < string_data,
               "int_data should be bigger than string_data");
  ASSERT_OK;

  ASSERT_FALSE(int_data <= string_data,
               "int_data should be bigger or equal than string_data");
  ASSERT_OK;

  ASSERT_FALSE(int_data += string_data,
               "int_data and string_data shouldn't add");
  ASSERT_TRUE(int_data.to_int() == 65, "string_data should be 65!");
  ASSERT_ERROR(ERROR_BOX_DATA_ADDING_STRING);
  BOX_ERROR_CLEAR;

  ASSERT_FALSE(int_data -= string_data,
               "int_data and string_data shouldn't substract");
  ASSERT_TRUE(int_data.to_int() == 65, "int_data should be 65!");
  ASSERT_ERROR(ERROR_BOX_DATA_SUBTRACTING_STRING);
  BOX_ERROR_CLEAR;

  ASSERT_FALSE(int_data *= string_data, "int_data and string_data shouldn't multiply");
  ASSERT_TRUE(int_data.to_int() == 65, "int_data should be 65!");
  ASSERT_ERROR(ERROR_BOX_DATA_MULTIPLYING_STRING);
  BOX_ERROR_CLEAR;

  ASSERT_FALSE(int_data /= string_data,
               "int_data and string_data shouldn't divide");
  ASSERT_TRUE(int_data.to_int() == 65, "int_data should be 65!");
  ASSERT_ERROR(ERROR_BOX_DATA_DIVIDING_STRING);
  BOX_ERROR_CLEAR;

  string_data = "35 Grupa Zana rules!";
  ASSERT_OK;

  ASSERT_FALSE(int_data == string_data,
               "int_data and string_data should not be equal");
  ASSERT_OK;

  ASSERT_TRUE(int_data != string_data,
              "int_data and string_data should not be equal");
  ASSERT_OK;

  ASSERT_TRUE(int_data > string_data,
              "int_data should be bigger than string_data");
  ASSERT_OK;

  ASSERT_TRUE(int_data >= string_data,
              "int_data should be bigger or equal than string_data");
  ASSERT_OK;

  ASSERT_FALSE(int_data < string_data,
               "int_data should be bigger than string_data");
  ASSERT_OK;

  ASSERT_FALSE(int_data <= string_data,
               "int_data should be bigger or equal than string_data");
  ASSERT_OK;

  ASSERT_FALSE(int_data += string_data,
               "int_data and string_data shouldn't add");
  ASSERT_TRUE(int_data.to_int() == 65, "string_data should be 65!");
  ASSERT_ERROR(ERROR_BOX_DATA_ADDING_STRING);
  BOX_ERROR_CLEAR;

  ASSERT_FALSE(int_data -= string_data,
               "int_data and string_data shouldn't substract");
  ASSERT_TRUE(int_data.to_int() == 65, "int_data should be 65!");
  ASSERT_ERROR(ERROR_BOX_DATA_SUBTRACTING_STRING);
  BOX_ERROR_CLEAR;

  ASSERT_FALSE(int_data *= string_data,
               "int_data and string_data shouldn't multiply");
  ASSERT_TRUE(int_data.to_int() == 65, "int_data should be 65!");
  ASSERT_ERROR(ERROR_BOX_DATA_MULTIPLYING_STRING);
  BOX_ERROR_CLEAR;

  ASSERT_FALSE(int_data /= string_data,
               "int_data and string_data shouldn't divide");
  ASSERT_TRUE(int_data.to_int() == 65, "int_data should be 65!");
  ASSERT_ERROR(ERROR_BOX_DATA_DIVIDING_STRING);
  BOX_ERROR_CLEAR;

  string_data = "0";

  ASSERT_FALSE(int_data /= string_data,
               "int_data and float_data shouldn't divide");
  ASSERT_ERROR(ERROR_BOX_DATA_DIVIDING_STRING);
  ASSERT_TRUE(int_data.to_int() == 65, "int_data should be 65");
  BOX_ERROR_CLEAR;

  ASSERT_FALSE(int_data %= string_data,
               "int_data and string_data shouldn't mod");
  ASSERT_ERROR(ERROR_BOX_DATA_INVALID_MODULUS);
  BOX_ERROR_CLEAR;

  ASSERT_FALSE(int_data = string_data,
               "string_data shouldn't assign to int_data");
  ASSERT_TRUE(int_data.to_int() == 65, "int_data should be 65");
  ASSERT_ERROR(ERROR_BOX_DATA_ASSIGNING_STRING);
  BOX_ERROR_CLEAR;

  ASSERT_TRUE(int_data = float_data, "float_data should assign to int_data");
  ASSERT_TRUE(int_data.to_int() == 0,
              "int_data should be %d",
              float_data.to_int());
  ASSERT_OK;

  int_data.print();

  orm::destroy(&int_data);
  BOX_OK;
  orm::destroy(&int_data2);
  BOX_OK;
  orm::destroy(&float_data);
  BOX_OK;
  orm::destroy(&string_data);
  BOX_OK;

  printf("\t-> %s()::OK\n", __FUNCTION__);
}

/**
 * Test basic data test.
 */
static void
box_data_test_basic()
{
  /* Completely invalid data */
  box_data &invalid_box_data = alloc_box_data("invalid_box_data");
  ASSERT_ERROR(ERROR_BOX_DATA_INVALID_DATA_TYPE);
  BOX_ERROR_CLEAR;

  ASSERT_FALSE((++invalid_box_data), "invalid_box_data shouldn't increase!");
  ASSERT_ERROR(ERROR_BOX_DATA_NULL_DATA);
  BOX_ERROR_CLEAR;

  ASSERT_FALSE((--invalid_box_data), "invalid_box_data shouldn't decrease!");
  ASSERT_ERROR(ERROR_BOX_DATA_NULL_DATA);
  BOX_ERROR_CLEAR;

  ASSERT_FALSE(invalid_box_data.print(), "invalid_box_data shouldn't print!");
  ASSERT_ERROR(ERROR_BOX_DATA_NULL_DATA);
  BOX_ERROR_CLEAR;

  ASSERT_FALSE(invalid_box_data.println(),
               "invalid_box_data shouldn't println!");
  ASSERT_ERROR(ERROR_BOX_DATA_NULL_DATA);
  BOX_ERROR_CLEAR;

  ASSERT_FALSE(invalid_box_data.scan(), "invalid_box_data shouldn't scan!");
  ASSERT_ERROR(ERROR_BOX_DATA_NULL_DATA);
  BOX_ERROR_CLEAR;

  ASSERT_TRUE(invalid_box_data.to_char() == 0,
              "invalid_box_data should return zero!");
  ASSERT_ERROR(ERROR_BOX_DATA_NULL_DATA);
  BOX_ERROR_CLEAR;

  ASSERT_TRUE(invalid_box_data.to_short() == 0,
              "invalid_box_data should return zero!");
  ASSERT_ERROR(ERROR_BOX_DATA_NULL_DATA);
  BOX_ERROR_CLEAR;

  ASSERT_TRUE(invalid_box_data.to_int() == 0,
              "invalid_box_data should return zero!");
  ASSERT_ERROR(ERROR_BOX_DATA_NULL_DATA);
  BOX_ERROR_CLEAR;

  ASSERT_TRUE(invalid_box_data.to_float() == 0.0f,
              "invalid_box_data should return zero!");
  ASSERT_ERROR(ERROR_BOX_DATA_NULL_DATA);
  BOX_ERROR_CLEAR;

  ASSERT_TRUE(invalid_box_data.to_long() == 0,
              "invalid_box_data should return zero!");
  ASSERT_ERROR(ERROR_BOX_DATA_NULL_DATA);
  BOX_ERROR_CLEAR;

  ASSERT_TRUE(invalid_box_data.to_double() == 0.0,
              "invalid_box_data should return zero!");
  ASSERT_ERROR(ERROR_BOX_DATA_NULL_DATA);
  BOX_ERROR_CLEAR;

  box_data &invalid_box_data_string = invalid_box_data.to_string();
  ASSERT_ERROR(ERROR_BOX_DATA_NULL_DATA);
  BOX_ERROR_CLEAR;

  int valid = 24;
  box_data &valid_data = alloc_box_data("valid_data", BOX_DATA_INT, &valid);
  ASSERT_OK;

  ASSERT_FALSE(valid_data = invalid_box_data,
               "valid_data and invalid should not assign!\n");
  ASSERT_ERROR(ERROR_BOX_DATA_NULL_DATA);
  BOX_ERROR_CLEAR;

  ASSERT_FALSE(valid_data += invalid_box_data,
               "valid_data and invalid should not add!\n");
  ASSERT_ERROR(ERROR_BOX_DATA_NULL_DATA);
  BOX_ERROR_CLEAR;

  orm::destroy(&invalid_box_data);
  BOX_OK;
  orm::destroy(&invalid_box_data_string);
  BOX_OK;
  orm::destroy(&valid_data);
  BOX_OK;
  printf("\t-> %s()::OK\n", __FUNCTION__);
}

/**
 * Test box data convert
 */
void box_data_test_convert()
{
  BOX_ERROR_CLEAR;
  box_data &string_data = alloc_box_data("string_data", BOX_DATA_STRING, "35");

  string_data.convert_itself(BOX_DATA_INT);
  ASSERT_OK;
  ASSERT_TRUE(*(int *)string_data.get_address() == 35, "data should be 35!");

  string_data.convert_itself(BOX_DATA_FLOAT);
  ASSERT_OK;
  ASSERT_TRUE(*(float *)string_data.get_address() == 35.0f, "data should be 35.0f!");

  string_data.convert_itself(BOX_DATA_STRING);
  ASSERT_OK;
  ASSERT_TRUE(strcmp((char *)string_data.get_address(), "35.000000") == 0,
              "data should be 35");

  printf("\t-> %s()::OK\n", __FUNCTION__);
}

/**
 * Test box data class.
 */
void box_data_test()
{
  printf("%s()\r\n", __FUNCTION__);

  virtual_memory = (box_virtual_memory *)orm::get_first("box_virtual_memory");

  BOX_TEST(box_data_test_basic);
  BOX_TEST(box_data_test_int);
  BOX_TEST(box_data_test_string);
  BOX_TEST(box_data_test_float);
  BOX_TEST(box_data_test_convert);
  printf("\r\n\r\n");
}
