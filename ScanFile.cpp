#include <stdlib.h>
#include <assert.h>
#include <sys/stat.h>

#include "Differentiator.h"

static const char* EXPR_FILE = "Expression.txt";

size_t ScanFile(struct file_t* file_struct, struct expr_t* expr)
{
    assert(file_struct->file_ptr);

    //printf("%s\n", __func__);

    if (CheckFile(file_struct->file_ptr) != -1)
    {
        BufferCtor(file_struct, expr);

        size_t number_elems = fread(expr->string, 1, file_struct->file_size, file_struct->file_ptr);

        for (int i = 0; i < file_struct->file_size; i++)
        {
            printf("%c", expr->string[i]);
        }
        printf("\n");

        if (number_elems != file_struct->file_size)
        {
            printf("fread return = %lu, stat return = %lu\n", number_elems, file_struct->file_size);
        }

        ChangeSymbolInBuffer(expr->string, file_struct->file_size, '\r', '\0');

        return file_struct->file_size;
    }

    else
    {
        return SCAN_FILE_POISON;
    }
}

void BufferCtor(struct file_t* tree, struct expr_t* expr)
{
    assert(tree);

    //printf("%s\n", __func__);

    struct stat tree_file = {};

    stat(EXPR_FILE, &tree_file);

    //expr->string = (char*) calloc(tree_file.st_size, sizeof(char));

    tree->file_size = tree_file.st_size;

    printf("%lu\n", tree->file_size);
}

size_t SkipSpacesForPrint(struct file_t* tree, size_t index)
{
    assert(tree);
    assert(tree->buffer);

    //printf("%s\n", __func__);

    for (size_t i = index; i < tree->file_size; i++)
    {
        if ((tree->buffer[i] != ' ') && (tree->buffer[i] != '\0'))
        {
            return i;
        }
    }

    return NO_SPACE;
}

size_t FindNoSpace(struct file_t* tree, size_t index)
{
    assert(tree);
    assert(tree->buffer);

    //printf("%s\n", __func__);

    size_t shift = 0;

    if (tree->buffer[index] == '}'){index += 1;}

    while ((tree->buffer[index] == ' ') || (tree->buffer[index] == '\n') || (tree->buffer[index] == '\0'))
    {
        index++;
        shift++;
    }

    return shift;
}
