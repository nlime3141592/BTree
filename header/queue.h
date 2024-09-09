#ifndef QUEUE_H
#define QUEUE_H

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
void enqueue(qarr* _q, void* _element);
void* dequeue(qarr _q);

void print_qarr(qarr _q);

#endif