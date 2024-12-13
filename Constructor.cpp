#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "Differentiator.h"

node_t* CreateNode(types arg, union value_t value, node_t* left, node_t* right, struct tree_t* root)
{
    assert(root);

    node_t* new_node = (node_t*) calloc(1, sizeof(node_t));

    assert(new_node);

    new_node->left  = left;
    new_node->right = right;

    new_node->type  = arg;

    new_node->value = value;
//     if (new_node->type == NUM)
//     {
//         new_node->value.number = value;
//     }
//
//     else if (new_node->type == OP)
//     {
//         char oper = WhatIsOperator(int(value));
//
//         printf("oper = %c\n", oper);
//
//         new_node->value.oper        = oper;
//         new_node->value.oper_number = int(value);
//
//         printf("node oper number = %d\n", new_node->value.oper_number);
//     }
//
//     else if (new_node->type == VAR)
//     {
//         new_node->value.number = value;
//     }
    root->addresses[root->number_nods] = new_node;

    root->number_nods++;

    if (root->number_nods == root->array_addr_size)
    {
        root->array_addr_size = root->array_addr_size * CAPACITY_COEF_UP;

        root->addresses = (node_t**) realloc(root->addresses, root->array_addr_size * sizeof(node_t*));

        FillingDataPoison(root->addresses + root->number_nods, root->array_addr_size * CAPACITY_COEF_SHIFT / CAPACITY_COEF_UP);
    }

    return new_node;
}

types WhatTypeIs(char* value)
{
    if (strcmp(value, "x") == 0)
    {
        return VAR;
    }

    else if ((strcmp(value, "+") == 0) || (strcmp(value, "-") == 0) || (strcmp(value, "*") == 0) || (strcmp(value, "/") == 0) || (strcmp(value, "^") == 0))
    {
        return OP;
    }

    else
    {
        return NUM;
    }
}

const char* WhatIsOperator(int value_oper)
{
    switch(value_oper)
    {
        case ADD:   return "+";

        case SUB:   return "-";

        case MULL:  return "*";

        case DIV:   return "/";

        case DEG:   return "^";

        case SIN:   return "sin";

        case COS:   return "cos";

        case TAN:   return "tg";

        case COT:   return "ctg";

        case SHX:   return "sh";

        case CHS:   return "ch";

        case THX:   return "th";

        case CTH:   return "cth";

        case LOG:   return "log";

        case LN :   return "ln";

        case EXP:   return "e";
        
        default:    printf("ERROR TYPE\n");
                    return ERROR_TYPE;
    }
}

void ExprCtor(struct expr_t* expr)
{
    expr->string = (char*) calloc(EXPR_SIZE, sizeof(char));

    assert(expr->string);
}

void ExprDtor(struct expr_t* expr)
{
    free(expr->string);

    expr->index = 0;
}

tree_t* TreeCtor(struct tree_t* tree)
{
    tree = (tree_t*) calloc(1, sizeof(tree_t));
    //printf("root ctor1 = %p\n", root);

    assert(tree);

    //root->word  = NULL;
    tree->func = __func__;
    tree->file = __FILE__;
    tree->line = __LINE__;

    tree->array_addr_size = DEFAULT_NUM_NODS;

    tree->addresses = (node_t**) calloc(tree->array_addr_size, sizeof(node_t*));

    tree->first_node = NULL;

    return tree;
}

int TreeDtor(struct tree_t* tree)
{
    NodsDtor(tree, tree->first_node);

    free(tree);

    return 0;
}

int NodsDtor(struct tree_t* tree, struct node_t* node)
{
    if(!node)
    {
        return 0;
    }

    NodsDtor(tree, node->left);
    NodsDtor(tree, node->right);

    for (int i = 0; i < tree->number_nods; i++)
    {
        if (node == tree->addresses[i])
        {
            tree->addresses[i] = NULL;
            break;
        }
    }

    free(node);
    //free(node);

    node = NULL;
    return 0;
}

void DeleteLocalNods(struct tree_t* tree, struct node_t* node)
{
    NodsDtor(tree, node->left);
    NodsDtor(tree, node->right);

    node->left  = NULL;
    node->right = NULL;
}

void DtorWithArray(struct tree_t* tree)
{
    for (int i = 0; i < tree->number_nods; i++)
    {
        if (tree->addresses[i] == NULL)
        {
            continue;
        }

        free(tree->addresses[i]);
    }
}


