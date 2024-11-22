#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "Differentiator.h"

node_t* CreateNode(types arg, int value, node_t* left, node_t* right, struct tree_t* root)
{
    node_t* new_node = (node_t*) calloc(1, sizeof(node_t));

    assert(new_node);

    new_node->left  = left;
    new_node->right = right;

    new_node->value = value;
    new_node->type  = arg;

    //new_node->parent = parent;
    if (root->number_nods == 0)
    {
        root->first_node = new_node;
    }
    
    root->number_nods++;

    return new_node;
}

types WhatTypeIs(char* value)
{
    if (strcmp(value, "x") == 0)
    {
        return VAR;
    }

    else if ((strcmp(value, "+") == 0) || (strcmp(value, "-") == 0) || (strcmp(value, "*") == 0) || (strcmp(value, "/") == 0))
    {
        return OP;
    }

    else
    {
        return NUM;
    }
}

tree_t* TreeCtor(struct tree_t* root)
{
    root = (tree_t*) calloc(1, sizeof(tree_t));
    //printf("root ctor1 = %p\n", root);

    assert(root);

    //root->word  = NULL;
    root->func = __func__;
    root->file = __FILE__;
    root->line = __LINE__;

    root->first_node = NULL;

    return root;
}

int TreeDtor(struct tree_t* root)
{
    NodsDtor(root->first_node);

    free(root);

    return 0;
}

int NodsDtor(struct node_t* node)
{
    if(!node)
    {
        return 0;
    }

    NodsDtor(node->left);
    NodsDtor(node->right);

    free(node);
    //free(node);

    node = NULL;
    return 0;
}
