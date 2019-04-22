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
#define ass(x, y) y = x


enum TYPE
{
    ass (100, FUNCTION),
    ass (101, VAR),
    ass (102, NUMBER)
};

enum FUCTION
{
    ass (1, PLUS),
    ass (2, MINUS),
    ass (3, MULTY),
    ass (4, DIVIDE),
    ass (5, LN),
    ass (6, COS),
    ass (7, SIN),
    ass (8, EXP)
};

enum VAR
{
    ass (11, X),
    ass (21, Y)
};

typedef int T;

typedef struct node_t
{
    T               value;
    struct node_t*  left;
    struct node_t*  right;
    int             type;
}node_t;

typedef struct tree_t
{
    node_t* head;
    int     count;
}tree_t;

tree_t* ConstructTree   (T value, int type);
int     DestructTree    (node_t* head);
int     TreeOk          (tree_t* head);
node_t* CreateElem      (T value, int type);

node_t*     Number  (node_t* root);
node_t*     Var     (node_t* root);
node_t*     Plus    (node_t* root);
node_t*     Minus   (node_t* root);
node_t*     Multy   (node_t* root);
node_t*     Divide  (node_t* root);

node_t*     Differentiator  (node_t* root);
int         PrintList       (node_t* root);

int     AddLeft         (node_t* node, T value, int type);
int     AddRight        (node_t* node, T value, int type);

node_t*     chitalka        (char* path);
int         pisalka         (node_t* node);

node_t* ReadAkinator    (char* buf, int* pos);
//==============================================================================
node_t* GetT (char* buf, int* pos);
node_t* GetF (char* buf, int* pos);

node_t* GetN (char* buf, int* pos)
{
    logs;
    int     val = 0;
    if (*(buf + *pos) == 'X')
    {
        node_t* new = CreateElem (X, VAR);
        (*pos)++;
        loge;
        return new;
    }
    else
    {
        while (*(buf + *pos) <= '9' && *(buf + *pos) >= '0')
        {
            val = val * 10 + *(buf + *pos) - '0';
            (*pos)++;
        }
        node_t* new = CreateElem (val, NUMBER);
        loge;
        return new;
    }
}

node_t* GetE (char* buf, int* pos)
{
    logs;
    node_t* left = GetT (buf, pos);

    node_t* oldLeft = NULL;
    node_t* res = left;
    while (*(buf + *pos) == '+' || *(buf + *pos) == '-') {
        char op = *(buf + *pos);
        (*pos)++;
        node_t* right = GetT (buf, pos);
        if (op == '+') {
            res = CreateElem(PLUS, FUNCTION);
            if (oldLeft == NULL) {
                res->left   = left;
                res->right  = right;
                oldLeft     = res;
            } else {
                res->left   = oldLeft;
                res->right  = right;
                oldLeft     = res;
            }
        } else {
            res = CreateElem(MINUS, FUNCTION);
            if (oldLeft == NULL) {
                res->left   = left;
                res->right  = right;
                oldLeft     = res;
            } else {
                res->left   = oldLeft;
                res->right  = right;
                oldLeft     = res;
            }
        }
    }
    loge;
    return res;
}

node_t* GetP (char* buf, int* pos)
{
    if (*(buf + *pos) == '(') {
        (*pos)++;
        node_t* res = GetE (buf,pos);
        (*pos)++;
        loge;
        return res;
    }
    if (*(buf + *pos) <= '9' && *(buf + *pos) >= '0' || *(buf + *pos) == 'X') {
        loge;
        return GetN(buf, pos);
    }
    if (*(buf + *pos) == 'L' || *(buf + *pos) == 'S' || *(buf + *pos) == 'C') {
        loge;
        return GetF(buf,pos);
    }
}

node_t* GetF (char* buf, int* pos)
{
    logs;
     switch (*(buf + *pos)) {
        case 'L': {
                    node_t* res = CreateElem(LN, FUNCTION);
                    *pos = *pos + 2;
                    res->left = GetE(buf,pos);
                    printf("\n\nSUKA BYAT BATYA IN BUILDING\n\n");
                    (*pos)++;
                    printf ("\n\nYA EBOSHOO FUNCTION pos[%d] = %c\n\n", *pos, *(buf + *pos));
                    DumpTree(res->left);
                    printf("\n\nсверху высер GET_E\n\n");
                    res->right = NULL;
                    loge;
                    return res;
                    break;
                  }
        case 'S': { node_t* res = CreateElem(SIN, FUNCTION);
                    *pos = *pos + 3;
                    res->left = GetE(buf,pos);
                    res->right = NULL;
                    loge;
                    return res;
                    break;
                  }
        case 'C': { node_t* res = CreateElem(COS, FUNCTION);
                    *pos = *pos + 3;
                    res->left = GetE(buf,pos);
                    res->right = NULL;
                    loge;
                    return res;
                    break;
                  }
    }
}

node_t* GetT (char* buf, int* pos)
{
    logs;
    node_t* left = GetP (buf, pos);
    node_t* oldLeft = NULL;
    node_t* res = left;
    while (*(buf + *pos) == '*' || *(buf + *pos) == '/') {
        char op = *(buf + *pos);
        (*pos)++;
        node_t* right = GetP (buf, pos);
        if (op == '*') {
            res = CreateElem(MULTY, FUNCTION);
            if (oldLeft == NULL) {
                res->left   = left;
                res->right  = right;
                oldLeft     = res;
            } else {
                res->left   = oldLeft;
                res->right  = right;
                oldLeft     = res;
            }
        } else {
            res = CreateElem(DIVIDE, FUNCTION);
            if (oldLeft == NULL) {
                res->left   = left;
                res->right  = right;
                oldLeft     = res;
            } else {
                res->left   = oldLeft;
                res->right  = right;
                oldLeft     = res;
            }
        }
    }
    loge;
    return res;
}

int DottyMaker  (FILE* fd, node_t* root)
{
    if (root->left != NULL)
    {
        DumpDot (fd, root->left);
    }
}
int DumpDot     (FILE* fd, node_t* root)
{
    logs;
    if (fd == NULL || root == NULL)
    {
        loge;
        ERROR(-1, EINVAL);
    }
    char res[10] = "";
    switch (root->type)
    {
        case FUNCTION:
        {
            switch (root->value)
            {
            case PLUS:
                fprintf (fd, "%s", "+");
                fprintf (fd, "%s", "->");
                if (root->right != NULL)
                    DumpDot (fd, root->right);
                return 0;
                break;
            case MINUS:
                fprintf (fd, "%s", "-");
                fprintf (fd, "%s", "->");
                if (root->right != NULL)
                    DumpDot (fd, root->right);
                return 0;
                break;
            case MULTY:
                fprintf (fd, "%s", "*");
                fprintf (fd, "%s", "->");
                if (root->right != NULL)
                    DumpDot (fd, root->right);
                return 0;
                break;
            case DIVIDE:
                fprintf (fd, "%s", "/");
                fprintf (fd, "%s", "->");
                if (root->right != NULL)
                    DumpDot (fd, root->right);
                return 0;
                break;
            case LN:
                fprintf (fd, "%s", "LN");
                fprintf (fd, "%s", "->");
                return 0;
                break;
            }
        }
        case VAR:
                fprintf (fd, "%s", "X");
                fprintf (fd, "%s", ";\n");
                return 0;
                break;

        case NUMBER:
                //itoa(root->value, res, 10);
                fprintf (fd, "%s", "10");
                fprintf (fd, "%s", ";\n");
                return 0;
                break;

    }
}

int main ()
{
    logs;
    char* buf = (char*) calloc(10, sizeof(char));
    buf = "LN(X-10)-10";
    int pos = 0;
    node_t* head = GetE(buf,&pos);
    //+++++++++++++++++++++++++++++++++++
    FILE* dot;
    dot = fopen("tree.txt", "w");
    fprintf (dot, "%s", "digraph G{\n");
    DumpDot (dot, head);
    fprintf (dot, "%s", "}\n");
    fclose(dot);
    //+++++++++++++++++++++++++++++++++++
    
    DumpTree        (head);
    printf("\nafter diff\n");
    node_t* diff  = Differentiator  (head);
    DumpTree        (diff);
    DestructTree    (diff);
    loge;
    return 0;
}

int     DumpTree        (node_t* node)
{
    logs;
    if (node == NULL)
    {
        loge;
        ERROR(-1,EINVAL);
    }

    if (node->type == NUMBER)
    {
        printf ("%d", node->value);
        loge;
        return 0;
    }

    if (node->type == VAR)
    {
        printf ("%c", 'X');
        loge;
        return 0;
    }
    printf ("( ");
    if (node->type == FUNCTION)
    {
        DumpTree (node->left);

        if (node->value == MULTY)
            printf ("%c", '*');
        if (node->value == PLUS)
            printf ("%c", '+');
        if (node->value == DIVIDE)
            printf ("%c", '/');
        if (node->value == MINUS)
            printf ("%c", '-');
        if (node->value == LN)
            printf ("%s", "ln");
        DumpTree (node->right);
    }
    printf (" )");

    loge;
    return 0;
}

int     AddLeft         (node_t* node, T value, int type)
{
    logs;
    if (node->left != NULL)
        AddLeft (node->left, value, type);
    else
    {
        node_t* new = CreateElem (value, type);
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

int     AddRight        (node_t* node, T value, int type)
{
    logs;
    if (node->right != NULL)
        AddRight (node->right, value, type);
    else
    {
        node_t* new = CreateElem (value, type);
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

node_t*     Copy    (node_t* root)
{
    logs;
    if (root == NULL)
    {
        loge;
        ERROR(NULL, EINVAL);
    }
    node_t* new = (node_t*)calloc (1, sizeof(node_t));
    new->type   = root->type;
    new->value  = root->value;

    new->left   = Copy (root->left);
    new->right  = Copy (root->right);

    loge;
    return new;
}

node_t*     Var     (node_t* root)
{
    logs;
    if (root == NULL)
    {
        loge;
        ERROR(NULL,EINVAL);
    }
    node_t* new = Copy (root);
    new->type = NUMBER;
    new->value = 1;

    loge;
    return new;
}

node_t*     Number     (node_t* root)
{
    logs;
    if (root == NULL)
    {
        loge;
        ERROR(NULL,EINVAL);
    }
    node_t* new = Copy (root);
    new->type   = NUMBER;
    new->value  = 0;
    loge;
    return new;
}

node_t*     Plus    (node_t* root)
{
    logs;
    if (root == NULL)
    {
        loge;
        ERROR(NULL,EINVAL);
    }

    node_t* new = Copy (root);
    new->left   = Differentiator (root->left);
    new->right  = Differentiator (root->right);
    loge;
    return new;
}

node_t*     Minus   (node_t* root)
{
    logs;
    if (root == NULL)
    {
        loge;
        ERROR(NULL,EINVAL);
    }

    node_t* new = Copy (root);
    new->left   = Differentiator (root->left);
    new->right  = Differentiator (root->right);

    loge;
    return new;
}

node_t*     Multy   (node_t* root)
{
    logs;
    if (root == NULL)
    {
        loge;
        ERROR(NULL,EINVAL);
    }

    node_t* right   = Copy (root->right);
    node_t* left    = Copy (root->left);

    node_t* new     = CreateElem (PLUS, FUNCTION);
    new->left       = CreateElem (MULTY, FUNCTION);
    new->right      = CreateElem (MULTY, FUNCTION);

    new->left->left     = Differentiator (left);
    new->left->right    = right;

    new->right->left    = left;
    new->right->right   = Differentiator (right);

    loge;
    return new;
}

node_t*     Divide      (node_t* root)
{
    logs;
    if (root == NULL)
    {
        loge;
        ERROR(NULL,EINVAL);
    }

    node_t* right   = Copy (root->right);
    node_t* left    = Copy (root->left);

    node_t* new     = CreateElem (DIVIDE, FUNCTION);
    new->left       = CreateElem (MINUS, FUNCTION);
    new->right      = CreateElem (MULTY, FUNCTION);

    new->left->left     = CreateElem (MULTY, FUNCTION);
    new->left->right    = CreateElem (MULTY, FUNCTION);

    new->left->left->left   = Differentiator (left);
    new->left->left->right  = right;

    new->left->right->left  = left;
    new->left->right->right = Differentiator (right);

    new->right->left    = CreateElem (root->right->value, root->right->type);
    new->right->right   = right;

    loge;
    return new;
}

node_t*     Ln             (node_t* root)
{
    logs;
    if (root == NULL)
    {
        loge;
        ERROR(NULL, EINVAL);
    }

    node_t* left = Copy (root->left);

    node_t* new         = CreateElem (MULTY, FUNCTION);
    new->left           = CreateElem (DIVIDE, FUNCTION);
    new->left->left     = CreateElem (1, NUMBER);
    new->left->right    = left;
    new->right          = Differentiator (left);

    loge;
    return new;
}

node_t*     Differentiator (node_t* root)
{
    logs;
    if (root == NULL)
    {
        loge;
        ERROR(NULL, EINVAL);
    }
    switch (root->type)
    {
        case FUNCTION:
        {
            switch (root->value)
            {
            case PLUS:
                root = Plus (root);
                return root;
                break;
            case MINUS:
                root = Minus (root);
                return root;
                break;
            case MULTY:
                root = Multy (root);
                return root;
                break;
            case DIVIDE:
                root = Divide (root);
                return root;
                break;
            case LN:
                root = Ln (root);
                return root;
                break;
            }
        }
        case VAR:
            root = Var (root);
            return root;
            break;
        case NUMBER:
            root = Number (root);
            return root;
            break;
    }
    loge;
    return NULL;
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

node_t* CreateElem  (T value, int type)
{
    logs;
    if (type < 0)
    {
        loge;
        ERROR(NULL, EINVAL);
    }
    node_t* new = (node_t*)calloc (1, sizeof(node_t));
    if (new == NULL)
    {
        loge;
        return NULL;
    }
    new->value   = value;
    new->left    = NULL;
    new->right   = NULL;
    new->type    = type;
    loge;
    return new;
}

tree_t* ConstructTree (T value, int type)
{
    logs;
    if (type < 0)
    {
        loge;
        ERROR(NULL, EINVAL);
    }

    node_t* head = CreateElem (value, type);
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

