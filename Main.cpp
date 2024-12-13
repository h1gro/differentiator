#include <stdlib.h>
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

    TreeDump(tree->first_node);

    Simplifier(tree->first_node, tree);

    TreeDump(tree->first_node);

    tree->first_node = Diffr(tree->first_node, tree);

    TreeDump(tree->first_node);

    Simplifier(tree->first_node, tree);

    TreeDump(tree->first_node);

    //NodsDtor(diffr_node);
    //TreeDtor(tree);

    for (int i = 0; i < tree->number_nods; i++)
    {
        printf("adr[%d] = %p\n", i, tree->addresses[i]);
    }

    DtorWithArray(tree);

    free(tree->addresses);
    free(tree);

    ExprDtor(&expression);

    return 0;
}
