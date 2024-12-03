#include <assert.h>

#include "Differentiator.h"
#include "Dump.h"

node_t* Diffr(struct node_t* node, struct tree_t* tree)
{
    assert(node);
    assert(tree);

    printf("type = %d\nnumber = %lg\noper number = %d\n", node->type, node->value.number, node->value.oper_number);

    if (node->type == NUM)
    {
        return CreateNode(NUM, 0, NULL, NULL, tree);
    }

    if (node->type == VAR)
    {
        return CreateNode(NUM, 1, NULL, NULL, tree);
    }

    if (node->type == OP)
    {
        switch(node->value.oper_number)
        {
            case ADD:
            {
                node_t* node_left    = Diffr(node->left, tree);
                node_t* node_right   = Diffr(node->right, tree);

                node_t* diffr_result = CreateNode(OP, ADD, node_left, node_right, tree);

                return diffr_result;
            }

            case SUB:
            {
                node_t* node_left    = Diffr(node->left, tree);
                node_t* node_right   = Diffr(node->right, tree);

                node_t* diffr_result = CreateNode(OP, SUB, node_left, node_right, tree);

                return diffr_result;
            }

            case MULL:
            {
                node_t* diff_node_left  = Diffr(node->left, tree);
                node_t* diff_node_right = Diffr(node->right, tree);

                node_t* copy_node_left  = Copy(node->left, tree);
                node_t* copy_node_right = Copy(node->right, tree);

                node_t* first_summand   = CreateNode(OP, MULL, diff_node_left, copy_node_right, tree);
                node_t* second_summand  = CreateNode(OP, MULL, copy_node_left, diff_node_right, tree);

                node_t* diffr_result    = CreateNode(OP, ADD, first_summand, second_summand, tree);

                return diffr_result;
            }

            case DIV:
            {
                if (IsEqual(node->right->value.number, 0)) //TODO функция проверки занчения value в юнионe, Отправляю тип и само число и проверяю их на совместимость
                {
                    printf("DIVISION BY ZERO!!!\n");
                    return NULL;
                }

                node_t* diff_node_left    = Diffr(node->left, tree);
                node_t* diff_node_right   = Diffr(node->right, tree);

                node_t* first_subtrahend  = CreateNode(OP, MULL, diff_node_left, Copy(node->right, tree), tree);
                node_t* second_subtrahend = CreateNode(OP, MULL, Copy(node->left, tree), diff_node_right, tree);

                node_t* numerator         = CreateNode(OP, SUB, first_subtrahend, second_subtrahend, tree);
                node_t* denominator       = CreateNode(OP, MULL, Copy(node->right, tree), Copy(node->right, tree), tree);

                node_t* diffr_result      = CreateNode(OP, DIV, numerator, denominator, tree);

                TreeDump(diffr_result);

                return diffr_result;
            }

            case DEG:
            {
                if (!IsEqual(node->right->value.number, 0)){return CreateNode(NUM, 0, NULL, NULL, tree);}

                node_t* coeff        = CreateNode(NUM, node->right->value.number, NULL, NULL, tree);

                //printf("\nnode->right->value.number = %lg\n", node->right->value.number);

                node_t* new_deg      = CreateNode(OP, SUB, Copy(node->right, tree), CreateNode(NUM, 1, NULL, NULL, tree), tree);

                //printf("done\n");

                TreeDump(new_deg);

                node_t* new_var      = CreateNode(OP, DEG, Copy(node->left, tree), new_deg, tree);

                node_t* diffr_result = CreateNode(OP, MULL, coeff, new_var, tree);

                return diffr_result;
            }

            default:    printf("ERROR IN DIFFERENTIATION\n");
        }
    }

    return NULL;
}

node_t* Copy(struct node_t* node, struct tree_t* tree)
{
    assert(node);
    assert(tree);
    printf("\nin copy\n");
    return CreateNode(node->type, node->value.number, node->left, node->right, tree);
}

// void WhatUnionType(struct union value, types expected_type)
// {
//
// }
