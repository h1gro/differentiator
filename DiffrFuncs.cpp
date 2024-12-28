#include <stdlib.h>
#include <assert.h>

#include "Differentiator.h"
#include "DiffrFuncs.h"
#include "Dump.h"

/*-------------------------Standart Operators:----------------------------*/

node_t* DiffrAdd(struct node_t* node, struct tree_t* tree)
{
    node_t* copy_node    = Copy(node, tree);

    node_t* node_left    = Diffr(node->left, tree);
    node_t* node_right   = Diffr(node->right, tree);

    node_t* diffr_result = CreateNode(OP, value_t{.oper_number = ADD}, node_left, node_right, tree);

    TreeDump(diffr_result, copy_node, DIFFERENTIATION);

    return diffr_result;
}

node_t* DiffrSub(struct node_t* node, struct tree_t* tree)
{
    node_t* copy_node    = Copy(node, tree);

    node_t* node_left    = Diffr(node->left, tree);
    node_t* node_right   = Diffr(node->right, tree);

    node_t* diffr_result = CreateNode(OP, value_t{.oper_number = SUB}, node_left, node_right, tree);

    TreeDump(diffr_result, copy_node, DIFFERENTIATION);

    return diffr_result;
}

node_t* DiffrMull(struct node_t* node, struct tree_t* tree)
{
    if (node->left->value.oper_number == EXP)
    {
        return DiffrMullExp(node, tree);
    }

    node_t* copy_node       = Copy(node, tree);

    node_t* diff_node_left  = Diffr(node->left, tree);
    node_t* diff_node_right = Diffr(node->right, tree);

    node_t* copy_node_left  = Copy(node->left, tree);
    node_t* copy_node_right = Copy(node->right, tree);

    node_t* first_summand   = CreateNode(OP, value_t{.oper_number = MULL}, diff_node_left, copy_node_right, tree);
    node_t* second_summand  = CreateNode(OP, value_t{.oper_number = MULL}, copy_node_left, diff_node_right, tree);

    node_t* diffr_result    = CreateNode(OP, value_t{.oper_number = ADD}, first_summand, second_summand, tree);

    TreeDump(diffr_result, copy_node, DIFFERENTIATION);

    return diffr_result;
}

node_t* DiffrDiv(struct node_t* node, struct tree_t* tree)
{
    node_t* diff_node_left = NULL;
    node_t* copy_node = Copy(node, tree);

    if (CheckUnionType(node->left, OP))
    {
        if (node->left->value.oper_number == EXP)
        {
            diff_node_left = Copy(node->left, tree);
        }
        else
        {
            diff_node_left = Diffr(node->left, tree);
        }
    }
    else
    {
        return node;
    }

    node_t* diff_node_right   = Diffr(node->right, tree);

    node_t* first_subtrahend  = CreateNode(OP, value_t{.oper_number = MULL}, diff_node_left, Copy(node->right, tree), tree);
    node_t* second_subtrahend = CreateNode(OP, value_t{.oper_number = MULL}, Copy(node->left, tree), diff_node_right, tree);

    node_t* numerator         = CreateNode(OP, value_t{.oper_number = SUB}, first_subtrahend, second_subtrahend, tree);
    node_t* denominator       = CreateNode(OP, value_t{.oper_number = MULL}, Copy(node->right, tree), Copy(node->right, tree), tree);

    node_t* diffr_result      = CreateNode(OP, value_t{.oper_number = DIV}, numerator, denominator, tree);

    TreeDump(diffr_result, copy_node, DIFFERENTIATION);

    return diffr_result;
}

node_t* DiffrDeg(struct node_t* node, struct tree_t* tree)
{
    node_t* copy_node    = Copy(node, tree);

    if ((node->left->type == NUM) && (node->right->type == NUM))
    {
        return CreateNode(NUM, value_t{.number = 0}, NULL, NULL, tree);
    }

    else if (((node->left->type == OP) && (node->right->type == NUM)) || ((node->left->type == VAR) && (node->right->type == NUM)))
    {
        if (node->left->value.oper_number == EXP)
        {
            return DiffrDegExp(node, tree);
        }

        node_t* coeff        = CreateNode(NUM, node->right->value, NULL, NULL, tree);

        node_t* new_deg      = CreateNode(OP, value_t{.oper_number = SUB}, Copy(node->right, tree), CreateNode(NUM, value_t{.number = 1}, NULL, NULL, tree), tree);

        node_t* new_var      = CreateNode(OP, value_t{.oper_number = DEG}, Copy(node->left, tree), new_deg, tree);

        node_t* diffr_result = CreateNode(OP, value_t{.oper_number = MULL}, coeff, new_var, tree);

        TreeDump(diffr_result, copy_node, DIFFERENTIATION);

        return diffr_result;
    }

    else if (((node->left->type == NUM) && (node->right->type == OP)) || ((node->right->type == NUM) && (node->left->type == VAR)))
    {
        node_t* ln          = CreateNode(OP, value_t{.oper_number = LN}, NULL, Copy(node->left, tree), tree);

        node_t* first_mull  = CreateNode(OP, value_t{.oper_number = MULL}, Copy(node, tree), ln, tree);

        node_t* second_mull = CreateNode(OP, value_t{.oper_number = MULL}, first_mull, Diffr(node->right, tree), tree);

        TreeDump(second_mull, copy_node, DIFFERENTIATION);

        return second_mull;
    }

    else if (((node->left->type == OP) && (node->right->type == OP)) || ((node->left->type == OP) && (node->right->type == VAR)) || ((node->left->type == VAR) && (node->right->type == VAR)) || ((node->left->type == VAR) && (node->right->type == OP)))
    {
        if (node->left->value.oper_number == EXP)
        {
            return DiffrDegExp(node, tree);
        }

        node_t* ln             = CreateNode(OP, value_t{.oper_number = LN}, NULL, Copy(node->left, tree), tree);

        node_t* second_summand = CreateNode(OP, value_t{.oper_number = MULL}, Copy(ln, tree), Diffr(node->right, tree), tree);

        node_t* first_summand  = CreateNode(OP, value_t{.oper_number = MULL}, Diffr(ln, tree), Copy(node->right, tree), tree);

        node_t* dif_part       = CreateNode(OP, value_t{.oper_number = ADD}, first_summand, second_summand, tree);

        node_t* node_mull      = CreateNode(OP, value_t{.oper_number = MULL}, Copy(node, tree), dif_part, tree);

        TreeDump(node_mull, copy_node, DIFFERENTIATION);

        return node_mull;
    }

    else
    {
        return NULL;
    }
}

/*-------------------------Trigonometric Operators:----------------------------*/

node_t* DiffrSin(struct node_t* node, struct tree_t* tree)
{
    node_t* copy_node    = Copy(node, tree);

    node_t* node_cos  = CreateNode(OP, value_t{.oper_number = COS}, NULL, Copy(node->right, tree), tree);

    node_t* node_mull = CreateNode(OP, value_t{.oper_number = MULL}, node_cos, Diffr(node->right, tree), tree);

    TreeDump(node_mull, copy_node, DIFFERENTIATION);

    return node_mull;
}

node_t* DiffrCos(struct node_t* node, struct tree_t* tree)
{
    node_t* copy_node = Copy(node, tree);

    node_t* node_sin  = CreateNode(OP, value_t{.oper_number = SIN}, NULL, Copy(node->right, tree), tree);

    node_t* coef      = CreateNode(NUM, value_t{.number = -1}, NULL, NULL, tree);

    node_t* coef_sin  = CreateNode(OP, value_t{.oper_number = MULL}, coef, node_sin, tree);

    node_t* node_mull = CreateNode(OP, value_t{.oper_number = MULL}, coef_sin, Diffr(node->right, tree), tree);

    TreeDump(node_mull, copy_node, DIFFERENTIATION);

    return node_mull;
}

node_t* DiffrTan(struct node_t* node, struct tree_t* tree)
{
    node_t* copy_node   = Copy(node, tree);

    node_t* numerator   = CreateNode(NUM, value_t{.number = 1}, NULL, NULL, tree);

    node_t* cos_node    = CreateNode(OP, value_t{.oper_number = COS}, NULL, Copy(node->right, tree), tree);

    node_t* deg         = CreateNode(NUM, value_t{.number = 2}, NULL, NULL, tree);

    node_t* deg_op      = CreateNode(OP, value_t{.oper_number  = DEG}, cos_node, deg, tree);

    node_t* dif_tg      = CreateNode(OP, value_t{.oper_number  = DIV}, numerator, deg_op, tree);

    node_t* node_mull   = CreateNode(OP, value_t{.oper_number  = MULL}, dif_tg, Diffr(node->right, tree), tree);

    TreeDump(node_mull, copy_node, DIFFERENTIATION);

    return node_mull;
}

node_t* DiffrCot(struct node_t* node, struct tree_t* tree)
{
    node_t* copy_node   = Copy(node, tree);

    node_t* numerator   = CreateNode(NUM, value_t{.number = 1}, NULL, NULL, tree);

    node_t* sin_node    = CreateNode(OP, value_t{.oper_number = SIN}, NULL, Copy(node->right, tree), tree);

    node_t* deg         = CreateNode(NUM, value_t{.number = 2}, NULL, NULL, tree);

    node_t* deg_op      = CreateNode(OP, value_t{.oper_number  = DEG}, sin_node, deg, tree);

    node_t* coef        = CreateNode(NUM, value_t{.number = -1}, NULL, NULL, tree);

    node_t* dif_tg      = CreateNode(OP, value_t{.oper_number  = DIV}, numerator, deg_op, tree);

    node_t* coef_dif_tg = CreateNode(OP, value_t{.oper_number = MULL}, coef, dif_tg, tree);

    node_t* node_mull   = CreateNode(OP, value_t{.oper_number  = MULL}, coef_dif_tg, Diffr(node->right, tree), tree);

    TreeDump(node_mull, copy_node, DIFFERENTIATION);

    return node_mull;
}

/*-------------------------hyperbolic Operators:----------------------------*/

node_t* DiffrShx(struct node_t* node, struct tree_t* tree)
{
    node_t* copy_node = Copy(node, tree);

    node_t* node_ch   = CreateNode(OP, value_t{.oper_number = CHS}, NULL, Copy(node->right, tree), tree);

    node_t* node_mull = CreateNode(OP, value_t{.oper_number = MULL}, node_ch, Diffr(node->right, tree), tree);

    TreeDump(node_mull, copy_node, DIFFERENTIATION);

    return node_mull;
}

node_t* DiffrChs(struct node_t* node, struct tree_t* tree)
{
    node_t* copy_node = Copy(node, tree);

    node_t* node_sh   = CreateNode(OP, value_t{.oper_number = SHX}, NULL, Copy(node->right, tree), tree);

    node_t* node_mull = CreateNode(OP, value_t{.oper_number = MULL}, node_sh, Diffr(node->right, tree), tree);

    TreeDump(node_mull, copy_node, DIFFERENTIATION);

    return node_mull;
}

node_t* DiffrThx(struct node_t* node, struct tree_t* tree)
{
    node_t* copy_node   = Copy(node, tree);

    node_t* numerator   = CreateNode(NUM, value_t{.number = 1}, NULL, NULL, tree);

    node_t* ch_node     = CreateNode(OP, value_t{.oper_number = CHS}, NULL, Copy(node->right, tree), tree);

    node_t* deg         = CreateNode(NUM, value_t{.number = 2}, NULL, NULL, tree);

    node_t* deg_op      = CreateNode(OP, value_t{.oper_number  = DEG}, ch_node, deg, tree);

    node_t* dif_th      = CreateNode(OP, value_t{.oper_number  = DIV}, numerator, deg_op, tree);

    node_t* node_mull   = CreateNode(OP, value_t{.oper_number  = MULL}, dif_th, Diffr(node->right, tree), tree);

    TreeDump(node_mull, copy_node, DIFFERENTIATION);

    return node_mull;
}

node_t* DiffrCth(struct node_t* node, struct tree_t* tree)
{
    node_t* copy_node    = Copy(node, tree);

    node_t* numerator    = CreateNode(NUM, value_t{.number = 1}, NULL, NULL, tree);

    node_t* sh_node      = CreateNode(OP, value_t{.oper_number = SHX}, NULL, Copy(node->right, tree), tree);

    node_t* deg          = CreateNode(NUM, value_t{.number = 2}, NULL, NULL, tree);

    node_t* deg_op       = CreateNode(OP, value_t{.oper_number  = DEG}, sh_node, deg, tree);

    node_t* coef         = CreateNode(NUM, value_t{.number = -1}, NULL, NULL, tree);

    node_t* dif_cth      = CreateNode(OP, value_t{.oper_number  = DIV}, numerator, deg_op, tree);

    node_t* coef_dif_cth = CreateNode(OP, value_t{.oper_number = MULL}, coef, dif_cth, tree);

    node_t* node_mull    = CreateNode(OP, value_t{.oper_number  = MULL}, coef_dif_cth, Diffr(node->right, tree), tree);

    TreeDump(node_mull, copy_node, DIFFERENTIATION);

    return node_mull;
}

/*----------------------------Math Operators:-------------------------------*/

node_t* DiffrLn(struct node_t* node, struct tree_t* tree)
{
    node_t* copy_node = Copy(node, tree);

    node_t* coef_one  = CreateNode(NUM, value_t{.number = 1}, NULL, NULL, tree);

    node_t* dif_ln    = CreateNode(OP, value_t{.oper_number = DIV}, coef_one, Copy(node->right, tree), tree);

    node_t* node_mull = CreateNode(OP, value_t{.oper_number = MULL}, dif_ln, Diffr(node->right, tree), tree);

    TreeDump(node_mull, copy_node, DIFFERENTIATION);

    return node_mull;
}

node_t* DiffrDegExp(struct node_t* node, struct tree_t* tree)
{
    node_t* copy_node = Copy(node, tree);

    if ((node->right->type == OP) || (node->right->type == VAR))
    {
        node_t* node_mull = CreateNode(OP, value_t{.oper_number = MULL}, Copy(node, tree), Diffr(node->right, tree), tree);

        TreeDump(node_mull, copy_node, DIFFERENTIATION);

        return node_mull;
    }

    if (node->right->type == NUM)
    {
        return CreateNode(NUM, value_t{.number = 0}, NULL, NULL, tree);
    }

    return NULL;
}

node_t* DiffrMullExp(struct node_t* node, struct tree_t* tree)
{
    if (node->right->type == VAR)
    {
        return Copy(node->left, tree);
    }

    if (node->right->type == OP)
    {
        return CreateNode(OP, value_t{.oper_number = MULL}, Copy(node->left, tree), Diffr(node->right, tree), tree);
    }

    if (node->right->type == NUM)
    {
        return CreateNode(NUM, value_t{.number = 0}, NULL, NULL, tree);
    }

    return NULL;
}
