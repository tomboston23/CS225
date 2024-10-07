/* Your code here! */
#include "dsets.h"

void DisjointSets::addelements(int num){
    for (int i = 0 ; i < num ; i++){
        elements.push_back(-1); // -1 represents a disconnected root node
    }
}

int DisjointSets::find(int elem){
    int root = elem;
    while(elements[root] >=0){ //while element has a parent
        root = elements[root]; //element becomes its parent
    } //iterate until it's negative, meaning we found the root of our element's path


    //path compression
    int path = elem;
    while(path != root){
        int parent = elements[path]; //temporarily store parent to iterate through the path
        elements[path] = root; //set all nodes to point to root
        path = parent; //iterate through path
    }
    return root;
}

void DisjointSets::setunion(int a, int b){
    int root_a = find(a);
    int root_b = find(b);
    if(root_a == root_b){ //if they're already in a union don't change
        return;
    }
    
    int size_a = size(a);
    int size_b = size(b);
    int n_size = size_a + size_b; //root needs to hold new size

    if(size_a < size_b){//set a to point to b root
        elements[root_a] = root_b;
        elements[root_b] = -(n_size); //update root value
    } else {            //set b to point to a root
        elements[root_b] = root_a;
        elements[root_a] = -(n_size); //update root value
    }
}

int DisjointSets::size(int elem){
    int root = find(elem); //get the root of the element
    return -elements[root]; //root holds -size in the vector
}