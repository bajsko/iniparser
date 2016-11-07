# INIparser
A simple and lightweight .INI file parser written in C.

Note: INIparser currently uses the stack for its memory.

## Usage

Simply include ```ini.h``` in your project and then call ```ini_parse(const char* filename, handler_func callback_reciever```.
``ìni_parse`` requires a callback receiver that recieves the current section, key and value parsed., eg:

```handle(const char* section, const char* key, const char* value)```

## Configuration

Inside ``ìni.h`` you can find some #DEFINES that sets how much memory is allocated for different stuff.
See ``ini.h`` for more info.

## Example

```
#include "ini.h"

static char lastSection[INI_SECTION_SIZE];

int handle(const char* section, const char* key, const char* value)
{
    if(strcmp(lastSection, section) != 0)
    {
        strcpy(lastSection, section);
        printf("[%s]\n", lastSection);
    }
    
    printf("%s=%s\n",key,value);
    
    return 0;
}

int main(int argc, const char * argv[]) {
    
    int ini_error = ini_parse("test.ini", handle);
    
    if(ini_error != INI_OK)
        printf("ini parse error: %d\n", ini_error);
    
    return 0;
}
```
