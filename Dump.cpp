#include <assert.h>
#include <string.h>

#include "Differentiator.h"
#include "Dump.h"

static const char* DUMP_DOT = "Dump.dot";
static FILE* tex_dump       = fopen("LaTeX/Dump_tex.tex", "w+");

/*-------------------------Graph Dump--------------------------*/

FILE* TreeDump(struct tree_t* tree, struct node_t* node, struct node_t* copy_node, dump_calls command)
{
    assert(node);
    assert(copy_node);

    FILE* dump_dot = fopen(DUMP_DOT, "w");

    PrintGraphHead(dump_dot);

    PrintDefaultList(dump_dot, node);

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
        _WRITE_TEX_ARTICLE_();

        number_tex_dump = 1;
    }

    LaTexDump(tree, node, copy_node, tex_dump, command);

    return tex_dump;
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

                PrintEdge(graph, node);

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

void LaTexDump(struct tree_t* tree, struct node_t* node, struct node_t* copy_node, FILE* dump, dump_calls command)
{
    assert(node);
    assert(dump);

    static int number_simplification = 1;
    static int final_dif             = 0;

    switch(command)
    {
        case DEFAULT_EXPR:
        {
                              fprintf(dump, "\\section{Expression}\n\\begin{center}\n\t"
                                            "\\text{In this simple example - function, that can solve my little brother\n\t"
                                            "I'll show you how works my Differentiator.}\n\t\\vspace{2ex}\n\n\t\\textit{$");

                              _WRITE_TEX_BRACKETS_(node, "f(x) = ", " ");

                              tree->expression = node;

                              break;
        }

        case FINAL_DERIVATIVE:
        {
                              fprintf(dump, "\\subsection{Final Derivative}\n\\begin{center}\n\t\\textit{$");

                              _WRITE_TEX_BRACKETS_(tree->expression, "(", ")' = ");

                              tree->derivative = node;

                              PrintExprInTex(node, dump);

                              final_dif = 1;

                              break;
        }

        case SIMPLIFICATION:
        {
                              const char* sentense = GetSentense(number_simplification);

                              number_simplification++;

                              fprintf(dump, "\\subsection{%s}\n\\begin{center}\n\t\\textit{$", sentense);

                              PrintExprInTex(copy_node, dump);

                              fprintf(dump, " = ");

                              PrintExprInTex(node, dump);

                              break;
        }

        case DIFFERENTIATION:
        {
                              const char* sentense = GetSentense(number_simplification);

                              number_simplification++;

                              fprintf(dump, "\\subsection{%s}\n\\begin{center}\n\t\\textit{$", sentense);

                              _WRITE_TEX_BRACKETS_(copy_node, "(", ")' = ");

                              PrintExprInTex(node, dump);

                              break;
        }

        default:              printf("\nERROR IN COMMAND FOR LATEX DUMP\n");
    }

    fprintf(dump, "$}\n\\end{center}\n");

    if (final_dif == 1)
    {
        fprintf(dump, "\\section{Result}\n\\begin{center}\n\t");

        _WRITE_TEX_BRACKETS_(tree->expression, "\\textit{$f(x) = ", "$}\n\t\\vspace{6ex}");

        fprintf(dump, "\n\n\t");

        _WRITE_TEX_BRACKETS_(tree->derivative, "\\textit{$f'(x) = ", "$}\n\\end{center}\n");

        fprintf(dump, "\\begin{center}\n\t\\vspace{12ex}\n\t\\textbf{\\Large{DERIVATIVE IS KILLED!!!}}\n\n\t"
                "\\vspace{2ex}\n\t\\textbf{Thank you for your attention!}\n\n\t\\vspace{2ex}\n\t\\textbf{BOTAITE!}\n\\end{center}\n");
    }
}

void PrintExprInTex(struct node_t* node, FILE* dump)
{
    assert(dump);

    if (CheckUnionType(node, OP))
    {
        const char* oper = WhatIsOperator(node->value.oper);

        if (strcmp(oper, "/") == 0)
        {
            _WRITE_TEX_BRACKETS_(node->left, "\\frac {", "}");

            _WRITE_TEX_BRACKETS_(node->right, "{", "} ");
        }

        else if ((strcmp(oper, "+") == 0) || (strcmp(oper, "-") == 0))
        {
            PrintExprInTex(node->left, dump);

            fprintf(dump, " %s ", oper);

            PrintExprInTex(node->right, dump);
        }

        else if (strcmp(oper, "*") == 0)
        {
            if (((CheckUnionType(node->left, NUM)) && (node->left->value.number < 0))
              || ((CheckUnionType(node->left, OP)) && ((node->left->value.oper_number == ADD)
                                                   || (node->left->value.oper_number == SUB))))
            {
                _WRITE_TEX_BRACKETS_(node->left, "(", ")");
            }

            else
            {
                PrintExprInTex(node->left, dump);
            }

            fprintf(dump, " \\cdot ");

            if (((CheckUnionType(node->right, NUM)) && (node->right->value.number < 0))
              || ((CheckUnionType(node->right, OP)) && ((node->right->value.oper_number == ADD)
                                                   || (node->right->value.oper_number == SUB))))
            {
                _WRITE_TEX_BRACKETS_(node->right, "(", ")");
            }

            else
            {
                PrintExprInTex(node->right, dump);
            }
        }

        else if (strcmp(oper, "^") == 0)
        {
            if ((CheckUnionType(node->left, OP)) && (node->left->value.oper_number != EXP) && (node->right->value.oper_number != DEG))
            {
                _WRITE_TEX_BRACKETS_(node->left, "(", ")");
            }

            else
            {
                _WRITE_TEX_BRACKETS_(node->left, "{", "}");
            }

            fprintf(dump, " %s ", oper);

            if ((CheckUnionType(node->right, OP)) && (node->right->value.oper_number != EXP) && (node->right->value.oper_number != DEG) && (node->right->value.oper_number != DIV))
            {
                _WRITE_TEX_BRACKETS_(node->right, "{(", ")}");
            }

            else
            {
                _WRITE_TEX_BRACKETS_(node->right, "{", "}");
            }
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

const char* GetSentense(int num)
{
    if (num > NUM_SENTENCES)
    {
        num = num % NUM_SENTENCES;
    }

    return sentences[num - 1];
}
