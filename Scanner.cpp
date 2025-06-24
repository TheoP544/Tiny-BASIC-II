//===========================================
//
//  Scanner.cpp
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
#include <sys\stat.h>
#include "Error.h"
#include "Misc.h"
#include "Scanner.h"

//===========================================
struct TokTblItem  // item of TokTbl
{
  TokCode Token;  // token code
  char Str[TOK_STR_LEN+1];  // token str
};
//===========================================
TokTblItem TokTbl[] =  // token table
{
// logical ops
  tcOR, "OR",
  tcAND, "AND",
  tcNOT, "NOT",

// commands
  tcIF, "IF",
  tcTHEN, "THEN",
  tcELSE, "ELSE",
  tcENDIF, "ENDIF",

  tcFOR, "FOR",
  tcTO, "TO",
  tcSTEP, "STEP",
  tcNEXT, "NEXT",

  tcWHILE, "WHILE",
  tcWEND, "WEND",

  tcDO, "DO",
  tcUNTIL, "UNTIL",

  tcBREAK, "BREAK",
  tcCONTINUE, "CONTINUE",

  tcGOTO, "GOTO",

  tcGOSUB, "GOSUB",
  tcRETURN, "RETURN",

  tcEND, "END",  // end of prog

  tcINPUT, "INPUT",
  tcPRINT, "PRINT",
  tcRANDOMIZE, "RANDOMIZE",

// built-in funcs
  tcABS, "ABS",
  tcSGN, "SGN",
  tcCINT, "CINT",
  tcFIX, "FIX",
  tcSQR, "SQR",
  tcPOW, "POW",
  tcEXP, "EXP",
  tcLOG, "LOG",
  tcRND, "RND",

// immediate commands
  tcPRECISION, "PRECISION",
  tcDEB_MODE, "DEB_MODE",

// values of DEB_MODE
  tcON, "ON",
  tcOFF, "OFF",

// arithmetic ops
  tcPLUS, "+",
  tcMINUS, "-",
  tcSTAR, "*",
  tcSLASH, "/",
  tcPERC, "%",

// parentheses ops
  tcLPAR, "(",
  tcRPAR, ")",

// relational ops
  tcLT, "<",
  tcLE, "<=",
  tcGT, ">",
  tcGE, ">=",
  tcEQ, "=",
  tcNE, "<>",

// misc
  tcCOMMA, ",",
  tcSEMI, ";",

  tcINVALID, ""  // terminal mark. Do not remove.
};
//===========================================
//===========================================
Scanner::Scanner()
{
  Source = Prog = NULL;
  Token = tcINVALID;
  TokStr[0] = 0;
  Line = 0;
}
//===========================================
Scanner::~Scanner()
{
  delete [] Source;
  Source = NULL;
}
//===========================================
// Return the file size in bytes of file fp.

int Scanner::GetFileSize(FILE* fp)
{
  struct _stat statbuf;
	
  _fstat(fileno(fp), &statbuf);
  return statbuf.st_size;
}
//===========================================
// Initialize the scanner. Load the file fname into Source buffer.

void Scanner::Init(const char* fname)
{
  int fsize, count;
  FILE* fp;

  if (fname == NULL)
    ErrRpt.FatalError(ecFNAME_NULL);

  if (fname[0] == 0)
    ErrRpt.FatalError(ecFNAME_EMPTY);

  fp = fopen(fname, "rb");

  if (fp == NULL)
    ErrRpt.FatalError(ecFOPEN, fname);

  fsize = GetFileSize(fp);
  Source = new char [fsize+1];

  if (Source == NULL)
    ErrRpt.FatalError(ecMEM_ALLOC);

  count = fread(Source, 1, fsize, fp);
  Source[count] = 0;
  fclose(fp);
  FilterCR();
  Prog = Source;
  Line = 1;
  ScanLabels();
}
//===========================================
// Preprocessor scan.
//  Scan the source for labels and insert them into the label table.

void Scanner::ScanLabels()
{
  bool done = false;

  Prog = Source;
  Line = 1;

  while (!done)
  {
    ReadToken();

    switch (Token)
    {
      case tcEOF:
        done = true;
        break;

      case tcEOL:
        break;

      case tcNUM:
        if (LblTbl.IsFull())  // lbl table is full, so we are done
          done = true;
        else
          LblTbl.Insert(TokStr, Prog, Line);

        SkipToEOL();
        break;

      // not a number => skip the rest of line and go to the next line
      default:
        SkipToEOL();
        break;
    }
  }

  Prog = Source;
  Line = 1;
}
//===========================================
// Filter out the CR (Carriage Return) chars from the Source buffer.

void Scanner::FilterCR()
{
  char* s = Source;  // ptr to source char
  char* d = s;  // ptr to destination char

  while (*s)
  {
    if (*s == '\r')  // CR char => skip it
      s++;
    else
    *d++ = *s++;  // any other char => copy it
  }

  *d = 0;
}
//===========================================
// Return token code corresponding to token string str.

TokCode Scanner::FindToken(const char* str)
{
  for (int i = 0; TokTbl[i].Token != tcINVALID; i++)
    if (!stricmp(TokTbl[i].Str, str))
      return TokTbl[i].Token;

  return tcINVALID;  // str is not a valid token string
}
//===========================================
// Return token string corresponding to token code tok.

const char* Scanner::FindTokStr(TokCode tok)
{
  for (int i = 0; TokTbl[i].Token != tcINVALID; i++)
    if (TokTbl[i].Token == tok)
      return TokTbl[i].Str;

  return NULL;  // tok is not a valid token
}
//===========================================
// Find loc in source of label name.

char* Scanner::FindLblLoc(const char* name)
{
  return LblTbl.FindLoc(name);
}
//===========================================
// Return true if char is a white char, i.e. space or tab.

bool Scanner::IsWhite(char ch)
{
  return ch == ' ' || ch == '\t';
}
//===========================================
// Skip white chars.

void Scanner::SkipWhite()
{
  while (IsWhite(*Prog))
    Prog++;
}
//===========================================
// Skip chars to end of line, then go to the beginning of next line.

void Scanner::SkipToEOL()
{
  while (*Prog != '\n' && *Prog)
    Prog++;

  if (*Prog == '\n')
  {
    Prog++;  // skip EOL
    Line++;
  }
}
//===========================================
// Read a comment.

void Scanner::ReadComment()
{
  SkipToEOL();  // skip everything to the end of line
  Token = tcEOL;  // a comment is equivalent to EOL
}
//===========================================
//  Read the EOL char.

void Scanner::ReadEOL()
{
  Prog++;
  Line++;
  Token = tcEOL;
}
//===========================================
// Read a number literal.

void Scanner::ReadNum()
{
  char* p = TokStr;

  while (isdigit(*Prog))  // read the int part
    *p++ = *Prog++;

  if (*Prog == '.')  // we have a decimal point
  {
    *p++ = *Prog++;

    while (isdigit(*Prog))  // read the fract part
      *p++ = *Prog++;
  }

  *p = 0;
  Token = tcNUM;
}
//===========================================
// Read a str literal.

void Scanner::ReadStr()
{
  char* p = TokStr;

  Prog++;  // skip "

  while (*Prog != '"' && *Prog != '\n' && *Prog)
    *p++ = *Prog++;

  *p = 0;

  if (*Prog == '"')  // str is terminated OK
  {
    Prog++;  // skip "
    Token = tcSTR;
    return;
  }

  ErrRpt.Error(ecQUOTE_MISSING, TokStr);  // no closing quote "

  if (*Prog == '\n')  // end of line
  {
    Prog++;
    Line++;
  }

  Token = tcINVALID;
}
//===========================================
// Read an identifier, i.e. var name, command or func name.
// An ID must begin with an alpha char and can contain the _ char.

void Scanner::ReadAlpha()
{
  char* p = TokStr;

  while (isalpha(*Prog) || *Prog == '_')
    *p++ = toupper(*Prog++);  // make ID uppercase

  *p = 0;

  if (strlen(TokStr) == 1)  // 1-char ID => var name
  {
    Token = tcVAR;
    return;
  }

  Token = FindToken(TokStr);  // look up ID in token table

  // ID is not in token table, so it's not a command or func name
  if (Token == tcINVALID)
    ErrRpt.Error(ecUNREC_TOKEN, TokStr);
}
//===========================================
// Read a 1-char token.

void Scanner::ReadOp1()
{
  switch (*Prog)
  {
    case '+': Token = tcPLUS; break;
    case '-': Token = tcMINUS; break;
    case '*': Token = tcSTAR; break;
    case '/': Token = tcSLASH; break;
    case '%': Token = tcPERC; break;
    case '(': Token = tcLPAR; break;
    case ')': Token = tcRPAR; break;
    case '=': Token = tcEQ; break;
    case ',': Token = tcCOMMA; break;
    case ';': Token = tcSEMI; break;
  }

  Prog++;
}
//===========================================
// Read a 1- or 2-char rel op that begins with <.

void Scanner::ReadOp2()
{
  Prog++;  // skip <

  switch (*Prog)
  {
    case '=': Token = tcLE; Prog++; break;  // <=
    case '>': Token = tcNE; Prog++; break;  // <>
    default:  Token = tcLT; break;  // <
  }
}
//===========================================
// Read a 1- or 2-char rel op that begins with >.

void Scanner::ReadOp3()
{
  Prog++;  // skip >

  switch (*Prog)
  {
    case '=': Token = tcGE; Prog++; break;  // >=
    default:  Token = tcGT; break;  // >
  }
}
//===========================================
// Read a token from the input stream.

TokCode Scanner::ReadToken()
{
  SkipWhite();  // skip leading white chars, if any

  if (*Prog == 0)  // end of file
    Token = tcEOF;
  else if (!_strnicmp(Prog, "REM", 3))  // comment
    ReadComment();
  else if (*Prog == '\n')  // end of line
    ReadEOL();
  else if (isdigit(*Prog))  // num literal
    ReadNum();
  else if (*Prog == '"')  // str literal
    ReadStr();
  else if (isalpha(*Prog))  // identifier
    ReadAlpha();
  else if (strchr("+-*%/()=,;", *Prog) != NULL)  // 1-char token
    ReadOp1();
  else if (*Prog == '<')  // 1- or 2-char rel op, beginning with <
    ReadOp2();
  else if (*Prog == '>')  // 1- or 2-char rel op, beginning with >
    ReadOp3();
  else
  {
    ErrRpt.Error(ecUNREC_TOKEN, TokStr);  // not a valid token
    Token = tcINVALID;
  }

  return Token;
}
//===========================================
// Display the source file.
// Useful for debug purposes.

void Scanner::DispSource() const
{
  char* p = Source;
  int count = 0, line = 1;  // char counter, line counter

  DispCh('=', SCR_LINE_WIDTH);
  printf("\nSource File:\n\n");
  printf("%3d   ", line);

  while (*p)
  {
    if (*p == '\n')
      printf("\n%3d   ", ++line);  // EOL char
    else
      printf("%c", *p);  // printable char

    p++;
    count++;
  }

  printf("\n\nLines = %d, Chars = %d\n", line, count);
  DispCh('=', SCR_LINE_WIDTH);
  DispCh('\n', 2);
}
//===========================================
// Display all the tokens of source.
// Useful for debug purposes.

void Scanner::DispTokens()
{
  int count = 0;  // token counter

  Prog = Source;
  Line = 1;

  DispCh('=', SCR_LINE_WIDTH);
  printf("\nTokens:\n\n");
  printf("Line  Token\n");
  DispCh('-', SCR_LINE_WIDTH);
  printf("\n");

  while (ReadToken() != tcEOF)
  {
    count++;

    switch (Token)
    {
      case tcVAR:
        printf("%3d   Token = Variable, Value = %s\n", Line, TokStr);
        break;

      case tcNUM:
        printf("%3d   Token = Number, Value = %s\n", Line, TokStr);
        break;

      case tcSTR:
        printf("%3d   Token = String, Value = %s\n", Line, TokStr);
        break;

       case tcEOL:
        printf("%3d   Token = EOL\n", Line-1);
        break;

       case tcINVALID:
        printf("%3d   Token = Invalid\n", Line-1);
        break;

      default:  // any other token
        printf("%3d   Token = %s\n", Line, FindTokStr(Token));
    }
  }

  DispCh('-', SCR_LINE_WIDTH);
  printf("\n\nTokens = %d\n", count);
  DispCh('=', SCR_LINE_WIDTH);
  DispCh('\n', 2);

  Prog = Source;
  Line = 1;
}
//===========================================
// Display label table.

void Scanner::DispLblTbl()
{
  LblTbl.Display();
}
//===========================================
