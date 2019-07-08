#ifndef InternalNodeH
  #define InternalNodeH

#include "BTreeNode.h"

class InternalNode:public BTreeNode
{
  int internalSize;
  BTreeNode **children;
  int *keys;
public:
  InternalNode(int ISize, int LSize, InternalNode *p,
    BTreeNode *left, BTreeNode *right);
  int getMinimum()const;
  InternalNode* insert(int value); // returns pointer to new InternalNode
    // if it splits else NULL
  void insert(BTreeNode *oldRoot, BTreeNode *node2); // if root splits use this
  void insert(BTreeNode *newNode); // from a sibling
  void print(Queue <BTreeNode*> &queue);
  void resetMinimum(BTreeNode* child);
  void insertRightSibling(BTreeNode *node, BTreeNode *max);
  void insertLeftSibling(BTreeNode *max);
  BTreeNode* addNode(BTreeNode *node, int index);
  void insertHere(BTreeNode *node, int index);
  void sortValues(BTreeNode *node, int index);
  void setMinimum(BTreeNode *node, int index);
  InternalNode* split(BTreeNode *max);
}; // InternalNode class

#endif
