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
    splitedNode->children[splitedNode->keyCount + 1] = _current->children[_current->keyCount];
    _current->keyCount = mid;

    return _parent;
}

int remove_key(bnode* _parent, int _idxParent, int _key)
{
    int i = -1;

    bnode* current = root;

    if(_parent != NULL)
        current = _parent->children[_idxParent];
    if(current == NULL)
        return 0; // NOTE: 정의되지 않은 노드에 대해 연산을 시도함

    while(1)
    {
        ++i;

        if(!current->isLeafNode) // NOTE: 자식 노드에서 키를 찾음
            return remove_key(current->children[i], i, _key);
        else if(i >= current->keyCount)
            return 0;
        else if(current->keys[i] > _key)
            return 0; // NOTE: 탐색할 필요 없음
        else if(current->keys[i] == _key) // NOTE: 현재 노드에서 키를 찾음
        {
            if(current->isLeafNode)
            {
                if(current->keyCount >= COUNT_MIN_CHILDREN)
                {
                    for(int j = i + 1; j < current->keyCount; ++j)
                        current->children[j] = current->children[j + 1];

                    --(current->keyCount);

                    return 1;
                }
                else // NOTE: 리프 노드에서 키 하나를 빼면 리프 노드가 불안정 상태에 빠짐
                {
                    if(_idxParent < _parent->keyCount && _parent->children[_idxParent + 1]->keyCount >= COUNT_MIN_CHILDREN)
                    {
                        // NOTE: 오른쪽 형제 노드에서 당겨올 수 있음
                        for(int j = i + 1; j < current->keyCount; ++j)
                            current->keys[j - 1] = current->keys[j];

                        current->keys[current->keyCount - 1] = _parent->keys[_idxParent];
                        _parent->keys[_idxParent] = _parent->children[_idxParent + 1]->keys[0];

                        for(int j = 1; j < _parent->children[_idxParent + 1]->keyCount; ++j)
                            _parent->children[_idxParent + 1]->keys[j - 1] = _parent->children[_idxParent + 1]->keys[j];

                        --(_parent->children[_idxParent + 1]->keyCount);

                        return 1;
                    }
                    else if(_idxParent > 0 && _parent->children[_idxParent - 1]->keyCount >= COUNT_MIN_CHILDREN)
                    {
                        // NOTE: 왼쪽 형제 노드에서 당겨올 수 있음
                        for(int j = i; j > 0; --j)
                            current->keys[j] = current->keys[j - 1];

                        current->keys[0] = _parent->keys[_idxParent - 1];
                        _parent->keys[_idxParent - 1] = _parent->children[_idxParent - 1]->keys[_parent->children[_idxParent - 1]->keyCount - 1];

                        --(_parent->children[_idxParent - 1]->keyCount);

                        return 1;
                    }
                    else
                    {
                        // NOTE: 형제 노드에서 어떤 값도 당겨올 수 없음
                    }
                }
            }
            else if(current->children[i]->keyCount < COUNT_MIN_CHILDREN && current->children[i + 1]->keyCount < COUNT_MIN_CHILDREN) // NOTE: 어떤 자식을 끌어올려와도 자식이 불안정 상태에 빠짐
            {
                
            }
            else if(current->children[i]->keyCount >= COUNT_MIN_CHILDREN) // NOTE: 왼쪽 자식을 당겨올 수 있음
            {
                bnode* leaf = current->children[i];

                while(!leaf->isLeafNode)
                    leaf = leaf->children[leaf->keyCount];

                current->keys[i] = leaf->keys[leaf->keyCount - 1];
                --(leaf->keyCount);

                return 1;
            }
            else // NOTE: 오른쪽 자식을 당겨올 수 있음
            {
                bnode* leaf = current->children[i + 1];

                while(!leaf->isLeafNode)
                    leaf = leaf->children[0];

                current->keys[i] = leaf->keys[0];
                --(leaf->keyCount);

                return 1;
            }
        }
    }
}

void print_node(bnode* _node)
{
    printf("%c", _node->isLeafNode ? '[' : '{');
    for(int i = 0; i < _node->keyCount; ++i)
    {
        printf("%d", _node->keys[i]);
        if(i < _node->keyCount - 1)
            printf(",");
    }
    printf("%c", _node->isLeafNode ? ']' : '}');
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
