#include <assert.h>
#include <stdlib.h>
#include <math.h>

#include "Differentiator.h"

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

void ChangeSymbolInBuffer(char* buffer, size_t size_buffer, char symbol1, char symbol2)
{
    assert(buffer);
    assert(size_buffer > 0);

    for (unsigned long int i = 0; i < size_buffer; i++)
    {
        if (buffer[i] == symbol1)
        {
            buffer[i] = symbol2;
        }
    }
}

void SystemWithIntArg(const char* str, int number_dump)
{
    char command_dot[SIZE_COMMAND] = "";

    sprintf(command_dot, str, number_dump);

    system(command_dot);
}

int IsEqual(double elem1, double elem2)
{
    return (fabs(elem1 - elem2) <= EPSILON);
}
