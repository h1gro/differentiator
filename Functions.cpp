#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "Differentiator.h"
#include "Dump.h"

node_t* GetDollar(struct expr_t* expr, struct tree_t* tree)
{
    assert(expr);
    assert(tree);
    assert(expr->string);
    assert(expr->index >= 0);

    node_t* val = GetAddSub(expr, tree);

    if (expr->string[expr->index] != '$')
    {
        SyntaxError(__FILE__, __func__, __LINE__);
        expr->index++;
    }

    return val;
}

node_t* GetVar(struct expr_t* expr, struct tree_t* tree, char varyable)
{
    assert(expr);
    assert(tree);
    assert(expr->string);
    assert(expr->index >= 0);

    node_t* node = NULL;

    if (varyable == 'x')
    {
        node = CreateNode(VAR, value_t{.oper_number = 1}, NULL, NULL, tree);
        expr->index++;

        TreeDump(node);
    }
    else if (varyable == 'y')
    {
        node = CreateNode(VAR, value_t{.oper_number = 2}, NULL, NULL, tree);
        expr->index++;

        TreeDump(node);
    }

    // TreeDump(node);

    return node;
}

node_t* GetNum(struct expr_t* expr, struct tree_t* tree)
{
    assert(expr);
    assert(tree);
    assert(expr->string);
    assert(expr->index >= 0);

    int val = 0;

    int old_p = expr->index;

    while (('0' <= expr->string[expr->index]) && (expr->string[expr->index] <= '9'))
    {
        val = val * 10 + expr->string[expr->index] - '0';
        expr->index++;
    }

    if (old_p == expr->index)
    {
        SyntaxError(__FILE__, __func__, __LINE__);
    }

    node_t* node = CreateNode(NUM, value_t{.number = val}, NULL, NULL, tree);

    TreeDump(node);

    return node;
}

node_t* GetAddSub(struct expr_t* expr, struct tree_t* tree)
{
    assert(expr);
    assert(tree);
    assert(expr->string);
    assert(expr->index >= 0);

    node_t* val = GetMullDiv(expr, tree);

    while ((expr->string[expr->index] == '+') || (expr->string[expr->index] == '-'))
    {
        int op = expr->string[expr->index];
        expr->index++;
        node_t* val2 = GetMullDiv(expr, tree);

        if (op == '+')
        {
            val = CreateNode(OP, value_t{.oper_number = ADD}, val, val2, tree);
        }

        else
        {
            val = CreateNode(OP, value_t{.oper_number = SUB}, val, val2, tree);
        }
    }

    TreeDump(val);

    return val;
}

node_t* GetMullDiv(struct expr_t* expr, struct tree_t* tree)
{
    assert(expr);
    assert(tree);
    assert(expr->string);
    assert(expr->index >= 0);

    node_t* val = GetBrackets(expr, tree);

    while ((expr->string[expr->index] == '*') || (expr->string[expr->index] == '/'))
    {
        int op = expr->string[expr->index];
        expr->index++;
        node_t* val2 = GetBrackets(expr, tree);

        if (op == '*')
        {
            val = CreateNode(OP, value_t{.oper_number = MULL}, val, val2, tree);
        }

        else
        {
            val = CreateNode(OP, value_t{.oper_number = DIV}, val, val2, tree);
        }
    }

    TreeDump(val);

    return val;
}

node_t* GetBrackets(struct expr_t* expr, struct tree_t* tree)
{
    assert(expr);
    assert(tree);
    assert(expr->string);
    assert(expr->index >= 0);

    if (expr->string[expr->index] == '(')
    {
        expr->index++;
        node_t* val = GetAddSub(expr, tree);

        if (expr->string[expr->index] != ')')
        {
            SyntaxError(__FILE__, __func__, __LINE__);
        }

        expr->index++;
        return val;
    }
    else if ((expr->string[expr->index] == 'x') || (expr->string[expr->index] == 'y'))
    {
        return GetVar(expr, tree, expr->string[expr->index]);
    }
    else
    {
        return GetNum(expr, tree);
    }
}

void SyntaxError(const char* file, const char* func, int line)
{
    printf("\n<<<<<SYNTAX ERROR in %s in %s in line:%d>>>>>\n", file, func, line);

    exit(0);
}

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

    root->number_nods++;

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

char WhatIsOperator(int value_oper)
{
    switch(value_oper)
    {
        case ADD:   return '+';

        case SUB:   return '-';

        case MULL:  return '*';

        case DIV:   return '/';

        case DEG:   return '^';

        default:    printf("ERROR TYPE\n");
                    return ERROR_TYPE;
    }
}
