#include <stdlib.h>
#include <stdio.h>
#include "avl.h"

int children(Tree root) {
    if (root->l && root->r)
        return 2;
    return root->l || root->r;
}

int height(Tree root) {
    if (!root)
        return 0;
    return MAX(height(root->l), height(root->r)) + 1;
}

Tree max(Tree root) {
    if (!root || !root->r)
        return root;
    return max(root->r);
}

Tree simple_rotation_left(Tree p) {
    Tree u = p->r;
    p->r = u->l;
    u->l = p;
    return u;
}

Tree simple_rotation_right(Tree p) {
    Tree u = p->l;
    p->l = u->r;
    u->r = p;
    return u;
}

Tree double_rotation_left(Tree p) {
    p->r = simple_rotation_right(p->r);
    return simple_rotation_left(p);
}

Tree double_rotation_right(Tree p) {
    p->l = simple_rotation_left(p->l);
    return simple_rotation_right(p);
}

Tree balance(Tree root) {
    if (root->bf > 0) {
        if (root->r->bf < 0) {
            root = double_rotation_left(root);
        } else {
            root = simple_rotation_left(root);

            root->l->bf = root->bf == 0 ? 1 : 0;
            root->bf = root->bf == 0 ? -1 : 0;
            
            return root;
        }
    } else {
        if (root->l->bf > 0) {
            root = double_rotation_right(root);
        } else {
            root = simple_rotation_right(root);

            root->r->bf = root->bf == 0 ? -1 : 0;
            root->bf = root->bf == 0 ? 1 : 0;

            return root;
        }
    }
    root->l->bf = root->bf == 1 ? -1 : 0;
    root->r->bf = root->bf == -1 ? 1 : 0;
    root->bf = 0;
    return root;
}

Tree _insert(Tree root, int v, int *changes) {
    if (!root) {
        root = malloc(sizeof *root);
        if (!root) {
            printf("Program ran out of memory");
            exit(1);
        }
        root->bf = 0;
        root->v = v;
        root->l = root->r = NULL;
        *changes = 1;
        return root;
    }

    int right = v > root->v;

    if (right)
        root->r = _insert(root->r, v, changes);
    else
        root->l = _insert(root->l, v, changes);

    if (*changes)
        *changes = (right ? root->bf++ : root->bf--) == 0;

    if (abs(root->bf) > 1)
        return balance(root);
    return root;
}

Tree _rem(Tree root, int v, int *changes) {
    if (!root)
        return root;

    int right = v > root->v;

    if (v == root->v) {
        *changes = 1;
        switch (children(root)) {
            case 0:
                free(root);
                return NULL;
            case 1: {
                Tree child = root->l != NULL ? root->l : root->r;
                free(root);
                return child;
            }
            case 2:
                root->v = max(root->l)->v;
                root->l = _rem(root->l, v, changes);
        }
    } else {
        if (right)
            root->r = _rem(root->r, v, changes);
        else
            root->l = _rem(root->l, v, changes);
    }

    if (*changes)
        *changes = abs(right ? root->bf-- : root->bf++) == 1; 

    if (abs(root->bf) > 1) {
        if (!(right ? root->l : root->r)->bf)
            *changes = 0;
        return balance(root);
    }
    return root;
}

void insert(Tree *root, int v) {
    int changes = 0;
    *root = _insert(*root, v, &changes);
}

void rem(Tree *root, int v) {
    int changes = 0;
    *root = _rem(*root, v, &changes);
}