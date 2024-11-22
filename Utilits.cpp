#include <assert.h>
#include <stdlib.h>

#include "Differentiator.h"
#include "Dump.h"

void ClearBuffer()
{
    int symbol = getchar();
    while ((symbol != EOF) && (symbol != '\n'))
    {
        symbol = getchar();
    }
}

int CheckFile(FILE* file)
{
    if (file == NULL)
    {
        printf("file not opened\n");
        return -1;
    }

    return 0;
}

void CheckFclose(FILE* file_ptr)
{
    assert(file_ptr);

    //printf("%s", __func__);

    int fclose_return = fclose(file_ptr);

    if (fclose_return != 0)
    {
        printf("file closed wrong, fclose_return = %d\n", fclose_return);
    }
}

void ChangeSymbolInBuffer(struct file_t* buf_struct, size_t size_buffer, char symbol1, char symbol2)
{
    assert(buf_struct);
    assert(size_buffer > 0);

    for (unsigned long int i = 0; i < size_buffer; i++)
    {
        if (buf_struct->buffer[i] == symbol1)
        {
            buf_struct->buffer[i] = symbol2;
        }
    }
}

void MySystem(const char* str, int number_dump) //TODO rename
{
    char command_dot[153] = ""; //TODO magic const

    sprintf(command_dot, str, number_dump);

    system(command_dot);

}
