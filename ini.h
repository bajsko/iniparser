// *****************************************
// Filename: ini.h
// Author: Klas Henriksson
//
// Purpose:
//      Provides simple access to .INI files
//
// Revision History:
//      -
//
// Copyright (c), Klas Henriksson.
//
// *****************************************

//currently uses stack, no heap at all.

#ifndef _INI_H_
#define _INI_H_

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#define INI_SECTION_SIZE 33 //32 characters + null terminator, crank this up if you'd like
#define INI_KEY_SIZE 65 //double (32*2) + 1
#define INI_LINE_SIZE 200 //199 chars + null terminator, increase if you need to
#define INI_VALUE_SIZE INI_LINE_SIZE - INI_VALUE_SIZE

#define INI_SURPRESS_WARNINGS 1 //set this to false if the parser should return after first failure.

enum
{
    INI_OK = 0,
    INI_ERROR_FOPEN = -1,
    INI_ERROR_FREAD = -2,
    INI_ERROR_VFIND = -3,
    INI_ERROR_SECTION_INCOMPLETE = -4,
    INI_ERROR_SECTION_TOO_LONG = -5,
    INI_ERROR_NO_SECTION = -6,
    INI_ERROR_INVALID_KEY = -7,
    INI_ERROR_INVALUD_VALUE = -8,
    INI_ERROR_KEY_TOO_LONG = -9,
};

typedef int (*handler_func)(const char* section, const char* key, const char* value);

int ini_parse(const char* filepath, handler_func handler);

#endif
