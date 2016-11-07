// *****************************************
// Filename: ini.c
// Author: Klas Henriksson
//
//
// Revision History:
//      -
//
// Copyright (c), Klas Henriksson.
//
// *****************************************

#include "ini.h"

//strips the string of any spaces
static void wsstrip(char* string)
{
    char* p = string;
    int length = (int)strlen(string) + 1;
    while(*p)
    {
        if(isspace(*p)) {
            char* end = string + length;
            while (isspace(*p))
                for(char* c = p; c != end; c++)
                    *c = *(c + 1);
        }
        else {
            p++;
        }
    }
}

//finds the first character of value (non space), returns 0 if null terminator is reached
static char* vfind(char* string)
{
    char* p = string;
    while(*p) {
        if(!isspace(*p))
            break;
        else
            p++;
    }
    
    return p;
}

//finds the first occurance of specified character, returns NULL if not found
static char* ffind(char* string, char character)
{
    char* p = string;
    while(*p != character && *p)
        p++;
    
    return p;
}

//finds the first occurance of specified characters, returns NULL if not found
static char* ffinda(char* string, char* characters, int arraysize)
{
    char* p = string;
    char found = 0;
    while(*p)
    {
        for(int i = 0; i < arraysize; i++) {
            if(*p == characters[i]) {
                found = 1;
                break;
            }
        }
        
        if(found)
            break;
        
        p++;
    }
    
    return p;
}

static int parse(FILE* file, handler_func handler)
{
    char line[INI_LINE_SIZE];
    char section[INI_SECTION_SIZE] = { 0 };
    char key[INI_KEY_SIZE] = { 0 };
    
    int error = INI_OK;
    
    while(ferror(file) == 0 && !feof(file))
    {
        fgets(line, INI_LINE_SIZE, file);
        
        wsstrip(line);
        char* start = vfind(line);
        
        if(*start == ';' || *start == '#' || *start == '\0') {
            continue; //skip comments and null-lines
        }
        
        
        if(*start == '[') //section
        {
            char* sn = start + 1;
            char* end = ffind(sn, ']');
            
            if(!end) {
                error =  INI_ERROR_SECTION_INCOMPLETE;

#if INI_SURPRESS_WARNINGS
                continue;
#else
                return error;
#endif
            }
            
            int diff = (int)((end-1)-sn);
            
            if(diff >= INI_SECTION_SIZE) {
                error = INI_ERROR_SECTION_TOO_LONG;
#if INI_SURPRESS_WARNINGS
                continue;
#else
                return error;
#endif
            }
            
            *end = '\0';
            strcpy(section, sn);
            
        } else //this has to be a key
        {
            char* end = ffind(start,'=');
            if(!end) {
                error = INI_ERROR_INVALID_KEY;
#if INI_SURPRESS_WARNINGS
                continue;
#else
                return error;
#endif
            }
            
            if(section[0] == '\0') {
                error = INI_ERROR_NO_SECTION;
#if INI_SURPRESS_WARNINGS
                continue;
#else
                return error;
#endif
            }
            
            int diff = (int)(end-start);
            
            if(diff >= INI_KEY_SIZE) {
                error = INI_ERROR_KEY_TOO_LONG;
#if INI_SURPRESS_WARNINGS
                continue;
#else
                return error;
#endif
            }
            
            *end = '\0';
            strcpy(key, start);
            
            char* value = end + 1;
            
            char filter[] = { ';', '#' };
            end = ffinda(value, filter, 2);
            *end='\0';
            
            handler(section, key, value);
        }
    }
    
    return error;
}


int ini_parse(const char* filepath, handler_func handler)
{
    FILE* file = fopen(filepath, "rb");
    if(!file)
        return INI_ERROR_FOPEN;
    
    int error = parse(file, handler);
    
    fclose(file);
    
    return error;
}
