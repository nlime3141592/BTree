#ifndef QUEUE_H
#define QUEUE_H

#ifdef x86
typedef int address_t;
#elif x64
typedef long long address_t;
#else
typedef int address_t;
#endif

typedef struct _qdata qdata;
typedef char* qarr;

struct _qdata
{
    int capacity;
    int count;
    int memSize;
    int ptrFront;
};

int get_count(qarr _q);
int get_capacity(qarr _q);

qarr create_queue(int _capacity);
void enqueue(qarr* _q, address_t* _element);
address_t* dequeue(qarr _q);

void print_qarr(qarr _q);

#endif