#include "rbtree.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  node_t *nil = (node_t *)calloc(1, sizeof(node_t));

  nil->color = RBTREE_BLACK;
  p->nil = nil;
  p->root = p->nil;

  return p;
}

void recursion_rbtree(rbtree *t, node_t *node)
{
  if (node->left != t->nil)
  recursion_rbtree(t, node->left);
  if (node->right != t->nil)
  recursion_rbtree(t, node->right);
  free(node);
}

void delete_rbtree(rbtree *t) {
  node_t *node = t->root;
  if (node != t->nil)
  {
    recursion_rbtree(t, node);
  }
  free(t->nil);
  free(t);
}

void rotate_left (rbtree *t, node_t *target)
{
  node_t *next = target->right;
  target->right = next->left;

  if (next->left != t->nil){
    next->left->parent = target;
  }
  next->parent = target->parent;

  if (target->parent == t->nil){
    t->root = next;
  } else if (target == target->parent->right){
    target->parent->right = next;
  } else {
    target->parent->left = next;
  }
  next->left = target;
  target->parent = next;

}


void rotate_right (rbtree *t, node_t *target) {
  node_t *next =target->left;
  target->left = next->right;

  if (next->right != t->nil){ 
    next->right->parent = target;
  }
  next->parent = target->parent; 

  if(target->parent == t->nil) { 
    t->root = next;
  } else if (target == target->parent->right) { 
    target->parent->right = next; 
  } else {  
    target->parent->left = next; 
  }
  next->right = target; 
  target->parent = next;
  
}

void check_and_correct(rbtree *t, node_t *new_node) {
  while (new_node->parent->color == RBTREE_RED) {
    if (new_node->parent == new_node->parent->parent->left) {
      node_t *uncle = new_node->parent->parent->right;
      if (uncle->color == RBTREE_RED) {
        new_node->parent->color = RBTREE_BLACK;
        uncle->color = RBTREE_BLACK;
        new_node->parent->parent->color = RBTREE_RED;
        new_node = new_node->parent->parent;
      } else {
       if (new_node == new_node->parent->right){
          new_node = new_node->parent;
          rotate_left(t, new_node);
        } 
        new_node->parent->color = RBTREE_BLACK;
        new_node->parent->parent->color = RBTREE_RED;
        rotate_right(t, new_node->parent->parent);
        
      }
    } else {
      node_t *uncle = new_node->parent->parent->left;
      if (uncle->color == RBTREE_RED) {
        new_node->parent->color = RBTREE_BLACK;
        uncle->color = RBTREE_BLACK;
        new_node->parent->parent->color = RBTREE_RED;
        new_node = new_node->parent->parent;
      } else {
        if (new_node == new_node->parent->left){
          new_node = new_node->parent;
          rotate_right(t, new_node);
        } 
        new_node->parent->color = RBTREE_BLACK;
        new_node->parent->parent->color = RBTREE_RED;
        rotate_left(t, new_node->parent->parent);
      }  
    }
  }
  t->root->color = RBTREE_BLACK;
}





node_t *rbtree_insert(rbtree *t, const key_t key) {
  node_t *new_node = (node_t *)calloc(1, sizeof(node_t));
  new_node->key = key;
  new_node->color = RBTREE_RED;
  new_node->left = new_node->right = t->nil;
  node_t *x = t->root;
  node_t *y = t->nil;
  while (x != t->nil) {
    y = x;
    if (new_node->key  <  x->key) {
      x = x->left; 
    } else {
      x = x->right; 
    }
  }

  new_node->parent = y;
  if ( y== t->nil) {
    t->root = new_node; 
  } else if (new_node->key < y->key) {
    y->left = new_node; 
  } else {
    y->right = new_node; 
  }

  check_and_correct(t, new_node);

  return new_node;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  node_t *x = t->root;
  while (x != t->nil)
  {
    if (x->key == key) {
      return x;
    } else if (x->key < key) {
      x = x->right;
    } else {
      x = x->left;
    }
  }
  return NULL;
}

node_t *find_minnode(const rbtree *t, node_t *subroot) {
  node_t *x = subroot;
  while(x->left != t->nil) {
    x = x->left;
  }
  return x;
}

node_t *rbtree_min(const rbtree *t) {
  return find_minnode(t, t->root);
}

node_t *rbtree_max(const rbtree *t) {
  node_t *x = t->root;
  while (x->right != t->nil)
  {
    x = x->right;
  }
  return x;
}

void transplant (rbtree *t, node_t *del, node_t *replace){
  if (del->parent == t->nil) {
    t->root = replace;
  }
  else if (del == del->parent->left){
    del->parent->left = replace;
  }
  else {
    del->parent->right = replace;
  }
  replace->parent = del->parent;
}


void check_erase_correct(rbtree *t, node_t *target) {
  while (target != t->root && target->color == RBTREE_BLACK) {
    if (target == target->parent->left) {
      node_t *uncle = target->parent->right;
      if ( uncle->color == RBTREE_RED) {
        uncle->color = RBTREE_BLACK;
        target->parent->color = RBTREE_RED;
        rotate_left(t, target->parent);
        uncle = target->parent->right;
      }
      if (uncle->left->color == RBTREE_BLACK && uncle->right->color == RBTREE_BLACK) {
        uncle->color = RBTREE_RED;
        target = target->parent;
      }
      else {
        if (uncle->right->color == RBTREE_BLACK) {
          uncle->left->color = RBTREE_BLACK;
          uncle->color = RBTREE_RED;
          rotate_right(t, uncle);
          uncle = target->parent->right;
        }
        uncle->color = target->parent->color;
        target->parent->color = RBTREE_BLACK;
        uncle->right->color = RBTREE_BLACK;
        rotate_left(t, target->parent);
        target = t->root;
      }
    }
    else {
      node_t *uncle = target->parent->left;
      if (uncle->color == RBTREE_RED) {
        uncle->color = RBTREE_BLACK;
        target->parent->color = RBTREE_RED;
        rotate_right(t, target->parent);
        uncle = target->parent->left;
      }
      if (uncle->right->color == RBTREE_BLACK && uncle->left->color == RBTREE_BLACK) {
        uncle->color = RBTREE_RED;
        target = target->parent;
      }
      else {
        if (uncle->left->color == RBTREE_BLACK) {
          uncle->right->color = RBTREE_BLACK;
          uncle->color = RBTREE_RED;
          rotate_left(t, uncle);
          uncle = target->parent->left;
        }
        uncle->color = target->parent->color;
        target->parent->color = RBTREE_BLACK;
        uncle->left->color = RBTREE_BLACK;
        rotate_right(t, target->parent);
        target = t->root;
      }
    }
  }
  target->color = RBTREE_BLACK;
}


int rbtree_erase(rbtree *t, node_t *p) {
  node_t *replace;
  node_t *successor;
  color_t original_color = p->color;

  if (p->left == t->nil){
    replace = p->right;
    transplant(t, p, replace);
  }
  else if (p->right == t->nil) {
    replace = p->left;
    transplant(t, p, replace);
  }
  else {
    successor = find_minnode(t, p->right);
    original_color = successor->color;
    replace = successor->right;
    if (successor->parent == p) {
      replace->parent = successor;
    }
    else {
      transplant(t, successor, replace);
      successor->right = p->right;
      successor->right->parent = successor;
    }
    transplant(t, p, successor);
    successor->left = p->left;
    successor->left->parent = successor;
    successor->color = p->color;
  }
  if (original_color == RBTREE_BLACK){
    check_erase_correct(t, replace);
  }
  
  free(p);
  return 0;
}

int inorder_search(const rbtree*t, node_t *p, int idx, key_t *arr, int n) {
  if (p == t-> nil || idx >= n) {
    return idx;
  }
  idx = inorder_search(t, p->left, idx, arr, n);
  arr[idx++] = p->key;
  idx = inorder_search(t, p->right, idx, arr, n);
  return idx;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  inorder_search(t, t->root, 0, arr, n);
  return 0;
}