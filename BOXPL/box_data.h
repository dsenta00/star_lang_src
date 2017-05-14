#ifndef BOX_DATA_H
#define BOX_DATA_H
#include <stdint.h>
#include <stdbool.h>
#include "box_monitor.h"
#include "box_virtual_memory.h"

/**
 * BOX data type enumeration.
 *
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
  sizeof(int8_t),    // BOX_DATA_CHAR,
  sizeof(int16_t),   // BOX_DATA_SHORT,
  sizeof(int32_t),   // BOX_DATA_INT,
  sizeof(float32_t), // BOX_DATA_FLOAT,
  sizeof(int64_t),   // BOX_DATA_LONG,
  sizeof(float64_t), // BOX_DATA_DOUBLE,
  8,                 // BOX_DATA_STRING, default size
  0                  // BOX_DATA_INVALID
};

/**
 * The box_data class. Represents data in BOX PL and defines
 * all operations in that scope.
 */
class box_data {
public:
  box_data();
  box_data(box_data_type type);
  box_data(box_data_type type, const void *value);
  box_data(const box_data &data);

  const char *get_type_str();

  int8_t to_char();
  int16_t to_short();
  int32_t to_int();
  float32_t to_float();
  int64_t to_long();
  float64_t to_double();
  box_data to_string();

  bool default_value();

  bool operator =  (const void *data);
  bool operator =  (box_data &data);
  bool operator += (box_data &data);
  bool operator -= (box_data &data);
  bool operator *= (box_data &data);
  bool operator /= (box_data &data);
  bool operator %= (box_data &data);
  bool operator ++ ();
  bool operator -- ();
  bool operator == (box_data &data);
  bool operator != (box_data &data);
  bool operator >  (box_data &data);
  bool operator <  (box_data &data);
  bool operator >= (box_data &data);
  bool operator <= (box_data &data);

  uintptr_t get_address();
  memory *get_memory();
  box_data_type get_type();

  bool print();
  bool println();
  bool scan();

  ~box_data();
protected:
  const char *get_string();

  box_data_type type;
  memory *mem;
};

#endif // BOX_DATA_H
