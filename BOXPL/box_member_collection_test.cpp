#include "box_member_collection_test.h"
#include "box_member_collection.h"
#include "box_assert.h"
#include <limits.h>

static void box_member_collection_test_basic()
{
  box_member_collection structure;

  ASSERT_OK;

  for (box_member_collection_key key = 0; key < USHRT_MAX; key++)
  {
    ASSERT_TRUE(structure[key] == NULL,
                "Should not fetch data from structure!");
    ASSERT_ERROR(ERROR_BOX_MEMBER_COLLECTION_INVALID_MEMBER);
    BOX_ERROR_CLEAR;
  }

  structure.add(0, NULL);
  ASSERT_ERROR(ERROR_BOX_MEMBER_COLLECTION_INVALID_DATA_MEMBER);
  BOX_ERROR_CLEAR;

  ASSERT_TRUE(structure[0] == NULL, "Should not fetch data from structure!");
  ASSERT_ERROR(ERROR_BOX_MEMBER_COLLECTION_INVALID_MEMBER);
  BOX_ERROR_CLEAR;

  int int_data = 10;
  structure.add(0, new box_data_member(new box_data(BOX_DATA_INT, &int_data)));
  ASSERT_OK;

  ASSERT_TRUE(structure[0] != NULL, "Should fetch data from structure!");
  ASSERT_OK;

  float float_data = 11.0;
  structure.add(0, new box_data_member(new box_data(BOX_DATA_FLOAT, &float_data)));
  ASSERT_ERROR(ERROR_BOX_MEMBER_COLLECTION_DUPLICATE_KEY);
  BOX_ERROR_CLEAR;

  structure.add(1, new box_data_member(new box_data(BOX_DATA_FLOAT, &float_data)));
  ASSERT_OK;

  printf("\t-> %s()::OK\n", __FUNCTION__);
}

void box_member_collection_test()
{
  printf("%s()\r\n", __FUNCTION__);
  box_member_collection_test_basic();
  printf("\r\n\r\n");
}
