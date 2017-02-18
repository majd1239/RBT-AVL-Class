//
//  BST.cpp
//  Void
//
//  Created by Majd Takieddine on 2/2/17.
//  Copyright © 2017 Majd Takieddine. All rights reserved.
//

#include "BST.hpp"


template<class T>
typename BST<T>::Tree* BST<T>::Make_Node(T key)
{
    Tree *Node = new Tree(key); return Node;
}

template<class T>
bool BST<T>::isRed(BST::Tree *root)
{
    return root && root->red;
}

template<class T>
typename BST<T>::Tree* BST<T>::Search_h(T key, Tree* root)
{
    return root ? root->value==key ? root : Search_h(key,root->child[key>root->value]) : NULL;
}


template<class T>
typename BST<T>::Tree* BST<T>::Rotate(Tree* root, int dir, type mode)
{
    Tree *temp=root->child[!dir];
    
    root->child[!dir]=temp->child[dir];
    
    temp->child[dir]=root;
    
    if(mode==RBT)
    {
        root->red=true;
        temp->red=false;
    }
    
    return temp;
}


template<class T>
typename BST<T>::Tree* BST<T>::Double_Rotate(Tree* root, int dir, type mode)
{
    root->child[!dir]=Rotate(root->child[!dir],!dir,mode);
    
    return Rotate(root, dir,mode);
}

template<class T>
typename BST<T>::Tree* BST<T>::Min_h(Tree* root)
{
    return !root->child[0] ? root : Min_h(root->child[0]);
}

template<class T>
typename BST<T>::Tree* BST<T>::Max_h(Tree* root)
{
    return !root->child[1] ? root : Max_h(root->child[1]);
}

template<class T>
void BST<T>::Bal_adj(BST<T>::Tree* root, int dir, int bal)
{
    typename BST<T>::Tree* grand_child= root->child[dir]->child[!dir];
    
    if(grand_child->balance ==0)
        root->balance=root->child[dir]->balance=0;
    
    else if(grand_child->balance == bal)
    {
        root->balance=-bal;
        
        root->child[dir]->balance=0;
        
    }
    else
    {
        root->balance=0;
        
        root->child[dir]->balance=bal;
    }
    
    grand_child->balance=0;
}

template<class T>
typename BST<T>::Tree* BST<T>::tree_balance(BST<T>::Tree* root, int dir)
{
    int  bal= dir ? 1 : -1;
    
    if(root->child[dir]->balance==bal)
    {
        root->balance=root->child[dir]->balance=0;
        
        root=Rotate(root, !dir,AVL);
        
    }
    else if(root->child[dir]->balance==0)
    {
        root->child[dir]->balance=-bal;
        
        root=Rotate(root, !dir, AVL);
    }
    else
    {
        Bal_adj(root, dir, bal);
        
        root=Double_Rotate(root, !dir, AVL);
    }
    
    return root;
}

template<class T>
typename BST<T>::Tree* BST<T>::insert_h(Tree* root, T key, int *done, type mode)
{
    if(!root)
        root=Make_Node(key);
    else
    {   int dir=key>root->value;
        
        root->child[dir]=insert_h(root->child[dir],key,done,mode);
        
        
        if(mode==RBT)
        {
            if(isRed(root->child[dir]))
            {
                if(isRed(root->child[!dir]))
                {
                    root->red=true;
                    
                    root->child[0]->red=root->child[1]->red=false;
                }
                
                else
                {
                    if(isRed(root->child[dir]->child[dir]))
                        root = Rotate(root,!dir, RBT);
                    
                    else if(isRed(root->child[dir]->child[!dir]))
                        root = Double_Rotate(root, !dir, RBT);
                }
            }
        }
        else
        {
            if(!*done)
            {
                root->balance += dir ? 1 : -1 ;
                
                if(root->balance==0)
                    *done=true;
                
                if(abs(root->balance)>1)
                {
                    root=tree_balance(root, dir);
                    *done=true;
                }
            }
        }
    }
    
    return root;
}

template<class T>
void BST<T>::print_h(Tree* root)
{
    if(!root) return;
    else
    {
        print_h(root->child[0]);
        cout<<root->value<<" ";
        print_h(root->child[1]);
    }
}


template<class T>
typename BST<T>::Tree* BST<T>::delete_fix(Tree* root,int dir,int *flag)
{
    Tree* sib=root->child[!dir], *par=root;
    
    if(isRed(sib))
    {
        root=Rotate(root,dir,RBT);  sib=par->child[!dir];
    }
    if(sib)
    {
        if(!isRed(sib->child[1]) && !isRed(sib->child[0]))
        {
            if(isRed(par)) *flag=1;
            
            par->red=false;
            sib->red=true;
            
        }
        else
        {
            bool color=par->red; bool case_new_par = root==par;
            
            par = isRed(sib->child[!dir]) ? Rotate(par,dir,RBT) : Double_Rotate(par, dir, RBT);
            
            par->red = color; par->child[0]->red = par->child[1]->red = false;
            
            case_new_par ? root = par : root->child[dir] = par;
            
            *flag=1;
            
        }
    }
    return root;
}



template<class T>
typename BST<T>::Tree*  BST<T>::remove_h(Tree* root, T key, int *done, type mode)
{
    Tree* succ;
    
    if(!root)
        cout<<"Key does not exist"<<endl;
    
    else if(key==root->value)
    {
        if(root->child[0] && root->child[1])
        {
            succ=Min_h(root->child[1]);
            root->value=succ->value;
            root->child[1]=remove_h(root->child[1], succ->value, done, mode);
        }
        else
        {
            succ=root;
            
            root=root->child[!root->child[0]];
            
            if(mode==RBT)
            {
                if (isRed(succ) || (isRed(root) && (root->red=false)) ) *done=1;
                
            }
            
            delete succ;
        }
    }
    else
    {
        int dir=key>root->value;
        
        root->child[dir]=remove_h(root->child[dir], key, done, mode);
        
        if(mode==RBT && !*done)
            root=delete_fix(root, dir, done);
        
        if(mode==AVL)
        {
            if(!*done)
            {
                dir ? --root->balance : ++root->balance;
                
                if(abs(root->balance)>1)
                    root=tree_balance(root, !dir);
                
                if(root->balance!=0)
                    *done=1;
            }
        }
    }
    return root;
}

template<class T>
AVL<T>::AVL(){  BST<T>::Root=NULL; }

template<class T>
bool AVL<T>::Search(T key)  {
    return BST<T>::Search_h(key, BST<T>::Root) ? true : false;
}

template<class T>
T AVL<T>::Maximum()  {
    return BST<T>::Max_h(BST<T>::Root)->value;
}

template<class T>
T AVL<T>::Minimum() {
    return BST<T>::Min_h(BST<T>::Root)->value;
}

template<class T>
void AVL<T>::Insert(T key)
{
    int status=0;
    BST<T>::Root=BST<T>::insert_h( BST<T>::Root, key, &status,BST<T>::AVL);
}


template<class T>
void AVL<T>::Print()
{
    if(BST<T>::Root) BST<T>::print_h(BST<T>::Root); else cout<<"Tree is empty";
}


template<class T>
void AVL<T>::Remove(T key)
{
    int status=0;
    
    BST<T>::Root=BST<T>::remove_h(BST<T>::Root, key, &status,BST<T>::AVL);
}


template<class T>
RBT<T>::RBT(){  BST<T>::Root=NULL; }

template<class T>
void RBT<T>::Insert(T key)
{
    int status=0;
    BST<T>::Root=BST<T>::insert_h(BST<T>::Root, key, &status,BST<T>::RBT);
    
    BST<T>::Root->red=false;
}

template<class T>
void RBT<T>::Remove(T key)
{
    int status=0;
    
    BST<T>::Root=BST<T>::remove_h(BST<T>::Root, key, &status,BST<T>::RBT);
    
    if(BST<T>::Root)
        BST<T>::Root->red=false;
}

/* Template instantiations */
template class AVL<char>;
template class RBT<char>;
template class AVL<short>;
template class RBT<short>;
template class AVL<int>;
template class RBT<int>;
template class AVL<long>;
template class RBT<long>;
template class AVL<double>;
template class RBT<double>;
template class AVL<float>;
template class RBT<float>;






