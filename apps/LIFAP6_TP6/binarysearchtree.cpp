#include "binarysearchtree.h"

BSTNode::BSTNode()
{

}

void BinarySearchTree::tree_print() const
{
    auto L = head->get_lines();
    for (const auto& l : L)
        std::cout << l << std::endl;
}

std::vector<std::string> BSTNode::get_lines() const
{
    static auto void_fill = [] (uint n){
        std::string r;
        for (uint k = 0;k<n;k++)
            r += ' ';
        return r;
    };
    using lines = std::vector<std::string>;
    std::string val = str(value);
    lines final,LL,RL;
    if (left)
        LL = left->get_lines();
    if (right)
        RL = right->get_lines();
    uint height = std::max(LL.size(),RL.size());
    if (height == 0)
        return {val};
    LL.resize(height);    RL.resize(height);

    auto mid_fill = void_fill(val.size());
    final.resize(height+1);
    final[0] = void_fill(LL[0].size()) + str(value) + void_fill(RL[0].size());
    for (uint k = 0;k<height;k++)
        final[k+1] = LL[k] + mid_fill + RL[k];
    return final;
}

void BSTNode::insert(element x)
{
    if (x == value)
        return;
    if (x < value){
        if (left)
            left->insert(x);
        else
            left = new BSTNode(x);
    }
    if (x > value){
        if (right)
            right->insert(x);
        else
            right = new BSTNode(x);
    }
}

BSTNode *BSTNode::copy() const
{
    BSTNode* c = new BSTNode(value);
    if (left)
        c->left = left->copy();
    if (right)
        c->right = right->copy();
    return c;
}

std::string BSTNode::infix() const
{
    std::string to_print = " " + str(value) + " ";
    if (left)
        to_print = left->infix() + to_print;
    if (right)
        to_print += right->infix();
    return to_print;
}

void BSTNode::prefix() const
{
    std::cout << ' ' << value << ' ';
    if (left)
        left->prefix();
    if (right)
        right->prefix();
}

void BSTNode::postfix() const
{
    if (left)
        left->postfix();
    if (right)
        right->postfix();
    std::cout << ' ' << value << ' ';
}

bool BSTNode::belong(element x) const
{
    if (x == value)
        return true;
    if (x < value){
        if (left)
            return left->belong(x);
    }
    if (x > value){
        if (right)
            return right->belong(x);
    }
    return false;
}

