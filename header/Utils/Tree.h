#ifndef INCLUDE_UTILS_TREE_H
#define INCLUDE_UTILS_TREE_H

#include <stdio.h>
#include <sys/types.h>

#include "Utils/List.h"

typedef struct tree Tree;

Tree *Tree_new();
/**
 * Resets a Tree node
 *
 * @param node Tree node to be reset
 *
 * TODO: Rewrite based on changes
 * @warning Potential memory leak. Children of specified node are left
 * parentless, and pointers to those children are lost.
 * If the intent is to
 * delete a full Tree starting at the specified node, then {@code
 * Tree_free(node)} should be used instead. If {@code node} is not allocated,
 * then {@code TreeList_freeItems(Tree_getChildren(node))} should be used
 * instead.
 * If the intent is to only reset the given {@code node} but keep its children
 * in memory, then the {@code TreeList} containing the children should be copied
 * before running this function.
 * */
void Tree_clear(Tree *node);
void Tree_free(Tree **root);
List *Tree_freeNode(Tree **node);

uint Tree_childCount(Tree *);
Tree *Tree_getChild(Tree *, uint idx);
List *Tree_getChildren(Tree *);
Tree *Tree_getParent(Tree *);
void *Tree_getData(Tree *);

void Tree_setFreeItemFunc(Tree *tree, void (*func)(void **));
void Tree_setParent(Tree *, Tree *parent);
void Tree_setData(Tree *, void *data);
void Tree_addChild(Tree *node, Tree *child);
void Tree_removeChild(Tree *node, Tree *child);

void Tree_print(Tree *tree, void (*valuePrinter)(void *));
void Tree_fprint(FILE *stream,
                 void (*valuePrinter)(void *),
                 Tree *tree,
                 uint depth);
void Tree_printNode(Tree *tree, void (*valuePrinter)(void *));
void Tree_fprintNode(FILE *stream, void (*valuePrinter)(void *), Tree *tree);

#endif // ! INCLUDE_UTILS_TREE_H
