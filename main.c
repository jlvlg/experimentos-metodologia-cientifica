#include <stdio.h>
#include <time.h>
#include <math.h>

int main() {
    Tree root = NULL;
    clock_t t;
    int op;
    long val;
    printf("elapsed_time\n");
    do {
        scanf("%d", &op);
        switch (op) {
            case 1:
                scanf("%ld", &val);
                t = clock();
                insert(&root, val);
                t = clock() - t;
                break;
            case 2:
                scanf("%ld", &val);
                t = clock();
                rem(&root, val);
                t = clock() - t;
                break;
            case 3:
                printf("%.15lf\n", ((double) t) / CLOCKS_PER_SEC);
        }
    } while (op != 0);
}