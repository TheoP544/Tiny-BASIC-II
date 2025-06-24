//===========================================
//
//  LblTable.h
//
// Author: Theo P. (theo_pap@otenet.gr)
// Language used: C++
// Copyright: No copyright. You can do with this software whatever
// you like.
// Warranty: No warranty. Use this software at your own risk.
//===========================================

#ifndef LBL_TABLE_H
#define LBL_TABLE_H

//===========================================
const int NUM_LBLS = 512;  // max num of lbls
const int LBL_NAME_LEN = 64;  // max lbl name len = TOK_STR_LEN
//===========================================
struct LblTblItem  // item of label table
{
  char Name[LBL_NAME_LEN+1];  // lbl name
  char* Loc;  // loc of lbl in source
  int Line;  // line num of lbl in source
};
//===========================================
class LblTable  // label table
{
public:
  LblTable();

  bool IsEmpty() const  { return Counter == 0; }
  bool IsFull() const  { return Counter == NUM_LBLS; }

  void Insert(const char* name, char* loc, int line);
  char* FindLoc(const char* name) const;

  void Display() const;

private:
  LblTblItem Array[NUM_LBLS];  // actual lbl table
  int Counter;  // num of lbls in table
};
//===========================================

#endif
