#include <assert.h>
#include <string.h>

#include "Differentiator.h"
#include "Dump.h"

static const char* DUMP_DOT = "Dump.dot";
static FILE* tex_dump  = fopen("Dump_tex.txt", "w+");

/*-------------------------Graph Dump--------------------------*/

FILE* TreeDump(struct node_t* tree)
{
    assert(tree);

    FILE* dump_dot = fopen(DUMP_DOT, "w");

    PrintGraphHead(dump_dot);

    PrintDefaultList(dump_dot, tree);

    fprintf(dump_dot, "}\n");
    fclose(dump_dot);

    static int number_dump = 0;
    SystemWithIntArg("dot Dump.dot -Tpng -o Graphs/Dump%03d.png", number_dump);
    if (number_dump == 0)
    {
        fprintf(dump_html, "<pre>\n");
        number_dump = 1;
    }

    fprintf(dump_html, "\n<img src = \"Graphs/Dump%03d.png\"/>\n", number_dump);

    printf("\n");

    static int number_tex_dump = 0;
    if (number_tex_dump == 0)
    {
        // fprintf(tex_dump, "\\documentclass{article}\n\\newpage\n\\begin{document}\n"
        //                   "\\maketitle\n\\begin{titlepage}\n\\begin{center}\n\t"
        //                   "\\textsc{Differentiator by Komarov Artem}\n\\end{center}\n\\end{titlepage}\n");
        fprintf(tex_dump, "\\documentclass[a4paper,12pt]{article}\n\\usepackage{cmap}\n\\usepackage[T2A]{fontenc}\n\\usepackage[utf8]{inputenc}\n"
        "\\usepackage[english,russian]{babel}\n\\usepackage{graphicx}\n\\graphicspath{{noiseimages/}}\n"
        "\\usepackage{enumitem}\n\\date{}\n\n\n\\newtheorem{task}{Задача}\n\\begin{document}\n\\begin{titlepage}\n"
        "\\begin{center}\n\t\\textsc{MOSKOW INSTITUTE OF PHYSICS AND TECHNOLOGY}\n\t\\vspace{2ex}\n\n"
        "\\end{center}\n\\vspace{10ex}\n\\begin{center}\n\t\\vspace{24ex}\n"
        "\n\t\\vspace{2ex}\n\t\\textbf{\\Large{Differentiator}}\n\t\\vspace{34ex}\n"
        "\t\\begin{flushright}\n\t\\noindent\n\tDone by:\n\t\\textit{Komarov Artem}\n\t\\end{flushright}\n\t\\vfill\n\tDolgoprudny, 2024\n"
        "\\end{center}\n\\end{titlepage}\n\\newpage\n");

        number_tex_dump = 1;
    }

    LaTexDump(tree, tex_dump);

    return tex_dump;
    //fprintf(tex_dump, "\\end{document}\n");
}

void PrintGraphHead(FILE* graph)
{
    assert(graph);

    fprintf(graph, "digraph G\n{\n    rankdir = TB;\n    bgcolor = \"white\"\n");

}

void PrintDefaultList(FILE* graph, struct node_t* node)
{
    assert(graph);
    assert(node);

    printf("type = %d\n", node->type);

    switch(node->type)
    {
        case VAR:
        {
                fprintf(graph, "    node%p [shape = Mrecord, style = filled, color = \"#4682B4\", fillcolor = aquamarine3, label = \"{ x | type VAR | { <f0> left %4p | <f1> right %4p}}\"];\n",
                                node, node->left, node->right);
                break;
        }

        case NUM:
        {
                fprintf(graph, "    node%p [shape = Mrecord, style = filled, color = red, fillcolor = \"#F08080\", label = \"{ %lg | type NUM | { <f0> left %4p | <f1> right %4p}}\"];\n",
                               node, node->value.number, node->left, node->right);
                break;
        }

        case OP:
        {
                const char* oper = WhatIsOperator(node->value.oper);
                fprintf(graph, "    node%p [shape = Mrecord, style = filled, color = \"#006400\", fillcolor = \"#98FB98\", label = \"{ \'%s\' | type OP | { <f0> left %4p | <f1> right %4p}}\"];\n",
                               node, oper, node->left, node->right);
                printf("node = %p, node->type = %d, node->value.oper_number = %d, node->value.oper = %c\n", node, node->type, node->value.oper_number, node->value.oper);
                PrintEdge(graph, node);
                printf("done\n");
                break;
        }

        default:    printf("ERROR TYPE IN DUMP\n");
    }

    if (node->left != NULL) {PrintDefaultList(graph, node->left);}

    if (node->right != NULL) {PrintDefaultList(graph, node->right);}
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
        fprintf(graph, "    node%p: <f0> -> node%p [color = \"#696969\"];\n",
                            node, node->left);
    }

    if (node->right != NULL)
    {
        fprintf(graph, "    node%p: <f1> -> node%p [color = \"#696969\"];\n",
                            node, node->right);
    }
}

// void OpenFiles(struct dumps_t* dump)
// {
//     assert(dump);
//
//     dump->dump_dot = fopen(DUMP_DOT, "w");
//
//     assert(dump->dump_dot);
//
//     dump->dump->html = fopen();
// }

void LaTexDump(struct node_t* node, FILE* dump)
{
    fprintf(dump, "\\section{Expression}\n\\begin{center}\n\t\\textit{$");

    PrintExprInTex(node, dump);

    fprintf(dump, "$}\n\\end{center}\n");
}

void PrintExprInTex(struct node_t* node, FILE* dump)
{
    if (CheckUnionType(node, OP))
    {
        const char* oper = WhatIsOperator(node->value.oper);

        if (strcmp(oper, "/") == 0) //TODO можно дефайн
        {
            fprintf(dump, "\\frac {");

            PrintExprInTex(node->left, dump);

            fprintf(dump, "} {");

            PrintExprInTex(node->right, dump);

            fprintf(dump, "} ");
        }

        else if ((strcmp(oper, "+") == 0) || (strcmp(oper, "-") == 0))
        {
            PrintExprInTex(node->left, dump);

            fprintf(dump, " %s ", oper);

            PrintExprInTex(node->right, dump);
        }

        else if (strcmp(oper, "*") == 0)
        {
            PrintExprInTex(node->left, dump);

            fprintf(dump, " \\cdot ");

            PrintExprInTex(node->right, dump);
        }

        else if (strcmp(oper, "e") == 0)
        {
            fprintf(dump, "e");
        }

        else
        {
            fprintf(dump, "\\%s(", oper);

            PrintExprInTex(node->right, dump);

            fprintf(dump, ") ");
        }
    }

    else if (CheckUnionType(node, NUM))
    {
        fprintf(dump, " %lg ", node->value.number);
    }

    else if (CheckUnionType(node, VAR))
    {
        fprintf(dump, " x ");
    }
}
