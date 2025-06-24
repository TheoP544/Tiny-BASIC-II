//===========================================
//
//  Scanner.h
//
// Author: Theo P. (theo_pap@otenet.gr)
// Language used: C++
// Copyright: No copyright. You can do with this software whatever
// you like.
// Warranty: No warranty. Use this software at your own risk.
//===========================================

#ifndef SCANNER_H
#define SCANNER_H

#include "LblTable.h"

//===========================================
// *** CONST ***

const int TOK_STR_LEN = 64;  // max token str len
//===========================================
// *** DEFINITIONS ***

enum TokCode  // token code
{
// logical ops
  tcOR,
  tcAND,
  tcNOT,

// commands
  tcIF,
  tcTHEN,
  tcELSE,
  tcENDIF,

  tcFOR,
  tcTO,
  tcSTEP,
  tcNEXT,

  tcWHILE,
  tcWEND,

  tcDO,
  tcUNTIL,

  tcBREAK,
  tcCONTINUE,

  tcGOTO,

  tcGOSUB,
  tcRETURN,

  tcEND,

  tcINPUT,
  tcPRINT,
  tcRANDOMIZE,

// built-in funcs
  tcABS,
  tcSGN,
  tcCINT,
  tcFIX,
  tcSQR,
  tcPOW,
  tcEXP,
  tcLOG,
  tcRND,

// immediate commands
  tcPRECISION,
  tcDEB_MODE,

// values of DEB_MODE
  tcON,
  tcOFF,

// arithmetic ops
  tcPLUS,  // +
  tcMINUS,  // -
  tcSTAR,  // *
  tcSLASH,  // /
  tcPERC,  // %

// parentheses ops
  tcLPAR,  // (
  tcRPAR,  // )

// relational ops
  tcLT,  // <
  tcLE,  // <=
  tcGT,  // >
  tcGE,  // >=
  tcEQ,  // =
  tcNE,  // <>

// misc
  tcCOMMA,  // ,
  tcSEMI,  // ;

// tokens with user-defined content
  tcVAR,  // variable
  tcNUM,  // number literal
  tcSTR,  // string literal

// special
  tcEOL,  // end of line
  tcEOF,  // end of file

  tcINVALID  // illegal token
};
//===========================================
//===========================================
class Scanner
{
public:
  Scanner();
  ~Scanner();

  void Init(const char* fname);

  TokCode GetToken()  { return Token; }
  char* GetTokStr()  { return TokStr; }
  char* GetProg()  { return Prog; }
  void SetProg(char* loc)  { Prog = loc; }

  TokCode ReadToken();

  void SkipToEOL();
  char* FindLblLoc(const char* name);

  TokCode FindToken(const char* str);
  const char* FindTokStr(TokCode tok);

  void DispSource() const;
  void DispTokens();
  void DispLblTbl();

private:
  int GetFileSize(FILE* fp);
  void FilterCR();

  void ScanLabels();

  bool IsWhite(char ch);
  void SkipWhite();

  void ReadComment();
  void ReadEOL();
  void ReadNum();
  void ReadStr();
  void ReadAlpha();
  void ReadOp1();
  void ReadOp2();
  void ReadOp3();

///////////////////////////////////////////

  char* Source;  // source buffer
  char* Prog;  // current loc in source
  TokCode Token;  // current token code
  char TokStr[TOK_STR_LEN+1];  // current token str

  LblTable LblTbl;  // label table
};
//===========================================

#endif
