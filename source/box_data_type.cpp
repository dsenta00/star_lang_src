#ifndef BOX_DATA_TYPE_CPP
#define BOX_DATA_TYPE_CPP

#include "box_data_type.h"

box_data_type get_from_token(std::string &str)
{
  if (str == "bool")
  {
    return BOX_DATA_BOOL;
  }
  else if (str == "char")
  {
    return BOX_DATA_CHAR;
  }
  else if (str == "int")
  {
    return BOX_DATA_INT;
  }
  else if (str == "float")
  {
    return BOX_DATA_FLOAT;
  }
  else if (str == "string")
  {
    return BOX_DATA_STRING;
  }
  else
  {
    return BOX_DATA_INVALID;
  }
}

#endif // BOX_DATA_TYPE_CPP
