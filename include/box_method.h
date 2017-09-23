#ifndef BOX_METHOD_H
#define BOX_METHOD_H

#include "ORM/entity.h"
#include "box_data_type.h"
#include "box_fw.h"

typedef enum {
  INSTRUCTION_OK,
  INSTRUCTION_ERROR,
  INSTRUCTION_FINISHED
} instruction_result;

/**
 * @brief The box_method class
 */
class box_method : public entity {
protected:
  entity *result;
  std::map<std::string, entity *> local_objects;
  std::vector<entity *> stack;
  instruction *current_instruction;
public:
  box_method(std::string id, std::vector<instruction *> &instructions);
  instruction_result execute_next();
  void add_local_object(entity *e);
  entity *get_local_object(std::string id);
  void push_stack(entity *e);
  entity *pop_stack();
};

#endif // BOX_METHOD_H
