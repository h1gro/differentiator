#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "Differentiator.h"
#include "RecursDown.h"
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

        //TreeDump(node);
    }
    else if (varyable == 'y')
    {
        node = CreateNode(VAR, value_t{.oper_number = 2}, NULL, NULL, tree);
        expr->index++;

        //TreeDump(node);
    }

    // TreeDump(node);

    return node;
}

node_t* GetPower(struct expr_t* expr, struct tree_t* tree)
{
    assert(expr);
    assert(tree);
    assert(expr->string);
    assert(expr->index >= 0);

    node_t* val = GetBrackets(expr, tree);

    while (expr->string[expr->index] == '^')
    {
        expr->index++;

        node_t* val2 = GetBrackets(expr, tree);

        val = CreateNode(OP, value_t{.oper_number = DEG}, val, val2, tree);
    }

    //TreeDump(val);

    return val;

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

    //TreeDump(node);

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

    //TreeDump(val);

    return val;
}

node_t* GetMullDiv(struct expr_t* expr, struct tree_t* tree)
{
    assert(expr);
    assert(tree);
    assert(expr->string);
    assert(expr->index >= 0);

    node_t* val = GetPower(expr, tree);

    while ((expr->string[expr->index] == '*') || (expr->string[expr->index] == '/'))
    {
        int op = expr->string[expr->index];
        expr->index++;
        node_t* val2 = GetPower(expr, tree);

        if (op == '*')
        {
            val = CreateNode(OP, value_t{.oper_number = MULL}, val, val2, tree);
        }

        else
        {
            val = CreateNode(OP, value_t{.oper_number = DIV}, val, val2, tree);
        }
    }

    //TreeDump(val);

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
    else if (('0' <= expr->string[expr->index]) && (expr->string[expr->index] <= '9'))
    {
        return GetNum(expr, tree);
    }
    else
    {
        return GetFunction(expr, tree);
    }
}

node_t* GetFunction(struct expr_t* expr, struct tree_t* tree)
{
    assert(expr);
    assert(tree);
    assert(expr->string);
    assert(expr->index >= 0);

    int word_lenght = 0;

    char function[FUNC_LENGHT] = {};

    while ((expr->string[expr->index] != ' ') && (expr->string[expr->index] != '('))
    {
        function[word_lenght] = expr->string[expr->index];

        printf("%c", function[word_lenght]);

        word_lenght++;

        expr->index++;

        if (expr->string[expr->index - 1] == 'e'){break;}
    }

    //printf("%c", expr->string[expr->index]);
    // if (expr->string[expr->index + 1] == '$')
    // {
    //     printf("\n\nGEEEET\n\n");
    //     expr->index--;
    // }

    printf("\n");

    //node_t* val = CompareFuncs(expr, tree, function, word_lenght);

    int oper_number = CompareFuncs(expr, tree, function, word_lenght);

    if (oper_number == -1) {return NULL;}

    if (expr->string[expr->index] == ' ') expr->index++;

    if (expr->string[expr->index] == '(')
    {
        expr->index++;
        node_t* val2 = GetAddSub(expr, tree);

        if (expr->string[expr->index] != ')')
        {
            SyntaxError(__FILE__, __func__, __LINE__);
        }

        expr->index++;

        node_t* val = CreateNode(OP, value_t{.oper_number = oper_number}, NULL, val2, tree);

        //TreeDump(val);

        return val;
    }
    else if (oper_number == EXP)
    {
        node_t* val = CreateNode(OP, value_t{.oper_number = EXP}, NULL, NULL, tree);

        //TreeDump(val);

        return val;
    }
    else
    {
        SyntaxError(__FILE__, __func__, __LINE__);
    }
}

int CompareFuncs(struct expr_t* expr, struct tree_t* tree, char* function, int word_lenght)
{
    for (int i = 0; i < NUM_FUNCS; i++)
    {
        printf("keyword = %s\nptr_struct = %p\n ptr_str = %p\n", keywords[i].func, &keywords[i], keywords[i].func);
        if (strncmp(function, keywords[i].func, word_lenght) == 0)
        {
            printf("op = %d\n", keywords[i].func_enum);
            return keywords[i].func_enum;
        }
    }

    return -1;
}

void SyntaxError(const char* file, const char* func, int line)
{
    printf("\n<<<<<SYNTAX ERROR in %s in %s in line:%d>>>>>\n", file, func, line);

    exit(0);
}
