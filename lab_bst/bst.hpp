/**
 * @file bst.cpp
 * Definitions of the binary tree functions you'll be writing for this lab.
 * You'll need to modify this file.
 */

#include <algorithm>

template <class K, class V>
V BST<K, V>::find(const K& key)
{
    Node* node = find(root, key);
    if (node)
        return node->value;
    else
        return V();
}

template <class K, class V>
typename BST<K, V>::Node* & BST<K, V>::find(Node* & subtree, const K& key)
{
    if (subtree == nullptr || subtree->key == key)
        return subtree;
    if (key < subtree->key)
        return find(subtree->left, key);
    else
        return find(subtree->right, key);
}

template <class K, class V>
void BST<K, V>::insert(const K & key, const V & value)
{
    Node*& node = find(root, key);
    if (node == nullptr) {
        insert(node, key, value);
    }
    // If node is not null, it means key is already present, so we don't need to do anything
}

template <class K, class V>
void BST<K, V>::insert(Node*& subtree, const K& key, const V& value)
{
    if (subtree == nullptr) {
        subtree = new Node(key, value);
    } else if (key < subtree->key) {
        insert(subtree->left, key, value);
    } else if (key > subtree->key) {
        insert(subtree->right, key, value);
    }
    // If key is equal to subtree->key, it means it's a duplicate, so we don't need to insert
}

template <class K, class V>
void BST<K, V>::swap(Node*& first, Node*& second)
{
    // your code here
    std::swap(first->key, second->key);
    std::swap(first->value, second->value);
}

template <class K, class V>
void BST<K, V>::remove(const K& key)
{
    remove(root, key);
}

template <class K, class V>
void BST<K, V>::remove(Node*& subtree, const K& key)
{
    Node*& node = find(subtree, key);
    if (node != nullptr)
    {
        Node* temp = node;
        if (node->left == nullptr)
        {
            node = node->right;
            delete temp;
        }
        else if (node->right == nullptr)
        {
            node = node->left;
            delete temp;
        }
        else
        {
            Node* predecessor = node->left;
            while (predecessor->right != nullptr)
                predecessor = predecessor->right;
            swap(node, predecessor);
            remove(node->left, key);
        }
    }
}

template <class K, class V>
BST<K, V> listBuild(std::vector<std::pair<K, V>> inList)
{
    BST<K, V> bst;
    for (const auto& pair : inList)
        bst.insert(pair.first, pair.second);
    return bst;
}

template <class K, class V>
std::vector<int> allBuild(std::vector<std::pair<K, V>> inList)
{
    std::vector<int> heightHistogram;
    std::sort(inList.begin(), inList.end());

    if(inList.empty()){
        return heightHistogram;
    }

    bool c = true;
    while(c){
        BST<K, V> tree;

        for(const auto &pair: inList){
            tree.insert(pair.first, pair.second);
        }
        int height = tree.height();
        if(height >= (int)heightHistogram.size()){
            heightHistogram.resize(height+1, 0);
        }
        heightHistogram[height]++;
        c = std::next_permutation(inList.begin(), inList.end());
    }

    return heightHistogram;
};