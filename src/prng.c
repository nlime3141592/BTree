#include <stdlib.h>

#include "prng.h"

int* get_random_numbers(int _count)
{
    int* array = (int*)malloc(sizeof(int) * _count);

    for(int i = 0; i < _count; ++i)
        array[i] = i + 1;

    for(int i = 0; i < _count - 1; ++i)
    {
        int j = rand() % (_count - i - 1) + i + 1;

        int tmp = array[i];
        array[i] = array[j];
        array[j] = tmp;
    }

    return array;
}