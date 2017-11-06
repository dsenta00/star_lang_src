//
// Created by Duje Senta on 11/5/17.
//

#ifndef STAR_PROGRAMMING_LANGUAGE_CREATE_AND_ASSIGN_OBJECT_INSTRUCTION_H
#define STAR_PROGRAMMING_LANGUAGE_CREATE_AND_ASSIGN_OBJECT_INSTRUCTION_H

#include "abstract_instruction.h"

/**
 * OP_CODE_CREATE_AND_ASSIGN_OBJECT <name> <obj_name>
 */
class create_and_assign_object_instruction : public abstract_instruction {
public:
    explicit create_and_assign_object_instruction(std::vector<std::string> &arg);
    static create_and_assign_object_instruction *create(std::string name, std::string obj_name);
    abstract_instruction *execute() override;
    bool validate() override;
};


#endif //STAR_PROGRAMMING_LANGUAGE_CREATE_AND_ASSIGN_OBJECT_INSTRUCTION_H
