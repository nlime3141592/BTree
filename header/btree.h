#ifndef BTREE_H
#define BTREE_H

#define COUNT_MAX_CHILDREN 4 // NOTE: should be greater than 2.
#define COUNT_MIN_CHILDREN ((COUNT_MAX_CHILDREN + 1) / 2)

typedef struct _bnode bnode;

struct _bnode
{
    int isLeafNode;
    int keyCount;
    int keys[COUNT_MAX_CHILDREN];
    bnode* children[COUNT_MAX_CHILDREN + 1];
};

bnode* create_node_pure();
bnode* create_node(int _initKey);

void add_key(bnode* _parent, int _idxParent, int _key);
void remove_key(bnode* _parent, int _key);
bnode* split_node(bnode* _parent, bnode* _current, int _idxParent, int _key);

void print_node(bnode* _node);
void print_node_dfs(bnode* _node, int _indent);
void print_node_bfs(bnode* _root);

#endif