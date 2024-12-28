#include <assert.h>

#include "Differentiator.h"
#include "DiffrFuncs.h"
#include "Dump.h"

node_t* Diffr(struct node_t* node, struct tree_t* tree)
{
    assert(node);
    assert(tree);

    printf("type = %d\nnumber = %lg\noper number = %d\n", node->type, node->value.number, node->value.oper_number);

    if (node->type == NUM)
    {
        return CreateNode(NUM, value_t{.number = 0}, NULL, NULL, tree);
    }

    if (node->type == VAR)
    {
        return CreateNode(NUM, value_t{.number = 1}, NULL, NULL, tree);
    }

    if (node->type == OP)
    {
        switch(node->value.oper_number)
        {
            case ADD:
            {
                return DiffrAdd(node, tree);
            }

            case SUB:
            {
                return DiffrSub(node, tree);
            }

            case MULL:
            {
                return DiffrMull(node, tree);
            }

            case DIV:
            {
                return DiffrDiv(node, tree);
            }

            case DEG:
            {
                return DiffrDeg(node, tree);
            }

            case SIN:
            {
                return DiffrSin(node, tree);
            }

            case COS:
            {
                return DiffrCos(node, tree);
            }

            case TAN:
            {
                return DiffrTan(node, tree);
            }

            case COT:
            {
                return DiffrCot(node, tree);
            }

            case SHX:
            {
                return DiffrShx(node, tree);
            }

            case CHS:
            {
                return DiffrChs(node, tree);
            }

            case THX:
            {
                return DiffrThx(node, tree);
            }

            case CTH:
            {
                return DiffrCth(node, tree);
            }

            case LN:
            {
                return DiffrLn(node, tree);
            }

            case EXP:
            {
                return CreateNode(NUM, value_t{.number = 0}, NULL, NULL, tree);
            }

            default: printf("ERROR IN DIFFERENTIATION\n");
        }
    }

    return NULL;
}

node_t* Copy(struct node_t* node, struct tree_t* tree)
{
    assert(tree);

    printf("\nin copy\n");

    if ((node->left == NULL) && (node->right != NULL))
    {
        return CreateNode(node->type, node->value, node->left, Copy(node->right, tree), tree);
    }

    if ((node->left != NULL) && (node->right == NULL))
    {
        return CreateNode(node->type, node->value, Copy(node->left, tree), node->right, tree);
    }

    if ((node->left == NULL) && (node->right == NULL))
    {
        return CreateNode(node->type, node->value, node->left, node->right, tree);
    }

    return CreateNode(node->type, node->value, Copy(node->left, tree), Copy(node->right, tree), tree);
}

bool CheckUnionType(struct node_t* node, types type)
{
    if (node == NULL) {return NULL;}

    if (node->type == type)
    {
        return TYPE_IS_RIGHT;
    }

    return TYPE_IS_NOT_RIGHT;
}
