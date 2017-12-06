//
// Created by Duje Senta on 11/5/17.
//

#ifndef CREATE_AND_ASSIGN_OBJECT_INSTRUCTION_H
#define CREATE_AND_ASSIGN_OBJECT_INSTRUCTION_H

#include "abstract_instruction.h"

/**
 * OP_CODE_CREATE_AND_ASSIGN_OBJECT <name> <obj_name>
 */
class create_and_assign_object_instruction : public abstract_instruction {
public:
    explicit create_and_assign_object_instruction(std::vector<std::wstring> &arg);
    static create_and_assign_object_instruction *create(std::wstring name, std::wstring obj_name);
    abstract_instruction *execute() override;
    bool validate() override;
};

#endif //CREATE_AND_ASSIGN_OBJECT_INSTRUCTION_H
