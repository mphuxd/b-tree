#include <iostream>
#include "LeafNode.h"
#include "InternalNode.h"
#include "QueueAr.h"

using namespace std;


LeafNode::LeafNode(int LSize, InternalNode *p,
  BTreeNode *left, BTreeNode *right) : BTreeNode(LSize, p, left, right)
{
  values = new int[LSize];
}  // LeafNode()

int LeafNode::getMinimum()const
{
  if(count > 0)  // should always be the case
    return values[0];
  else
    return 0;

} // LeafNode::getMinimum()

void LeafNode::insertLeftSibling(int value, int max){
	leftSibling->insert(values[0]);
	
	for(int i = 0; i < count -1; i++){
		values[i] = values[i+1];
	}
	values[count -1] = max;
	if(parent){
		parent->resetMinimum(this);
	}
}

void LeafNode::insertRightSibling(int value, int max){
	rightSibling->insert(max);
	
	if(value == values[0] && parent){
		parent->resetMinimum(this);
	}
}

void LeafNode::prepareSplit(int value, int &max){
	if(value > values[count-1]){
		max = value;
	}
	else{
		max = values[count-1];
		fullSort(value);
	}
}

LeafNode* LeafNode::split(int value, int max){//splits
	LeafNode* newSibling = new LeafNode(leafSize, parent, this, rightSibling);// node splits right
	
	if(rightSibling){//if rightsibling existed, set rightsiblings left sibling to new sibling
		rightSibling->setLeftSibling(newSibling);
	}
	rightSibling = newSibling;
	
	//give values to newsibling
	for(int i = (leafSize + 1)/2; i < leafSize; i++){ //give lower half of leaf to newsibling
		newSibling->values[newSibling->count++] = values[i]; 

	}
	
	newSibling->values[newSibling->count++] = max;
	count = (leafSize+1)/2;	
	
	if(value == values[0] && parent){
		parent->resetMinimum(this);
	}
	
return newSibling;
}

LeafNode* LeafNode::insert(int value)
{
	int max;
	if(count < leafSize){
		insertHere(value);
		count++;
		return NULL;
	}
	//add overflow value to leaf and sort
	prepareSplit(value, max);
	
	if(leftSibling && leftSibling->getCount() < leafSize){
		insertLeftSibling(value, max);
		return NULL;
	}
	else if(rightSibling && rightSibling->getCount() < leafSize){
		insertRightSibling(value,max);
		return NULL;
	}
		
	else{
		return split(value, max);
	}
	
  return NULL; // to avoid warnings for now.
}  // LeafNode::insert()

void LeafNode::print(Queue <BTreeNode*> &queue)
{
  cout << "Leaf: ";
  for (int i = 0; i < count; i++)
    cout << values[i] << ' ';
  cout << endl;
} // LeafNode::print()


void LeafNode::insertHere(int value){
	sortValues(value);
	setMinimum(value);
}

void LeafNode::setMinimum(int value){
	if(value == values[0] && parent){
		parent->resetMinimum(this);
	}
}// LeafNode::setMinimum;

void LeafNode::sortValues(int value){
	int i;
	for(i = count - 1; i >= 0 && values[i] > value; i--){//compare Values to sort order
		values[i+1]=values[i];
	}
	values[i+1]=value;
}

void LeafNode::fullSort(int value){
	int i;
	for(i = count - 2; i >= 0 && values[i] > value; i--){
		//compare Values to sort order
			values[i+1]=values[i];
		}
	values[i+1]=value;
}


