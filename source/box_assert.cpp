#include "box_assert.h"

/**
 * Check if statement is true.
 *
 * @param statement - the statement.
 * @param file - file where statement is checked.
 * @param line - line where statement is checked.
 * @param fmt - string format to write if statement is false.
 */
void
assert_true(bool statement,
            const char *file,
            int line,
            const char *fmt,
            ...)
{
  if (!statement)
  {
    va_list arg;

    printf("[%s:%d] -> ", file, line);

    va_start(arg, fmt);
    printf("  ");
    vfprintf(stdout, fmt, arg);
    va_end(arg);

    printf("\n");
    exit(0);
  }
}

/**
 * Check if statement is false.
 *
 * @param statement - the statement.
 * @param file - file where statement is checked.
 * @param line - line where statement is checked.
 * @param fmt - string format to write if statement is true.
 */
void
assert_false(bool statement,
             const char *file,
             int line,
             const char *fmt,
             ...)
{
  if (statement)
  {
    va_list arg;

    printf("[%s:%d] -> ", file, line);

    va_start(arg, fmt);
    printf("  ");
    vfprintf(stdout, fmt, arg);
    va_end(arg);

    printf("\n");
    exit(0);
  }
}