#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

#define FileName "will.txt"
#define PTR_CONST 2
#define ERROR(ret, errval)          \
    do                              \
    {                               \
        errno = errval;             \
        return ret;                 \
    } while( 0 )

/**
 * @brief The struct contains pointer and size of string.
 */
typedef struct string_t
{
    char*   str;
    int     size;
} string_t;


/**
 * @brief The function creates array of pointer to struct.
 *
 * @param buf is pointer to buffer.
 * @param nSymbol is numbers of symbol in buffer.
 * @param nString is numbers of string in buffer.
 *
 * @return pointer to array of struct string_t.
 */
string_t* CreateArrayOfPtr (char* buf, const long nSymbol, const int nString);

/**
 * @brief The function opens file and creates buffer.
 *
 * @param Name is name of file.
 * @param nSymbol is number of symbol in buffer.
 *
 * @return pointer to buffer.
 */
char*   OpFileAndCrBuf (char* Name, long* nSymbol);


/**
 * @brief The function print array.
 *
 * @param array_str pointer to array.
 * @param nLines is number of string in buffer.
 *
 * @return dump of array.
 */
int     writeline (string_t* array_str, int nLines);

/**
 * @brief The function swaps symbol "delete" on symbol "new".
 *
 * @param text is pointer to buffer.
 * @param delite is a deleted symbol.
 * @param new is new sybol
 *
 * @return number of string in buffer.
 */
int     rewriteSting (char* text, char delete, char new);
/**
 * @brief The function compares two string from last symbol to the first.
 *
 * @param str1 is pointer to string number 1.
 * @param str2 is pointer to string number 2.
 *
 * @return difference between string.
 */
int     inverseCompare (const void* a, const void* b);

/**
 * @brief The function compares two string from first symbol to the last.
 *
 * @param str1  is pointer to string number 1.
 * @param str2  is pointer to string number 2.
 *
 * @return differencr between string.
 */
int     strightCompare (const void *a, const void *b);

/**
 * @brief The function counts number of symbol in file.
 *
 * @param path pointer of file.
 *
 * @return numbers of symbol in file.
 */
int     ReturnSizeOfFile (FILE* path);

int main (int argc, char* const argv[])
{
    if(argc != 2)
        return EXIT_FAILURE;

    long    nSymbol = 0;
    char*   buf     = OpFileAndCrBuf (argv[1], &nSymbol);
    if (buf == NULL)
        ERROR(-1,EINVAL);

    int     nString = rewriteSting (buf, '\n', '\0');
    if (nString < 0)
        ERROR(-1,EINVAL);

    string_t* ptr = CreateArrayOfPtr (buf, nSymbol, nString);
    if (ptr == NULL)
        ERROR(-1,EINVAL);

    qsort (ptr, nString, sizeof(string_t), inverseCompare);
    writeline (ptr, nString);

    free (buf);
    free (ptr);

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

int     ReturnSizeOfFile (FILE* path)
{
    if (path == NULL)
        ERROR(-1, EINVAL);

    int     StartPosition   = 0;
    int     SizeFile        = 0;

    StartPosition = ftell (path);
    if (StartPosition == -1L)
        return -1;
    if (fseek (path, 0, SEEK_END) != 0)
        return -1;
    SizeFile = ftell (path);
    if (SizeFile < 0)
        return -1;
    if (fseek (path, 0, StartPosition) != 0)
        return -1;
    return SizeFile;
}

string_t* CreateArrayOfPtr (char* buf, const long nSymbol, const int nString)
{
    if ((buf == NULL) || (nSymbol < 0) || (nString < 0))
        ERROR(NULL, EINVAL);

    string_t* ArrPtr = (string_t*)calloc (nString, sizeof(string_t));
    if (ArrPtr == NULL)
        return NULL;

    long    i = 0;
    int     k = 1;
    int     lastSize = 0;

    ArrPtr[0].str = buf;
    for (; i != nSymbol - 1; i++)
    {
        if (buf[i] == '\0')
        {
            ArrPtr[k].str = buf + i + 1;
            ArrPtr[k - 1].size = ArrPtr[k].str - (ArrPtr[k - 1].str);
            k++;
            lastSize =  buf + nSymbol - ArrPtr[k - 1].str;
        }
    }
    ArrPtr[k - 1].size =  lastSize;
    return ArrPtr;
}

int     rewriteSting (char* text, char delete, char new)
{
    if ((text == NULL) || (delete == '\0'))
        ERROR(-1,EINVAL);

    long    i = 0;
    int     countStr = 0;

    while (text[i] != '\0')
    {
        if (text[i] == delete)
        {
            text[i] = new;
            countStr++;
        }
    i++;
    }
    return countStr;
}

int strightCompare (const void*a, const void* b)
{
    if ((a == NULL) || (b == NULL))
        ERROR(-1,EINVAL);
    string_t str1 = *(string_t*)a;
    string_t str2 = *(string_t*)b;

    while ((*str1.str != '\0') && (*str2.str != '\0')) {
        if (isalpha (*str1.str) && isalpha (*str2.str)) {
            if (*str1.str == *str2.str) {
                 while (*str1.str == *str2.str) {
                    if ((*(str1.str + 1) != '\0') && (*(str2.str + 1) != '\0')) {
                        str1.str++;
                        str2.str++;
                    } else
                        return 0;
                }
            } else
                return *str1.str - *str2.str;
        } else {
            if (!isalpha (*str1.str))
                str1.str++;
            else
                str2.str++;
        }
    }
}

int inverseCompare (const void* a, const void* b)
{
    if ((a == NULL) || (a == NULL))
        ERROR(-1,EINVAL);

    string_t str1 = *(string_t*)a;
    string_t str2 = *(string_t*)b;

    char* ptr1 = str1.str + str1.size - PTR_CONST;
    char* ptr2 = str2.str + str2.size - PTR_CONST;

    while ((ptr1 != str1.str) && (ptr2 != str2.str)) {
        if ((isalpha (*ptr1)) && (isalpha (*ptr2))) {
            if (*ptr1 == *ptr2) {
                while (*ptr1 == *ptr2) {
                    if (((ptr1 - 1) != str1.str) && ((ptr2 - 1) != str2.str)){
                        ptr1--;
                        ptr2--;
                    } else
                        return str1.size - str2.size;
                }
            } else
                return *ptr1 - *ptr2;
        } else {
            if (!isalpha (*ptr1)) {
                if (ptr1 - 1 != str1.str)
                    ptr1--;
                else
                    return str1.size - str2.size;
            } else {
                if (ptr2 - 1 != str2.str)
                    ptr2--;
                else
                    return str1.size - str2.size;
            }
        }
    }
}


int writeline (string_t* array_str, int nLines)
{
    if (array_str == NULL)
        ERROR(-1,EINVAL);
    int i = 0;

    for (i = 0; i < nLines; i++)
    {
        printf ("%s\n", array_str[i].str);
    }
    return 0;
}

