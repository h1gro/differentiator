#include <stdlib.h>
#include <assert.h>

#include "Differentiator.h"

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

void DeleteLocalNods(struct node_t* node)
{
    NodsDtor(node->left);
    NodsDtor(node->right);

    node->left  = NULL;
    node->right = NULL;
}
