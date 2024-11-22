#include <assert.h>

#include "Differentiator.h"
#include "Dump.h"

static const char* DUMP_DOT = "Dump.dot";

/*-------------------------Graph Dump--------------------------*/

void TreeDump(struct node_t* tree)
{
    assert(tree);

    FILE* dump = fopen(DUMP_DOT, "w");

    assert(dump);

    PrintGraphHead(dump);

    PrintDefaultList(dump, tree);

    fprintf(dump, "}\n");
    fclose(dump);

    static int number_dump = 0;
    MySystem("dot Dump.dot -Tpng -o Graphs/Dump%03d.png", number_dump);
    number_dump++;

    printf("\n");
}

void PrintGraphHead(FILE* graph)
{
    assert(graph);

    fprintf(graph, "digraph G\n{\n    rankdir = TB;\n    bgcolor = \"pink2\"\n"
                   "    node[color = \"#b503fc\", fontsize = 14];\n"
                   "    edge[color = black, fontsize = 12, splines = ortho];\n\n");

}

void PrintDefaultList(FILE* graph, struct node_t* tree) //TODO переделать проверки сделать оптимальнее
{
    assert(graph);
    assert(tree);

    /*ОПРЕДЕЛЕНИЕ ТИПА*/

    if (tree->type == NUM)
    {
        fprintf(graph, "    %d [shape = Mrecord, label = \"{ %d | type NUM | { <f0> left %4p | <f1> right %4p}}\"];\n",
                            tree->value, tree->value, tree->left, tree->right);
    }

    else if (tree->type == VAR)
    {
        if (tree->value == 1)
        {
            fprintf(graph, "    %d [shape = Mrecord, label = \"{ x | type VAR | { <f0> left %4p | <f1> right %4p}}\"];\n",
                               tree->value, tree->left, tree->right);
        }
    }

    else if (tree->type == OP)
    {
        char oper = WhatIsOperator(tree->value);

        fprintf(graph, "    \"%c\" [shape = Mrecord, label = \"{ \'%c\' | type OP | { <f0> left %4p | <f1> right %4p}}\"];\n",
                            oper, oper, tree->left, tree->right);
    }

    /*Рекурсия*/

/*--------------------------LEFT----------------------------*/

    if (tree->left != NULL)
    {
        if ((tree->left->type == OP) && (tree->type == OP))
        {
            char oper1 = WhatIsOperator(tree->value);
            char oper_left = WhatIsOperator(tree->left->value);

            fprintf(graph, "    \"%c\" : <f0> -> \"%c\" [style = \"filled\"]\n", oper1, oper_left);
            PrintDefaultList(graph, tree->left);
        }

        else if (tree->type == OP)
        {
            char oper1 = WhatIsOperator(tree->value);

            fprintf(graph, "    \"%c\" : <f0> -> \"%d\" [style = \"filled\"]\n", oper1, tree->left->value);
            PrintDefaultList(graph, tree->left);
        }

        else
        {
            fprintf(graph, "    \"%d\" : <f0> -> \"%d\" [style = \"filled\"]\n", tree->value, tree->left->value);
            PrintDefaultList(graph, tree->left);
        }
    }

/*--------------------------RIGHT----------------------------*/

    if (tree->right != NULL)
    {
        if ((tree->right->type == OP) && (tree->type == OP))
        {
            char oper2 = WhatIsOperator(tree->value);
            char oper_right = WhatIsOperator(tree->left->value);

            fprintf(graph, "    \"%c\" : <f1> -> \"%c\" [style = \"filled\"]\n", oper2, oper_right);
            PrintDefaultList(graph, tree->right);
        }

        else if (tree->type == OP)
        {
            char oper2 = WhatIsOperator(tree->value);

            fprintf(graph, "    \"%c\" : <f1> -> \"%d\" [style = \"filled\"]\n", oper2, tree->right->value);
            PrintDefaultList(graph, tree->right);
        }

        else
        {
            fprintf(graph, "    \"%c\" : <f1> -> \"%d\" [style = \"filled\"]\n", tree->value, tree->right->value);
            PrintDefaultList(graph, tree->right);
        }
    }
}

/*-------------------------Inorder--------------------------*/

void WriteTree(struct node_t* node, FILE* file_ptr)
{
    assert(node);
    assert(file_ptr);

    fprintf(file_ptr, "{");

    if (node->left != NULL)
    {
        WriteTree(node->left, file_ptr);
    }

    switch(node->type)
    {
        case OP:    fprintf(file_ptr, "{\"%c\"}", WhatIsOperator(node->value));
                    break;

        case NUM:   fprintf(file_ptr, "\"%d\"", node->value);
                    break;

        case VAR:   if (node->value == 1) {fprintf(file_ptr, "\"x\"");}

                    else if (node->value == 2) {fprintf(file_ptr, "\"x\"");}
                    break;

        default:    printf("ERROR IN VALUE\n");

    }

    if (node->right != NULL)
    {
        WriteTree(node->right, file_ptr);
    }

    fprintf(file_ptr, "}");
}

char WhatIsOperator(int value_oper)
{
    switch(value_oper)
    {
        case ADD:   return '+';

        case SUB:   return '-';

        case MULL:  return '*';

        case DIV:   return '/';

        default:    printf("ERROR TYPE\n");
                    return ERROR_TYPE;
    }
}

// void PrintSubtreeWithNumber(FILE* graph, struct node_t* tree)
// {
//     fprintf(graph, "    %4d [shape = Mrecord, label = \"{ %4d | type NUM { <f0> left %4p | <f1> right %4p}}\"];\n",
//                             tree->value, tree->value, tree->left, tree->right);
//
//     if (tree->left != NULL)
//     {
//         fprintf(graph, "    %4d : <f0> -> %4s [style = \"filled\"]\n", tree->value, tree->left->value);
//         PrintDefaultList(graph, tree->left);
//     }
//
//     if (tree->right != NULL)
//     {
//         fprintf(graph, "    %4s : <f1> -> %4s [style = \"filled\"]\n", tree->value, tree->right->value);
//         PrintDefaultList(graph, tree->right);
//     }
// }
