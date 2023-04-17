#ifndef BST_H
#define BST_H

#define MAX(a,b) (a > b ? a : b)

typedef struct node {
    long v;
    struct node *l, *r;
} Node;

typedef Node *Tree;

int children(Tree root);
int height(Tree root);
Tree max(Tree root);
void insert(Tree *root, long v);
void rem(Tree *root, long v);

#endif