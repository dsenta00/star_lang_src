#ifndef BOX_ASSERT_H
#define BOX_ASSERT_H

#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>

void assert_true(bool statement,
                 const char *file,
                 int line,
                 const char *fmt,
                 ...);

void assert_false(bool statement,
                  const char *file,
                  int line,
                  const char *fmt,
                  ...);

#define ASSERT_TRUE(__statement__, __fmt__, ...) \
  assert_true(__statement__, __FILE__, __LINE__, __fmt__, ##__VA_ARGS__)

#define ASSERT_FALSE(__statement__, __fmt__, ...) \
  assert_false(__statement__, __FILE__, __LINE__, __fmt__, ##__VA_ARGS__)

#define ASSERT_OK \
  ASSERT_TRUE(BOX_OK, "BOX should be OK. (%s)", BOX_LAST_ERROR_STRING)

#define ASSERT_ERROR(__error__) \
  ASSERT_TRUE(BOX_LAST_ERROR == __error__, \
              "BOX_LAST_ERROR != " #__error__ " (%s)", \
              BOX_LAST_ERROR_STRING)

#endif // BOX_ASSERT_H
