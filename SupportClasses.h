//===========================================
//
//  SupportClasses.h
//
// Author: Theo P. (theo_pap@otenet.gr)
// Language used: C++
// Copyright: No copyright. You can do with this software whatever
// you like.
// Warranty: No warranty. Use this software at your own risk.
//===========================================

#ifndef SUPPORT_CLASSES_H
#define SUPPORT_CLASSES_H

#include "Scanner.h"

//===========================================
// *** CONST ***

const int MAX_STACK = 100;  // stack size
const int NUM_GOSUB_NEST = 32; // max num of gosub nesting levels
const int NUM_FOR_NEST = 32;  // max num of FOR nesting levels
const int NUM_WHILE_NEST = 32; // max num of WHILE nesting levels
const int NUM_DO_NEST = 32;  // max num of DO nesting levels
const int NUM_VARS = 26;  // num of predefined vars A ... Z
//===========================================
class Stack
{
public:
  Stack();

  bool IsEmpty() const  { return Tos == 0; }
  bool IsFull() const  { return Tos == MAX_STACK; }

  void Push(double num);
  double Pop();

private:
  double Array[MAX_STACK];
  int Tos;
};
//===========================================
//===========================================
class GosubStack
{
public:
  GosubStack();

  bool IsEmpty() const  { return Tos == 0; }
  bool IsFull() const  { return Tos == NUM_GOSUB_NEST; }

  void Push(char* loc);
  char* Pop();

private:
  char* Array[NUM_GOSUB_NEST];
  int Tos;
};
//===========================================
//===========================================
struct ForStkItem  // item of FOR stack
{
  char Var;  // counter var name
  double EndValue;  // end value of counter
  double StepValue;  // step value of counter
  char* Loc;  // loc of FOR command in source
};
//===========================================
class ForStack
{
public:
  ForStack();

  bool IsEmpty() const  { return Tos == 0; }
  bool IsFull() const  { return Tos == NUM_FOR_NEST; }

  void Push(ForStkItem& i);
  ForStkItem& Pop();
  ForStkItem& Peek();

private:
  ForStkItem Array[NUM_FOR_NEST];
  int Tos;
};
//===========================================
//===========================================
struct WhileStkItem  // item of WHILE stack
{
  char Var;  // control var name
  TokCode Op;  // relational op
  double Expr;  // value to compare Var against
  char* Loc;  // loc of WHILE command in source
};
//===========================================
class WhileStack
{
public:
  WhileStack();

  bool IsEmpty()  { return Tos == 0; }
  bool IsFull()  { return Tos == NUM_WHILE_NEST; }

  void Push(WhileStkItem& i);
  WhileStkItem& Pop();
  WhileStkItem& Peek();

private:
  WhileStkItem Array[NUM_WHILE_NEST];
  int Tos;
};
//===========================================
//===========================================
struct DoStkItem  // item of DO stack
{
  char Var;  // control var name
  TokCode Op;  // relational op
  double Expr;  // value to compare Var against
  char* Loc;  // loc of DO command in source
};
//===========================================
class DoStack
{
public:
  DoStack();

  bool IsEmpty()  { return Tos == 0; }
  bool IsFull()  { return Tos == NUM_DO_NEST; }

  void Push(DoStkItem& i);
  DoStkItem& Pop();

private:
  DoStkItem Array[NUM_DO_NEST];
  int Tos;
};
//===========================================
//===========================================
class VarTable  // predefined variables table
{
public:
  VarTable();

  void Set(char var, double value);
  double Get(char var);

private:
  double Array[NUM_VARS];  // actual var table
};
//===========================================

#endif
