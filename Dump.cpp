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
    SystemWithIntArg("dot Dump.dot -Tpng -o Graphs/Dump%03d.png", number_dump);
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

void PrintDefaultList(FILE* graph, struct node_t* tree)
{
    assert(graph);
    assert(tree);

    switch(tree->type)
    {
        case VAR:
        {
                 fprintf(graph, "    node%p [shape = Mrecord, label = \"{ x | type VAR | { <f0> left %4p | <f1> right %4p}}\"];\n",
                                tree, tree->left, tree->right);
                break;
        }

        case NUM:
        {
                fprintf(graph, "    node%p [shape = Mrecord, label = \"{ %lg | type NUM | { <f0> left %4p | <f1> right %4p}}\"];\n",
                               tree, tree->value.number, tree->left, tree->right);
                break;
        }

        case OP:
        {
                fprintf(graph, "    node%p [shape = Mrecord, label = \"{ \'%c\' | type OP | { <f0> left %4p | <f1> right %4p}}\"];\n",
                               tree, tree->value.oper, tree->left, tree->right);
                printf("node = %p, node->type = %d, node->value.oper_number = %d, node->value.oper = %c\n", tree, tree->type, tree->value.oper_number, tree->value.oper);
                PrintEdge(graph, tree);

                break;
        }

        default:    printf("ERROR TYPE IN DUMP\n");
    }

    if (tree->left != NULL) {PrintDefaultList(graph, tree->left);}

    if (tree->right != NULL) {PrintDefaultList(graph, tree->right);}
}

/*-------------------------Inorder--------------------------*/

void WriteTree(struct node_t* node, FILE* file_ptr)
{
    assert(node);
    assert(file_ptr);

    fprintf(file_ptr, "{");

    WriteNode(node, file_ptr);

    fprintf(file_ptr, "}");
}

void WriteNode(struct node_t* node, FILE* file_ptr)
{
    assert(node);
    assert(file_ptr);

//     /*-------------------------Preorder--------------------------*/
//
//     switch(node->type)
//     {
//         case OP:    fprintf(file_ptr, "{\"%c\"}", WhatIsOperator(node->value));
//                     break;
//
//         case NUM:   fprintf(file_ptr, "\"%d\"", node->value);
//                     break;
//
//         case VAR:   if (node->value == 1) {fprintf(file_ptr, "\"x\"");}
//
//                     else if (node->value == 2) {fprintf(file_ptr, "\"x\"");}
//                     break;
//
//         default:    printf("ERROR IN VALUE\n");
//
//     }
//
//     if (node->left != NULL)
//     {
//         WriteTree(node->left, file_ptr);
//     }
//
//     if (node->right != NULL)
//     {
//         WriteTree(node->right, file_ptr);
//     }
//
//     fprintf(file_ptr, "}");

    /*-------------------------Inorder--------------------------*/

    if (node->left != NULL)
    {
        WriteNode(node->left, file_ptr);
    }

    switch(node->type)
    {
        case OP:    printf("{\"%c\"}", node->value.oper);
                    fprintf(file_ptr, "(\"%c\")", node->value.oper);
                    break;

        case NUM:   fprintf(file_ptr, "(\"%d\")", node->value.oper_number);
                    break;

        case VAR:   if (IsEqual(node->value.number, 1)) {fprintf(file_ptr, "(\"x\")");}

                    else if (IsEqual(node->value.number, 2)) {fprintf(file_ptr, "(\"y\")");}
                    break;

        default:    printf("ERROR IN VALUE\n");

    }

    if (node->right != NULL)
    {
        WriteNode(node->right, file_ptr);
    }
}

void PrintEdge(FILE* graph, struct node_t* node)
{
    assert(graph);
    assert(node);

    if (node->left != NULL)
    {
        fprintf(graph, "    node%p: <f0> -> node%p [style = \"filled\"];\n",
                            node, node->left);
    }

    if (node->right != NULL)
    {
        fprintf(graph, "    node%p: <f1> -> node%p [style = \"filled\"];\n",
                            node, node->right);
    }
}
