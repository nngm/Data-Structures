#include "Tree.h"

#define BLACK 0
#define RED 1

template <class keyT, class valT>
class RBTree_t : public Tree_t<keyT, valT> {
    size_t check_rb(Node_t<keyT, valT>* n) {
        // Checks RB tree property
        size_t b, br, bl;
        br = bl = b = 1;
        assert(n);
        if(n && n->right) {
            br = check_rb(n->right);
            assert(n->meta == BLACK || n->right->meta == BLACK);
        }
        if(n && n->left) {
            bl = check_rb(n->left);
            assert(n->meta == BLACK || n->left->meta == BLACK);
        }
        b = br;
        if(n == this->root && br != bl) {
            //dump_subtree(this->root);
        }
        assert(n != this->root || br == bl);
        b = br;
        if(n->meta == BLACK) b += 1;
        if(n == this->root) assert(n->meta == BLACK);

        return b;
    }

    // Optional private functions.
    // These are here to guide you, but you can just ignore these.
    void balance(Node_t<keyT, valT>* p) {
        // Make tree balanced
        if (p == this->root)
            return;
        if (p->meta != RED)
            return;

        const bool LEFT = false, RIGHT = true;

        Node_t<keyT, valT> *pp = p->parent;
        if (pp->meta == RED) {
            Node_t<keyT, valT> *gp = pp->parent, *uncle;
            uncle = (gp->left == pp) ? gp->right : gp->left;

            if (uncle && uncle->meta == RED) {          // XYr
                pp->meta = BLACK;
                gp->meta = RED;
                flip(uncle);
                balance(gp);
            }
            else {
                if (gp->left == pp && pp->left == p) {  // LLb
                    gp->meta = RED;
                    pp->meta = BLACK;
                    this->rotate(gp, RIGHT);
                }
                if (gp->right == pp && pp->right == p) {// RRb
                    gp->meta = RED;
                    pp->meta = BLACK;
                    this->rotate(gp, LEFT);
                }
                if (gp->left == pp && pp->right == p) { // LRb
                    gp->meta = RED;
                    p->meta = BLACK;
                    this->rotate(pp, LEFT);
                    this->rotate(gp, RIGHT);
                }
                if (gp->right == pp && pp->left == p) { // RLb
                    gp->meta = RED;
                    p->meta = BLACK;
                    this->rotate(pp, RIGHT);
                    this->rotate(gp, LEFT);
                }
            }
        }
    }

    void flip(Node_t<keyT, valT>* n) {
        // Flip the color of this node and all children
        if (!n)
            return;
        n->meta = !n->meta;
        flip(n->left);
        flip(n->right);
    }

    void resolve_double_black(Node_t<keyT, valT>* x, Node_t<keyT, valT>* y) {
        if (!y)
            return;
        
        assert(x->meta == BLACK);
        
        Node_t<keyT, valT> *v, *z, *w;
        bool v_direction;
        if (y->left == x) {
            v = y->right;
            v_direction = true;
            assert(v);
            z = v->left;
            w = v->right;
        }
        else {
            v = y->left;
            v_direction = false;
            assert(v);
            z = v->right;
            w = v->left;
        }

        if (y->meta == BLACK && z->meta == BLACK && w->meta == BLACK) {
            if (v->meta == BLACK) {
                // case 0
                v->meta = RED;
                resolve_double_black(y, y->parent);
            }
            else {
                // case 1
                y->meta = RED;
                v->meta = BLACK;
                this->rotate(y, !v_direction);
                resolve_double_black(x, y);
            }
        }
        else if (y->meta == RED && v->meta == w->meta && z->meta == BLACK) {
            // case 2
            y->meta = BLACK;
            v->meta = RED;
        }
        else if (v->meta == w->meta && z->meta == RED) {
            // case 3
            v->meta = RED;
            z->meta = BLACK;
            this->rotate(v, v_direction);
            resolve_double_black(x, y);
        }
        else if (v->meta == BLACK && w->meta == RED) {
            // case 4
            w->meta = BLACK;
            this->rotate(y, !v_direction);
        }
    }

    void remove_node(Node_t<keyT, valT>* n) {
        // Helper function for remove()
        int deleted_color = n->meta;
        Node_t<keyT, valT>* new_node;
        if(n->left && n->right) {
            Node_t<keyT, valT>* r = this->get_min(n->right);
            n->key = r->key;
            n->value = r->value;
            remove_node(r);
            return;
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
            new_node = n->left;
            delete n;
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
            new_node = n->right;
            delete n;
        } else {
            if(n->parent) {
                if(n == n->parent->left) n->parent->left = nullptr;
                else n->parent->right = nullptr;
            }
            if(n == this->root) this->root = nullptr;
            new_node = nullptr;
            delete n;
        }

        if (deleted_color == RED)
            return;
        
        if (new_node && new_node->meta == RED)
            new_node->meta = BLACK;
        else
            resolve_double_black(new_node, new_node->parent);
    }


public:
    void insert(keyT key, valT value) {
        // If there is no node that has the given key,
        // create a new one, place it in the right place, and store the value.
        // If there already is a node that has the given key,
        // update the value, rather than making a new one.
        if (!this->root) {
            this->insert_internal(key, value, BLACK);
        }
        else {
            Node_t<keyT, valT> *new_node = this->insert_internal(key, value, RED);
            balance(new_node);
        }
        this->root->meta = BLACK;
    }


    bool remove(keyT key) {
        // Find the node that has the given key and remove that node.
        if(!this->root) return false;
        Node_t<keyT, valT>* n = this->search_subtree(this->root, key);
        if (n && n->key == key) {
            remove_node(n);
            assert(this->check_order());
            return true;
        }
        else return false;
    }

};
