#ifndef INTERVAL_TREE_H
#define INTERVAL_TREE_H

#include "cnc_types.h"
#include "../geometry/combinatorial_complexes.h"
#include <vector>
#include <set>

namespace cnc {

namespace algo {

namespace data_structure {


// class representing the node of interval tree
template<class T>
struct IntervalTreeNode{
    range R;
    scalar max;
    IntervalTreeNode* left;
    IntervalTreeNode* right;
    T value;

    IntervalTreeNode(range r,T val) {
        R = r;
        value = val;
        max = r.second;
        left = right = nullptr;
    }
    ~IntervalTreeNode() {
        if (left != nullptr)
            delete left;
        if (right != nullptr)
            delete right;
    }
};

template<class T>
class IntervalTree{
private:
    scalar eps = 1e-10;

    IntervalTreeNode<T>* root = nullptr;

    // Function to create a new Node
    static IntervalTreeNode<T>* newNode(range R,T val) {
        return new IntervalTreeNode<T>(R,val);

    }
    // Function to insert an scalarerval in scalarerval tree
    IntervalTreeNode<T>* insert(IntervalTreeNode<T>* root, range R,T val) {
        if (root == nullptr) {
            return newNode(R,val);
        }

        if (R.first< root->R.first) {
            root->left = insert(root->left,R,val);
        } else if (R.first> root->R.first) {
            root->right = insert(root->right,R,val);
        } else {
            if (R.second< root->R.second) {
                root->left = insert(root->left,R,val);
            } else {
                root->right = insert(root->right,R,val);
            }
        }

        // If current node's max is less than r, then update max
        if (root->max <R.second) {
            root->max =R.second;
        }

        return root;
    }

    static bool checkOverlap(IntervalTreeNode<T>* root, range R) {
        // If current node is null, return false
        if (root == nullptr) {
            return false;
        }

        // If overlaps return true
        if (root->R.first<=R.second&&R.first<= root->R.second) {
            return true;
        }

        // If max value of current is greater than starting point of I(l)
        // search in left subtree
        if (root->left != nullptr && root->left->max >=R.first) {
            return checkOverlap(root->left,R);
        }

        // Else search in right subtree
        return checkOverlap(root->right,R);
    }

    static std::vector<IntervalTreeNode<T>*> checkOverlap(IntervalTreeNode<T>* root, scalar x) {
        // If current node is null, return false
        if (root == nullptr) {
            return {};
        }
        std::vector<IntervalTreeNode<T>*> rslt,sub;

        // If overlaps return true
        if (root->R.first < x && x < root->R.second ) {
            rslt.push_back(root);
        }

        // If max value of current is greater than starting point of I(l)
        // search in left subtree
        if (root->left != nullptr && root->left->max > x)
            sub = checkOverlap(root->left,x);
        else
            sub = checkOverlap(root->right,x);
        rslt.insert(rslt.begin(),sub.begin(),sub.end());
        return rslt;
    }


    // Function to delete a binary tree, same as normal delete in a BST
    IntervalTreeNode<T>* deleteInterval(IntervalTreeNode<T>* root, IntervalTreeNode<T>* node) {
        if (root == nullptr) {
            return nullptr;
        }

        const auto& R = node->R;

        if (node->R.first < root->R.first-eps) {
            root->left = deleteInterval(root->left,node);
        } else if (R.first-eps> root->R.first) {
            root->right = deleteInterval(root->right,node);
        } else {
            if (R.second< root->R.second-eps) {
                root->left = deleteInterval(root->left,node);
            } else if (R.second -eps> root->R.second) {
                root->right = deleteInterval(root->right,node);
            } else {
                // This is the interval to be deleted
                if (root->left == nullptr)
                    return root->right;
                else if (root->right == nullptr)
                    return root->left;

                // Find the minimum value in the right subtree of root
                IntervalTreeNode<T>* curr = root->right;
                while (curr->left != nullptr) {
                    curr = curr->left;
                }
                root->R.first= curr->R.first;
                root->R.second= curr->R.second;

                root->right = deleteInterval(root->right,node);
            }
        }

        return root;
    }

    static std::vector<std::string> get_lines(IntervalTreeNode<T>* node)
    {
        static auto void_fill = [] (uint n){
            std::string r;
            for (uint k = 0;k<n;k++)
                r += ' ';
            return r;
        };
        using lines = std::vector<std::string>;
        std::string val = "[" + std::to_string(node->R.first) + "," + std::to_string(node->R.second) + "]";
        lines LL,RL;
        if (node->left)
            LL = get_lines(node->left);
        if (node->right)
            RL = get_lines(node->right);
        uint height = std::max(LL.size(),RL.size());
        if (height == 0)
            return {val};
        LL.resize(height);    RL.resize(height);

        auto mid_fill = void_fill(val.size());
        lines final(height+1);
        final[0] = void_fill(LL[0].size()) + val + void_fill(RL[0].size());
        for (uint k = 0;k<height;k++)
            final[k+1] = LL[k] + mid_fill + RL[k];
        return final;
    }
public:
    IntervalTree(){}
    ~IntervalTree(){
        if (root != nullptr)
            delete root;
    }
    IntervalTreeNode<T>* insert(range r,T val){
        if (root == nullptr){
            root = newNode(r,val);
            return root;
        }
        else{
            return insert(root,r,val);
        }
    }
    bool checkOverlap(range R) const {
        if (root == nullptr)
            return false;
        return checkOverlap(root,R);
    }
    std::vector<IntervalTreeNode<T>*> checkOverlap(scalar x) const {
        if (root == nullptr)
            return {};
        return checkOverlap(root,x);
    }
    std::set<T> checkOverlapValues(scalar x) const {
        if (root == nullptr)
            return {};
        auto O = checkOverlap(root,x);
        std::set<T> S;
        for (auto x : O)
            S.insert(x->value);
        return S;
    }
    IntervalTreeNode<T>* deleteInterval(IntervalTreeNode<T>* node) {
        root = deleteInterval(root,node);
    }
    void print() const{
        auto L = get_lines(root);
        for (const auto& l : L)
            std::cout << l << std::endl;
    }
};
}

}

}


#endif // INTERVAL_TREE_H
