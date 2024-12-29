#include <assert.h>
#include <math.h>

#include "Differentiator.h"
#include "Dump.h"

node_t* Simplifier(struct node_t* node, struct tree_t* tree)
{
    assert(node);
    assert(tree);

    node_t* copy_node = Copy(node, tree);

    if (node->left != NULL)
    {
        node->left  = Simplifier(node->left, tree);
    }

    if (node->right != NULL)
    {
        node->right = Simplifier(node->right, tree);
    }

    if ((CheckUnionType(node, OP)) && (node->value.oper_number != EXP))
    {
        DoNodeOperation(tree, node);
    }

    if (node->type == OP)
    {
        switch(node->value.oper)
        {
            case ADD:
            {
                if ((node->right->type == NUM) && (IsEqual(node->right->value.number, 0)))
                {
                    ChangeNodeAddSub(tree, node, node->left, node->right);

                    TreeDump(tree, node, copy_node, SIMPLIFICATION);

                    return node;
                }

                if ((node->left->type == NUM) && (IsEqual(node->left->value.number, 0)))
                {
                    ChangeNodeAddSub(tree, node, node->right, node->left);

                    TreeDump(tree, node, copy_node, SIMPLIFICATION);

                    return node;
                }

                return node;
            }

            case SUB:
            {
                if ((node->right->type == NUM) && (IsEqual(node->right->value.number, 0)))
                {
                    ChangeNodeAddSub(tree, node, node->left, node->right);

                    TreeDump(tree, node, copy_node, SIMPLIFICATION);

                    return node;
                }

                if ((node->left->type == NUM)  && (IsEqual(node->left->value.number, 0)))
                {
                    ChangeNodeAddSub(tree, node, node->right, node->left);

                    TreeDump(tree, node, copy_node, SIMPLIFICATION);

                    return node;
                }

                return node;
            }

            case MULL:
            {
                if ((node->left->type == NUM)  && (IsEqual(node->left->value.number, 0)))
                {
                    ChangeNodeMullDeg(tree, node, node->left);

                }

                else if ((node->right->type == NUM) && (IsEqual(node->right->value.number, 0)))
                {
                    ChangeNodeMullDeg(tree, node, node->right);
                }

                else if ((CheckUnionType(node->left, NUM)) && (IsEqual(node->left->value.number, 1)))
                {
                    ChangeNodeMullDeg(tree, node, node->right);
                }

                else if ((CheckUnionType(node->right, NUM)) && (IsEqual(node->right->value.number, 1)))
                {
                    ChangeNodeMullDeg(tree, node, node->left);
                }

                return node;
            }

            case DEG:
            {
                if ((CheckUnionType(node->right, NUM)) && (IsEqual(node->right->value.number, 0)))
                {
                    node_t* one = CreateNode(NUM, value_t{.number = 1}, NULL, NULL, tree);

                    ChangeNodeMullDeg(tree, node, one);
                }

                else if ((CheckUnionType(node->right, NUM))  && (IsEqual(node->right->value.number, 1)))
                {
                    ChangeNodeMullDeg(tree, node, node->left);
                }

                return node;
            }

            case DIV:
            {
                if ((CheckUnionType(node->right, NUM)) && (IsEqual(node->right->value.number, 1)))
                {
                    ChangeNodeMullDeg(tree, node, node->left);
                }

                else if ((CheckUnionType(node->right, NUM)) && (IsEqual(node->right->value.number, 0)))
                {
                    printf("\n\n<<<<<<<<<<<DIVISION BY ZERO!!!!>>>>>>>>>>>>>\n\n");
                }

                if ((CheckUnionType(node->right, NUM)) && (node->right->value.number < 1) && (node->right->value.number > 0))
                {
                    node->value.oper_number = MULL;
                    node->right->value.number = 1 / node->right->value.number;
                }

                return node;
            }

            case EXP:
            {
                return node;
            }

            default:    printf("error in simplifier\n");
        }
    }

    return node;
}

void ChangeNodeAddSub(struct tree_t* tree, struct node_t* node, node_t* desired_node, node_t* undesired_node)
{
    assert(node);
    assert(desired_node);
    assert(undesired_node);

    node->type = desired_node->type;
    node->value = desired_node->value;

    node->left  = desired_node->left;
    node->right = desired_node->right;

    desired_node->left  = NULL;
    desired_node->right = NULL;

    NodsDtor(tree, desired_node);
    NodsDtor(tree, undesired_node);
}

void ChangeNodeMullDeg(struct tree_t* tree, struct node_t* node, struct node_t* new_node)
{
    assert(node);

    node_t* fict1 = node->right;
    node_t* fict2 = node->left;

    node->right = new_node->right;
    node->left  = new_node->left;

    new_node->right = NULL;
    new_node->left  = NULL;

    node->type  = new_node->type;
    node->value = new_node->value;

    NodsDtor(tree, fict1);
    NodsDtor(tree, fict2);
}

void DoNodeOperation(struct tree_t* tree, struct node_t* node)
{
    node_t* copy_node = Copy(node, tree);

    if (!CheckUnionType(node, OP) || (node == NULL))
    {
        return;
    }

    switch(node->value.oper_number)
    {
        case ADD:
        {
            if ((CheckUnionType(node->left, NUM)) && (CheckUnionType(node->right, NUM)))
            {
                node->value.number = node->left->value.number + node->right->value.number;
            }

            else if ((CheckUnionType(node->left, OP))   &&
                     (CheckUnionType(node->right, NUM)) &&
                     (node->left->value.oper_number == EXP))
            {
                node->value.number = exp(1) + node->right->value.number;
            }

            else if ((CheckUnionType(node->right, OP)) &&
                     (CheckUnionType(node->left, NUM)) &&
                     (node->right->value.oper_number == EXP))
            {
                node->value.number = exp(1) + node->left->value.number;
            }

            else if ((node->right->value.oper_number == EXP) &&
                     (node->left->value.oper_number == EXP)  &&
                     (CheckUnionType(node->right, OP))       &&
                     (CheckUnionType(node->left, OP)))
            {
                node->value.number = 2 * exp(1);
            }

            else
            {
                return;
            }

            node->type = NUM;

            DeleteLocalNods(tree, node);

            break;
        }

        case MULL:
        {
            if ((CheckUnionType(node->left, NUM)) && (CheckUnionType(node->right, NUM)))
            {
                node->value.number = node->left->value.number * node->right->value.number;

                node->type = NUM;

                DeleteLocalNods(tree, node);
            }

            break;
        }

        case DIV:
        {
            if ((CheckUnionType(node->left, NUM)) && (CheckUnionType(node->right, NUM)))
            {
                if (!IsEqual(node->right->value.number, 0))
                {
                    node->value.number = node->left->value.number / node->right->value.number;

                    node->type = NUM;

                    DeleteLocalNods(tree, node);
                }
            }

            break;
        }

        case SUB:
        {
            if ((CheckUnionType(node->left, NUM)) && (CheckUnionType(node->right, NUM)))
            {
                node->value.number = node->left->value.number - node->right->value.number;
            }

            else if ((CheckUnionType(node->left, OP))   &&
                     (CheckUnionType(node->right, NUM)) &&
                     (node->left->value.oper_number == EXP))
            {
                node->value.number = exp(1) - node->right->value.number;
            }

            else if ((CheckUnionType(node->right, OP)) &&
                     (CheckUnionType(node->left, NUM)) &&
                     (node->right->value.oper_number == EXP))
            {
                node->value.number = exp(1) - node->left->value.number;
            }

            else if ((node->right->value.oper_number == EXP) &&
                     (node->left->value.oper_number == EXP)  &&
                     (CheckUnionType(node->right, OP))       &&
                     (CheckUnionType(node->left, OP)))
            {
                node->value.number = 0;
            }

            else
            {
                return;
            }

            node->type = NUM;

            DeleteLocalNods(tree, node);

            break;
        }

        case DEG:
        {
            if ((CheckUnionType(node->left, NUM)) && (CheckUnionType(node->right, NUM)))
            {
                if ((IsEqual(node->left->value.number, 0) && (IsEqual(node->right->value.number, 0))))
                {
                    node->value.number = 1;
                }

                else
                {
                    node->value.number = pow(node->left->value.number, node->right->value.number);
                }

                node->type = NUM;

                DeleteLocalNods(tree, node);
            }

            break;
        }

        case SIN:
        {
            _COUNT_FUNCTION_(sin);
        }

        case COS:
        {
            _COUNT_FUNCTION_(cos);
        }

        case TAN:
        {
            _COUNT_FUNCTION_(tan);
        }

        case COT:
        {
            _COUNT_FUNCTION_(1/tan);
        }

        case SHX:
        {
            _COUNT_FUNCTION_(sinh);
        }

        case CHS:
        {
            _COUNT_FUNCTION_(cosh);
        }

        case THX:
        {
            _COUNT_FUNCTION_(tanh);
        }

        case CTH:
        {
            _COUNT_FUNCTION_(1/tanh);
        }

        default:    printf("error in donodeoperation\n");
    }
}
