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

//             case LOG:
//             {
//                 node_t* coef_one    = CreateNode(NUM, value_t{.number = 1}, NULL, NULL, tree);
//
//                 node_t* ln          = CreateNode(OP, value_t{.oper_number = LN}, NULL, Copy(node->left, tree);
//
//                 node_t* denominator = CreateNode(OP, value_t{.oper_number = MULL}, ln, Copy(node->right, tree), tree);
//
//                 node_t* dif_lg      = CreateNode(OP, value_t{.oper_number = DIV}, coef_one, denominator, tree);
//
//                 node_t* node_mull   = CreateNode(OP, value_t{.oper_number = MULL}, dif_lg, Diffr(node->right, tree), tree);
//
//                 return node_mull;
//             }
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

// bool CheckUnionType(union value_t value, types type)
// {
//       if (value->oper_number == )
//     {
//         return TYPE_IS_RIGHT;
//     }
//
//     return TYPE_IS_NOT_RIGHT;
// }
