#ifndef BOX_DATA_TYPE_H
#define BOX_DATA_TYPE_H
#include <stdint.h>
#include <stdbool.h>
#include <string>

typedef float float32_t;
typedef double float64_t;

/**
 * BOX data type enumeration.
 *
 * @value BOX_DATA_BOOL    - represents 8 bit boolean.
 * @value BOX_DATA_CHAR    - represents 8 bit character type.
 * @value BOX_DATA_SHORT   - represents 16 bit signed type.
 * @value BOX_DATA_INT     - represents 32 bit signed type.
 * @value BOX_DATA_FLOAT   - represents 32 bit floating point type.
 * @value BOX_DATA_LONG    - represents 64 bit signed type.
 * @value BOX_DATA_DOUBLE  - represents 64 bit floating point type.
 * @value BOX_DATA_STRING  - represents string type (the character array
 *                           that ends with value of 0)
 * @value BOX_DATA_INVALID - invalid type.
 */
typedef enum {
  BOX_DATA_BOOL,
  BOX_DATA_CHAR,
  BOX_DATA_SHORT,
  BOX_DATA_INT,
  BOX_DATA_FLOAT,
  BOX_DATA_LONG,
  BOX_DATA_DOUBLE,
  BOX_DATA_STRING,
  BOX_DATA_INVALID
} box_data_type;

const char box_data_type_string[][64] =
{
  "BOX_DATA_BOOL"
  "BOX_DATA_CHAR",
  "BOX_DATA_SHORT",
  "BOX_DATA_INT",
  "BOX_DATA_FLOAT",
  "BOX_DATA_LONG",
  "BOX_DATA_DOUBLE",
  "BOX_DATA_STRING",
  "BOX_DATA_INVALID"
};

const char BOX_DATA_TYPE_FORMAT[][8] =
{
  "%d",  // BOX_DATA_BOOL,
  "%c",  // BOX_DATA_CHAR,
  "%hd", // BOX_DATA_SHORT,
  "%d",  // BOX_DATA_INT,
  "%f",  // BOX_DATA_FLOAT,
  "%ld", // BOX_DATA_LONG,
  "%lf", // BOX_DATA_DOUBLE,
  "%s",  // BOX_DATA_STRING,
  ""     // BOX_DATA_INVALID
};

const uint8_t BOX_DATA_TYPE_SIZE[] =
{
  sizeof(bool),      // BOX_DATA_BOOL,
  sizeof(int8_t),    // BOX_DATA_CHAR,
  sizeof(int16_t),   // BOX_DATA_SHORT,
  sizeof(int32_t),   // BOX_DATA_INT,
  sizeof(float32_t), // BOX_DATA_FLOAT,
  sizeof(int64_t),   // BOX_DATA_LONG,
  sizeof(float64_t), // BOX_DATA_DOUBLE,
  8,                 // BOX_DATA_STRING, default size
  0                  // BOX_DATA_INVALID
};

box_data_type get_from_token(std::string &str);

#endif // BOX_DATA_TYPE_H
