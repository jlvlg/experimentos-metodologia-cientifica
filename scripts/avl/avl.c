#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "tree.h"
#include "avl.h"
#include "types.h"

int main() {
    AVL root = NULL;
    clock_t t;
    int op, val, changes, error;
    printf("elapsed_time\n");
    do {
        scanf("%d", &op);
        switch (op) {
            case 1:
                scanf("%d", &val);
                t = clock();
                root = avl.insert(root, avl.create(types.Int(val)), &changes, &error);
                t = clock() - t;
                break;
            case 2:
                scanf("%d", &val);
                t = clock();
                root = avl.remove(root, types.Int(val), &changes, types.copy);
                t = clock() - t;
                break;
            case 3:
                printf("%.15lf\n", ((double) t) / CLOCKS_PER_SEC);
        }
    } while (op != 0);
}