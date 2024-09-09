#include <stdio.h>
#include <stdlib.h>

#include "btree.h"
#include "queue.h"

bnode* root;

bnode* create_node_pure()
{
    bnode* node = (bnode*)malloc(sizeof(bnode));

    node->isLeafNode = 1;
    node->keyCount = 0;

    for(int i = 0; i <= COUNT_MAX_CHILDREN; ++i)
        node->children[i] = NULL;

    return node;
}

bnode* create_node(int _initKey)
{
    bnode* node = create_node_pure();

    node->keys[0] = _initKey;
    node->keyCount = 1;

    return node;
}

void insert_key(bnode* _node, int _key)
{
    int insertionIndex = 0;

    while(insertionIndex < _node->keyCount && _key > _node->keys[insertionIndex])
        ++insertionIndex;

    for(int i = _node->keyCount - 1; i >= insertionIndex; --i)
    {
        _node->keys[i + 1] = _node->keys[i];
        _node->children[i + 2] = _node->children[i + 1];
    }

    _node->keys[insertionIndex] = _key;
    ++(_node->keyCount);
}

void add_key(bnode* _parent, int _idxParent, int _key)
{
    bnode* current = root;

    if(current == NULL)
    {
        root = create_node(_key);
        return;
    }
    else if(_parent != NULL)
        current = _parent->children[_idxParent];

    // NOTE: 1. 깊이 우선 탐색 및 키 삽입
    if(current->isLeafNode)
        insert_key(current, _key);
    else
    {
        int nextParentIndex = 0;

        while(nextParentIndex < current->keyCount && _key > current->keys[nextParentIndex])
            ++nextParentIndex;

        add_key(current, nextParentIndex, _key);
    }

    // NOTE: 2. 상향식 노드 분할
    bnode* splitedNode = split_node(_parent, current, _idxParent, _key);

    if(splitedNode != NULL)
        root = splitedNode;
}

bnode* split_node(bnode* _parent, bnode* _current, int _idxParent, int _key)
{
    if(_current->keyCount < COUNT_MAX_CHILDREN)
        return _parent;

    int mid = _current->keyCount / 2;
    bnode* splitedNode = create_node(_current->keys[mid + 1]);
    splitedNode->isLeafNode = _current->isLeafNode;

    if(_parent == NULL)
    {
        _parent = create_node_pure();
        _parent->isLeafNode = 0;
        _parent->children[0] = _current;
    }

    for(int i = _parent->keyCount - 1; i >= _idxParent; --i)
    {
        _parent->keys[i + 1] = _parent->keys[i];
        _parent->children[i + 2] = _parent->children[i + 1];
    }

    _parent->children[_idxParent + 1] = splitedNode;
    _parent->keys[_idxParent] = _current->keys[mid];
    ++(_parent->keyCount);

    for(int i = mid + 1; i <= _current->keyCount; ++i)
    {
        splitedNode->keys[i - mid - 1] = _current->keys[i];
        splitedNode->children[i - mid - 1] = _current->children[i];
    }

    splitedNode->keyCount = _current->keyCount = mid - 1;
    splitedNode->children[splitedNode->keyCount] = _current->children[_current->keyCount];
    _current->keyCount = mid;

    return _parent;
}

void remove_key(bnode* _parent, int _key)
{

}

void print_node(bnode* _node)
{
    printf("[");
    for(int i = 0; i < _node->keyCount; ++i)
    {
        printf("%d", _node->keys[i]);
        if(i < _node->keyCount - 1)
            printf(",");
    }
    printf("]");
}

void print_indent(int _indent)
{
    for(int i = 0; i < _indent; ++i)
        printf("  ");
}

void print_node_dfs(bnode* _node, int _indent)
{
    if(_node == NULL)
        return;

    print_indent(_indent);
    printf("- ");
    print_node(_node);
    printf("\n");

    for(int i = 0; i <= _node->keyCount; ++i)
        print_node_dfs(_node->children[i], _indent + 1);
}

void print_node_bfs(bnode* _root)
{
    qarr q = create_queue(16);
    enqueue(&q, (void*)_root);
    enqueue(&q, NULL);

    while(get_count(q) > 0)
    {
        void* dequeuedValue = dequeue(q);

        if(dequeuedValue == NULL)
        {
            printf("\n");

            if(get_count(q) > 0)
                enqueue(&q, NULL);
        }
        else if(dequeuedValue == ((void*)1))
            printf("\t");
        else
        {
            bnode* node = (bnode*)dequeuedValue;
            print_node(node);

            if(node->isLeafNode)
                continue;

            for(int i = 0; i <= node->keyCount; ++i)
                enqueue(&q, (void*)node->children[i]);

            enqueue(&q, ((void*)1));
        }
    }
}
