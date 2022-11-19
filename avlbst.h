#ifndef RBBST_H
#define RBBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor and setting
* the color to red since every new node will be red when it is first inserted.
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
		//rotate left rotate right insert fix remove fix
		virtual void rotateRight(AVLNode<Key, Value>* node);
		virtual void rotateLeft(AVLNode<Key, Value>* node);
		virtual void insertFix(AVLNode<Key, Value>* parent, AVLNode<Key, Value>* node);
		virtual void removeFix(AVLNode<Key, Value>* node, int diff);
};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item) {
		AVLNode<Key, Value>* temp = static_cast<AVLNode<Key, Value>*>(this->root_);
		AVLNode<Key, Value>* tempParent = nullptr;
		if (temp == NULL) {
				this->root_ = new AVLNode<Key, Value>(new_item.first, new_item.second, NULL);
				static_cast<AVLNode<Key, Value>*>(this->root_)->setBalance(0);
				return;
		}
		while (temp != NULL) {
			if (new_item.first > temp->getKey()) { //go right
				tempParent = temp;
				temp = temp->getRight();
			}
			else if (new_item.first < temp->getKey()) { //go left
				tempParent = temp;
				temp = temp->getLeft();
			}
			else if (new_item.first == temp->getKey()) { //equal to each other
					temp->setValue(new_item.second);
					return;
			}
		}
		AVLNode<Key, Value>* final = new AVLNode<Key, Value>(new_item.first, new_item.second, tempParent);
		if(new_item.first < tempParent->getKey()){
			tempParent->setLeft(final);
		}
		else if (new_item.first > tempParent->getKey()){
			tempParent->setRight(final);
		}
		final->setBalance(0);
		
		//call insertfix if balance off
		if (tempParent->getBalance() == 1) {
			tempParent->setBalance(0);
			return;
		}
		else if (tempParent->getBalance() == -1){
			tempParent->setBalance(0);
			return;
		}
		else {
			if (final == tempParent->getLeft()) {
				tempParent->setBalance(-1);
			}
			else if (final == tempParent->getRight()) {
				tempParent->setBalance(1);
			}
			insertFix(tempParent, final);
		}
}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::remove(const Key& key) {
    // TODO
		AVLNode<Key, Value>* temp = static_cast<AVLNode<Key, Value>*>(this->internalFind(key));
		if (!temp) {
			return;
		}

		//diff check
		int diff = 0;
		AVLNode<Key, Value> * parent = temp->getParent();
		if (parent != NULL) {
			if (parent->getLeft() == temp) {
				diff = 1;
			}	
			if (parent->getRight() == temp) {
				diff = -1;
			}
		}

		//two children
		if (temp->getRight() != NULL && temp->getLeft() != NULL) {
			AVLNode<Key, Value> * pred = static_cast<AVLNode<Key, Value>*>(this->predecessor(temp));
			nodeSwap(temp, pred);
		}

		//no children
		if (temp->getRight() == NULL && temp->getLeft() == NULL) {
			if (temp == this->root_) { 
				this->root_ = nullptr;
			} 
			else {
				AVLNode<Key, Value> * parent = temp->getParent(); //get parent 
				//check if left or right
				if (parent->getRight() == temp) { //set parent right to null
						parent->setRight(nullptr);
				}
				if (parent->getLeft() == temp) { //set parent left to null
						parent->setLeft(nullptr);
				}
			}
		}

		//one child
		else {
			if (temp == this->root_) { 
				AVLNode<Key, Value> * child = nullptr;
				if (temp->getRight() != NULL && temp->getLeft() == NULL) { //right child
						child = temp->getRight();
				}
				if (temp->getRight() == NULL && temp->getLeft() != NULL) { //left child
						child = temp->getLeft();
				}
				this->root_ = child;
				child->setParent(NULL);
			} 
			else {
				AVLNode<Key, Value> * parent = temp->getParent();
				AVLNode<Key, Value> * child = nullptr;
				if (parent->getRight() != NULL && temp->getLeft() == NULL) { //right child
						child = temp->getRight();
				}
				if (temp->getRight() == NULL && temp->getLeft() != NULL) { //left child
						child = temp->getLeft();
				}
				if (parent->getRight() == temp) { //temp is parent's right child
						parent->setRight(child);
						child->setParent(parent);
				}
				if (parent->getLeft() == temp) { //temp is parent's left child
						parent->setLeft(child);
						child->setParent(parent);
				}
			}
		} 
		if (parent != NULL) {
			if (parent->getLeft() == temp) {
				if (temp->getLeft() != NULL) {
					parent->setLeft(temp->getLeft());
				}
				if (temp->getRight() != NULL) {
					parent->setLeft(temp->getRight());
				}
				else {
					parent->setLeft(nullptr);
				}
			}
			if (parent->getRight() == temp) {
				if (temp->getLeft() != NULL) {
					parent->setRight(temp->getLeft());
				}
				if (temp->getRight() != NULL) {
					parent->setRight(temp->getRight());
				}
				else {
					parent->setRight(nullptr);
				}
			}
		}
		delete temp;
		temp = NULL;
		removeFix(parent, diff);
}

//ROTATE RIGHT
template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key, Value>* node) {
	AVLNode<Key, Value>* parent = node->getParent();
	AVLNode<Key, Value>* leftChild = node->getLeft();
	AVLNode<Key, Value>* childTemp = leftChild->getRight();

	if (parent != NULL) {
		if (node == parent->getLeft()) {
			parent->setLeft(leftChild);
		}
		else {
			parent->setRight(leftChild);
		}
	} else {
		this->root_ = leftChild;
	}
	leftChild->setParent(parent);
	node->setParent(leftChild);
	leftChild->setRight(node);
	node->setLeft(childTemp);
	if (childTemp != NULL) childTemp->setParent(node);
}

//ROTATE LEFT
template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key, Value>* node) {
	AVLNode<Key, Value>* parent = node->getParent();
	AVLNode<Key, Value>* rightChild = node->getRight();
	AVLNode<Key, Value>* childTemp = rightChild->getLeft();

	if (parent != NULL) {
		if (node == parent->getLeft()) {
			parent->setLeft(rightChild);
		}
		else {
			parent->setRight(rightChild);
		}
	} else {
		this->root_ = rightChild;
	}
	rightChild->setParent(parent);
	node->setParent(rightChild);
	rightChild->setLeft(node);
	node->setRight(childTemp);
	if (childTemp != NULL) childTemp->setParent(node);
}

//insert fix add param
template<class Key, class Value>
void AVLTree<Key, Value>::insertFix(AVLNode<Key, Value>* parent, AVLNode<Key, Value>* node) {
	//call isBalanced?
	if (parent == NULL || parent->getParent() == NULL) {
		return;
	}
	AVLNode<Key, Value>* gp = parent->getParent();

	//left
	if (gp->getLeft() == parent) {
		gp->updateBalance(-1);

		if (gp->getBalance() == 0) {
			return;
		}

		else if (gp->getBalance() == -1) {
			insertFix(gp, parent);
		}

		else if (gp->getBalance() == -2) {
			//zig zig
			if (node == parent->getLeft()) {
				rotateRight(gp);
				parent->setBalance(0);
				gp->setBalance(0);
			}
			//zig zag CHECK HERE
			else {
					rotateLeft(parent);
					rotateRight(gp);
					if (node->getBalance() == -1) {
						//set balances
						parent->setBalance(0);
						gp->setBalance(1);
						node->setBalance(0);
					}
					else if (node->getBalance() == 0) {
						//set balances
						parent->setBalance(0);
						gp->setBalance(0);
						node->setBalance(0);
					}
					else if (node->getBalance() == 1) {
						//set balances
						parent->setBalance(-1);
						gp->setBalance(0);
						node->setBalance(0);
					}
				}
			}
		}


	//right
	else {
		gp->updateBalance(1);
		if (gp->getBalance() == 0) {
			return;
		}

		else if (gp->getBalance() == 1) {
			insertFix(gp, parent);
		}

		else if (gp->getBalance() == 2) {

			//zig zig
			if (node == parent->getRight()) {
					rotateLeft(gp);
					parent->setBalance(0);
					gp->setBalance(0);
			}

			//zig zag
			else {
				rotateRight(parent);
				rotateLeft(gp);
				if (node->getBalance() == -1) {
					//set balances
					parent->setBalance(1);
					gp->setBalance(0);
					node->setBalance(0);
				}

				else if (node->getBalance() == 0) {
					//set balances
					parent->setBalance(0);
					gp->setBalance(0);
					node->setBalance(0);
				}
				
				else if (node->getBalance() == 1) {
					//set balances
					parent->setBalance(0);
					gp->setBalance(-1);
					node->setBalance(0);
				}
			}
		}
	}
}

//REMOVE FIX
template<class Key, class Value>
void AVLTree<Key, Value>::removeFix(AVLNode<Key, Value>* node, int diff) {
	if (node == NULL) return;
	AVLNode<Key, Value>* parent = node->getParent();
	int ndiff;
	if (parent != NULL) {
		if (parent->getLeft() == node) ndiff = 1;
		else ndiff = -1;
	}


	//diff = -1
	if (diff == -1) {
		//CASE 1
		if (node->getBalance() + diff == -2) {
			AVLNode<Key, Value>* c = node->getLeft();
			//CASE 1A
			if (c->getBalance() == -1) {
				rotateRight(node);
				node->setBalance(0);
				c->setBalance(0);
				removeFix(parent, ndiff);
			}
			//CASE 1B
			else if (c->getBalance() == 0) {
				rotateRight(node);
				node->setBalance(-1);
				c->setBalance(1);
			}
			//CASE 1C
			else if (c->getBalance() == 1) {
				AVLNode<Key, Value>* g = c->getRight();
				rotateLeft(c);
				rotateRight(node);
				if (g->getBalance() == 1) {
					node->setBalance(0);
					c->setBalance(-1);
					g->setBalance(0);
				}
				else if (g->getBalance() == 0) {
					node->setBalance(0);
					c->setBalance(0);
					g->setBalance(0);
				}
				else if (g->getBalance() == -1) {
					node->setBalance(1);
					c->setBalance(0);
					g->setBalance(0);
				}
				removeFix(parent, ndiff);
			}
		}

		//CASE 2
		else if (node->getBalance() + diff == -1) {
			node->setBalance(-1);
		}

		//CASE 3
		else if (node->getBalance() + diff == 0) {
			node->setBalance(0);
			removeFix(parent, ndiff);
		}
	}


	//diff = 1
	else if (diff == 1) {
		//CASE 1A
		if (node->getBalance() + diff == 2) {
			AVLNode<Key, Value>* c = node->getRight();
			//changed from getLeft
			if (c->getBalance() == 1) {
				rotateLeft(node);
				//changed form rotateRight
				node->setBalance(0);
				c->setBalance(0);
				removeFix(parent, ndiff);
			}
			//CASE 1B
			else if (c->getBalance() == 0) {
				rotateLeft(node);
				//changed from rotateLeft
				node->setBalance(1);
				c->setBalance(-1);
			}
			//CASE 1C
			else if (c->getBalance() == -1) {
				AVLNode<Key, Value>* g = c->getLeft();
				//int8_t gBalance = g->getBalance();
				if (g == NULL) {
					removeFix(parent, ndiff);
					return;
				}
				//changed from getRight
				rotateRight(c);
				rotateLeft(node);
				//changed right and left
				if (g->getBalance() == -1) {
					node->setBalance(0);
					c->setBalance(1);
					g->setBalance(0);
				}
				else if (g->getBalance() == 0) {
					node->setBalance(0);
					c->setBalance(0);
					g->setBalance(0);
				}
				else if (g->getBalance() == 1) {
					node->setBalance(-1);
					c->setBalance(0);
					g->setBalance(0);
				}
				removeFix(parent, ndiff);
			}
		}

		//CASE 2
		else if (node->getBalance() + diff == 1) {
			node->setBalance(1);
		}

		//CASE 3
		else if (node->getBalance() + diff == 0) {
			node->setBalance(0);
			removeFix(parent, ndiff);
		}
	}
}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif
