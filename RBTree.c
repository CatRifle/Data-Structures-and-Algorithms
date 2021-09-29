/*
 * RBTree.c
 *
 *  Created on: Jul 27, 2020
 *      Author: MSI
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include<stdbool.h>
#include <math.h>
#include "RBTree.h"

#define PRINT_COLOUR_RED   "\x1B[31m"
#define PRINT_COLOUR_RESET "\x1B[0m"

#define data(tree)   ((tree)->data)
#define left(tree)   ((tree)->left)
#define right(tree)  ((tree)->right)
#define colour(tree) ((tree)->colour)
#define isRed(tree)  ((tree) != NULL && (tree)->colour == RED)

typedef enum {RED,BLACK} Colr;

typedef struct Node {
   int  data;
   Colr colour;
   Tree left, right;
} Node;

// create a new empty Tree
Tree newTree() {
   return NULL;
}

// make a new node containing data
Tree newNode(Item it) {
   Tree new = malloc(sizeof(Node));
   assert(new != NULL);
   data(new) = it;
   colour(new) = RED;
   left(new) = right(new) = NULL;
   return new;
}

Tree rotateRight(Tree);
Tree rotateLeft(Tree);

// InsertRB def
Tree InsertRB(Tree t, Item i, bool inRight){
	if (t == NULL){
		return newNode(i);
	}
	if (data(t)== i){
		return t;
	}
	if ( isRed(right(t)) && isRed(left(t)) ){
		colour(t) = RED;
		colour(right(t)) = BLACK;
		colour(left(t)) = BLACK;
	}
	if (i >= data(t)){
		right(t) = InsertRB(right(t), i, true);
		if (isRed(right(t)) && isRed(right(right(t)))){
			t= rotateLeft(t);
			colour(left(t)) = RED;
			colour(t) = BLACK;

		}
		if (!inRight && isRed(t) && isRed(right(t)) ){
			t = rotateLeft(t);
		}
	}
	else {
		left(t) = InsertRB(left(t), i, false);
		if ( isRed(left(left(t))) && isRed(left(t))){
			t = rotateRight(t);
			colour(right(t))= RED;
			colour(t) = BLACK;
		}
		if (inRight && isRed(t) && isRed(left(t))){
			t = rotateRight(t);
		}
	}
	return t;
}


// insert a new item into a tree
Tree TreeInsert(Tree t, Item it) {

   Tree new_tree = InsertRB(t, it, false);
   colour(new_tree) = BLACK;
   return new_tree;
}

// check whether a key is in a Tree
bool TreeSearch(Tree t, Item it) {
   if (t == NULL)
      return false;
   else if (it < data(t))
      return TreeSearch(left(t), it);
   else if (it > data(t))
      return TreeSearch(right(t), it);
   else                                 // it == data(t)
      return true;
}

Tree rotateRight(Tree n1) {
   if (n1 == NULL || left(n1) == NULL)
      return n1;
   Tree n2 = left(n1);
   left(n1) = right(n2);
   right(n2) = n1;
   return n2;
}

Tree rotateLeft(Tree n2) {
   if (n2 == NULL || right(n2) == NULL)
      return n2;
   Tree n1 = right(n2);
   right(n2) = left(n1);
   left(n1) = n2;
   return n1;
}

// free memory associated with Tree
void freeTree(Tree t) {
   if (t != NULL) {
      freeTree(left(t));
      freeTree(right(t));
      free(t);
   }
}

// display Tree sideways
void showTreeR(Tree t, int depth) {
   if (t != NULL) {
      showTreeR(right(t), depth+1);
      int i;
      for (i = 0; i < depth; i++)
	 putchar('\t');            // TAB character
      if (isRed(t))
	 printf("%s%d%s\n", PRINT_COLOUR_RED, data(t), PRINT_COLOUR_RESET);
       else
	 printf("%d\n", data(t));
      showTreeR(left(t), depth+1);
   }
}

void showTree(Tree t) {
   showTreeR(t, 0);
}
