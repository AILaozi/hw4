#ifndef AVLBST_H
#define AVLBST_H

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
* An explicit constructor to initialize the elements by calling the base class constructor
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
    virtual void rotateRight(AVLNode<Key,Value>* target);
    virtual void rotateLeft(AVLNode<Key,Value>* target);
    virtual void insertFix(AVLNode<Key,Value>* p, AVLNode<Key,Value>* n);
    virtual void removeFix(AVLNode<Key,Value>* n, int8_t diff);
};

template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key,Value>* target) {
    if (target == nullptr) return;

    AVLNode<Key, Value>* parent = target->getParent();
    AVLNode<Key, Value>* child = target->getLeft();
    if (child == nullptr) return;
    AVLNode<Key, Value>* childRight = child->getRight();

    child->setParent(parent);
    if (parent == nullptr) {
        this->root_ = child;
    }
    else if (parent->getLeft() == target) {
        parent->setLeft(child);
    }
    else {
        parent->setRight(child);
    }
    child->setRight(target);
    target->setParent(child);
    target->setLeft(childRight);
    if (childRight != nullptr) childRight->setParent(target);
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key,Value>* target) {
    if (target == nullptr) return;

    AVLNode<Key, Value>* parent = target->getParent();
    AVLNode<Key, Value>* child = target->getRight();
    if (child == nullptr) return;
    AVLNode<Key, Value>* childLeft = child->getLeft();

    child->setParent(parent);
    if (parent == nullptr) {
        this->root_ = child;
    }
    else if (parent->getLeft() == target) {
        parent->setLeft(child);
    }
    else {
        parent->setRight(child);
    }
    child->setLeft(target);
    target->setParent(child);
    target->setRight(childLeft);
    if (childLeft != nullptr) childLeft->setParent(target);
}

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO
    const Key& key = new_item.first;
    const Value& value = new_item.second;

    if (this->root_ == nullptr) {
        this->root_ = new AVLNode<Key, Value>(key, value, nullptr);
        return;
    }

    AVLNode<Key, Value>* curr = static_cast<AVLNode<Key, Value>*>(this->root_);
    AVLNode<Key, Value>* parent = nullptr;

    while (curr != nullptr) {
        if (key == curr->getKey()) {
            curr->setValue(value);
            return;
        }
        else if (key < curr->getKey()) {
            parent = curr;
            curr = static_cast<AVLNode<Key, Value>*>(curr->getLeft());
        }
        else {
            parent = curr;
            curr = static_cast<AVLNode<Key, Value>*>(curr->getRight());
        }
    }

    AVLNode<Key, Value>* n = new AVLNode<Key, Value>(key, value, parent);

    if (key < parent->getKey()) {
        parent->setLeft(n);
    }
    else {
        parent->setRight(n);
    }

    if (parent->getBalance() == -1 || parent->getBalance() == 1) {
        parent->setBalance(0);
        return;
    }
    
    if (parent->getLeft() == n) {
        parent->updateBalance(-1);
    }
    else {
        parent->updateBalance(1);
    }
    insertFix(parent, n);
}

template<class Key, class Value>
void AVLTree<Key, Value>::insertFix(AVLNode<Key,Value>* p, AVLNode<Key,Value>* n) {

    if (p == nullptr) return;
    if (p->getParent() == nullptr) return;

    AVLNode<Key, Value>* g = p->getParent();

    if (p == g->getLeft()) {
        g->updateBalance(-1);
        if (g->getBalance() == 0) return;

        if (g->getBalance() == -1) {
            insertFix(g, p);
        }

        if (g->getBalance() == -2) {
            if (p->getBalance() < 0) {
                rotateRight(g);
                p->setBalance(0);
                g->setBalance(0);
            }
            else {
                rotateLeft(p);
                rotateRight(g);

                if (n->getBalance() == -1) {
                    p->setBalance(0);
                    g->setBalance(1);
                    n->setBalance(0);
                }
                else if (n->getBalance() == 0) {
                    p->setBalance(0);
                    g->setBalance(0);
                    n->setBalance(0);
                }
                else if (n->getBalance() == 1){
                    p->setBalance(-1);
                    g->setBalance(0);
                    n->setBalance(0);
                }
            }
        }

    }
    else if (p == g->getRight()) {
        g->updateBalance(1);
        if (g->getBalance() == 0) return;

        if (g->getBalance() == 1) {
            insertFix(g, p);
        }

        if (g->getBalance() == 2) {
            if (p->getBalance() > 0) {
                rotateLeft(g);
                p->setBalance(0);
                g->setBalance(0);
            }
            else {
                rotateRight(p);
                rotateLeft(g);

                if (n->getBalance() == 1) {
                    p->setBalance(0);
                    g->setBalance(-1);
                    n->setBalance(0);
                }
                else if (n->getBalance() == 0) {
                    p->setBalance(0);
                    g->setBalance(0);
                    n->setBalance(0);
                }
                else if (n->getBalance() == -1){
                    p->setBalance(1);
                    g->setBalance(0);
                    n->setBalance(0);
                }
            }
        }

    }
}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    // TODO
    AVLNode<Key, Value>* temp = static_cast<AVLNode<Key, Value>*>(this->internalFind(key));
    if (temp == nullptr) return;

    if (temp->getLeft() != nullptr && temp->getRight() != nullptr) {
        AVLNode<Key, Value>* pred = static_cast<AVLNode<Key, Value>*>(this->predecessor(temp));
        nodeSwap(temp, pred);
    }

    AVLNode<Key, Value>* child = nullptr;
    if (temp->getLeft() != nullptr) {
        child = temp->getLeft();
    }
    else {
        child = temp->getRight();
    }
    AVLNode<Key, Value>* parent = temp->getParent();

    if (parent == nullptr) {
        this->root_ = child;
        if (child != nullptr) child->setParent(nullptr);
        delete temp;
        return;
    }

    int8_t diff = 0;
    if (parent->getLeft() == temp){
        diff = 1;
        parent->setLeft(child);
        if (child != nullptr) child->setParent(parent);
    }
    else {
        diff = -1;
        parent->setRight(child);
        if (child != nullptr) child->setParent(parent);
    }
    delete temp;

    removeFix(parent, diff);

}

template<class Key, class Value>
void AVLTree<Key, Value>::removeFix(AVLNode<Key,Value>* n, int8_t diff) {
    if (n == nullptr) return;
    AVLNode<Key, Value>* p = n->getParent();

    int8_t ndiff = 0;
    if (p != nullptr) {
        if (p->getLeft() == n) {
            ndiff = 1;
        }
        else {
            ndiff = -1;
        }
    }

    if (diff == -1) {
        if (n->getBalance() + diff == -2) {
            AVLNode<Key, Value>* c = n->getLeft();

            if (c->getBalance() == -1) {
                rotateRight(n);
                n->setBalance(0);
                c->setBalance(0);
                if(p != nullptr) removeFix(p, ndiff);
            }
            else if (c->getBalance() == 0) {
                rotateRight(n);
                n->setBalance(-1);
                c->setBalance(1);
                return;
            }
            else if (c->getBalance() == 1) {
                AVLNode<Key, Value>* g = c->getRight();
                rotateLeft(c);
                rotateRight(n);

                if (g->getBalance() == 1) {
                    n->setBalance(0);
                    c->setBalance(-1);
                    g->setBalance(0);
                }
                else if (g->getBalance() == 0) {
                    n->setBalance(0);
                    c->setBalance(0);
                    g->setBalance(0);
                }
                else if (g->getBalance() == -1) {
                    n->setBalance(1);
                    c->setBalance(0);
                    g->setBalance(0);
                }
                if(p != nullptr) removeFix(p, ndiff);
            }

        }
        else if (n->getBalance() + diff == -1) {
            n->setBalance(-1);
            return;
        }
        else if (n->getBalance() + diff == 0) {
            n->setBalance(0);
            if(p != nullptr) removeFix(p, ndiff);
        }
    }
    else if (diff == 1) {
        if (n->getBalance() + diff == 2) {
            AVLNode<Key, Value>* c = n->getRight();

            if (c->getBalance() == 1) {
                rotateLeft(n);
                n->setBalance(0);
                c->setBalance(0);
                if(p != nullptr) removeFix(p, ndiff);
            }
            else if (c->getBalance() == 0) {
                rotateLeft(n);
                n->setBalance(1);
                c->setBalance(-1);
                return;
            }
            else if (c->getBalance() == -1) {
                AVLNode<Key, Value>* g = c->getLeft();
                rotateRight(c);
                rotateLeft(n);

                if (g->getBalance() == -1) {
                    n->setBalance(0);
                    c->setBalance(1);
                    g->setBalance(0);
                }
                else if (g->getBalance() == 0) {
                    n->setBalance(0);
                    c->setBalance(0);
                    g->setBalance(0);
                }
                else if (g->getBalance() == 1) {
                    n->setBalance(-1);
                    c->setBalance(0);
                    g->setBalance(0);
                }
                if(p != nullptr) removeFix(p, ndiff);
            }

        }
        else if (n->getBalance() + diff == 1) {
            n->setBalance(1);
            return;
        }
        else if (n->getBalance() + diff == 0) {
            n->setBalance(0);
            if(p != nullptr) removeFix(p, ndiff);
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
