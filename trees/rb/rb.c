#include <stdlib.h>
#include <stdio.h>
#include "rb.h"

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

Tree parent(Tree node) {
    return node ? node->parent : NULL;
}

Tree grandparent(Tree node) {
    return parent(parent(node));
}

int is_black(Tree node) {
    return !node || node->is_black;
}

int is_red(Tree node) {
    return !is_black(node);
}

int is_root(Tree node) {
    return !parent(node);
}

int is_left(Tree node) {
    return !is_root(node) && node->parent->l == node;
}

Tree sibling(Tree node) {
    if (node && !is_root(node)) {
        Tree tree_parent = parent(node);
        return is_left(node) ? tree_parent->r : tree_parent->l;
    }
    return NULL;
}

Tree uncle(Tree node) {
    return sibling(parent(node));
}

Tree closest_nephew(Tree node) {
    Tree sibling_tree = sibling(node);
    return is_left(node) ? sibling_tree->l : sibling_tree->r;
}

Tree farthest_nephew(Tree node) {
    Tree sibling_tree = sibling(node);
    return is_left(node) ? sibling_tree->r : sibling_tree->l;
}

void redden(Tree node) {
    node->is_black = 0;
}

void blacken(Tree node) {
    node->is_black = 1;
}

void update_parents(Tree *root, Tree node, Tree new, int left) {
    if (new) {
        new->parent = node->parent;
    }
    if (is_root(node)) {
        *root = new;
    } else if (left)  {
        node->parent->l = new;
    } else {
        node->parent->r = new;
    }
}

Tree simple_rotation_left_helper(Tree p) {
    Tree u = p->r;
    p->r = u->l;
    u->l = p;
    return u;
}

Tree simple_rotation_right_helper(Tree p) {
    Tree u = p->l;
    p->l = u->r;
    u->r = p;
    return u;
}

void rotate(Tree *root, Tree p, Tree u, Tree t, Tree rotate_function(Tree node)) {
    int left = is_left(p);

    u->parent = p->parent;
    p->parent = u;
    if (t)
        t->parent = p;
    
    p = rotate_function(p);

    update_parents(root, p, p, left);
}

void simple_rotation_left(Tree *root, Tree p) {
    Tree u = p->r;
    Tree t = u->l;

    rotate(root, p, u, t, simple_rotation_left_helper);
}

void simple_rotation_right(Tree *root, Tree p) {
    Tree u = p->l;
    Tree t = u->r;

    rotate(root, p, u, t, simple_rotation_right_helper);
}

void double_rotation_right(Tree *root, Tree p) {
    simple_rotation_left(root, p->l);
    simple_rotation_right(root, p);
}

void double_rotation_left(Tree *root, Tree p) {
    simple_rotation_right(root, p->r);
    simple_rotation_left(root, p);
}

void adjust(Tree *root, Tree leaf) {
    while (is_red(leaf) && is_red(parent(leaf))) {
        if (is_red(uncle(leaf))) {
            blacken(uncle(leaf));
            blacken(parent(leaf));
            redden(grandparent(leaf));
            leaf = grandparent(leaf);
            continue;
        }
        if (is_left(leaf)) {
            if (is_left(parent(leaf))) {
                simple_rotation_right(root, grandparent(leaf));
                blacken(parent(leaf));
                redden(sibling(leaf));
            } else {
                double_rotation_left(root, grandparent(leaf));
                blacken(leaf);
                redden(leaf->l);
            }
        } else {
            if (!is_left(parent(leaf))) {
                simple_rotation_left(root, grandparent(leaf));
                blacken(parent(leaf));
                redden(sibling(leaf));
            } else {
                double_rotation_right(root, grandparent(leaf));
                blacken(leaf);
                redden(leaf->r);
            }
        }
    }
    blacken(*root);
}

void remove_double_black(Tree *root, Tree node, int nil) {
    if (nil) {
        update_parents(root, node, NULL, is_left(node));
        free(node);
    }
}

void handle_double_black(Tree *root, Tree node, int nil) {
    if (is_root(node)) {
        remove_double_black(root, node, nil);
        return;
    }
    int left = is_left(node);
    Tree parent_node = parent(node);
    Tree grandparent_node = parent(parent_node);
    Tree sibling_node = sibling(node);
    Tree closest_nephew_node = closest_nephew(node);
    Tree farthest_nephew_node = farthest_nephew(node);
    if (is_black(parent_node) &&
        is_red(sibling_node) &&
        is_black(closest_nephew_node) &&
        is_black(farthest_nephew_node)) {
            redden(parent_node);
            blacken(sibling_node);
            if (left) {
                simple_rotation_left(root, parent_node);
            } else {
                simple_rotation_right(root, parent_node);
            }
            handle_double_black(root, node, nil);
            return;
        }
    if (is_black(parent_node) &&
        is_black(sibling_node) &&
        is_black(closest_nephew_node) &&
        is_black(farthest_nephew_node)) {
            remove_double_black(root, node, nil);
            redden(sibling_node);
            handle_double_black(root, parent_node, 0);
            return;
        }
    if (is_red(parent_node) &&
        is_black(sibling_node) &&
        is_black(closest_nephew_node) &&
        is_black(farthest_nephew_node)) {
            blacken(parent_node);
            redden(sibling_node);
            remove_double_black(root, node, nil);
            return;
        }
    if (is_black(sibling_node) &&
        is_red(closest_nephew_node) &&
        is_black(farthest_nephew_node)) {
            redden(sibling_node);
            blacken(closest_nephew_node);
            if (left) {
                simple_rotation_right(root, sibling_node);
            } else {
                simple_rotation_left(root, sibling_node);
            }
            handle_double_black(root, node, nil);
            return;
        }
    if (is_black(sibling_node) &&
        is_red(farthest_nephew_node)) {
            sibling_node->is_black = parent_node->is_black;
            blacken(farthest_nephew_node);
            blacken(parent_node);
            remove_double_black(root, node, nil);
            if (left) {
                simple_rotation_left(root, parent_node);
            } else {
                simple_rotation_right(root, parent_node);
            }
            return;
        }
}

void insert(Tree *root, int val) {
    int cmp;
    Tree track = NULL;
    
    for (Tree search = *root; search;) {
        track = search;
        cmp = val > track->v;

        if (cmp)
            search = search->r;
        else
            search = search->l;
    }

    Tree leaf = malloc(sizeof(Node));
    if (!leaf) {
        printf("Program ran out of memory");
        exit(1);
    }
    leaf->is_black = 0;
    leaf->l = leaf->r = NULL;
    leaf->parent = track;

    if (!track) {
        *root = leaf;
    } else {
        if (cmp)
            track->r = leaf;
        else
            track->l = leaf;
    }
    
    adjust(root, leaf);
}

void rem(Tree *root, int val) {
    for (Tree search = *root; search;) {
        if (val == search->v) {
            switch (children(search)) {
                case 0:
                    if (is_root(search) || is_red(search)) {
                        update_parents(root, search, NULL, is_left(search));
                        free(search);
                    } else {
                        handle_double_black(root, search, 1);
                    }
                    break;
                case 1: {
                    Tree child = (search->l ? search->l : search->r);
                    blacken(child);
                    update_parents(root, search, child, is_left(search));
                    free(search);
                } break;
                case 2: {
                    int child = max(search->l)->v;
                    rem(root, child);
                    search->v = child;
                }
            }
        } else {
            if (val > search->v)
                search = search->r;
            else
                search = search->l;
        }
    }
}