#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define logs                                        \
do                                                  \
    {                                               \
        fprintf(stderr, "%s started\n", __func__);  \
        fflush(stderr);                             \
    }while( 0 )

#define loge                                        \
do                                                  \
    {                                               \
        fprintf(stderr, "%s finished\n", __func__); \
        fflush(stderr);                             \
    }while( 0 )

#define ERROR(ret, errval)          \
    do                              \
    {                               \
        errno = errval;             \
        return ret;                 \
    } while( 0 )

#define MAX_NAME 100

typedef char* T;

typedef struct node_t
{
    T               value;
    struct node_t*  left;
    struct node_t*  right;
    int             numb;
}node_t;

typedef struct tree_t
{
    node_t* head;
    int     count;
}tree_t;

int     TreeOk          (tree_t* head);
node_t* CreateElem      (T       value);
tree_t* ConstructTree   (T       value);
int     DestructTree    (node_t* head);
int     DumpTree        (node_t* head);
int     AddElem         (node_t* head, T value);
int     Search          (node_t* point);
int     AddRight        (node_t* node, T value);
int     AddLeft         (node_t* node, T value);
node_t* ReadAkinator    (char* buf, int* pos);
int     WriteNode        (FILE* fd, node_t* head);
int     WriteTree       (char* path, node_t* head);

char* OpFileAndCrBuf     (char* path, long* nSymbol);
int     ReturnSizeOfFile (FILE* path);

int main ()
{
    logs;
    long nSymb = 0;
    int i = 0;
    char* buffer = OpFileAndCrBuf ("newtree.txt", &nSymb);
    tree_t* tree1 = ConstructTree ("");
    tree1->head = ReadAkinator (buffer, &i);
    printf ("buffer :%s\n", buffer);
//    DumpTree (tree1->head);
    Search (tree1->head);
    WriteTree ("newtree.txt", tree1->head);
//    DumpTree (tree1->head);
    free(buffer);
    DestructTree (tree1->head);
    free (tree1);
    loge;
    return 0;
}

int     WriteTree       (char* path, node_t* head)
{
    logs;
    if ((path == NULL) || (head == NULL))
        ERROR(-1, EINVAL);

    FILE* original;
    original = fopen (path, "w");
    if (original == NULL)
    {
        loge;
        return -1;
    }

    WriteNode (original, head);

    fclose(original);
    return 0;
}

int     WriteNode        (FILE* fd, node_t* head)
{
    logs;
    if ((fd == NULL) || (head == NULL))
    {
        loge;
        ERROR(-1,EINVAL);
    }
    static int ident = 0;
    ident++;
    int k = ident;
    while (k > 0)
    {
        fputc (' ', fd);
        k--;
    }
    fputc ('(', fd);
    fprintf (fd, "%s", head->value);
    fputc ('\n', fd);
    if (head->left != NULL)
        WriteNode (fd, head->left);
    if (head->right != NULL)
        WriteNode (fd, head->right);

    k = ident;
    while (k > 0)
    {
        fputc (' ', fd);
        k--;
    }
    ident--;
    fputc (')', fd);
    fputc ('\n', fd);
    loge;
    return 0;
}

node_t* ReadAkinator    (char* buf, int* pos)
{
    logs;
    if ((buf == NULL) || (pos == NULL))
        ERROR(NULL,EINVAL);

    int offset = 0;
    sscanf(buf + *pos, "%*[^()]%n", &offset);
    *pos = *pos + offset;
    offset = 0;
    if (*(buf + *pos) == ')')
    {
        (*pos)++;
        loge;
        return NULL;
    }
    else if (*(buf + *pos) == '(')
    {
        (*pos)++;
        char* name = (char*)calloc (MAX_NAME, sizeof(char));
        sscanf(buf + *pos, "%*[ \n\t\v]%n", &offset);
        *pos = *pos + offset;

        sscanf (buf + *pos, "%[^()\n]%n", name, &offset);
        *pos = *pos + offset;

        node_t* node  = CreateElem (name);
        node_t* right = NULL;
        node_t* left  = ReadAkinator (buf, pos);
        if (left != NULL)
        {
            while (*(buf + *pos + 1) != '(')
                (*pos)++;
            node_t* right = ReadAkinator (buf, pos);
            node->right = right;
            node->left  = left;
            loge;
            return node;
        }
        loge;
        return node;
    }
    loge;
    return NULL;
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

int     AddLeft         (node_t* node, T value)
{
    logs;
    if (node->left != NULL)
        AddLeft (node->left, value);
    else
    {
        node_t* new = CreateElem (value);
        if (new == NULL)
        {
            loge;
            return -1;
        }
        node->left = new;
    }
    loge;
    return 0;
}

int     AddRight        (node_t* node, T value)
{
    logs;
    if (node->right != NULL)
        AddRight (node->right, value);
    else
    {
        node_t* new = CreateElem (value);
        if (new == NULL)
        {
            loge;
            return -1;
        }
        node->right = new;
    }
    loge;
    return 0;
}

int     Search          (node_t* point)
{
    logs;
    if (point == NULL)
        ERROR(-1,EINVAL);

    char ans = '\0';
    do
    {
        if (ans != '\n')
            printf ("%s?\n", point->value);
        ans = getchar();
        if (ans == 'y') {
            point = point->left;
        }
        else if (ans == 'n')
        {
            if (point->right != NULL)
                point = point->right;
            else
            {
                char name[512] = "";
                printf ("Кто это был?\n");
                scanf ("%s", name);
                point->left = CreateElem (strdup(name));

                point->right = CreateElem (point->value);

                char question[512] = "";
                printf("В чем разница между %s и %s\n", point->left->value,
                        point->right->value);
                scanf ("%s%[^ ]", question);
                point->value = strdup(question);
                printf ("Я учту!\n");
                return 0;
            }
        }
    }while (point != NULL);

    printf ("Я же говорила!\n");
    loge;
    return 0;
}

int     DumpTree        (node_t* head)
{
    logs;
    if (head == NULL)
    {
        loge;
        ERROR(-1,EINVAL);
    }

    static int indent = 0;
    indent++;
    printf ("%*s( ", indent * 4, "");
    printf ("%s\n", head->value);
    if (head->left != NULL)
        DumpTree (head->left);
    if (head->right != NULL)
        DumpTree (head->right);

    printf ("%*s )\n", indent * 4, "");
    indent--;
    loge;
    return 0;
}

int     DestructTree    (node_t* head)
{
    logs;
    if (head == NULL)
    {
        loge;
        ERROR(-1,EINVAL);
    }

    if (head->left != NULL)
        DestructTree (head->left);
    if (head->right != NULL)
        DestructTree (head->right);

    free(head);

    loge;
    return 0;
}

int     TreeOk          (tree_t* head)
{
    logs;
    if (head == NULL)
    {
        loge;
        ERROR(-1,EINVAL);
    }

    if (head->count < 0)
    {
        loge;
        return(-1);
    }

    loge;
    return 0;
}

node_t* CreateElem  (T value)
{
    logs;
    /*if (isnan(value) != 0)
    {
        loge;
        ERROR(NULL, EINVAL);
    }*/
    node_t* new = (node_t*)calloc (1, sizeof(node_t));
    if (new == NULL)
    {
        loge;
        return NULL;
    }
    new->value   = value;
    new->left    = NULL;
    new->right   = NULL;
    new->numb    = 0;
    loge;
    return new;
}

tree_t* ConstructTree (T value)
{
    logs;
    /*if (??value)
    {
        loge;
        ERROR(NULL, EINVAL);
    }*/

    node_t* head = CreateElem (value);
    if (head == NULL)
    {
        loge;
        return NULL;
    }

    tree_t* tree = (tree_t*)calloc (1, sizeof(tree_t));
    if (tree == NULL)
    {
        loge;
        free (head);
        return NULL;
    }

    tree->head  = head;
    tree->count = 0;
    if (TreeOk(tree) != 0)
    {
        loge;
        free(head);
        free(tree);
        return NULL;
    }
    loge;
    return tree;
}

int     AddElem (node_t* head, T value)
{
    logs;
    if (head == NULL)
    {
        loge;
        ERROR(-1, EINVAL);
    }
    /*if (TreeOk(head) != 0)
    {
        loge;
        return -1;
    }*/

    if (head->value > value)
    {
        if (head->left != NULL)
            AddElem (head->left, value);
        else
        {
             node_t* elem = CreateElem(value);
            if (elem == NULL)
            {
                loge;
                return -1;
            }
            head->left = elem;
        }
    }
    if (head->value < value)
    {
        if (head->right != NULL)
            AddElem (head->right, value);
        else
        {
             node_t* elem = CreateElem(value);
            if (elem == NULL)
            {
                loge;
                return -1;
            }
            head->right = elem;
        }
    }
    if (head->value == value)
        head->numb++;

      // head->count++;

   /* if (TreeOk(head) != 0)
    {
        loge;
        free(elem);
        return -1;
    }*/

    loge;
    return 0;
}

