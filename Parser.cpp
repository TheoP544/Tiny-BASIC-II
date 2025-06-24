//===========================================
//
//  Parser.cpp
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

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "Error.h"
#include "Misc.h"
#include "Parser.h"

//===========================================
Parser::Parser()
{
  Precision = 0;  // by default, all numbers displayed as integers
  DebMode = false;  // by default, no debug info displayed
}
//===========================================
// Initialize the parser. Load the source file fname.

void Parser::Init(const char* fname)
{
  Scn.Init(fname);
}
//===========================================
// Display the source file.

void Parser::DispSource()
{
  Scn.DispSource();
}
//===========================================
// Display all the tokens.

void Parser::DispTokens()
{
  Scn.DispTokens();
}
//===========================================
// Display label table.

void Parser::DispLblTbl()
{
  Scn.DispLblTbl();
}
//===========================================
// Find token str corresponding to token tok.

const char* Parser::FindTokStr(TokCode tok)
{
  return Scn.FindTokStr(tok);
}
//===========================================
// Find token corresponding to token string str.

TokCode Parser::FindToken(const char* str)
{
  return Scn.FindToken(str);
}
//===========================================
// Return true if token tok is a relational op, i.e. one of:
//   < <= > >= = <>

bool Parser::IsRelOp(TokCode tok)
{
  return tok >= tcLT && tok <= tcNE;
}
//===========================================
// Compare numbers opnd1 and opnd2 using relational operator op.

bool Parser::Compare(TokCode op, double opnd1, double opnd2)
{
  bool res;

  switch (op)
  {
    case tcLT: res = opnd1 < opnd2; break;  // <
    case tcLE: res = opnd1 <= opnd2; break;  // <=
    case tcGT: res = opnd1 > opnd2; break;  // >
    case tcGE: res = opnd1 >= opnd2; break;  // >=
    case tcEQ: res = opnd1 == opnd2; break;  // =
    case tcNE: res = opnd1 != opnd2; break;  // <>
  }

  if (DebMode)
  {
    DispFloat(opnd1, Precision);
    printf(" ");
    printf(FindTokStr(op));
    printf(" ");
    DispFloat(opnd2, Precision);
    printf(" = ");
    DispLogValue(res);
    printf("\n");
  }

  return res;
}
//===========================================
// Skip tokens until tok is reached.

void Parser::SkipUntilToken(TokCode tok)
{
  TokCode t;

  do
  {
    t = Scn.ReadToken();
  } while (t != tok && t != tcEND && t != tcEOF);
}
//===========================================
// Skip tokens until either tok1 or tok2 is reached.

void Parser::SkipUntilToken2(TokCode tok1, TokCode tok2)
{
  TokCode t;

  do
  {
    t = Scn.ReadToken();
  } while (t != tok1 && t != tok2 && t != tcEND && t != tcEOF);
}
//===========================================
// Skip tokens until either tok1, tok2 or tok3 is reached.

void Parser::SkipUntilToken3(TokCode tok1, TokCode tok2,
  TokCode tok3)
{
  TokCode t;

  do
  {
    t = Scn.ReadToken();
  } while (t != tok1 && t != tok2 && t != tok3 && t != tcEND &&
       t != tcEOF);
}
//===========================================
// *** EXPR CALCULATOR ***
//===========================================
// Entry point to expr calculator.
//
// Evaluate an expression.
// An expression can contain arithmetic, logical and comparison ops.

double Parser::EvalExpr()
{
  double res;

  EvalOr();  // start from bottom, i.e. from level 0
  res = Stk.Pop();  // get the result from stack
  return res;
}
//===========================================
// level 0
// OR
// res = opnd1 OR opnd2

void Parser::EvalOr()
{
  TokCode op;
  double opnd1, opnd2, res;

  EvalAnd();

  while ((op = Scn.GetToken()) == tcOR)
  {
    Scn.ReadToken();
    EvalAnd();
    opnd2 = Stk.Pop();
    opnd1 = Stk.Pop();
    res = opnd1 || opnd2;
    Stk.Push(res);

    if (DebMode)
    {
      DispLogValue(opnd1);
      printf(" OR ");
      DispLogValue(opnd2);
      printf(" = ");
      DispLogValue(res);
      printf("\n");
    }
  }
}
//===========================================
// level 1
// AND
// res = opnd1 AND opnd2

void Parser::EvalAnd()
{
  TokCode op;
  double opnd1, opnd2, res;

  EvalComp();

  while ((op = Scn.GetToken()) == tcAND)
  {
    Scn.ReadToken();
    EvalComp();
    opnd2 = Stk.Pop();
    opnd1 = Stk.Pop();
    res = opnd1 && opnd2;
    Stk.Push(res);

    if (DebMode)
    {
      DispLogValue(opnd1);
      printf(" AND ");
      DispLogValue(opnd2);
      printf(" = ");
      DispLogValue(res);
      printf("\n");
    }
  }
}
//===========================================
// level 2
// Comparison
// res = opnd1 op opnd2
// op = rel op, one of:  < <= > >= = <>.

void Parser::EvalComp()
{
  TokCode op;
  double opnd1, opnd2, res;

  EvalAddSub();
  op = Scn.GetToken();

  if (!IsRelOp(op))  // not a rel op, so do nothing
    return;

  Scn.ReadToken();
  EvalAddSub();
  opnd2 = Stk.Pop();
  opnd1 = Stk.Pop();
  res = Compare(op, opnd1, opnd2);
  Stk.Push(res);
}
//===========================================
// level 3
// Add/Subtract
// res = opnd1 op opnd2
// op =  + -

void Parser::EvalAddSub()
{
  TokCode op;
  double opnd1, opnd2, res;
	
  EvalMultDivMod();
	
  while ((op = Scn.GetToken()) == tcPLUS || op == tcMINUS)
  {
    Scn.ReadToken();
    EvalMultDivMod();
    opnd2 = Stk.Pop();
    opnd1 = Stk.Pop();
		
    switch (op)
    {
      case tcPLUS:
        res = opnd1 + opnd2;
        break;

      case tcMINUS:
        res = opnd1 - opnd2;
        break;
    }

    Stk.Push(res);

    if (DebMode)
    {
      DispFloat(opnd1, Precision);
      printf(" %s ", FindTokStr(op));
      DispFloat(opnd2, Precision);
      printf(" = ");
      DispFloat(res, Precision);
      printf("\n");
    }
  }
}
//===========================================
// level 4
// Multiply/Divide/Modulus
// res = opnd1 op opnd2
// op =  * / %
// NOTE: The operands of % must be integer.

void Parser::EvalMultDivMod()
{
  TokCode op;
  double opnd1, opnd2, res;

  EvalNot();

  while ((op = Scn.GetToken()) == tcSTAR || op == tcSLASH ||
    op == tcPERC)
  {
    Scn.ReadToken();
    EvalNot();
    opnd2 = Stk.Pop();
    opnd1 = Stk.Pop();
		
    switch (op)
    {
      case tcSTAR:
        res = opnd1 * opnd2;
        break;

      case tcSLASH:
        if (opnd2 == 0.0)
        {
          ErrRpt.Error(ecDIV_ZERO);  // division by 0 is illegal
          res = 0.0;
        }
        else
          res = opnd1 / opnd2;
        break;

      case tcPERC:
        if (!IsInt(opnd1))  // opnd1 must be integer
        {
           ErrRpt.Error(ecMOD_OPND_NOT_INT);
           opnd1 = RoundOff(opnd1);
        }
        if (!IsInt(opnd2))  // opnd2 must be integer
        {
           ErrRpt.Error(ecMOD_OPND_NOT_INT);
           opnd2 = RoundOff(opnd2);
        }
        res = double(int(opnd1) % int(opnd2));
        break;
    }

    Stk.Push(res);

    if (DebMode)
    {
      DispFloat(opnd1, Precision);
      printf(" %s ", FindTokStr(op));
      DispFloat(opnd2, Precision);
      printf(" = ");
      DispFloat(res, Precision);
      printf("\n");
    }
  }
}
//===========================================
// level 5
// NOT
// res = NOT opnd

void Parser::EvalNot()
{
  TokCode op;
  double opnd, res;

  if ((op = Scn.GetToken()) == tcNOT)
    Scn.ReadToken();

  EvalUnPlusMinus();

  if (op == tcNOT)
  {
    opnd = Stk.Pop();
    res = !opnd;
    Stk.Push(res);

    if (DebMode)
    {
      printf("NOT ");
      DispLogValue(opnd);
      printf(" = ");
      DispLogValue(res);
      printf("\n");
    }
  }
}
//===========================================
// level 6
// Unary + -
// res = op opnd
// op =  + -

void Parser::EvalUnPlusMinus()
{
  TokCode op;
  double opnd, res;

  if ((op = Scn.GetToken()) == tcPLUS || op == tcMINUS)
    Scn.ReadToken();

  EvalPar();

  if (op == tcPLUS || op == tcMINUS)
  {
    opnd = Stk.Pop();
    res = (op == tcPLUS) ? opnd : -opnd;
    Stk.Push(res);  

    if (DebMode)
    {
      printf("%s(", FindTokStr(op));
      DispFloat(opnd, Precision);
      printf(") = ");
      DispFloat(res, Precision);
      printf("\n");
    }
  }
}
//===========================================
// level 7
// Parentheses
// ( )

void Parser::EvalPar()
{
  if (Scn.GetToken() != tcLPAR)  // no parenthesis, so do nothing
  {
    EvalFactor();
    return;
  }

  if (DebMode)
    printf("(\n");

  Scn.ReadToken();
  EvalOr();

  if (Scn.GetToken() != tcRPAR)
   ErrRpt.Error(ecRPAR_MISSING); 
  else
    if (DebMode)
      printf(")\n");

  Scn.ReadToken();
}
//===========================================
// level 8
// Factor
// num  var  func()

void Parser::EvalFactor()
{
  double res;
	
  switch (Scn.GetToken())
  {
    case tcNUM:
      res = atof(Scn.GetTokStr());
      Stk.Push(res);
      Scn.ReadToken();
      break;

    case tcVAR:
      res = VarTbl.Get(Scn.GetTokStr()[0]);
      Stk.Push(res);
      Scn.ReadToken();
      break;

    case tcABS: res = EvalAbs(); Stk.Push(res); break;
    case tcSGN: res = EvalSgn(); Stk.Push(res); break;
    case tcCINT: res = EvalCInt(); Stk.Push(res); break;
    case tcFIX: res = EvalFix(); Stk.Push(res); break;
    case tcSQR: res = EvalSqr(); Stk.Push(res); break;
    case tcPOW: res = EvalPow(); Stk.Push(res); break;
    case tcEXP: res = EvalExp(); Stk.Push(res); break;
    case tcLOG: res = EvalLog(); Stk.Push(res); break;
    case tcRND: res = EvalRnd(); Stk.Push(res); break;

    default:
      ErrRpt.Error(ecUNEXP_TOKEN, Scn.GetTokStr());
      res = 0.0;
      Stk.Push(res);
      Scn.ReadToken();
      break;
  }
}
//===========================================
// *** BUILT-IN FUNCS ***
//===========================================
// ABS function
// y = ABS(x)

double Parser::EvalAbs()
{
  double x, y;

  Scn.ReadToken();  // read (

  if (Scn.GetToken() != tcLPAR)
  {
    ErrRpt.Error(ecLPAR_MISSING);
    return 0.0;
  }

  Scn.ReadToken();  // read x
  x = EvalExpr();

  if (Scn.GetToken() != tcRPAR)
  {
    ErrRpt.Error(ecRPAR_MISSING);
    return 0.0;
  }

  Scn.ReadToken();
  y = (x < 0.0) ? -x : x;

  if (DebMode)
  {
    printf("ABS(");
    DispFloat(x, Precision);
    printf(") = ");
    DispFloat(y, Precision);
    printf("\n");
  }

  return y;
}
//===========================================
// SGN function
// y = SGN(x)

double Parser::EvalSgn()
{
  double x, y;

  Scn.ReadToken();  // read (

  if (Scn.GetToken() != tcLPAR)
  {
    ErrRpt.Error(ecLPAR_MISSING);
    return 0.0;
  }

  Scn.ReadToken();  // read x
  x = EvalExpr();

  if (Scn.GetToken() != tcRPAR)
  {
    ErrRpt.Error(ecRPAR_MISSING);
    return 0.0;
  }

  Scn.ReadToken();

  if (x < 0.0)
    y = -1.0;
  else if (x > 0.0)
    y = 1.0;
  else
    y = 0.0;

  if (DebMode)
  {
    printf("SGN(");
    DispFloat(x, Precision);
    printf(") = ");
    DispFloat(y, Precision);
    printf("\n");
  }

  return y;
}
//===========================================
// CINT function
// Roun-off a double to the nearest int. CINT = Convert to INT.
// y = CINT(x)

double Parser::EvalCInt()
{
  double x, y;

  Scn.ReadToken();  // read (

  if (Scn.GetToken() != tcLPAR)
  {
    ErrRpt.Error(ecLPAR_MISSING);
    return 0.0;
  }

  Scn.ReadToken();  // read x
  x = EvalExpr();

  if (Scn.GetToken() != tcRPAR)
  {
    ErrRpt.Error(ecRPAR_MISSING);
    return 0.0;
  }

  Scn.ReadToken();
  y = double(RoundOff(x));

  if (DebMode)
  {
    printf("CINT(");
    DispFloat(x, Precision);
    printf(") = ");
    DispFloat(y, Precision);
    printf("\n");
  }

  return y;
}
//===========================================
// FIX function
// Truncate a double to the smallest int.
// y = FIX(x)

double Parser::EvalFix()
{
  double x, y;

  Scn.ReadToken();  // read (

  if (Scn.GetToken() != tcLPAR)
  {
    ErrRpt.Error(ecLPAR_MISSING);
    return 0.0;
  }

  Scn.ReadToken();  // read x
  x = EvalExpr();

  if (Scn.GetToken() != tcRPAR)
  {
    ErrRpt.Error(ecRPAR_MISSING);
    return 0.0;
  }

  Scn.ReadToken();
  y = double(Trunc(x));

  if (DebMode)
  {
    printf("FIX(");
    DispFloat(x, Precision);
    printf(") = ");
    DispFloat(y, Precision);
    printf("\n");
  }

  return y;
}
//===========================================
// SQR function
// Must be x >= 0.
// y = SQR(x)

double Parser::EvalSqr()
{
  double x, y;

  Scn.ReadToken();  // read (

  if (Scn.GetToken() != tcLPAR)
  {
    ErrRpt.Error(ecLPAR_MISSING);
    return 0.0;
  }

  Scn.ReadToken();  // read x
  x = EvalExpr();

  if (x < 0.0)
  {
    ErrRpt.Error(ecSQR_ARG_NEG);
    return 0.0;
  }

  if (Scn.GetToken() != tcRPAR)
  {
    ErrRpt.Error(ecRPAR_MISSING);
    return 0.0;
  }

  Scn.ReadToken();
  y = sqrt(x);

  if (DebMode)
  {
    printf("SQR(");
    DispFloat(x, Precision);
    printf(") = ");
    DispFloat(y, Precision);
    printf("\n");
  }

  return y;
}
//===========================================
// POW() function
// POW(b, n) = b^n. n must be integer >= 0.
// y = POW(b, n)

double Parser::EvalPow()
{
  double b, n, y;

  Scn.ReadToken();  // read (

  if (Scn.GetToken() != tcLPAR)
  {
    ErrRpt.Error(ecLPAR_MISSING);
    return 0.0;
  }

  Scn.ReadToken();  // read b
  b = EvalExpr();

  if (Scn.GetToken() != tcCOMMA)
  {
    ErrRpt.Error(ecCOMMA_MISSING);
    return 0.0;
  }

  Scn.ReadToken();  // read n
  n = EvalExpr();

  if (n < 0.0)
  {
    ErrRpt.Error(ecEXP_NEG);
    return 0.0;
  }

  if (!IsInt(n))
  {
    ErrRpt.Error(ecEXP_NOT_INT);
    n = RoundOff(n);
  }

  if (Scn.GetToken() != tcRPAR)
  {
    ErrRpt.Error(ecRPAR_MISSING);
    return 0.0;
  }

  Scn.ReadToken();
  y = pow(b, n);

  if (DebMode)
  {
    printf("POW(");
    DispFloat(b, Precision);
    printf(", ");
    DispFloat(n, 0);  // n is integer
    printf(") = ");
    DispFloat(y, Precision);
    printf("\n");
  }

  return y;
}
//===========================================
// EXP() function
// EXP(x) = e^x
// y = EXP(x)

double Parser::EvalExp()
{
  double x, y;

  Scn.ReadToken();  // read (

  if (Scn.GetToken() != tcLPAR)
  {
    ErrRpt.Error(ecLPAR_MISSING);
    return 0.0;
  }

  Scn.ReadToken();  // read x
  x = EvalExpr();

  if (Scn.GetToken() != tcRPAR)
  {
    ErrRpt.Error(ecRPAR_MISSING);
    return 0.0;
  }

  Scn.ReadToken();
  y = exp(x);

  if (DebMode)
  {
    printf("EXP(");
    DispFloat(x, Precision);
    printf(") = ");
    DispFloat(y, Precision);
    printf("\n");
  }

  return y;
}
//===========================================
// LOG() function
// LOG(x) = ln(x) = natural logarithm of x. Must be x > 0.
// y = LOG(x)

double Parser::EvalLog()
{
  double x, y;

  Scn.ReadToken();  // read (

  if (Scn.GetToken() != tcLPAR)
  {
    ErrRpt.Error(ecLPAR_MISSING);
    return 0.0;
  }

  Scn.ReadToken();  // read x
  x = EvalExpr();

  if (x <= 0.0)
  {
    ErrRpt.Error(ecLOG_ARG_NEG);
    return 0.0;
  }

  if (Scn.GetToken() != tcRPAR)
  {
    ErrRpt.Error(ecRPAR_MISSING);
    return 0.0;
  }

  Scn.ReadToken();
  y = log(x);

  if (DebMode)
  {
    printf("LOG(");
    DispFloat(x, Precision);
    printf(") = ");
    DispFloat(y, Precision);
    printf("\n");
  }

  return y;
}
//===========================================
// RND function
// Return a pseudo-random r in the range: a <= r <= b.
// Must be: a, b = unsigned int, a < b.
// y = RND(a, b)

double Parser::EvalRnd()
{
  double a, b, y;

  Scn.ReadToken();  // read (

  if (Scn.GetToken() != tcLPAR)
  {
    ErrRpt.Error(ecLPAR_MISSING);
    return 0.0;
  }

  Scn.ReadToken();  // read a
  a = EvalExpr();

  if (a < 0.0)
  {
    ErrRpt.Error(ecRND_ARG_NEG);
    return 0.0;
  }

  if (!IsInt(a))
  {
    ErrRpt.Error(ecRND_ARG_INT);
    a = RoundOff(a);
  }

  if (Scn.GetToken() != tcCOMMA)
  {
    ErrRpt.Error(ecCOMMA_MISSING);
    return 0.0;
  }

  Scn.ReadToken();  // read b
  b = EvalExpr();

  if (b < 0.0)
  {
    ErrRpt.Error(ecRND_ARG_NEG);
    return 0.0;
  }

  if (!IsInt(b))
  {
    ErrRpt.Error(ecRND_ARG_INT);
    b = RoundOff(b);
  }

  if (a >= b)
  {
    ErrRpt.Error(ecRND_WRONG_ARG);
    return 0.0;
  }

  if (Scn.GetToken() != tcRPAR)
  {
    ErrRpt.Error(ecRPAR_MISSING);
    return 0.0;
  }

  Scn.ReadToken();
  y = double(int(double(rand()) / RAND_MAX * (b - a) + a + 0.5));

  if (DebMode)
  {
    printf("RND(");
    DispFloat(a, 0);
    printf(", ");
    DispFloat(b, 0);
    printf(") = ");
    DispFloat(y, 0);
    printf("\n");
  }

  return y;
}
//===========================================
// *** COMMAND EXECUTOR ***
//===========================================
// Entry point to command executor.

// Execute a command.

void Parser::Execute()
{
  bool done = false;

  Scn.ReadToken();

  while (!done)  // execution loop
  {
    switch (Scn.GetToken())
    {
      case tcVAR: ExecAssign(); break;
      case tcIF: ExecIf(); break;
      case tcELSE: ExecElse(); break;
      case tcENDIF: ExecEndIf(); break;
      case tcGOTO: ExecGoto(); break;
      case tcGOSUB: ExecGosub(); break;
      case tcRETURN: ExecReturn(); break;
      case tcFOR: ExecFor(); break;
      case tcNEXT: ExecNext(); break;
      case tcWHILE: ExecWhile(); break;
      case tcWEND: ExecWend(); break;
      case tcDO: ExecDo(); break;
      case tcUNTIL: ExecUntil(); break;
      case tcBREAK: ExecBreak(); break;
      case tcCONTINUE: ExecContinue(); break;
      case tcINPUT: ExecInput(); break;
      case tcPRINT: ExecPrint(); break;
      case tcRANDOMIZE: ExecRandomize(); break;
      case tcPRECISION: ExecPrecision(); break;
      case tcDEB_MODE: ExecDebMode(); break;
      case tcEND: done = true; break;
      case tcEOF: done = true; break;
      default: Scn.ReadToken(); break;
    }
  }

  if (Scn.GetToken() != tcEND)
    ErrRpt.Error(ecEND_MISSING);  // no END at the end of source
}
//===========================================
// Assignment command
// Assign an expr to a var.
// var = expr

void Parser::ExecAssign()
{
  char var;  // var name
  double value;  // value of expr

  var = toupper(Scn.GetTokStr()[0]);
  Scn.ReadToken();  // read =

  if (Scn.GetToken() != tcEQ)
  {
    ErrRpt.Error(ecEQ_MISSING);
    return;
  }

  Scn.ReadToken();  // read expr
  value = EvalExpr();
  VarTbl.Set(var, value);  // assign value to var
}
//===========================================
// IF command
// version 1 = simplified IF
// IF expr THEN
//   block1
// ENDIF
// version 2 = full IF
// IF expr THEN
//   block1
// ELSE
//   block2
// ENDIF

void Parser::ExecIf()
{
  double expr;  // value of expr

  Scn.ReadToken();  // read expr
  expr = EvalExpr();

  if (Scn.GetToken() != tcTHEN)
  {
    ErrRpt.Error(ecTHEN_MISSING);
    return;
  }

  Scn.ReadToken();

  if (!expr)  // expr is false, so skip block1
    SkipUntilToken2(tcELSE, tcENDIF);

  Scn.ReadToken();
}
//===========================================
// ELSE command
// IF expr THEN
//   block1
// ELSE
//   block2
// ENDIF

void Parser::ExecElse()
{
  SkipUntilToken(tcENDIF);  // skip block2
  Scn.ReadToken();
}
//===========================================
// ENDIF command
// Exit the IF statement
// version 1 = simplified IF
// IF expr THEN
//   block1
// ENDIF
// version 2 = full IF
// IF expr THEN
//   block1
// ELSE
//   block2
// ENDIF

void Parser::ExecEndIf()
{
  Scn.ReadToken();  // get out of last block, either block1 or block2
}
//===========================================
// GOTO command
// Jump to label loc.
// GOTO label

void Parser::ExecGoto()
{
  char* loc;

  Scn.ReadToken();  // read label

  if (Scn.GetToken() != tcNUM)  // not a valid label
  {
    ErrRpt.Error(ecLBL_INVALID);
    return;
  }

  loc = Scn.FindLblLoc(Scn.GetTokStr());

  if (loc == NULL)  // no such label
  {
    ErrRpt.Error(ecLBL_UNDEF);
    return;
  }

  Scn.SetProg(loc);  // jump to loc
  Scn.ReadToken();
}
//===========================================
// GOSUB command
// Jump to label loc.
// GOSUB label

void Parser::ExecGosub()
{
  char* loc;

  Scn.ReadToken();  // read label

  if (Scn.GetToken() != tcNUM)  // not a valid label
  {
    ErrRpt.Error(ecLBL_INVALID);
    return;
  }

  loc = Scn.FindLblLoc(Scn.GetTokStr());

  if (loc == NULL)  // no such label
  {
    ErrRpt.Error(ecLBL_UNDEF);
    return;
  }

  // push the current loc on GOSUB stack = return address
  GosubStk.Push(Scn.GetProg());
  Scn.SetProg(loc);  // jump to loc
  Scn.ReadToken();
}
//===========================================
// RETURN command
// Return from a GOSUB subroutine. Jump to the 1st loc following the 
// GOSUB.
// RETURN

void Parser::ExecReturn()
{
  // pop the return loc from the GOSUB stack
  Scn.SetProg(GosubStk.Pop());
  Scn.ReadToken();  // jump to loc
}
//===========================================
// FOR command
// FOR var = start_value TO end_value [ STEP step_value ]
//   block
// NEXT

void Parser::ExecFor()
{
  char var;  // counter var name
  double start_value, end_value, step_value;
  bool skip_loop;
  ForStkItem i;

  Scn.ReadToken();  // read var name

  if (Scn.GetToken() != tcVAR)
  {
    ErrRpt.Error(ecNOT_VAR);
    return;
  }

  var = toupper(Scn.GetTokStr()[0]);
  Scn.ReadToken();  // read =

  if (Scn.GetToken() != tcEQ)
  {
    ErrRpt.Error(ecEQ_MISSING);
    return;
  }

  Scn.ReadToken();  // read start_value
  start_value = EvalExpr();

  if (Scn.GetToken() != tcTO)
  {
    ErrRpt.Error(ecTO_MISSING);
    return;
  }

  Scn.ReadToken();  // read end_value
  end_value = EvalExpr();

  if (Scn.GetToken() != tcSTEP)  // no STEP clause
    step_value = 1.0;  // so use the default value 1
  else  // STEP clause present
  {
    Scn.ReadToken();  // read step_value
    step_value = EvalExpr();

    if (step_value == 0.0)
    {
      ErrRpt.Error(ecSTEP_ZERO);  // 0 step is illegal
      step_value = 1.0;
    }
  }

  if (step_value > 0.0)  // counting up
    skip_loop = start_value > end_value;
  else  // counting down
    skip_loop = start_value < end_value;

  if (skip_loop)  // skip the loop
  {
    SkipUntilToken(tcNEXT);

    if (Scn.GetToken() != tcNEXT)
      ErrRpt.Error(ecNEXT_MISSING);
    else
      Scn.ReadToken();

    return;
  }

  // stay in loop
  // save current value of counter var in VarTbl
  VarTbl.Set(var, start_value);
  i.Var = var;  // var name
  i.EndValue = end_value;
  i.StepValue = step_value;
  i.Loc = Scn.GetProg();  // save the FOR command loc
  ForStk.Push(i);  // save info on FOR stack
  Scn.ReadToken();  // read the 1st token of block
}
//===========================================
// NEXT command
// FOR var = start_value TO end_value [ STEP step_value ]
//   block
// NEXT

void Parser::ExecNext()
{
  char var;  // counter var name
  double var_value, end_value, step_value;
  bool skip_loop;
  ForStkItem i;

  if (ForStk.IsEmpty())
  {
    ErrRpt.Error(ecNEXT_WITHOUT_FOR);  // too many NEXTs
    return;
  }

  i = ForStk.Peek();
  var = i.Var;
  var_value = VarTbl.Get(var);  // get current value of counter var
  end_value = i.EndValue;
  step_value =  i.StepValue;
  var_value += step_value;  // increment counter var by step
  VarTbl.Set(var, var_value);  // save counter var in VarTbl

  if (step_value > 0.0)  // counting up
    skip_loop = var_value > end_value;
  else  // counting down
    skip_loop = var_value < end_value;

  if (skip_loop)  // skip the loop
  {
    var_value -= step_value;  // cancel incrementing of counter var
    VarTbl.Set(var, var_value);  // save counter var value in VarTbl
    ForStk.Pop();  // remove the top item from stack
    Scn.ReadToken();  // skip NEXT
    return;
  }

  // stay in loop
  Scn.SetProg(i.Loc);  // jump back to FOR command
  Scn.ReadToken();
}
//===========================================
// WHILE command
// WHILE var op expr
//   block
// WEND

void Parser::ExecWhile()
{
  char var;  // control var name
  double var_value, expr;
  TokCode op;  // rel op
  bool res;  // result of comparison
  WhileStkItem i;

  Scn.ReadToken();  // read var name

  if (Scn.GetToken() != tcVAR)
  {
    ErrRpt.Error(ecNOT_VAR); // not a valid var
    return;
  }

  var = toupper(Scn.GetTokStr()[0]);  // get var name

  // get the current value of var from the VarTbl
  var_value = VarTbl.Get(var);

  op = Scn.ReadToken();  // read op

  if (!IsRelOp(op))  // not a rel op
  {
    ErrRpt.Error(ecREL_OP_MISSING);
    return;
  }

  Scn.ReadToken();  // read expr
  expr = EvalExpr();

  // compare var_value with expr using op
  res = Compare(op, var_value, expr);

  if (!res)  // res is false, so skip loop
  {
    SkipUntilToken(tcWEND);

    if (Scn.GetToken() == tcWEND)
      Scn.ReadToken();
    else
      ErrRpt.Error(ecWEND_MISSING);

    return;
  }

  // res is true, so stay in loop
  if (WhileStk.IsFull())
  {
    ErrRpt.Error(ecTOO_MANY_WHILE_NEST);  // too many WHILEs
    return;
  }

  i.Var = var;  // save control var name
  i.Op = op;  // save op
  i.Expr = expr;  // save value of expr
  i.Loc = Scn.GetProg();  // save WHILE command loc
  WhileStk.Push(i);  // save info on stack
  Scn.ReadToken();  // read the 1st token of block
}
//===========================================
// WEND command
// WHILE var op expr
//   block
// WEND

void Parser::ExecWend()
{
  char var;  // control var name
  double var_value, expr;
  TokCode op;  // rel op
  bool res;  // result of comparison
  WhileStkItem i;

  if (WhileStk.IsEmpty())
  {
    ErrRpt.Error(ecWEND_WITHOUT_WHILE);  // too many WENDs/
    return;
  }

  i = WhileStk.Peek();
  var = i.Var;
  op = i.Op;
  expr = i.Expr;

  // get current value of control var from var table
  var_value = VarTbl.Get(var);

  // compare var_value with expr using op
  res = Compare(op, var_value, expr);

  if (!res)  // res is false, so exit loop
  {
    WhileStk.Pop();  // remove the top item from stack
    Scn.ReadToken();  // skip WEND
    return;
  }

  // res is true, so stay in loop
  Scn.SetProg(i.Loc);  // jump back to WHILE command loc
  Scn.ReadToken();
}
//===========================================
// DO command
// DO
//   block
// UNTIL var op expr

void Parser::ExecDo()
{
  DoStkItem i;

  i.Loc = Scn.GetProg();  // save the DO command loc
  DoStk.Push(i);
  Scn.ReadToken();
}
//===========================================
// UNTIL command
// DO
//   block
// UNTIL var op expr

void Parser::ExecUntil()
{
  char var;  // control var name
  double var_value, expr;
  TokCode op;  // rel op
  bool res;  // result of comparison
  DoStkItem i;

  Scn.ReadToken();  // read var name

  if (Scn.GetToken() != tcVAR)
  {
    ErrRpt.Error(ecNOT_VAR);  // not a valid var
    return;
  }

  var = toupper(Scn.GetTokStr()[0]);  // get var name

  // get current value of control var from VarTbl
  var_value = VarTbl.Get(var);
  op = Scn.ReadToken();  // read op

  if (!IsRelOp(op))
  {
    ErrRpt.Error(ecREL_OP_MISSING);  // not a rel op
    return;
  }

  Scn.ReadToken();  // read expr
  expr = EvalExpr();  // get expr value

  // compare var_value with expr using op
  res = Compare(op, var_value, expr);

  if (res)  // res is true, so exit loop
  {
    DoStk.Pop();  // remove the top item from stack
    Scn.ReadToken();
    return;
  }

  // res is false, so stay in loop
  if (DoStk.IsFull())
  {
    ErrRpt.Error(ecTOO_MANY_DO_NEST);  // too many DOs
    return;
  }

  i = DoStk.Pop();  // get top stack item
  i.Var = var;
  i.Op = op;
  i.Expr = expr;
  DoStk.Push(i);  // update top stack item
  // save cirrent value of control var in VarTbl
  VarTbl.Set(var, var_value);
  Scn.SetProg(i.Loc);  // jump back to DO command loc
  Scn.ReadToken();
}
//===========================================
// BREAK command
// Used in FOR ... NEXT, WHILE ... WEND and DO ... UNTIL loops.
// Causes immediate exit from loop.
// BREAK

void Parser::ExecBreak()
{
  SkipUntilToken3(tcNEXT, tcWEND, tcUNTIL);
  Scn.ReadToken();
}
//===========================================
// CONTINUE command
// Used in FOR ... NEXT, WHILE ... WEND and DO ... UNTIL loops.
// Causes jump to the end of loop. No exit from loop.
// CONTINUE

void Parser::ExecContinue()
{
  SkipUntilToken3(tcNEXT, tcWEND, tcUNTIL);
}
//===========================================
// INPUT command
// Input a var value. The prompt is optional.
// INPUT [ prompt, ] var

void Parser::ExecInput()
{
  char var;  // var name
  float value;  // var value

  Scn.ReadToken();  // read prompt or var name

  if (Scn.GetToken() == tcSTR)  // we have a user-defined prompt
  {
    printf("%s ", Scn.GetTokStr());  // display prompt
    Scn.ReadToken();  // read ,

    if (Scn.GetToken() != tcCOMMA)
    {
      ErrRpt.Error(ecCOMMA_MISSING);
      return;
    }

    Scn.ReadToken();  // read var name
  }
  else  // no user-defined prompt present
    printf("? ");  // display the default prompt ?

  if (Scn.GetToken() != tcVAR)  // no var name
  {
    ErrRpt.Error(ecVAR_MISSING);
    return;
  }

  var = toupper(Scn.GetTokStr()[0]);  // get var name
  scanf("%f", &value);
  VarTbl.Set(var, double(value));  // save var value in VarTbl
  Scn.ReadToken();
}
//===========================================
// PRINT command
// Print a comma- or tab-separated list of strings or expressions.
// The comma , causes the display of a space.
// The semicolon ; causes the display of a tab.
// PRINT
// PRINT str [, ...]
// PRINT expr [, ...]

void Parser::ExecPrint()
{
  double value;  // expr value
  bool done = false;

  Scn.ReadToken();

  while (!done)
  {
    switch (Scn.GetToken())
    {
      case tcEOL:  // terminate loop
        printf("\n");
        Scn.ReadToken();
        done = true;
        break;

      case tcCOMMA:  // print a space
        printf(" ");
        Scn.ReadToken();
        break;

      case tcSEMI:  // print a tab
        printf("\t");
        Scn.ReadToken();
        break;

      case tcSTR:  // str literal
        printf(Scn.GetTokStr());  // print it
        Scn.ReadToken();
        break;

      default:  // expr
        value = EvalExpr();  // get value of expr
        DispFloat(value, Precision);  // print it
        break;
    }
  }
}
//===========================================
// RANDOMIZE command
// Set the seed value for the random-number generator.
// Seed must be unsigned int.
// RANDOMIZE seed

void Parser::ExecRandomize()
{
  double seed;

  Scn.ReadToken();  // read seed
  seed = EvalExpr();  // get value of seed

  if (seed < 0.0)  // must be >= 0
  {
    ErrRpt.Error(ecRAND_ARG_NEG);
    return;
  }

  if (!IsInt(seed))  // must be integer
  {
    ErrRpt.Error(ecRAND_ARG_INT);
    seed = RoundOff(seed);
  }

  srand((unsigned int)seed);

  if (DebMode)
  {
    printf("Seed = ");
    DispFloat(seed, 0);
    printf("\n");
  }
}
//===========================================
// PRECISION command
// Set the precison of displayed numbers.
// Must be 0 <= prec <= 6.
// PRECISION prec

void Parser::ExecPrecision()
{
  double prec;  // precision value = num of dec places to display

  Scn.ReadToken();  // read prec
  prec = EvalExpr();  // get prec value

  if (prec < 0.0)  // must be >= 0
  {
    ErrRpt.Error(ecPREC_ARG_NEG);
    prec = 0.0;
  }

  if (!IsInt(prec))  // must be integer
  {
    ErrRpt.Error(ecPREC_ARG_INT);
    prec = RoundOff(prec);
  }

  Precision = int(prec);

  if (DebMode)
  {
    printf("Precision = ");
    DispFloat(prec, 0);
    printf("\n");
  }
}
//===========================================
// DEB_MODE command
// Set the DebMode var to true/false value.
// DEB_MODE ON | OFF

void Parser::ExecDebMode()
{
  TokCode tok;

  tok = Scn.ReadToken();  // read the on/off value

  if (tok != tcON && tok != tcOFF)
  {
    ErrRpt.Error(ecON_OFF_MISSING); 
    return;
  }

  DebMode = (Scn.GetToken() == tcON);
  Scn.ReadToken();

  if (DebMode)
  {
    printf("Debug Mode = ");
    DebMode ? printf("ON") : printf("OFF");
    printf("\n");
  }
}
//===========================================

