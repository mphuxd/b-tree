#ifndef LeafNodeH
#define LeafNodeH

#include "BTreeNode.h"

class LeafNode:public BTreeNode
{
  int *values;
public:
  LeafNode(int LSize, InternalNode *p, BTreeNode *left,
    BTreeNode *right);
  int getMinimum() const;
  void setMinimum(int value);
  void sortValues(int value);
  void fullSort(int value);
  void insertHere(int value);
  void insertLeftSibling(int value, int max);
  void insertRightSibling(int value, int max);
  void prepareSplit(int value, int& max);
  LeafNode* split(int value, int max);
  LeafNode* insert(int value); // returns pointer to new Leaf if splits
  // else NULL
  void print(Queue <BTreeNode*> &queue);
}; //LeafNode class

#endif
