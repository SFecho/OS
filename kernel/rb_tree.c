#include <rb_tree.h>
#include <stddef.h>

rb_node *rb_get_parent(rb_node *node)
{
    if (node != NULL)
        return node->parent;
    return NULL;
}

void rb_set_parent(rb_node *node, rb_node *parent)
{
    if (node != NULL)
        node->parent = parent;
}

rb_node *rb_get_left(rb_node *node)
{
    if (node != NULL)
        return node->left;
    return NULL;
}

void rb_set_left(rb_node *node, rb_node *left)
{
    if (node != NULL)
        node->left = left;
}

rb_node *rb_get_right(rb_node *node)
{
    if (node != NULL)
        return node->right;
    return NULL;
}

void rb_set_right(rb_node *node, rb_node *right)
{
    if (node != NULL)
        node->right = right;
}

rb_color rb_get_color(rb_node *node)
{
    if (node != NULL)
        return node->color;
    return RB_BLACK;
}

void rb_set_color(rb_node *node, rb_color color)
{
    if (node != NULL)
        node->color = color;
}

static void left_rotate(rb_tree *tree, rb_node *node)
{
    if (node == NULL)
        return;

    rb_node *rotate_node = rb_get_right(node);
    rb_set_right(node, rb_get_left(rotate_node));

    if (rb_get_left(rotate_node) != NULL)
        rb_set_parent(rb_get_left(rotate_node), node);
    rb_set_left(rotate_node, node);

    if (rb_get_parent(node) == NULL)
        tree->root = rotate_node;
    else
    {
        if (rb_get_left(rb_get_parent(node)) == node)
            rb_set_left(rb_get_parent(node), rotate_node);
        else
            rb_set_right(rb_get_parent(node), rotate_node);
    }
    rb_set_parent(rotate_node, rb_get_parent(node));
    rb_set_parent(node, rotate_node);
}

static void right_rotate(rb_tree *tree, rb_node *node)
{
    if (node == NULL)
        return;
    rb_node *rotate_node = rb_get_left(node);

    rb_set_left(node, rb_get_right(rotate_node));

    if (rb_get_right(rotate_node) != NULL)
        rb_set_parent(rb_get_right(rotate_node), node);

    rb_set_right(rotate_node, node);

    if (rb_get_parent(node) == NULL)
        tree->root = rotate_node;
    else
    {
        if (rb_get_left(rb_get_parent(node)) == node)
            rb_set_left(rb_get_parent(node), rotate_node);
        else
            rb_set_right(rb_get_parent(node), rotate_node);
    }
    rb_set_parent(rotate_node, rb_get_parent(node));
    rb_set_parent(node, rotate_node);
}

void rb_insert_fixup(rb_tree *tree, rb_node *node)
{
    while (node != NULL && node != tree->root && rb_get_color(node->parent) == RB_RED)
    {
        rb_node *parent_node = rb_get_parent(node);
        rb_node *grandparent_node = rb_get_parent(parent_node);
        rb_node *uncle_node;
        if (rb_get_left(grandparent_node) == parent_node)
        {
            uncle_node = rb_get_right(grandparent_node);
            if (getColor(uncle_node) == RB_RED)
            {
                rb_set_color(uncle_node, RB_BLACK);
                rb_set_color(parent_node, RB_BLACK);
                rb_set_color(grandparent_node, RB_RED);
                node = grandparent_node;
            }
            else
            {
                if (node == rb_get_right(parent_node))
                {
                    node = parent_node;
                    left_rotate(tree, node);
                    parent_node = rb_get_parent(node);
                    grandparent_node = rb_get_parent(parent_node);
                }
                rb_set_color(parent_node, RB_BLACK);
                rb_set_color(grandparent_node, RB_RED);
                right_rotate(tree, grandparent_node);
            }
        }
        else
        {
            uncle_node = rb_get_left(grandparent_node);
            if (rb_get_color(uncle_node) == RB_RED)
            {
                rb_set_color(uncle_node, RB_BLACK);
                rb_set_color(parent_node, RB_BLACK);
                rb_set_color(grandparent_node, RB_RED);
                node = grandparent_node;
            }
            else
            {
                if (node == rb_get_left(parent_node))
                {
                    node = parent_node;
                    right_rotate(tree, node);
                    parent_node = rb_get_parent(node);
                    grandparent_node = rb_get_parent(parent_node);
                }
                rb_set_color(parent_node, RB_BLACK);
                rb_set_color(grandparent_node, RB_RED);
                left_rotate(tree, grandparent_node);
            }
        }
    }
    rb_set_color(tree->root, RB_BLACK);
}

void rb_delete_fixup(rb_tree *tree, rb_node *node)
{
    while (node != tree->root && rb_get_color(node) == RB_BLACK)
    {
        if (node == rb_get_left(rb_get_parent(node)))
        {
            rb_node *sib = rb_get_right(rb_get_parent(node));

            if (rb_get_color(sib) == RB_RED)
            {
                rb_set_color(sib, RB_BLACK);
                rb_set_color(rb_get_parent(node), RB_RED);
                left_rotate(tree, rb_get_parent(node));
                sib = rb_get_right(rb_get_parent(node));
            }

            if (rb_get_color(rb_get_left(sib)) == RB_BLACK &&
                rb_get_color(rb_get_right(sib)) == RB_BLACK)
            {
                rb_set_color(sib, RB_RED);
                node = rb_get_parent(node);
            }
            else
            {
                if (rb_get_color(rb_get_right(sib)) == RB_BLACK)
                {
                    rb_set_color(rb_get_left(sib), RB_BLACK);
                    rb_set_color(sib, RB_RED);
                    right_rotate(tree, sib);
                    sib = rb_get_right(rb_get_parent(node));
                }
                rb_set_color(sib, rb_get_color(rb_get_parent(node)));
                rb_set_color(rb_get_parent(node), RB_BLACK);
                rb_set_color(rb_get_right(sib), RB_BLACK);
                left_rotate(tree, rb_get_parent(node));
                node = tree->root;
            }
        }
        else
        {
            rb_node *sib = rb_get_left(rb_get_parent(node));

            if (rb_get_color(sib) == RB_RED)
            {
                rb_set_color(sib, RB_BLACK);
                rb_set_color(rb_get_parent(node), RB_RED);
                right_rotate(tree, rb_get_parent(node));
                sib = rb_get_left(rb_get_parent(node));
            }

            if (rb_get_color(rb_get_right(sib)) == RB_BLACK &&
                rb_get_color(rb_get_left(sib)) == RB_BLACK)
            {
                rb_set_color(sib, RB_RED);
                node = rb_get_parent(node);
            }
            else
            {
                if (rb_get_color(rb_get_left(sib)) == RB_BLACK)
                {
                    rb_set_color(rb_get_right(sib), RB_BLACK);
                    rb_set_color(sib, RB_RED);
                    left_rotate(tree, sib);
                    sib = rb_get_left(rb_get_parent(node));
                }
                rb_set_color(sib, rb_get_color(rb_get_parent(node)));
                rb_set_color(rb_get_parent(node), RB_BLACK);
                rb_set_color(rb_get_left(sib), RB_BLACK);
                right_rotate(tree, rb_get_parent(node));
                node = tree->root;
            }
        }
    }
    rb_set_color(node, RB_BLACK);
}
