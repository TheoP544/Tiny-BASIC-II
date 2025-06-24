//===========================================
//
//  Interpreter.cpp
//
// Author: Theo P. (theo_pap@otenet.gr)
// Language used: C++
// Copyright: No copyright. You can do with this software whatever
// you like.
// Warranty: No warranty. Use this software at your own risk.
//===========================================

#include <stdio.h>
#include "Parser.h"

#include <stdlib.h>

//===========================================
void main0()
{
  Parser p;

  p.Init("Test0.bas.");
  p.DispSource();
  p.DispLblTbl();
  p.DispTokens();
  printf("\n");
}
//===========================================
void main(int argc, const char* argv[])
{
  Parser p;

  if (argc != 2)
  {
    printf("Usage: argv[0] <file_name>\n");
    return;
  }

  p.Init(argv[1]);
  p.DispSource();
  p.Execute();
  printf("\n");
}
//===========================================
