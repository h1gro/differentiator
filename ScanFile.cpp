#include <stdlib.h>
#include <assert.h>
#include <sys/stat.h>

#include "Differentiator.h"

static const char* EXPR_FILE = "Input.txt";

size_t ScanFile(struct file_t* file_struct, struct expr_t* expr)
{
    assert(file_struct->file_ptr);

    if (CheckFile(file_struct->file_ptr) == CHECK_FILE_ERROR)
    {
        return SCAN_FILE_POISON;
    }

    FileSize(file_struct, expr);

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

void FileSize(struct file_t* file, struct expr_t* expr)
{
    assert(file);

    struct stat input = {};

    stat(EXPR_FILE, &input);

    file->file_size = input.st_size;

    printf("%lu\n", file->file_size);
}

size_t SkipSpacesForPrint(struct file_t* tree, size_t index)
{
    assert(tree);
    assert(tree->buffer);

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

    size_t shift = 0;

    if (tree->buffer[index] == '}'){index += 1;}

    while ((tree->buffer[index] == ' ') || (tree->buffer[index] == '\n') || (tree->buffer[index] == '\0'))
    {
        index++;
        shift++;
    }

    return shift;
}
