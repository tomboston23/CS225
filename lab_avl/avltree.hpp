/**
 * @file avltree.cpp
 * Definitions of the binary tree functions you'll be writing for this lab.
 * You'll need to modify this file.
 */

template <class K, class V>
V AVLTree<K, V>::find(const K& key) const
{
    return find(root, key);
}

template <class K, class V>
V AVLTree<K, V>::find(Node* subtree, const K& key) const
{
    if (subtree == NULL)
        return V();
    else if (key == subtree->key)
        return subtree->value;
    else {
        if (key < subtree->key)
            return find(subtree->left, key);
        else
            return find(subtree->right, key);
    }
}

template <class K, class V>
void AVLTree<K, V>::rotateLeft(Node*& t)
{
    functionCalls.push_back("rotateLeft"); // Stores the rotation name (don't remove this)
    // your code here
    Node* y = t->right;
    t->right = y->left;
    y->left = t;

    // Update heights
    t->height = std::max(heightOrNeg1(t->left), heightOrNeg1(t->right)) + 1;
    y->height = std::max(heightOrNeg1(y->left), heightOrNeg1(y->right)) + 1;

    t = y; // Update node pointer to point to the new root after rotation
}

template <class K, class V>
void AVLTree<K, V>::rotateLeftRight(Node*& t)
{
    functionCalls.push_back("rotateLeftRight"); // Stores the rotation name (don't remove this)
    // Implemented for you:
    rotateLeft(t->left);
    rotateRight(t);
}

template <class K, class V>
void AVLTree<K, V>::rotateRight(Node*& t)
{
    functionCalls.push_back("rotateRight"); // Stores the rotation name (don't remove this)
    // your code here
    Node* y = t->left;
    t->left = y->right;
    y->right = t;

    // Update heights
    t->height = std::max(heightOrNeg1(t->left), heightOrNeg1(t->right)) + 1;
    y->height = std::max(heightOrNeg1(y->left), heightOrNeg1(y->right)) + 1;

    t = y; // Update node pointer to point to the new root after rotation
}

template <class K, class V>
void AVLTree<K, V>::rotateRightLeft(Node*& t)
{
    functionCalls.push_back("rotateRightLeft"); // Stores the rotation name (don't remove this)
    // your code here
    rotateRight(t->right);
    rotateLeft(t);
}

template <class K, class V>
void AVLTree<K, V>::rebalance(Node*& subtree)
{
    // your code here
    int balance = heightOrNeg1(subtree->left) - heightOrNeg1(subtree->right);

    // If subtree is unbalanced
    if (balance == -2) {
        // Right subtree is taller
        if (heightOrNeg1(subtree->right->left) - heightOrNeg1(subtree->right->right) == 1) {
            // Right-left rotation
            rotateRightLeft(subtree);
        } else {
            // Right rotation
            rotateLeft(subtree);
        }
    } else if (balance == 2) {
        // Left subtree is taller
        if (heightOrNeg1(subtree->left->left) - heightOrNeg1(subtree->left->right) == -1) {
            // Left-right rotation
            rotateLeftRight(subtree);
        } else {
            // Left rotation
            rotateRight(subtree);
        }
    }

    // Update the height of the current node
    subtree->height = std::max(heightOrNeg1(subtree->left), heightOrNeg1(subtree->right)) + 1;
}

template <class K, class V>
void AVLTree<K, V>::insert(const K & key, const V & value)
{
    insert(root, key, value);
}

template <class K, class V>
void AVLTree<K, V>::insert(Node*& subtree, const K& key, const V& value)
{
    // your code here
    if (subtree == NULL) {
        subtree = new Node(key, value);
    } else if (key < subtree->key) {
        insert(subtree->left, key, value);
    } else if (key > subtree->key) {
        insert(subtree->right, key, value);
    }

    // Update the height of the current node
    subtree->height = std::max(heightOrNeg1(subtree->left), heightOrNeg1(subtree->right)) + 1;

    // Rebalance the tree
    rebalance(subtree);
}

template <class K, class V>
void AVLTree<K, V>::remove(const K& key)
{
    remove(root, key);
}

template <class K, class V>
void AVLTree<K, V>::remove(Node*& subtree, const K& key)
{
    if (key < subtree->key) {
        remove(subtree->left, key);
    } else if (key > subtree->key) {
        remove(subtree->right, key);
    } else {
        if (subtree->left == NULL && subtree->right == NULL) {
            // No children
            delete subtree;
            subtree = NULL;
        } else if (subtree->left != NULL && subtree->right != NULL) {
            // Two children
            Node* successor = subtree->right;
            while (successor->left != NULL) {
                successor = successor->left;
            }
            swap(subtree, successor);
            remove(subtree->right, key);
        } else {
            // One child
            Node* child = (subtree->left != NULL) ? subtree->left : subtree->right;
            delete subtree;
            subtree = child;
        }
    }

    if (subtree != NULL) {
        // Update the height of the current node
        subtree->height = std::max(heightOrNeg1(subtree->left), heightOrNeg1(subtree->right)) + 1;

        // Rebalance the tree
        rebalance(subtree);
    }
}
