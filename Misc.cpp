//===========================================
//
//  Misc.cpp
//
// Author: Theo P. (theo_pap@otenet.gr)
// Language used: C++
// Copyright: No copyright. You can do with this software whatever
// you like.
// Warranty: No warranty. Use this software at your own risk.
//===========================================

#include <stdio.h>
#include <conio.h>
#include "Misc.h"

//===========================================
// *** GLOBAL VAR ***

int Line;  // current line num in source
//===========================================
// Round-off a double num to the nearest int.
// e.g.:
// 2.4 -> 2, 2.5 -> 3
// -2.4 -> -2, -2.5 -> -3

int RoundOff(double num)
{
  int res;
  bool is_neg = false;

  if (num < 0.0)
  {
    is_neg = true;
    num = -num;
  }

  res = int(num + 0.5);

  if (is_neg)
    res = -res;

  return res;
}
//===========================================
// Truncate a double num to the smaller int.
// e.g.:
// 2.4 -> 2, 2.5 -> 2
// -2.4 -> -2, -2.5 -> -2

int Trunc(double num)
{
  int res;
  bool is_neg = false;

  if (num < 0.0)
  {
    is_neg = true;
    num = -num;
  }

  res = int(num);

  if (is_neg)
    res = -res;

  return res;
}
//===========================================
// Return true if num is an integer.

bool IsInt(double num)
{
  return num == double(int(num));
}
//===========================================
// Display a char count times.

void DispCh(char ch, int count)
{
  while (count)
  {
    _putch(ch);
    count--;
  }
}
//===========================================
// Display a logical value as TRUE or FALSE.

void DispLogValue(double value)
{
  value ? printf("TRUE") : printf("FALSE");
}
//===========================================
// Display a double num with the given precision ndp.
// ndp = number of decimal places.
// Must be 0 <= ndp <= 6.

void DispFloat(double num, int ndp)
{
  int i, ip;  // ip = integer part
  double fp;  // fp = fractional part

  if (ndp > 6)
    ndp = 6;  // max precision possible

  if (num == 0.0)
  {
    printf("0");

    if (ndp == 0)
      return;

    printf(".");

    for (i = 0; i < ndp; i++)
      printf("0");

    return;
  }

  if (num < 0.0)
  {
    printf("-");
    num = -num;
  }

  for (i = 0; i < ndp; i++)
    num *= 10;

  num = double(int(num + 0.5));

  for (i = 0; i < ndp; i++)
    num /= 10;

  ip = int(num);
  fp = num - double(ip);
  printf("%d", ip);

  if (ndp == 0)
    return;

  printf(".");

  if (fp == 0.0)
  {
    for (i = 0; i < ndp; i++)
      printf("0");

    return;
  }

  for (i = 0; i < ndp; i++)
    fp *= 10;

  printf("%d", int(fp));
}
//===========================================

