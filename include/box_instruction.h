#ifndef BOX_INSTRUCTION_H
#define BOX_INSTRUCTION_H

#include "ORM/entity.h"
#include "ORM/orm_fw.h"

typedef enum {
  CREATE,
  CREATE_AND_ASSIGN_CONSTANT,
  CREATE_AND_ASSIGN_OBJECT,
  POP_AND_STORE
} box_op_code;

#define MAX_NO_OF_ARGUMENTS (16)

/**
 * @brief The instruction class
 */
class instruction : public entity {
protected:
  box_op_code op_code;
  std::string arg[MAX_NO_OF_ARGUMENTS];

  void create();
  void create_and_assign_constant();
  void create_and_assign_object();
  void pop_and_store();

public:
  instruction(box_op_code op_code,
              instruction *next_instruction = nullptr,
              instruction *branch_result_false = nullptr);

  box_op_code &get_op_code();

  instruction *execute();
};

#endif // BOX_INSTRUCTION_H
