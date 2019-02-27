#ifndef _RBTREE_H_
#define _RBTREE_H_

#include <types.h>

typedef struct rb_node rb_node;
typedef struct rb_tree rb_tree;
typedef struct rb_iterator rb_iterator;
typedef enum rb_color rb_color;

enum rb_color
{
    RB_RED,
    RB_BLACK
};

struct rb_node
{
    rb_node *left;
    rb_node *right;
    rb_node *parent;
    rb_color color;
};

struct rb_tree
{
    rb_node *root;
    int size;
};

struct rb_iterator
{
    rb_node *itr_ptr;
};

rb_node *rb_get_parent(rb_node *node);
void rb_set_parent(rb_node *node, rb_node *parent);

rb_node *rb_get_left(rb_node *node);
void rb_set_left(rb_node *node, rb_node *left);

rb_node *rb_get_right(rb_node *node);
void rb_set_right(rb_node *node, rb_node *right);

rb_color rb_get_color(rb_node *node);
void rb_set_color(rb_node *node, rb_color color);

void rb_insert_fixup(rb_tree *tree, rb_node *node);
void rb_delete_fixup(rb_tree *tree, rb_node *node, rb_node *parent);

rb_node *rb_delete(rb_tree *tree, rb_node *del_node);

#endif