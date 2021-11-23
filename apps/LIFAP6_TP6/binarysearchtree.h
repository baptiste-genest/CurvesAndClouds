#ifndef BINARYSEARCHTREE_H
#define BINARYSEARCHTREE_H

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <functional>

using element = int;
inline std::string str(element x){
    std::stringstream s;
    s << x;
    return s.str();
}

class BSTNode
{
public:
    BSTNode();
    ~BSTNode(){
        if (left){
            delete left;
            left = nullptr;
        }
        if (right){
            delete right;
            left = nullptr;
        }
    }

private:
    std::vector<std::string> get_lines() const;
    void insert(element x);
    bool belong(element x) const;
    BSTNode* copy() const;
    std::string infix() const;
    void prefix() const;
    void postfix() const;
    BSTNode(element x) : value(x){}
    BSTNode* left = nullptr;
    BSTNode* right = nullptr;
    element value;
    friend class BinarySearchTree;
};

class BinarySearchTree {
public:
    BinarySearchTree() {}
    inline BinarySearchTree(const BinarySearchTree& other){
        if (head){
            delete head;
            head = nullptr;
        }
        if (other.head)
            head = other.head->copy();
    }
    void operator=(const BinarySearchTree& other){
        if (head)
            delete head;
        if (other.head)
            head = other.head->copy();
    }

    inline bool belong(element x) const {
        if (!head)
            return false;
        return head->belong(x);
    }

    inline void insert(element x){
        if (!head)
            head = new BSTNode(x);
        else
            head->insert(x);
    }
    inline void postfixPrint() const{
        if (head)
            head->postfix();
        std::cout << std::endl;

    }
    void infixPrint() const{
        if (head)
            std::cout << head->infix() << std::endl;
    }
    void prefixPrint() const{
        if (head)
            head->prefix();
    }

    inline void clear(){
        delete head;
        head = nullptr;
    }
    ~BinarySearchTree() {
        clear();
    }

    void tree_print() const;
private:
    BSTNode* head = nullptr;

};

#endif // BINARYSEARCHTREE_H
