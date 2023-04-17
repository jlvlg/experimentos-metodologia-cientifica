#include <stdlib.h>
#include <stdio.h>
#include "bst.h"

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

Tree _insert(Tree root, long v) {
    if (!root) {
        root = malloc(sizeof *root);
        if (!root) {
            printf("Program ran out of memory");
            exit(1);
        }
        root->v = v;
        root->l = root->r = NULL;
    } else {
        if (v > root->v)
            root->r = _insert(root->r, v);
        else
            root->l = _insert(root->l, v);
    }
    return root;
}

Tree _rem(Tree root, long v) {
    if (!root)
        return root;
    if (v == root->v) {
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
                root->l = _rem(root->l, v);
        }
    }
    return root;
}

void insert(Tree *root, long v) {
    *root = _insert(*root, v);
}

void rem(Tree *root, long v) {
    *root = _rem(*root, v);
}