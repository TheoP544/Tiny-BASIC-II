//===========================================
//
//  SupportClasses.cpp
//
// Author: Theo P. (theo_pap@otenet.gr)
// Language used: C++
// Copyright: No copyright. You can do with this software whatever
// you like.
// Warranty: No warranty. Use this software at your own risk.
//===========================================

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "Error.h"
#include "Misc.h"
#include"SupportClasses.h"

//===========================================
Stack::Stack()
{
  for (int i = 0; i < MAX_STACK; i++)
    Array[i] = 0.0;

  Tos = 0;
}
//===========================================
void Stack::Push(double num)
{
  if (IsFull())
  {
    ErrRpt.Error(ecSTK_FULL);
    return;
  }

  Array[Tos++] = num;
}
//===========================================
double Stack::Pop()
{
  if (IsEmpty())
  {
    ErrRpt.Error(ecSTK_EMPTY);
    return 0.0;
  }
	
  return Array[--Tos];
}
//===========================================
//===========================================
GosubStack::GosubStack()
{
  for (int i = 0; i < NUM_GOSUB_NEST; i++)
    Array[i] = NULL;

  Tos = 0;
}
//===========================================
void GosubStack::Push(char* loc)
{
  if (IsFull())
  {
    ErrRpt.Error(ecGOSUB_FULL);
    return;
  }

  Array[Tos++] = loc;
}
//===========================================
char* GosubStack::Pop()
{
  if (IsEmpty())
  {
    ErrRpt.Error(ecGOSUB_EMPTY);
    return NULL;
  }

  return Array[--Tos];
}
//===========================================
//===========================================
// Invalid FOR stack item. Used as retutn value in Pop() and Peek();
ForStkItem fsi_INVALID = { 0, 0.0, 0.0, NULL };
//===========================================
ForStack::ForStack()
{
  for (int i = 0; i < NUM_FOR_NEST; i++)
  {
    Array[i].Var = 0;
    Array[i].EndValue = Array[i].StepValue = 0.0;
    Array[i].Loc = NULL;
  }

  Tos = 0;
}
//===========================================
void ForStack::Push(ForStkItem& i)
{
  if (IsFull())
  {
    ErrRpt.Error(ecFOR_FULL);
    return;
  }

  Array[Tos++] = i;
}
//===========================================
ForStkItem& ForStack::Pop()
{
  if (IsEmpty())
  {
    ErrRpt.Error(ecFOR_EMPTY);
    return fsi_INVALID;
  }

  return Array[--Tos];
}
//===========================================
ForStkItem& ForStack::Peek()
{
  if (IsEmpty())
  {
    ErrRpt.Error(ecFOR_EMPTY2);
    return fsi_INVALID;
  }

  return Array[Tos-1];
}
//===========================================
//===========================================
// Invalid WHILE stack item. Used as retutn value in Pop() and Peek();
WhileStkItem wsi_INVALID = { 0, tcINVALID, 0.0, NULL };
//===========================================
WhileStack::WhileStack()
{
  for (int i = 0; i < NUM_WHILE_NEST; i++)
  {
    Array[i].Var = 0;
//    Array[i].Op = tcINVALID;
    Array[i].Op = tcINVALID;
    Array[i].Expr = 0.0;
    Array[i].Loc = NULL;
  }

  Tos = 0;
}
//===========================================
// Push an item on WHILE stack.

void WhileStack::Push(WhileStkItem& i)
{
  if (IsFull())
  {
    ErrRpt.Error(ecWHILE_FULL);
    return;
  }

  Array[Tos++] = i;
}
//===========================================
// Pop an item from WHILE stack.

WhileStkItem& WhileStack::Pop()
{
  if (IsEmpty())
  {
    ErrRpt.Error(ecWHILE_EMPTY);
    return wsi_INVALID;;
  }

  return Array[--Tos];
}
//===========================================
// Get the top item from WHILE stack, without removing it.

WhileStkItem& WhileStack::Peek()
{
  if (IsEmpty())
  {
    ErrRpt.Error(ecWHILE_EMPTY2);
    return wsi_INVALID;
  }

  return Array[Tos-1];
}
//===========================================
//===========================================
DoStkItem dsi_INVALID = { 0, tcINVALID, 0.0, NULL };
//===========================================
DoStack::DoStack()
{
  for (int i = 0; i < NUM_DO_NEST; i++)
  {
    Array[i].Var = 0;
    Array[i].Op = tcINVALID;
    Array[i].Expr = 0.0;
    Array[i].Loc = NULL;
  }

  Tos = 0;
}
//===========================================
// Push an item on DO stack.

void DoStack::Push(DoStkItem& i)
{
  if (IsFull())
  {
    ErrRpt.Error(ecDO_FULL);
    return;
  }

  Array[Tos++] = i;
}
//===========================================
// Pop an item from DO stack.

DoStkItem& DoStack::Pop()
{
  if (IsEmpty())
  {
    ErrRpt.Error(ecDO_EMPTY);
    return dsi_INVALID;
  }

  return Array[--Tos];
}
//===========================================
//===========================================
VarTable::VarTable()
{
  for (int i = 0; i < NUM_VARS; i++)
    Array[i] = 0.0;
}
//===========================================
// Set var value.
// var = var name = A ... Z

void VarTable::Set(char var, double value)
{
  if (!isalpha(var))
  {
    ErrRpt.Error(ecILL_VAR_NAME);
    return;
  }

  Array[toupper(var) - 'A'] = value;
}
//===========================================
// Get var value.
// var = var name = A ... Z

double VarTable::Get(char var)
{
  if (!isalpha(var))
  {
    ErrRpt.Error(ecILL_VAR_NAME);
    return 0.0;
  }

  return Array[toupper(var) - 'A'];
}
//===========================================
