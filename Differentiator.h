#ifndef DIFFERENTIATOR
#define DIFFERENTIATOR

#include <stdio.h>

enum types
{
    NUM = 0,
    OP  = 1,
    VAR = 2,
};

enum operators
{
    ADD  = 10,
    SUB  = 20,
    MULL = 30,
    DIV  = 40,
};

struct node_t
{
    types     type;
    int       value;    //TODO read about union
    node_t*   left;
    node_t*   right;
    node_t*   parent;
};

struct tree_t
{
    node_t*     root;
    const char* func;
    int         line;
    int         number_nods;
    char*       file;
    node_t*     first_node;
};

struct file_t
{
    FILE*  file_ptr;
    size_t file_size;
    size_t index_buf;
    char*  buffer;
};

const size_t NO_SPACE         = 15;
const size_t SCAN_FILE_POISON = 153153;

const int ERROR_TYPE = -1;

size_t ScanFile           (struct file_t* file_struct);
size_t FindNoSpace        (struct file_t* tree, size_t index);
size_t SkipSpacesForPrint (struct file_t* tree, size_t index);

char WhatIsOperator       (int value_oper);

void ClearBuffer          ();
void CheckFclose          (FILE* file_ptr);
void ChangeSymbolInBuffer (struct file_t* buf_struct, size_t size_buffer, char symbol1, char symbol2);

tree_t* TreeCtor          (struct tree_t* root);

node_t* ReadTree          (struct file_t* tree, struct tree_t* root);
node_t* CreateNode        (types arg, int value, node_t* left, node_t* right, struct tree_t* root);

types WhatTypeIs          (char* value);

int WhatIsOperEnum        (char* value);
int CheckFile             (FILE* file);
int NodsDtor              (struct node_t* node);
int TreeDtor              (struct tree_t* root);

char* BufferCtor          (struct file_t* tree);

#endif
