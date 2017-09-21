#include "box_instruction.h"
#include "ORM/relationship.h"
#include "ORM/orm.h"
#include "box_method.h"
#include "box_data.h"
#include "box_monitor.h"

instruction::instruction(box_op_code op_code,
                         instruction *next_instruction,
                         instruction *branch_result_false) : entity::entity("instruction", 0)
{
  this->op_code = op_code;

  this->add_relationship("method_instructions", MANY_TO_ONE);
  this->add_relationship("next_instruction", ONE_TO_ONE);
  this->add_relationship("branch_result_false", ONE_TO_ONE);
  this->add_relationship("first_operand", ONE_TO_ONE);
  this->add_relationship("second_operand", ONE_TO_ONE);

  if (next_instruction)
  {
    relationship *r = this->get_relationship("next_instruction");
    r->add_entity(next_instruction);
  }

  if (branch_result_false)
  {
    relationship *r = this->get_relationship("branch_result_false");
    r->add_entity(branch_result_false);
  }
}

box_op_code &
instruction::get_op_code()
{
  return this->op_code;
}

/**
 * create <name> <type>
 */
void
instruction::create()
{
  box_method *method = (box_method *)this->get_relationship("method_instructions")->front();
  box_data *data = box_data::create(this->arg[0], get_from_token(this->arg[1]));
  method->add_local_object((entity *)data);
}

/**
 * create_and_assign_constant <name> <type> <constant>
 */
void
instruction::create_and_assign_constant()
{
  box_method *method = (box_method *)this->get_relationship("method_instructions")->front();
  box_data *data = box_data::create(this->arg[0],
                                    get_from_token(this->arg[1]),
                                    this->arg[2].c_str());

  method->add_local_object((entity *)data);
}

/**
 * create_and_assign_object <name> <obj_name>
 */
void
instruction::create_and_assign_object()
{
  box_method *method = (box_method *)this->get_relationship("method_instructions")->front();
  box_data *data2 = (box_data *)method->get_local_object(this->arg[1]);

  if (!data2)
  {
    BOX_ERROR(ERROR_BOX_INSTRUCTION_OBJECT_DOES_NOT_EXIST);
    return;
  }

  box_data *data = box_data::create(this->arg[0], *data2);
  method->add_local_object((entity *)data);
}

/**
 * pop_and_store <name>
 */
void
instruction::pop_and_store()
{
  box_method *method = (box_method *)this->get_relationship("method_instructions")->front();
  box_data *data2 = (box_data *)method->pop_stack();

  if (!data2)
  {
    BOX_ERROR(ERROR_BOX_INSTRUCTION_OBJECT_DOES_NOT_EXIST);
    return;
  }

  box_data *data = (box_data *)method->get_local_object(this->arg[0]);

  if (!data)
  {
    BOX_ERROR(ERROR_BOX_INSTRUCTION_OBJECT_DOES_NOT_EXIST);
    return;
  }

  *data = *data2;
}

/**
 * Instruction execution.
 *
 * @return next instruction to execute.
 */
instruction *
instruction::execute()
{
  switch (this->op_code)
  {
    case CREATE:
      this->create();
      break;
    case CREATE_AND_ASSIGN_CONSTANT:
      this->create_and_assign_constant();
      break;
    case CREATE_AND_ASSIGN_OBJECT:
      this->create_and_assign_object();
      break;
    case POP_AND_STORE:
      this->pop_and_store();
      break;
    default:
      BOX_ERROR(ERROR_BOX_INSTRUCTION_UNKNOWN);
      break;
  }

  return (instruction *)this->get_relationship("next_instruction")->front();
}
