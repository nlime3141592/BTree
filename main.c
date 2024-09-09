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
    int* array = get_random_numbers(count);
    // int array[] = {7,9,5,3,6,1,2,4,10,8};

    for(int i = 0; i < count; ++i)
    {
        add_key(NULL, 0, array[i]);
        printf("add %d:\n", array[i]);
        // print_node_all_dfs(root, 0);

        // print_node(root, 0);
        
        if(root != NULL)
            print_node_bfs(root);
    }

    print_node_dfs(root, 0);
    // print_node_bfs(root);
}