#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "btree.h"
#include "queue.h"
#include "prng.h"

extern bnode* root;

int main()
{
    // root = create_node();
    srand(time(NULL));

    int count = 15;
    // int* array = get_random_numbers(count);
    int array[] = {13,3,5,9,12,14,15,7,1,4,6,8,2,10,11};

    for(int i = 0; i < count; ++i)
    {
        add_key(NULL, 0, array[i]);
        // printf("add %d:\n", array[i]);
        // print_node_all_dfs(root, 0);

        // print_node(root, 0);
        
        // if(root != NULL) print_node_bfs(root);
    }

    // print_node_dfs(root, 0);
    print_node_bfs(root);

    remove_key(NULL, 0, 8);
    print_node_bfs(root);
}