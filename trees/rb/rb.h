#ifndef RB_H
#define RB_H

#define MAX(a,b) (a > b ? a : b)

typedef struct node {
    int v, is_black;
    struct node *l, *r, *parent;
} Node;

typedef Node *Tree;

int children(Tree root);
int height(Tree root);
Tree max(Tree root);
void rotate(Tree *root, Tree p, Tree u, Tree t, Tree rotate_function(Tree node));
Tree simple_rotation_left_helper(Tree p);
Tree simple_rotation_right_helper(Tree p);
void simple_rotation_left(Tree *root, Tree p);
void simple_rotation_right(Tree *root, Tree p);
void double_rotation_left(Tree *root, Tree p);
void double_rotation_right(Tree *root, Tree p);
void insert(Tree *root, int val);
void rem(Tree *root, int val);

#endif