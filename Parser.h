//===========================================
//
//  Parser.h
//
// Author: Theo P. (theo_pap@otenet.gr)
// Language used: C++
// Copyright: No copyright. You can do with this software whatever
// you like.
// Warranty: No warranty. Use this software at your own risk.
//
// Precedence Table
// ------------------------------------------------
// op                     level  func
// ------------------------------------------------
// num  var  func       8   EvalFactor()
// ( )                        7   EvalPar()
// un+ un-                6   EvalUnPlusMinus()
// NOT                     5   EvalNot()
// * / %                   4   EvalMultDivMod()
// + -                      3   EvalAddSub()
// < <= > >= = <>  2   EvalComp()
// AND                     1   EvalAnd()
// OR                       0   EvalOr()
// ------------------------------------------------
//===========================================

#ifndef PARSER_H
#define PARSER_H

#include "SupportClasses.h"
#include "Scanner.h"

//===========================================
class Parser
{
public:
  Parser();

  void Init(const char* fname);

  void Execute();  // entry point to command executor

  void DispSource();
  void DispTokens();
  void DispLblTbl();

private:
  const char* FindTokStr(TokCode tok);
  TokCode FindToken(const char* str);

  bool IsRelOp(TokCode tok);
  bool Compare(TokCode op, double opnd1, double opnd2);
  void SkipUntilToken(TokCode tok);
  void SkipUntilToken2(TokCode tok1, TokCode tok2);
  void SkipUntilToken3(TokCode tok1, TokCode tok2, TokCode tok3);

  // expr calculator
  double EvalExpr();          // entry point
  void EvalOr();                // level 0
  void EvalAnd();              // level 1
  void EvalComp();           // level 2
  void EvalAddSub();        // level 3
  void EvalMultDivMod();   // level 4
  void EvalNot();              // level 5
  void EvalUnPlusMinus();  // level 6
  void EvalPar();               // level 7
  void EvalFactor();           // level 8

  // built-in funcs
  double EvalAbs();
  double EvalSgn();
  double EvalCInt();
  double EvalFix();
  double EvalSqr();
  double EvalPow();
  double EvalExp();
  double EvalLog();
  double EvalRnd();

  // command executor
  void ExecAssign();
  void ExecIf();
  void ExecElse();
  void ExecEndIf();
  void ExecGoto();
  void ExecGosub();
  void ExecReturn();
  void ExecFor();
  void ExecNext();
  void ExecWhile();
  void ExecWend();
  void ExecDo();
  void ExecUntil();
  void ExecBreak();
  void ExecContinue();
  void ExecInput();
  void ExecPrint();
  void ExecRandomize();
  void ExecPrecision();
  void ExecDebMode();

///////////////////////////////////////////////////

  GosubStack GosubStk;
  ForStack ForStk;
  WhileStack WhileStk;
  DoStack DoStk;
  Stack Stk;
  VarTable VarTbl;
  Scanner Scn;

  int Precision;  // num of decimal places to display

  // true => we are in debug mode (debug info displayed)
  // false => we are in regular mode (debug info not displayed)
  bool DebMode;  // debug mode on/off toggle switch
};
//===========================================

#endif

