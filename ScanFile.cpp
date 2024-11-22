#include <stdlib.h>
#include <assert.h>
#include <sys/stat.h>

#include "Differentiator.h"

static const char* TREE_FILE = "Tree.txt";

size_t ScanFile(struct file_t* file_struct)
{
    assert(file_struct->file_ptr);

    //printf("%s\n", __func__);

    if (CheckFile(file_struct->file_ptr) != -1)
    {
        char* file_struct_buffer = BufferCtor(file_struct);

        size_t number_elems = fread(file_struct_buffer, 1, file_struct->file_size, file_struct->file_ptr);

        if (number_elems != file_struct->file_size)
        {
            printf("fread return = %lu, stat return = %lu\n", number_elems, file_struct->file_size);
        }

        ChangeSymbolInBuffer(file_struct, file_struct->file_size, '\r', '\0');

        return file_struct->file_size;
    }

    else
    {
        return SCAN_FILE_POISON;
    }
}

char* BufferCtor(struct file_t* tree)
{
    assert(tree);

    //printf("%s\n", __func__);

    struct stat tree_file = {};

    stat(TREE_FILE, &tree_file);

    tree->buffer = (char*) calloc(tree_file.st_size, sizeof(char));

    tree->file_size = tree_file.st_size;

    printf("%lu\n", tree->file_size);

    assert(tree->buffer);

    return tree->buffer;
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
