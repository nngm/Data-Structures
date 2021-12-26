#include "Tree.h"

template <class keyT, class valT>
class AVLTree_t : public Tree_t<keyT, valT> {
    long check_bf(Node_t<keyT, valT>* n) {
        // Checks balance factor values in tree.
        long hr, hl, h;
        hr = hl = 0;
        if(!n) return 0;
        hr = check_bf(n->right);
        hl = check_bf(n->left);
        if(hr > hl) h = hr;
        else h = hl;
        h += 1;
        long bf = hl - hr;
        assert(bf == n->meta);
        assert(bf < 2);
        assert(bf > -2);
        return h;
    }

    // Optional private functions.
    // These are here to guide you, but you can just ignore these.
    void balance(Node_t<keyT, valT>* n) {
        // Make tree balanced
        // std::cout<<"starting balancing" << std::endl;
        update_bf(this->root);
        // std::cout<<"update finished" << std::endl;

        Node_t<keyT, valT> *A_node = n;

        for (; A_node; A_node = A_node->parent)
            if (A_node->meta == 2 || A_node->meta == -2)
                break;
        
        // std::cout<<"check A_node"<<std::endl;
        
        if (!A_node)
            return;

        // std::cout << "A_node found"<<std::endl;

        // std::cout << "A_node: " << A_node->key << ", " << A_node->value << std::endl;

        const bool LEFT = false, RIGHT = true;

        if (A_node->meta == 2) {
            if (A_node->left->meta == -1)
                this->rotate(A_node->left, LEFT);
            this->rotate(A_node, RIGHT);
        }
        if (A_node->meta == -2) {
            if (A_node->right->meta == 1)
                this->rotate(A_node->right, RIGHT);
            this->rotate(A_node, LEFT);
        }

        update_bf(this->root);
    }

    Node_t<keyT, valT>* insert_internal(keyT key, valT value) {
        // Helper function for insert()
        return Tree_t<keyT, valT>::insert_internal(key, value);
    }

    Node_t<keyT, valT>* remove_node(Node_t<keyT, valT>* n) {
        // Helper function for remove()
        if(n->left && n->right) {
            Node_t<keyT, valT>* r = this->get_min(n->right);
            n->key = r->key;
            n->value = r->value;
            return remove_node(r);
        } else if (n->left) {
            n->left->parent = n->parent;
            if(n->parent) {
                if(n == n->parent->left) n->parent->left = n->left;
                else n->parent->right = n->left;
            }
            if(n == this->root) {
                this->root = n->left;
                n->left->parent = nullptr;
            }
            
        } else if (n->right) {
            n->right->parent = n->parent;
            if(n->parent) {
                if(n == n->parent->left) n->parent->left = n->right;
                else n->parent->right = n->right;
            }
            if(n == this->root) {
                this->root = n->right;
                n->right->parent = nullptr;
            }
            
        } else {
            if(n->parent) {
                if(n == n->parent->left) n->parent->left = nullptr;
                else n->parent->right = nullptr;
            }
            if(n == this->root) this->root = nullptr;
            
        }
        auto ret = n->parent;
        delete n;
        return ret;
    }

    int height(Node_t<keyT, valT> *n) {
        if (!n)
            return 0;
        int left = height(n->left);
        int right = height(n->right);
        return left > right ? left + 1 : right + 1; 
    }

    void update_bf(Node_t<keyT, valT> *n) {
        if (!n)
            return;

        n->meta = height(n->left) - height(n->right);

        update_bf(n->left);
        update_bf(n->right);
    }

public:
    void insert(keyT key, valT value) {
        // If there is no node that has the given key,
        // create a new one, place it in the right place, and store the value.
        // If there already is a node that has the given key,
        // update the value, rather than making a new one.
        Node_t<keyT, valT> *new_node = insert_internal(key, value);

        balance(new_node);
    }

    bool remove(keyT key) {
        // Find the node that has the given key and remove that node.
        if(!this->root) return false;
        Node_t<keyT, valT>* n = this->search_subtree(this->root, key), *new_node;
        if (n && n->key == key) {
            new_node = remove_node(n);
            assert(this->check_order());
        }
        else return false;

        balance(new_node);

        return true;
    }
};
