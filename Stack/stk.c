#include <stdio.h>
#include <math.h>
#include <errno.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define ERROR(ret,erval)    \
    do {                    \
            errno = erval;  \
            return ret;     \
    }while(0)

#define MAKE_STACK(name, nElem)                 \
    stack_t* name = StkConstuct(nElem, #name)

#define canary      0xBEDABEDA
#define allocAcid   666
#define delAcid     777
#define allocConst  2

enum codeError
{
    badCanary   = -2,
    badCount    = -3,
    badPointer  = -4
};

typedef int stk_data_t;

typedef struct stack_t
{
    int         canaryL;
    char        name[1024];
    int         size;
    int         capacity;
    stk_data_t  *data;
    int         canaryR;
} stack_t;

/**
 * @brief The function constructs struct of stack.
 *
 * @param nElem is number of elem in stack.
 * @param name is name of stack.
 *
 * @return pointer to struct of stack.
 */
stack_t*    StkConstuct (const int nElem, const char* name);

/**
 * @brief The function destructs struct of stack.
 *
 * @param stk is pointer to stack.
 *
 * @return 0 if struct of stack was destruct.
 */
int         StkDestruct (stack_t* stk);

/**
 * @brief The function cheks struct of stack on correct.
 *
 * @param stk is pointer to stack.
 *
 * @return 0 if struct of stack is OK.
 */
int         StackOk     (stack_t* stk);

/**
 * @brief The function print struct of stack on screen.
 *
 * @param stk is pointer to stack.
 * @param state is condition of stack.
 *
 * @return 0 if struct of stack dumped on screen.
 */
int         StkDump     (stack_t* stk, const char* state);

/**
 * @brief The function adds new element in struct of stack.
 *
 * @param stk os pointer to struct.
 * @param elem is value of new element.
 *
 * @return 0 if element added in struct of stack.
 */
int         StkPush     (stack_t* stk, stk_data_t elem);

/**
 * @brief The function takes element from struct of stack.
 *
 * @param stk is pointer to stack.
 *
 * @return value of last element in stack.
 */
stk_data_t  StkPop      (stack_t* stk, stk_data_t* elem);

/**
 * @brief The function realizes memory
 *
 * @param stk is pointer to stack.
 * @param newSize is value of new capacity of stack.
 *
 * @return pointer to struct of stack with new size.
 */
stack_t* ReallocMem  (stack_t* stk, const int newSize);

/**
 * @brief The function makes the same structure of stack.
 *
 * @param stk is pointer to original stack.
 *
 * @return pointer to copy of struct of stack.
 */
stack_t* CopyStk (stack_t* stk);

int main ()
{
    MAKE_STACK(s, 10);

    for(int i = 0; i != 20; i++)
        StkPush (s, 19);
    StkDump (s, "Ok");
    stack_t* s1 = CopyStk (s);
    StkDestruct (s);
    StkDump (s1, "zbs");
    StkDestruct (s1);
    return 0;
}

stack_t* CopyStk (stack_t* stk)
{
    if (stk == NULL)
        ERROR(NULL,EINVAL);

    if (StackOk (stk) != 0)
        ERROR(NULL,EINVAL);

    stack_t* copy = (stack_t*)calloc (1, sizeof(stack_t));
    if (copy == NULL)
        return NULL;

    copy->canaryL   = stk->canaryL;
    strcat (copy->name, "new <- ");
    strcat (copy->name, stk->name);
    copy->size      = stk->size;
    copy->capacity  = stk->capacity;
    stk_data_t* new = NULL;
    new = (stk_data_t*)calloc (copy->capacity, sizeof(stk_data_t));
    if (new == NULL)
    {
        free (copy);
        return NULL;
    }
    copy->data = new;
    for (int i = 0; i != copy->capacity; i++)
            copy->data[i] = stk->data[i];
    copy->canaryR   = stk->canaryR;

    if ((StackOk (stk) != 0) || (StackOk (copy) != 0))
        ERROR(NULL,EINVAL);

    return copy;
}

stack_t* ReallocMem (stack_t* stk, const int newSize)
{
    if ((stk == NULL) || (newSize < 0))
        ERROR(NULL,EINVAL);

    if (StackOk (stk) != 0)
        ERROR(NULL, EINVAL);

    stk->capacity = newSize;
    stk_data_t* new = NULL;
    new = (stk_data_t*)realloc (stk->data, newSize * sizeof(stk_data_t));
    if (new == NULL)
        return NULL;
    stk->data = new;

    if (StackOk (stk) != 0)
        ERROR(NULL,EINVAL);

    return stk;
}

int         StkPush (stack_t* stk, stk_data_t elem)
{
    if (stk == NULL)
        ERROR(-1,EINVAL);

    if (StackOk (stk) != 0)
        return StackOk (stk);

    if (stk->size == stk->capacity - 1)
    {
        stk = ReallocMem (stk, stk->capacity * allocConst);
        if (stk->data == NULL)
            return -1;
        stk->data[stk->size] = elem;
        stk->size++;

        for (int i = stk->size; i != stk->capacity; i++)
            stk->data[i] = allocAcid;
    }
    else
    {
        stk->data[stk->size] = elem;
        stk->size++;
    }

    if (StackOk (stk) != 0)
        return StackOk (stk);

    return 0;
}

int StkPop (stack_t* stk, stk_data_t* elem)
{
    if ((stk == NULL) || (elem == NULL))
        ERROR(-1,EINVAL);

    if (StackOk (stk) != 0)
        return StackOk (stk);

    if (stk->size == 0)
        ERROR(-1,EPERM);

    else
    {
        stk->size--;
        *elem = stk->data[stk->size];
        stk->data[stk->size] = delAcid;
    }

    if (StackOk (stk) != 0)
        return StackOk (stk);

    return 0;
}

stack_t*    StkConstuct (const int nElem, const char* name)
{
    if ((nElem < 0) || (name == NULL))
        ERROR(NULL,EINVAL);

    stack_t* stk = (stack_t*)calloc (1, sizeof(stack_t));
    if (stk == NULL)
        return NULL;

    stk->canaryL            = canary;
    strcpy (stk->name, name);
    stk->size               = 0;
    stk->capacity           = nElem;
    stk_data_t* new   = (stk_data_t*)calloc (nElem, sizeof(stk_data_t));
    if (new == NULL)
    {
        free (stk);
        return NULL;
    }
    stk->data = new;
    stk->canaryR            = canary;

    for(int i = 0; i != nElem; i++)
        stk->data[i] = allocAcid;

    if (StackOk (stk) != 0)
        ERROR(NULL,EINVAL);

    return stk;
}

int         StkDestruct (stack_t* stk)
{
    if (stk == NULL)
        ERROR(-1,EINVAL);

    if (StackOk (stk) != 0)
        return StackOk (stk);

    stk->canaryL    = 0;
    strcpy(stk->name, "DESTRUCT");
    stk->size       = 0;
    stk->capacity   = 0;
    free(stk->data);
    stk->canaryR    = 0;
    free(stk);

    return 0;
}

int         StkDump (stack_t* stk, const char* state)
{
    if ((stk == NULL) || (state == NULL))
        ERROR(-1,EINVAL);

    int i = 0;
    printf ("Stack '%s'; adress [%p]; state (%s)\n", stk->name, stk, state);
    printf ("size = %d\n", stk->size);
    printf ("capacity = %d\n", stk->capacity);
    for (; i != stk->capacity; i++)
    {
        if (i < stk->size)
            printf ("* data[%d] = %d\n", i, stk->data[i]);
        else
            printf ("  data[%d] = %d\n", i, stk->data[i]);
    }

    return 0;
}

int         StackOk (stack_t* stk)
{
    if (stk == NULL)
        ERROR(-1,EINVAL);

    if ((stk->canaryL != canary) || (stk->canaryR != canary))
        return badCanary;
    if ((stk->size < 0) || (stk->capacity < 0) || (stk->size >= stk->capacity))
        return badCount;
    if ((stk->data == NULL) || (stk->name == NULL))
        return badPointer;

    return 0;
}
