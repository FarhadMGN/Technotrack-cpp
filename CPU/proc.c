#include <stdio.h>
#include <math.h>
#include <string.h>

#define name "inv.txt"

enum comand
{
    push = 1,
    pop  = 2,
    add  = 3,
    sub  = 4,
    mul  = 5,
    div  = 6,
    out  = 7,
    end  = 8
};

int main ()
{
    char* comandArr = OpFileAndCrBuf (name, nSymb);
    int* arr = Compile (commandArr);
    int res = CPU (arr);
    printf("%d\n", res);
    return 0;
}

char* OpFileAndCrBuf (char* path, long* nSymbol)
{
    if (path == NULL)
        ERROR(NULL, EINVAL);

    FILE* original;
    original = fopen (path, "r");
    if (original == NULL)
        return NULL;

    int SizeFile = ReturnSizeOfFile (original);
    if (SizeFile < 0)
        ERROR(NULL,EINVAL);

    char* text = (char*)calloc ((SizeFile + 1), 1);
    if (text == NULL)
        return NULL;

    if (fread (text, 1, SizeFile, original) != SizeFile)
    {
        free (text);
        return NULL;
    }
    fclose (original);
    *nSymbol = SizeFile;
    return text;
}

int* Compile (const char** arrPtr)
{
    FILE* res;
    start = fopen (path, "wb");

    char* buf = (char*)calloc (nElem + 1, sizeof(char));
    fread (buf, 1, nElem, start);

    int i = 0;
    while (str[i] != '\0')
    {
        
    }

    fclose (start);
    return res;
}
