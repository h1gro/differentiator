#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/stat.h>

#include "Differentiator.h"

node_t* ReadTree(struct file_t* tree, struct tree_t* root)
{
    node_t* new_subtree = NULL;

    //printf("number_elems = %lu\nakin->index_buf = %lu\n", akin->file_size, akin->index_buf);

    for (size_t i = 0; i < tree->file_size; i++)
    {
        //if (SkipSpacesForPrint(tree, i) != i) {continue;}
        //printf("buffer[%lu] = %c\n", i, akin->buffer[i]);
        printf("%c", tree->buffer[i]);
    }

    int symbol = tree->buffer[tree->index_buf];

    if (symbol == '{')
    {
        char* value = (char*) calloc(10, sizeof(char));
        //printf("%p\n", word);
        sscanf(&tree->buffer[tree->index_buf], "{\"%[^\"]\"}\"", value);
//
//         printf("word = %s\n", word);
//         printf("index = %lu\n", akin->index_buf);

        switch(WhatTypeIs(value))
        {
            case VAR:
            {
                        CreateNode(VAR, 1, NULL, NULL, root);

                        break;
            }

            case NUM:
            {
                        double num = strtod((const char*)value, NULL);
                        CreateNode(NUM, (int)num, NULL, NULL, root);
                        break;
            }

            case OP:
            {           int oper = WhatIsOperEnum(value);
                        CreateNode(OP, oper, NULL, NULL, root);
                        break;
            }

            default:    printf("ERROR IN READ WORD\n");
        }

        free(value);

        symbol = 0;
        tree->index_buf++;
        //SkipSpacesToSymbols(akin, symbol, (int)'{', (int)'}', (int)EOF);

        while ((symbol != '{') && (symbol != '}') && (symbol != EOF))
        {
            symbol = tree->buffer[tree->index_buf];
            //printf("buffer[%lu] = %c\n", akin->index_buf, symbol);
            tree->index_buf++;
        }
        tree->index_buf--;
        //printf("index after first while = %lu\n", akin->index_buf);

        if (symbol == '}')
        {
            //SkipSpacesToSymbols(akin, symbol, (int)'{', (int)EOF, (int)';');
            while ((symbol != '{') && (symbol != EOF))
            {
                symbol = tree->buffer[tree->index_buf];
                //printf("buffer[%lu] = %c\n", akin->index_buf, symbol);
                tree->index_buf++;
            }
            tree->index_buf--;

            return new_subtree;
        }

        else
        {
            new_subtree->left  = ReadTree(tree, root);
            new_subtree->right = ReadTree(tree, root);
        }

        return new_subtree;
    }

    return NULL;

}

int WhatIsOperEnum(char* value)
{
    if (strcmp(value, "+") == 0)
    {
        return ADD;
    }

    if (strcmp(value, "-") == 0)
    {
        return SUB;
    }

    if (strcmp(value, "*") == 0)
    {
        return MULL;
    }

    if (strcmp(value, "/") == 0)
    {
        return DIV;
    }

    else
    {
        printf("ERROR IN OPERATOR\n");
    }
}
