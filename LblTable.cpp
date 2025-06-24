//===========================================
//
//  LblTable.cpp
//
// Author: Theo P. (theo_pap@otenet.gr)
// Language used: C++
// Copyright: No copyright. You can do with this software whatever
// you like.
// Warranty: No warranty. Use this software at your own risk.
//===========================================

#include <stdio.h>
#include <string.h>
#include "Misc.h"
#include "Error.h"
#include "LblTable.h"

//===========================================
LblTable::LblTable()
{
  for (int i = 0; i < NUM_LBLS; i++)
  {
    Array[i].Name[0] = 0;
    Array[i].Loc = NULL;
    Array[i].Line = 0;
  }

  Counter = 0;
}
//===========================================
// Insert label name into the label table.

void LblTable::Insert(const char* name, char* loc, int line)
{
  char* lbl_loc;

  if (IsFull())
  {
    ErrRpt.Error(ecLBL_FULL);  // table is full
    return;
  }

  lbl_loc = FindLoc(name);

  if (lbl_loc)
  {
    ErrRpt.Error(ecLBL_DUPL);  // duplicate lbl, don't insert
    return;
  }

  strcpy(Array[Counter].Name, name);
  Array[Counter].Loc = loc;
  Array[Counter].Line = line;
  Counter++;
}
//===========================================
// Find location of label name.

char* LblTable::FindLoc(const char* name) const
{
  for (int i = 0; i < Counter; i++)
    if (!stricmp(Array[i].Name, name))
      return Array[i].Loc;

  return NULL;  // no such label
}
//===========================================
// Display the label table.
// This is useful for debug purposes.

void LblTable::Display() const
{
  if (IsEmpty())
  {
    printf("Label table is empty.\n\n");
    return;
  }

  DispCh('=', SCR_LINE_WIDTH);
  printf("\nLabel Table:\n\n");

  printf("Name   Line      Loc\n");
  DispCh('-', SCR_LINE_WIDTH);
  printf("\n");

  for (int i = 0; i < Counter; i++)
    printf("%s     %4d      0x%p\n", Array[i].Name, Array[i].Line,
     Array[i].Loc);

  DispCh('-', SCR_LINE_WIDTH);
  printf("\n\nLabels = %d\n", Counter);
  DispCh('=', SCR_LINE_WIDTH);
  DispCh('\n', 2);
}
//===========================================
