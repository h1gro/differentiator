#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "Differentiator.h"
#include "RecursDown.h"
#include "Dump.h"

static const char* INPUT_FILE = "Input.txt";

int main()
{
    struct expr_t expression = {};
    struct file_t file_expr  = {};

    file_expr.file_ptr = fopen(INPUT_FILE, "r");

    ExprCtor(&expression);

    if (ScanFile(&file_expr, &expression) == SCAN_FILE_POISON)
    {
        return -1;
    }

    tree_t* tree = NULL;

    tree = TreeCtor(tree);

    tree->first_node = GetDollar(&expression, tree);

    node_t* copy_node00 = Copy(tree->first_node, tree);

    FILE* tex_dump0 = TreeDump(tree, tree->first_node, copy_node00, DEFAULT_EXPR);

    Simplifier(tree->first_node, tree);

    node_t* copy_node_dif = Copy(tree->first_node, tree);

    tree->first_node = Diffr(tree->first_node, tree);

    tex_dump0 = TreeDump(tree, tree->first_node, copy_node_dif, DIFFERENTIATION);

    node_t* copy_node = Copy(tree->first_node, tree);

    Simplifier(tree->first_node, tree);

    tex_dump0 = TreeDump(tree, tree->first_node, copy_node, FINAL_DERIVATIVE);

    fprintf(tex_dump0, "\\end{document}\n");

    /*adresses of nods*/
    // for (int i = 0; i < tree->number_nods; i++)
    // {
    //     printf("adr[%d] = %p\n", i, tree->addresses[i]);
    // }

    DtorWithArray(tree);

    free(tree->addresses);
    free(tree);

    ExprDtor(&expression);

    CheckFclose(dump_html);
    CheckFclose(tex_dump0);

    return 0;
}
