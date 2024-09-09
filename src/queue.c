#include <stdio.h>
#include <stdlib.h>

#include "queue.h"

int get_count(qarr _q)
{
    qdata* qdat = (qdata*)(_q - sizeof(qdata));
    return qdat->count;
}

int get_capacity(qarr _q)
{
    qdata* qdat = (qdata*)(_q - sizeof(qdata));
    return qdat->capacity;
}

qarr create_queue(int _capacity)
{
    int memSize = sizeof(qdata) + _capacity * sizeof(address_t*);
    qdata* qdat = (qdata*)malloc(memSize);

    qdat->capacity = _capacity;
    qdat->count = 0;
    qdat->memSize = memSize;
    qdat->ptrFront = 0;

    return (qarr)(qdat + 1);
}

qdata* grow(qdata* _qdatOld)
{
    qarr qNew = create_queue(_qdatOld->capacity * 2);
    qdata* qdatNew = (qdata*)(qNew - sizeof(qdata));

    qdatNew->count = _qdatOld->count;
    qdatNew->ptrFront = _qdatOld->capacity + _qdatOld->ptrFront;

    int* datOld = (int*)(_qdatOld + 1);
    int* datNew = (int*)qNew;

    for(int i = 0; i < _qdatOld->ptrFront; ++i)
        datNew[i] = datOld[i];

    for(int i = _qdatOld->ptrFront; i < _qdatOld->capacity; ++i)
        datNew[_qdatOld->capacity + i] = datOld[i];

    free(_qdatOld);

    return qdatNew;
}

void enqueue(qarr* _qAddress, address_t* _element)
{
    qdata* qdat = (qdata*)(*_qAddress - sizeof(qdata));

    if(qdat->count == qdat->capacity)
    {
        qdat = grow(qdat);
        *_qAddress = (qarr)(qdat + 1);
    }

    int appendIndex = (qdat->ptrFront + qdat->count) % (qdat->capacity);
    ((address_t*)*_qAddress)[appendIndex] = (address_t)_element;
    ++(qdat->count);
}

address_t* dequeue(qarr _q)
{
    qdata* qdat = (qdata*)(_q - sizeof(qdata));

    if(qdat->count == 0)
        return NULL;

    address_t dequeuedElement = ((address_t*)_q)[qdat->ptrFront];
    --(qdat->count);
    qdat->ptrFront = (qdat->ptrFront + 1) % (qdat->capacity);

    return (address_t*)dequeuedElement;
}

void print_qarr(qarr _q)
{
    qdata* qdat = (qdata*)(_q - sizeof(qdata));

    for(int i = 0; i < qdat->capacity; ++i)
    {
        if((i - qdat->ptrFront + qdat->capacity) % qdat->capacity < qdat->count)
            printf("[%08x] ", ((int*)_q)[i]);
        else
            printf("[--------] ");
    }

    printf("\n");
}