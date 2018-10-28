#include <stdio.h>
#include <math.h>
#include <errno.h>
#include <stdlib.h>
#include <assert.h>

#define ERROR(ret,erval)    \
    do {                    \
            errno = erval;  \
            return ret;     \
    }while(0)

#define canary      0xBEDABEDA
#define allocAcid   666
#define delAcid     777
#define allocConst  2

typedef int stk_data_t;

typedef struct stack_t
{
    int         canaryL;
    char*       name;
    int         size;
    int         capacity;
    stk_data_t  *data;
    int         canaryR;
}stack_t;

stack_t*    StkConstuct (const int nElem, const char* name);
int         StkDestruct (stack_t* stk);
int         StackOk     (stack_t* stk);
int         StkDump     (stack_t* stk, const char* state);
int         StkPush     (stack_t* stk, stk_data_t elem);
stk_data_t  StkPop      (stack_t* stk);
stack_t* ReallocMem  (stack_t* data, const int newSize);

int main ()
{
    stack_t* s = StkConstuct (10, "s");
    for(int i = 0; i != 13; i++)
        StkPush (s, 19);
    StkPop (s);
    StkDump (s, "Ok");
    StkDestruct (s);
    return 0;
}

stack_t* ReallocMem (stack_t* stk, const int newSize)
{
    if ((stk == NULL) || (newSize < 0))
        ERROR(NULL,EINVAL);

    if (StackOk (stk) != 0)
    {
        StkDump (stk, "Bad");
        assert (!"ReallocMem");
    }
    stk->capacity = newSize;
    stk_data_t* new;
    new = (stk_data_t*)realloc (stk->data, newSize * sizeof(stk_data_t));
    if (new == NULL)
        return NULL;
    stk->data = new;

    if (StackOk (stk) != 0)
    {
        StkDump (stk, "Bad");
        assert (!"ReallocMem");
    }
    return stk;
}

int         StkPush (stack_t* stk, stk_data_t elem)
{
    if (stk == NULL)
        ERROR(-1,EINVAL);

    if (StackOk (stk) != 0)
    {
        StkDump (stk, "Bad");
        assert (!"StkPush");
    }

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
    {
        StkDump (stk, "Bad");
        assert (!"StkPush");
    }
    return 0;
}

stk_data_t StkPop (stack_t* stk)
{
    if (stk == NULL)
        ERROR(NAN,EINVAL);

    if (StackOk (stk) != 0)
    {
        StkDump (stk, "Bad");
        assert (!"StkPop");
    }

    int elem = 0;
    if (stk->size == 0)
    {
        StkDump (stk, "vrode ok");
        assert (!"StkPop");
        ERROR(NAN,EPERM);
    }
    else
    {
        stk->size--;
        elem = stk->data[stk->size];
        stk->data[stk->size] = delAcid;
    }

    if (StackOk (stk) != 0)
    {
        StkDump (stk, "Bad");
        assert (!"StkPop");
    }

    return elem;
}

stack_t*    StkConstuct (const int nElem, const char* name)
{
    if ((nElem < 0) || (name == NULL))
        ERROR(NULL,EINVAL);

    stack_t* stk = (stack_t*)calloc (1, sizeof(stack_t));
    if (stk == NULL)
        return NULL;

    stk->canaryL            = canary;
    stk->name               = name;
    stk->size               = 0;
    stk->capacity           = nElem;
    stk_data_t* new   = (stk_data_t*)calloc (nElem, sizeof(stk_data_t));
    if (new == NULL)
        return NULL;
    stk->data = new;
    stk->canaryR            = canary;

    int i = 0;
    for(; i != nElem; i++)
        stk->data[i] = allocAcid;

    if (StackOk (stk) != 0)
    {
        StkDump (stk, "Bad");
        assert (!"StkConstruct");
    }
    return stk;
}

int         StkDestruct (stack_t* stk)
{
    if (stk == NULL)
        ERROR(-1,EINVAL);

    if (StackOk (stk) != 0)
    {
        StkDump (stk, "Bad");
        assert (!"StkDestruct");
    }
    stk->canaryL    = 0;
    stk->name       = "NO NAME";
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
    printf ("Stack '%s'; adress [%d]; state (%s)\n", stk->name, stk, state);
    printf ("size = %d\n", stk->size);
    printf ("capacity = %d\n", stk->capacity);
    for (; i != stk->capacity; i++)
    {
        if (i < stk->size)
            printf ("* data[%d] = %d\n", i, stk->data[i]);
        else
            printf ("  data[%d] = %d\n", i, stk->data[i]);
    }
    if (StackOk (stk) != 0)
    {
        StkDump (stk, "Bad");
        assert (!"StkDump");
    }
    return 0;
}

int         StackOk (stack_t* stk)
{
    if (stk == NULL)
        ERROR(-1,EINVAL);

    if ((stk->canaryL != canary) || (stk->canaryR != canary))
        return -2;
    if ((stk->size < 0) || (stk->capacity < 0))
        return -3;
    if (stk->size >= stk->capacity)
        return -4;
    if (stk->data == NULL)
        return -5;

    return 0;
}
