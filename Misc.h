//===========================================
//
//  Misc.h
//
// Author: Theo P. (theo_pap@otenet.gr)
// Language used: C++
// Copyright: No copyright. You can do with this software whatever
// you like.
// Warranty: No warranty. Use this software at your own risk.
//===========================================

#ifndef MISC_H
#define MISC_H

//===========================================
// *** GLOBAL CONST ***

// Width of separator line displayed on screen
const int SCR_LINE_WIDTH = 50;
//===========================================
// *** GLOBAL VAR ***

extern int Line;  // current line num in source
//===========================================
bool IsInt(double num);
int RoundOff(double num);
int Trunc(double num);
void DispCh(char ch, int count = 1);
void DispLogValue(double value);
void DispFloat(double num, int ndp);
//===========================================

#endif
