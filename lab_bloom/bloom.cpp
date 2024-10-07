/**
 * @file bloom.cpp
 * Definitions of the binary tree functions you'll be writing for this lab.
 * You'll need to modify this file.
 */

 #include "bloom.h"

BF::BF(uint64_t size, std::vector<hashFunction> hashList){

    // Your code here 
    bv.resize(size, false);
    hv = hashList;
}

BF::BF(const BF& other){
    // Your code here 
    bv.resize(other.bv.size());
    for(unsigned long i =0; i < bv.size(); i++){
        bv[i] = other.bv[i];
    }
    hv = other.hv;
}

BF::~BF(){
    return;
}

void BF::add(const int & key){

    // Your code here 
    for(hashFunction h : hv){
        int idx = h(key) % bv.size();
        bv[idx] = true;
    }
}

bool BF::contains(const int& key) const{
    // Your code here 
    for(hashFunction h : hv){
        int idx = h(key) %bv.size();
        if(!bv[idx]){
            return false;
        }
    }
    //passes all tests
    return true;
}

void BF::bit_union(const BF& other){
    // Your code here 
    for(int i = 0; i < (int)bv.size(); i++){
        bv[i] = (bv[i] || other.bv[i]);
    }
}

void BF::intersect(const BF& other){
    // Your code here 
    for(int i = 0; i < (int)bv.size(); i++){
        bv[i] = (bv[i] && other.bv[i]);
    }
}

float measureFPR(std::vector<int> inList, uint64_t size, std::vector<hashFunction> hashList, unsigned max){
    // Your code here
    BF bloom = BF(size, hashList);
    for(int j : inList){
        bloom.add(j);
    }
    float tn = 0;
    float fp = 0;
    for(int i = 0; i < (int)max; i++){
        bool in = false;
        for(int m : inList){
            if(i == m) in = true;
        }
        bool contained = bloom.contains(i);
        if(!contained) tn++;
        if(contained && !in) fp++;
    }
    if(fp+tn == 0){
        return 1;
    }

    return (fp/(fp+tn));

}

bool getBitFromArray(std::vector<char> bv, int index){
    // Your code here
    int charindex = index/8;
    int bitindex = index%8;
    char c = bv[charindex];
    bool bit = getBitFromByte(c, bitindex);
    return bit;
}

bool getBitFromByte(char in, int index){
    // Your code here
    int real = 7-index;
    in = in >> real; //get the desired bit in the 0 position
    in &= 1;
    bool bit = (bool)in;

    return bit;

}

