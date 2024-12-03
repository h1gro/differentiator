#include <assert.h>

#include "Differentiator.h"
#include "Dump.h"

node_t* Simplifier(struct node_t* node, struct tree_t* tree)
{
    assert(node);
    assert(tree);

    if ((node->left == NULL) || (node->right == NULL)){return node;}

    node->left  = Simplifier(node->left, tree);
    node->right = Simplifier(node->right, tree);

    if ((node->left == NULL) || (node->right == NULL)){return node;}

    if ((node->right->type == NUM) && (node->left->type == NUM))
    {
        DoNodeOperation(node);

        TreeDump(node);

        return node;
    }

    if (node->type == OP)
    {
        switch(node->value.oper)
        {
            case ADD:
            {
                if ((node->right->type == NUM) && (IsEqual(node->right->value.number, 0))){ChangeNodeAddSub(node, node->left, node->right);TreeDump(node);return node;}

                if ((node->left->type == NUM)  && (IsEqual(node->left->value.number, 0))){ChangeNodeAddSub(node, node->right, node->left);TreeDump(node);return node;}

                return node;
            }

            case SUB:
            {
                if ((node->right->type == NUM) && (IsEqual(node->right->value.number, 0))){ChangeNodeAddSub(node, node->left, node->right);TreeDump(node);return node;}

                if ((node->left->type == NUM)  && (IsEqual(node->left->value.number, 0))){ChangeNodeAddSub(node, node->right, node->left);TreeDump(node);return node;}

                return node;
            }

            case MULL:
            {
                printf("node = %p\nnode->right = %p\nnode->left = %p\nnode->right->type = %d\nnode->left->type = %d\n", node, node->right, node->left, node->right->type, node->left->type);

                if ((node->left->type == NUM)  && (IsEqual(node->left->value.number, 0))){ChangeNodeMullDeg(node, 0);TreeDump(node);return node;}

                if ((node->right->type == NUM) && (IsEqual(node->right->value.number, 0))){ChangeNodeMullDeg(node, 0);TreeDump(node);return node;}

                return node;
            }

            case DEG:
            {
                if ((node->left->type == NUM)  && (IsEqual(node->left->value.number, 0))){ChangeNodeMullDeg(node, 1);TreeDump(node);return node;}

                return node;
            }

            default:    printf("error in simplifier\n");
        }
    }

    return node;
}

void ChangeNodeAddSub(struct node_t* node, node_t* desired_node, node_t* undesired_node)
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

    NodsDtor(desired_node);
    NodsDtor(undesired_node);
}

void ChangeNodeMullDeg(struct node_t* node, int value)
{
    assert(node);

    node->type         = NUM;
    node->value.number = value;

    DeleteLocalNods(node);
}

void DoNodeOperation(struct node_t* node)
{
    switch(node->value.oper)
    {
        case ADD:
        {
            node->value.number = node->left->value.number + node->right->value.number;

            break;
        }

        case MULL:
        {
            node->value.number = node->left->value.number * node->right->value.number;

            break;
        }

        case DIV:
        {
            if (!IsEqual(node->right->value.number, 0))
            {
                node->value.number = node->left->value.number / node->right->value.number;
            }

            break;
        }

        case SUB:
        {
            node->value.number = node->left->value.number - node->right->value.number;

            break;
        }

        case DEG:
        {
            node->value.number = RaiseToDegree(node->left->value.number, node->right->value.number);

            break;
        }

        default:    printf("error in donodeoperation\n");
    }

    node->type = NUM;

    DeleteLocalNods(node);
}

double RaiseToDegree(double number, double power)
{
    double number1 = 1;

    for (int i = 1; i <= power; i++)
    {
        number1 *= number;
    }

    return number1;
}
