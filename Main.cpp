#include <stdio.h>
#include <stdlib.h>

#include "Differentiator.h"
#include "Dump.h"

const char* TREE_FILE = "Tree.txt";

int main()
{
    tree_t* root = NULL;
    struct file_t tree = {};

    root = TreeCtor(root);

    tree.file_ptr = fopen(TREE_FILE, "a+");

    if (ScanFile(&tree) == SCAN_FILE_POISON)
    {
        return -1;
    }

    ReadTree(&tree, root);
    // node_t* arg1      = CreateNode(NUM, 2, NULL, NULL, root);
    // node_t* arg2      = CreateNode(VAR, 1, NULL, NULL, root);
    // node_t* oper_plus = CreateNode(OP, ADD, arg1, arg2, root);

    //root->first_node = oper_plus;

    //WriteTree(oper_plus, tree.file_ptr);
    TreeDump(root->first_node);

    CheckFclose(tree.file_ptr);
    TreeDtor(root);
    free(tree.buffer);
    return 0;
}
