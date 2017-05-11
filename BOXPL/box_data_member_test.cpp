#include "box_data_member_test.h"
#include "box_data_member.h"
#include "box_assert.h"
#include "box_data.h"
#include "box_array.h"
#include <limits.h>

/**
 * Test data member as array.
 */
static void
box_data_member_test_array_data()
{
#define ARRAY_SIZE (10)
  box_data_member box_if(new box_array(BOX_DATA_INT, ARRAY_SIZE));
  ASSERT_OK;

  for (uint32_t index = 0; index < ARRAY_SIZE; index++)
  {
    ASSERT_TRUE(box_if.get_data(index) != NULL,
                "Data should be fetched from box_if");
    ASSERT_OK;
  }

  ASSERT_TRUE(box_if.get_data() == NULL, "Data shouldn't' be fetched!");
  ASSERT_ERROR(ERROR_BOX_DATA_MEMBER_ARRAY_AS_DATA);
  BOX_ERROR_CLEAR;

  for (uint32_t index = 0; index < ARRAY_SIZE; index++)
  {
    *(int32_t *)(box_if.get_data(index)->get_address()) = index;
  }

  box_data str = box_if.to_string();
  ASSERT_TRUE(strcmp((const char *)str.get_address(),
                     "0 1 2 3 4 5 6 7 8 9") == 0,
              "String should be \"0 1 2 3 4 5 6 7 8 9\"! (%s)",
              (const char *)str.get_address());
  ASSERT_OK;

  box_if.clean();

  ASSERT_TRUE(box_if.get_data() == NULL,
              "Data should not be fetched from box_if");
  ASSERT_ERROR(ERROR_BOX_DATA_MEMBER_NULL_REFERENCE);
  BOX_ERROR_CLEAR;

  for (uint32_t index = 0; index < USHRT_MAX; index++)
  {
    ASSERT_TRUE(box_if.get_data(index) == NULL,
                "Data should not be fetched from box_if");
    ASSERT_ERROR(ERROR_BOX_DATA_MEMBER_NULL_REFERENCE);
    BOX_ERROR_CLEAR;
  }

  printf("\t-> %s()::OK\n", __FUNCTION__);
}

/**
 * Test data member as primary data.
 */
static void
box_data_member_test_primary_data()
{
  int int_data = 5;
  box_data_member box_if(new box_data(BOX_DATA_INT, &int_data));
  ASSERT_OK;

  ASSERT_TRUE(box_if.get_data() != NULL,
              "Data should be fetched!");
  ASSERT_OK;

  box_data str = box_if.to_string();
  ASSERT_TRUE(strcmp((const char *)str.get_address(), "5") == 0,
              "Number should be 5! (%s)",
              (const char *)str.get_address());
  ASSERT_OK;

  for (uint32_t index = 0; index < USHRT_MAX; index++)
  {
    ASSERT_TRUE(box_if.get_data(index) == NULL,
                "Data should not be fetched from box_if");
    ASSERT_ERROR(ERROR_BOX_DATA_MEMBER_DATA_AS_ARRAY);
    BOX_ERROR_CLEAR;
  }

  box_if.clean();

  ASSERT_TRUE(box_if.get_data() == NULL,
              "Data should not be fetched from box_if");
  ASSERT_ERROR(ERROR_BOX_DATA_MEMBER_NULL_REFERENCE);
  BOX_ERROR_CLEAR;

  ASSERT_TRUE(strcmp((const char *)box_if.to_string().get_address(),
                     "") == 0,
              "Empty string should be fetched from box_if");
  ASSERT_ERROR(ERROR_BOX_DATA_MEMBER_NULL_REFERENCE);
  BOX_ERROR_CLEAR;

  for (uint32_t index = 0; index < USHRT_MAX; index++)
  {
    ASSERT_TRUE(box_if.get_data(index) == NULL,
                "Data should not be fetched from box_if");
    ASSERT_ERROR(ERROR_BOX_DATA_MEMBER_NULL_REFERENCE);
    BOX_ERROR_CLEAR;
  }

  printf("\t-> %s()::OK\n", __FUNCTION__);
}

/**
 * Test data member basics.
 */
static void
box_data_member_test_basic()
{
  box_data_member box_if_invalid;
  ASSERT_ERROR(ERROR_BOX_DATA_MEMBER_NULL_REFERENCE);
  BOX_ERROR_CLEAR;

  ASSERT_TRUE(box_if_invalid.get_data() == NULL,
              "Data should not be fetched from box_if_invalid");
  ASSERT_ERROR(ERROR_BOX_DATA_MEMBER_NULL_REFERENCE);
  BOX_ERROR_CLEAR;

  box_if_invalid.to_string();
  ASSERT_ERROR(ERROR_BOX_DATA_MEMBER_NULL_REFERENCE);
  BOX_ERROR_CLEAR;

  for (uint32_t index = 0; index < USHRT_MAX; index++)
  {
    ASSERT_TRUE(box_if_invalid.get_data(index) == NULL,
                "Data should not be fetched from box_if_invalid");
    ASSERT_ERROR(ERROR_BOX_DATA_MEMBER_NULL_REFERENCE);
    BOX_ERROR_CLEAR;
  }

  box_if_invalid.clean();

  printf("\t-> %s()::OK\n", __FUNCTION__);
}

/**
 * Test data member class.
 */
void box_data_member_test()
{
  printf("%s()\r\n", __FUNCTION__);
  box_data_member_test_basic();
  box_data_member_test_primary_data();
  box_data_member_test_array_data();
  printf("\r\n\r\n");
}
