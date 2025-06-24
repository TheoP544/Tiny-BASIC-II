//===========================================
//
//  Error.cpp
//
// Author: Theo P. (theo_pap@otenet.gr)
// Language used: C++
// Copyright: No copyright. You can do with this software whatever
// you like.
// Warranty: No warranty. Use this software at your own risk.
//===========================================

#include <stdio.h>
#include "Misc.h"
#include "Error.h"

//===========================================
struct ErrTblItem  // item of ErrTable
{
  ErrCode Code;  // error code
  const char* Msg;  // error message
};
//===========================================
ErrTblItem ErrTbl[] =  // error table
{
  ecMEM_ALLOC, "memory allocation failure",

  ecFNAME_NULL, "null file name supplied",
  ecFNAME_EMPTY, "empty file name supplied",
  ecFOPEN, "cannot open file",

  ecEXPR_MISSING, "no expression present",
  ecEQ_MISSING, "equal sign = expected",
  ecCOMMA_MISSING, "comma , expected",
  ecVAR_MISSING, "variable expected",
  ecREL_OP_MISSING, "relational operator expected",
  ecQUOTE_MISSING, "closing quote \" missing:",
  ecLPAR_MISSING, "left parenthesis ( missing",
  ecRPAR_MISSING, "right parenthesis ) missing",
  ecEND_MISSING, "END missing",
  ecTO_MISSING, "TO expected",
  ecSTEP_MISSING, "STEP expected",
  ecTHEN_MISSING, "THEN expected",
  ecNEXT_MISSING, "NEXT expected",
  ecWEND_MISSING, "WEND expected",

  ecUNBAL_PAR, "unbalanced parentheses",
  ecNOT_VAR, "not a valid variable",
  ecNOT_REL_OP, "not a relational operator",
  ecUNREC_TOKEN, "unrecognized token",
  ecUNEXP_TOKEN, "unexpected token",
  ecILL_VAR_NAME, "illegal var name",
  ecILL_VAR_NAME_FOR, "illegal var name in FOR loop",
  ecRAND_ARG_NEG, "RANDOMIZE argument cannot be negative",
  ecRAND_ARG_INT, "RANDOMIZE argument must be integer",
  ecRND_ARG_NEG, "RND() argument canot be negative",
  ecRND_ARG_INT, "RND() argument must be integer",
  ecRND_WRONG_ARG, "wrong RND() arguments: must be a < b",
  ecDIV_ZERO, "division by 0 is illegal",
  ecEXP_NEG, "negative exponent is illegal",
  ecEXP_NOT_INT, "exponent must be integer number",
  ecSQR_ARG_NEG, "SQR() argument cannot be negative",
  ecLOG_ARG_NEG, "LOG() argument must be non-negative",
  ecMOD_OPND_NOT_INT, "% operands must be integers",
  ecPREC_ARG_NEG,  "PRECISION argument cannot be negative",
  ecPREC_ARG_INT,  "PRECISION argument must be integer",
  ecON_OFF_MISSING,  "ON or OFF expected",

  ecTOO_MANY_FOR_NEST, "too many nested FORs",
  ecNEXT_WITHOUT_FOR, "NEXT without FOR",
  ecSTEP_ZERO, "step is zero",

  ecTOO_MANY_WHILE_NEST, "too many nested WHILEs",
  ecWEND_WITHOUT_WHILE, "WEND without WHILE",

  ecTOO_MANY_DO_NEST, "too many nested DOs",
  ecUNTIL_WITHOUT_DO, "UNTIL without DO",

  ecTOO_MANY_GOSUB_NEST, "too many nested GOSUBs",
  ecRET_WITHOUT_GOSUB, "RETURN without GOSUB",

  ecLBL_FULL, "label table full",
  ecLBL_EMPTY, "label table empty",
  ecLBL_DUPL, "duplicate label",
  ecLBL_UNDEF, "undefined label",
  ecLBL_INVALID, "invalid label",

  ecSTK_FULL, "cannot push: stack is full",
  ecSTK_EMPTY, "cannot pop: stack is empty",

  ecGOSUB_FULL, "cannot push: GOSUB stack is full",
  ecGOSUB_EMPTY, "cannot pop: GOSUB stack is empty",

  ecFOR_FULL, "cannot push: FOR stack is full",
  ecFOR_EMPTY, "cannot pop: FOR stack is empty",
  ecFOR_EMPTY2, "cannot peek: FOR stack is empty",

  ecWHILE_FULL, "cannot push: WHILE stack is full",
  ecWHILE_EMPTY, "cannot pop: WHILE stack is empty",
  ecWHILE_EMPTY2, "cannot peek: WHILE stack is empty",

  ecDO_FULL, "cannot push: DO stack is full",
  ecDO_EMPTY, "cannot pop: DO stack is empty",

  ecEOT,  ""  /* end of table = terminal mark. Do not remove. */
};
//===========================================
// Display an error message.

void ErrReporter::Error(ErrCode ec, const char* s)
{
  for (int i = 0; ErrTbl[i].Code != ecEOT; i++)
    if (ErrTbl[i].Code == ec)
    {
      printf("\nERROR: Line = %d, Msg = %s", Line, ErrTbl[i].Msg);
    
      if (s)  // s is optional
        printf(" %s", s);

      printf(".\n\n");
      Counter++;

      if (Counter = MAX_ERRORS)
      {
        printf("\nToo many errors. Program aborted.\n\n");
        exit(1);
      }
    }
}
//===========================================
// Fatal error happened. Display error message and abort the program.

void ErrReporter::FatalError(ErrCode ec, const char* s)
{
  for (int i = 0; ErrTbl[i].Code != ecEOT; i++)
    if (ErrTbl[i].Code == ec)
    {
      printf("\nERROR: %s", ErrTbl[i].Msg);
    
      if (s)
        printf(" %s", s);

      printf(".\n\n");
      exit(1);
    }
}
//===========================================
ErrReporter ErrRpt;  // the globar error reporter object
//===========================================
