#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "tree.h"
#include "rb.h"
#include "types.h"

int main() {
    RB root = NULL;
    clock_t t;
    int op, val, changes, error;
    printf("elapsed_time\n");
    do {
        scanf("%d", &op);
        switch (op) {
            case 1:
                scanf("%d", &val);
                t = clock();
                rb.insert(&root, rb.create(types.Int(val)), &error);
                t = clock() - t;
                break;
            case 2:
                scanf("%d", &val);
                t = clock();
                rb.remove(&root, types.Int(val), types.copy);
                t = clock() - t;
                break;
            case 3:
                printf("%.15lf\n", ((double) t) / CLOCKS_PER_SEC);
        }
    } while (op != 0);
}