//
//  BST.hpp
//  Void
//
//  Created by Majd Takieddine on 1/19/17.
//  Copyright © 2017 Majd Takieddine. All rights reserved.
//

#ifndef BST_hpp
#define BST_hpp

#include <iostream>
using namespace std;

#endif /* BST_hpp */

template<class T>
class BST
{
public:
    
    enum type
    {  AVL, RBT };
    
    struct Tree
    {
        T value;
        Tree* child[2];
        
        short balance; //AVL
        bool red;   //RBT
        
        Tree(T key) : value(key) { child[0]=NULL; child[1]=NULL; red=true; balance=0; }
    };
    
    Tree* Root;
    
protected:
    
    Tree* Make_Node(T key);
    
    Tree* Search_h(T key, Tree* root);
    
    Tree* Rotate(Tree* root, int dir, type mode);
    
    Tree* Double_Rotate(Tree* root, int dir, type mode);
    
    Tree* Min_h(Tree* root);
    
    Tree* Max_h(Tree* root);
    
    Tree* insert_h(Tree* root, T key, int *done, type mode);
    
    Tree* remove_h(Tree* root, T key, int *status, type mode);
    
    Tree* delete_fix(Tree* root, int dir, int *flag);
    
    Tree* tree_balance(Tree* root, int dir);
    
    void Bal_adj(Tree* root, int dir, int bal);
    
    bool isRed(Tree* root);
    
    void print_h(Tree* root);
    
};


template <class T>
class AVL : public BST<T>
{

public:
    
    AVL();
    
    bool Search(T key);
    
    virtual void Insert(T key);
    
    virtual void Remove(T key);
    
    void Print();
    
    T Maximum();
    
    T Minimum();
};


template <class T>
class RBT :  public AVL<T>
{

public:
    
    RBT();
    
    void Insert(T key);
    
    void Remove(T key);
    
};


