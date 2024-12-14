#ifndef DIFFERENTIATOR
#define DIFFERENTIATOR

#include <stdio.h>

const int    EXPR_SIZE           = 1000;
const int    SIZE_COMMAND        = 153;
const int    DEFAULT_NUM_NODS    = 40;
const int    CAPACITY_COEF_UP    = 4;
const int    CAPACITY_COEF_SHIFT = 3;
const int    FUNC_LENGHT         = 6;
const int    NUM_FUNCS           = 20;

const double EPSILON             = 1E-6;

static const char*  ERROR_TYPE   = "ERROR_OPERATOR";

enum returns
{
    CHECK_FILE_OK      = 77,
    CHECK_FILE_ERROR   = -1,
    NO_SPACE           = 109901,
    SCAN_FILE_POISON   = 153153,
    TYPE_IS_RIGHT      = 1,
    TYPE_IS_NOT_RIGHT  = 0,
};

enum types
{
    NUM = 0,
    OP  = 1,
    VAR = 2,
};

enum operators
{
    /*standart binary operators*/
    ADD  = 1,
    MULL = 2,
    SUB  = 3,
    DIV  = 4,
    DEG  = 5,
    /*trigonometric functions*/
    SIN  = 10,
    COS  = 11,
    TAN  = 12,
    COT  = 13,
    /*hyperbolic functions*/
    SHX  = 64,
    CHS  = 65,
    THX  = 66,
    CTH  = 67,
    /*math funcs*/
    LOG  = 73,
    LN   = 74,
    EXP  = 75,
};

struct func_t
{
    const char* func;
    operators   func_enum;
};

union value_t
{
    int    oper_number;
    char   oper;
    double number;
};

struct node_t
{
    types         type;
    union value_t value;
    node_t*       left;
    node_t*       right;
    node_t*       parent;
};

struct tree_t
{
    node_t*       root;
    const char*   func;
    int           line;
    int           number_nods;
    const char*   file;
    node_t*       first_node;
    node_t**      addresses;
    int           array_addr_size;
    const func_t* ptr_on_keywords;
};

struct expr_t
{
    char* string;
    int   index;
};

struct file_t
{
    FILE*  file_ptr;
    size_t file_size;
    size_t index_buf;
    char*  buffer;
};

const struct func_t keywords[NUM_FUNCS] = {{"sin", SIN}, {"cos", COS}, {"tg", TAN}, {"ctg", COT},
                                           {"sh",  SHX}, {"ch",  CHS}, {"th", THX}, {"cth", CTH},
                                           {"lg",  LOG}, {"ln",  LN }, {"e",  EXP}};

types WhatTypeIs           (char* value);

bool CheckUnionType        (struct node_t* node, types type);

double RaiseToDegree       (double number, double power);

const char* WhatIsOperator (int value_oper);

int CheckFile              (FILE* file);
int TreeDtor               (struct tree_t* tree);
int IsEqual                (double elem1, double elem2);
int NodsDtor               (struct tree_t* tree, struct node_t* node);
int CompareFuncs           (struct expr_t* expr, struct tree_t* tree, char* function, int word_lenght);

void ClearBuffer           ();
void CheckFclose           (FILE* file_ptr);

void DtorWithArray         (struct tree_t* tree);
void ExprDtor              (struct expr_t* expr);
void ExprCtor              (struct expr_t* expr);
void DoNodeOperation       (struct tree_t* tree, struct node_t* node);
void DeleteLocalNods       (struct tree_t* tree, struct node_t* node);
void ChangeNodeMullDeg     (struct tree_t* tree, struct node_t* node, struct node_t* new_node);

void FillingDataPoison     (node_t** data, int num_el_poison);
void FileSize              (struct file_t* file, struct expr_t* expr);
void SyntaxError           (const char* file, const char* func, int line);
void ChangeSymbolInBuffer  (char* buffer, size_t size_buffer, char symbol1, char symbol2);
void ChangeNodeAddSub      (struct tree_t* tree, struct node_t* node, node_t* desired_node, node_t* undesired_node);

size_t FindNoSpace         (struct file_t* akin, size_t index);
size_t SkipSpacesForPrint  (struct file_t* akin, size_t index);
size_t ScanFile            (struct file_t* akin, struct expr_t* expr);

tree_t* TreeCtor           (struct tree_t* root);

node_t* Diffr              (struct node_t* node, struct tree_t* tree);
node_t* Copy               (struct node_t* node, struct tree_t* tree);
node_t* Simplifier         (struct node_t* node, struct tree_t* tree);
node_t* CreateNode         (types arg, union value_t value, node_t* left, node_t* right, struct tree_t* root);

#endif
