#include <iostream>
#include "InternalNode.h"

using namespace std;

InternalNode::InternalNode(int ISize, int LSize,
  InternalNode *p, BTreeNode *left, BTreeNode *right) :
  BTreeNode(LSize, p, left, right), internalSize(ISize)
{
  keys = new int[internalSize]; // keys[i] is the minimum of children[i]
  children = new BTreeNode* [ISize];
} // InternalNode::InternalNode()


int InternalNode::getMinimum()const
{
  if(count > 0)   // should always be the case
    return children[0]->getMinimum();
  else
    return 0;
} // InternalNode::getMinimum()

BTreeNode* InternalNode::addNode(BTreeNode *node, int index){
	if(index == internalSize){
		return node;
	}
	BTreeNode *max = children[count-1];
	
	for(int i = count -2; i >= index; i--){
		children[i+1] = children[i];
		keys[i+1] = keys[i];
	}
	
	children[index] = node;
	keys[index]=node->getMinimum();
	node->setParent(this);
	return max;
}

void InternalNode::insertLeftSibling(BTreeNode *max){
	((InternalNode*)leftSibling)->insert(children[0]);
	for(int i = 0; i < count -1; i++){
		children[i] = children[i+1];
		keys[i] = keys[i+1];
	}
	children[count - 1] = max;
	keys[count-1] = max->getMinimum();
	max->setParent(this);
	if(parent){
		parent->resetMinimum(this);
	}
}

void InternalNode::insertRightSibling(BTreeNode *node, BTreeNode *max){
	((InternalNode*)rightSibling)->insert(max);
	if(node == children[0] && parent)
		parent->resetMinimum(this);
}


void InternalNode::insertHere(BTreeNode *node, int index){
	int i;
	for(i = count - 1; i >= index; i--){//compare Values to sort order
		keys[i+1] = keys[i];
		children[i+1]=children[i];
	}
	
	keys[index]=node->getMinimum();
	children[index]=node;
	count++;
	node->setParent(this);
	
	if(index == 0 && parent){
		parent->resetMinimum(this);
	}
}

/*
void InternalNode::insertHere(BTreeNode *node, int index){
	sortValues(node, index);
	setMinimum(node, index);
}

void InternalNode::setMinimum(BTreeNode *node, int index){
	if(index == 0 && parent){
		parent->resetMinimum(this);
	}
}// LeafNode::setMinimum;

void InternalNode::sortValues(BTreeNode *node, int index){
	int i;
	for(i = count - 1; i >= index; i--){//compare Values to sort order
		keys[i+1] = keys[i];
		children[i+1]=children[i];
	}
	keys[index]=node->getMinimum();
	children[index]=node;
	count++;
	node->setParent(this);
	
}
*/


InternalNode* InternalNode::insert(int value)
{
	int index;
	for(index = count - 1; index > 0 && keys[index] > value; index--){};
		
	BTreeNode *max, *node = children[index]->insert(value);
	if(!node){
		return NULL;
	}
	if(count < internalSize){
		insertHere(node, index + 1);
		return NULL;
	}
	max = addNode(node, index + 1);
		
	if(leftSibling && leftSibling->getCount() < internalSize){
		insertLeftSibling(max);
		return NULL;
	}
	else{
		if(rightSibling && rightSibling->getCount() < internalSize){
			insertRightSibling(node, max);
			return NULL;
		}
		else{
			return split(max);
		}
	}
	return NULL;
} // InternalNode::insert()

void InternalNode::insert(BTreeNode *oldRoot, BTreeNode *node2) //insert leafnodes into children
{ // Node must be the root, and node1
	oldRoot->setParent(this);
	node2->setParent(this);
	keys[0] = oldRoot->getMinimum();
	keys[1] = node2->getMinimum();
	children[0] = oldRoot; count++;
	children[1] = node2; count++;
	children[0]->setLeftSibling(NULL);
	children[0]->setRightSibling(children[1]);
	children[1]->setLeftSibling(children[0]);
	children[1]->setRightSibling(NULL);
  // students must write this
} // InternalNode::insert()

void InternalNode::insert(BTreeNode *newNode) // from a sibling
{
	int index;
	
	if(newNode->getMinimum() <= keys[0]){
		index = 0;
	}
	else{
		index = count;
	}
	insertHere(newNode, index);
} // InternalNode::insert()

void InternalNode::resetMinimum(BTreeNode* child){
	for(int i = 0; i < count; i++){
		if(children[i] == child){
			keys[i] = children[i]->getMinimum();
			if(i == 0 && parent)
				parent->resetMinimum(this);
			break;
		}
	}
}

InternalNode* InternalNode::split(BTreeNode *max){
	InternalNode *newINode = new InternalNode(internalSize, leafSize, parent, this, rightSibling);
	
	if(rightSibling){
		rightSibling->setLeftSibling(newINode);
	}
	
	rightSibling = newINode;
	
	for(int i = (internalSize + 1)/2; i < internalSize; i++){
		newINode->children[newINode->count] = children[i];
		newINode->keys[newINode->count++] = keys[i];
		children[i]->setParent(newINode);
	}
	
	newINode->children[newINode->count] = max;
	newINode->keys[newINode->count++] = max->getMinimum();
	max->setParent(newINode);
	count = (internalSize+1)/2;
	return newINode;

}



void InternalNode::print(Queue <BTreeNode*> &queue)
{
  int i;

  cout << "Internal: ";
  for (i = 0; i < count; i++)
    cout << keys[i] << ' ';
  cout << endl;

  for(i = 0; i < count; i++)
    queue.enqueue(children[i]);

} // InternalNode::print()


