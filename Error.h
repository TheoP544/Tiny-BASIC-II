//===========================================
//
//  Error.h
//
// Author: Theo P. (theo_pap@otenet.gr)
// Language used: C++
// Copyright: No copyright. You can do with this software whatever
// you like.
// Warranty: No warranty. Use this software at your own risk.
//===========================================

#ifndef ERROR_H
#define ERROR_H

#include <stdlib.h>

//===========================================
// max num of errors before prog is terminated
const int MAX_ERRORS = 10;
//===========================================
enum ErrCode  // error code
{
  ecMEM_ALLOC,

  ecFNAME_NULL,
  ecFNAME_EMPTY,
  ecFOPEN,

  ecEXPR_MISSING,
  ecEQ_MISSING,
  ecCOMMA_MISSING,
  ecVAR_MISSING,
  ecREL_OP_MISSING,
  ecQUOTE_MISSING,
  ecLPAR_MISSING,
  ecRPAR_MISSING,
  ecEND_MISSING,
  ecTO_MISSING,
  ecSTEP_MISSING,
  ecTHEN_MISSING,
  ecNEXT_MISSING,
  ecWEND_MISSING,

  ecUNBAL_PAR,
  ecNOT_VAR,
  ecNOT_REL_OP,
  ecUNREC_TOKEN,
  ecUNEXP_TOKEN,
  ecILL_VAR_NAME,
  ecILL_VAR_NAME_FOR,
  ecRAND_ARG_NEG,
  ecRAND_ARG_INT,
  ecRND_ARG_NEG,
  ecRND_ARG_INT,
  ecRND_WRONG_ARG,
  ecDIV_ZERO,
  ecEXP_NEG,
  ecEXP_NOT_INT,
  ecSQR_ARG_NEG,
  ecLOG_ARG_NEG,
  ecMOD_OPND_NOT_INT,
  ecPREC_ARG_NEG,
  ecPREC_ARG_INT,
  ecON_OFF_MISSING,

  ecTOO_MANY_FOR_NEST,
  ecNEXT_WITHOUT_FOR,
  ecSTEP_ZERO,

  ecTOO_MANY_WHILE_NEST,
  ecWEND_WITHOUT_WHILE,

  ecTOO_MANY_DO_NEST,
  ecUNTIL_WITHOUT_DO,

  ecTOO_MANY_GOSUB_NEST,
  ecRET_WITHOUT_GOSUB,

  ecLBL_FULL,
  ecLBL_EMPTY,
  ecLBL_DUPL,
  ecLBL_UNDEF,
  ecLBL_INVALID,

  ecSTK_FULL,
  ecSTK_EMPTY,

  ecGOSUB_FULL,
  ecGOSUB_EMPTY,

  ecFOR_FULL,
  ecFOR_EMPTY,  // for pop
  ecFOR_EMPTY2,  // for peek

  ecWHILE_FULL,
  ecWHILE_EMPTY,
  ecWHILE_EMPTY2,

  ecDO_FULL,
  ecDO_EMPTY,

  ecEOT  // end of table = terminal mark. Do not remove.
};
//===========================================
class ErrReporter  // error reporter
{
public:
  ErrReporter()  { Counter = 0; }

  void Error(ErrCode ec, const char* s = NULL);
  void FatalError(ErrCode ec, const char* s = NULL);

private:
  int Counter;  // num of errors happened so far
};
//===========================================
extern ErrReporter ErrRpt;  // the global error reporter object
//===========================================

#endif


