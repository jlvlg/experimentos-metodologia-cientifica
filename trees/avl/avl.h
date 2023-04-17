#ifndef AVL_H
#define AVL_H

#define MAX(a,b) (a > b ? a : b)

typedef struct node {
    int v, bf;
    struct node *l, *r;
} Node;

typedef Node *Tree;

int children(Tree root);
int height(Tree root);
Tree max(Tree root);
Tree simple_rotation_left(Tree p);
Tree simple_rotation_right(Tree p);
Tree double_rotation_left(Tree p);
Tree double_rotation_right(Tree p);
Tree balance(Tree root);
void insert(Tree *root, int v);
void rem(Tree *root, int v);

#endif