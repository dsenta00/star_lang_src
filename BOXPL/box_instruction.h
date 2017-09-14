#ifndef BOX_INSTRUCTION_H
#define BOX_INSTRUCTION_H
#include <stdint.h>

typedef enum {
  MOV,
  ADD,
  MUL,
  DIV,
  SUB,
  MOD,
  RAND,
  SCAN,
  PRINT,
  PRINTLN,
  ADI,
  MUC,
  DIC,
  SUC,
  MODC,
  CMPE,
  CMPNE,
  CMPG,
  CMPL,
  CMPGE,
  CMPLE,
  LOOPE,
  LOOPNE,
  LOOPG,
  LOOPL,
  LOOPGE,
  LOOPLE,
  NEW,
  NEWV,
  FREE,
  OPENW,
  OPENR,
  OPENA,
  PUT,
  GET,
  GETLINE,
  REPLOOP,
  RUN,
  START,
  ESTART
} box_op_code;

#endif // BOX_INSTRUCTION_H
