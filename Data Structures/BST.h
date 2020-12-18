#pragma once
#include <initializer_list>
#include <iterator>
#include <utility>
#include <cassert>

template <typename T>
class BST {
protected:
    struct Node {
        T mData;
        Node* mParent;
        Node* mLeft;
        Node* mRight;
        Node(const T& Data, Node* Parent = nullptr, Node* Left = nullptr, Node* Right = nullptr)
            : mData(Data), mParent(Parent), mLeft(Left), mRight(Right)
        {

        }
    };

    /*
    Find minimal element in the tree with given root.
    */
    Node* min_element(Node* root)
    {
        if (root == nullptr)
            return root;

        Node* cur = root;
        while (cur->mLeft)
            cur = cur->mLeft;
        return cur;
    }

    /*
    Find maximal element in the tree with given root.
    */
    Node* max_element(Node* root)
    {
        if (root == nullptr)
            return root;

        Node* cur = root;
        while (cur->mRight)
            cur = cur->mRight;
        return cur;
    } 

    /*
    Find successor for the given node.
    1. If right subtree of node is not nullptr, then successor is the minimum element of right subtree.
    2. If right subtree of node is nullptr, then successor is one of the ancestors. 
    Travel up using the parent pointer until you see a node which is left child of its parent. The parent of such a node is the successor.
    */
    Node* successor(Node* node)
    {
        if (node == nullptr)
            return node;

        if (node->mRight)
            return min_element(node->mRight);
        
        Node* cur = node;
        
        while (cur->mParent && cur->mParent->mLeft != cur)
            cur = cur->mParent;
        return cur->mParent;
    }

    /*
    Finds the node with a given value, returns nullptr if such node doesn't exist.
    */
    Node* find_helper(const T& value)
    {
        Node* cur = mRoot;

        while (cur && cur->mData != value)
        {
            if (cur->mData < value)
                cur = cur->mRight;
            else
                cur = cur->mLeft;
        }

        return cur;
    }

    /*
    Inserts a new node in the tree with a given value. Returns newly created node.
    */
    Node* insert_helper(const T& value) 
    {
        if (empty())
        {
            ++mSize;
            return mRoot = new Node(value);
        }

        Node* cur = mRoot;
        Node* prev = nullptr;

        while (cur)
        {
            prev = cur;
            if (cur->mData < value)
                cur = cur->mRight;
            else
                cur = cur->mLeft;
        }

        if (prev->mData < value)
            prev->mRight = new Node(value, prev);
        else
            prev->mLeft = new Node(value, prev);

        ++mSize;
        return cur;
    }

    /*
    Find predecessor for the given node.
    1. If left subtree of node is not nullptr, then predecessor is the maximum element of left subtree.
    2. If left subtree of node is nullptr, then predecessor is one of the ancestors. 
    Travel up using the parent pointer until you see a node which is right child of its parent. The parent of such a node is the predecessor.
    */
    Node* predecessor(Node* node)
    {
        if (node == nullptr)
            return node;

        if (node->mLeft)
            return max_element(node->mLeft  );

        Node* cur = node;

        while (cur->mParent && cur->mParent->mRight != cur)
            cur = cur->mParent;
        return cur->mParent;
    }

    /*
    Erase given node from tree.
    1. If node is the root:
        1.1. If the root has no children, delete it and set mRoot to nullptr;
        1.2. If the root has a right child, set the root to that child and hang the the left subtree (if it's not empty)
        from the minimal element of the right subtree;
        1.3. Otherwise root has only a left child, set the root to it.
    2. If node is leaf (has no children), simply remove it from the tree.
    3. If node has only one child, set that child as the new child for node's parent and delete the node.
    4. If node has two children, find successor (predecessor) of the node. Copy it's data to the node and delete the successor (predecessor).
    */
    void erase_helper(Node* node)
    {
       if (node == mRoot)
       {
           if (mSize == 1)
           {
               delete node;
               --mSize;
               mRoot = nullptr;
               return;
           }
           
           Node* left = node->mLeft;
           Node* right = node->mRight;

           if (right)
           {
               if (left)
               {
                   Node* rmin = min_element(right);
                   rmin->mLeft = left;

                   left->mParent = rmin;
               }
               
               mRoot = right;
           }
           else
           {
               mRoot = left;
           }

           mRoot->mParent = nullptr;
           delete node;
           --mSize;
       }
       
       else if(node->mLeft == nullptr && node->mRight == nullptr)
       {
           Node* parent = node->mParent;

           if (parent->mLeft == node)
               parent->mLeft = nullptr;
           else
               parent->mRight = nullptr;

           delete node;
           --mSize;
       }

       else if (node->mLeft == nullptr || node->mRight == nullptr)
       {
           Node* child;
           if (node->mLeft)
               child = node->mLeft;
           else
               child = node->mRight;

           if (node->mParent->mLeft == node)
               node->mParent->mLeft = child;
           else
               node->mParent->mRight = child;

           child->mParent = node->mParent;

           delete node;
           --mSize;
       }

       else
       {
           Node* next = successor(node);
           node->mData = next->mData;
           erase_helper(next);
       }
    }

    /*
    Delete the tree with given root recursively (to be used in destructor).
    */
    void delete_node_recursive(Node* node)
    {
        if (node == nullptr)
            return;

        delete_node_recursive(node->mLeft);
        delete_node_recursive(node->mRight);
        delete node;
    }
public:
    struct BST_iterator {
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = T;
        using difference_type = int;
        using pointer = T*;
        using reference = T&;

        /*
        Constructor for iterator.
        */
        explicit BST_iterator(Node* node = nullptr, BST* bst = nullptr) : mNode(node), mBst(bst)
        {

        }


        /*
        Dereference operator overloading. Returns underlying objects data.
        If object is not valid throw assertion.
        */
        T& operator*()
        {
            assert(mNode);
            return mNode->mData;
        }

        /*
        Assignment operator overloading. Change underlying object to the rhs object.
        */
        BST_iterator& operator=(const BST_iterator& rhs) 
        {
            mNode = rhs.mNode;
            mBst = rhs.mBst;
            return *this;   
        }

        /*
        Check if underlying objects are the same.
        */
        bool operator==(const BST_iterator& rhs) const
        {
            return mNode == rhs.mNode && mBst == rhs.mBst;
        }
        
        /*
        Check if underlying objects are not the same.
        */
        bool operator!=(const BST_iterator& rhs) const
        {
            return mNode != rhs.mNode || mBst != rhs.mBst;
        }   

        /*
        Preincrement operator overloading. After this iterator should point to its successor element. If object is not valid throw assertion.
        Return reference to target (*this) iterator.
        */
        BST_iterator& operator++()
        {
            assert(mNode);
            mNode = mBst->successor(mNode);
            return *this;
        }

        /*
        Postincrement operator overloading. After this iterator should point to its successor element. If object is not valid throw assertion.
        Return targets value before this operation.
        */
        BST_iterator operator++(int)
        {
            assert(mNode);
            BST_iterator temp = *this;
            operator++();
            return temp;
        }

        /*
        Predecrement operator overloading. After this iterator should point to its predecessor element. If object is not valid throw assertion.
        Return reference to target (*this) iterator.
        */
        BST_iterator& operator--()
        {
            assert(mNode);
            mNode = mBst->predecessor(mNode);
            return *this;
        }

        /*
        Postdecrement operator overloading. After this iterator should point to its predecessor element. If object is not valid throw assertion.
        Return targets value before this operation.
        */
        BST_iterator operator--(int)
        {
            assert(mNode);
            BST_iterator temp = *this;
            operator--();
            return temp;
        }

        Node* mNode;
        BST* mBst;
    };

protected:
    using iterator = BST_iterator;

    /*
    min_element version with iterators.
    */
    iterator min_element(iterator root)
    {
        return iterator(min_element(root.mNode));
    }

    /*
    max_element version with iterators.
    */
    iterator max_element(iterator root)
    {
        return iterator(max_element(root.mNode));
    }

    /*
    successor version with iterators.
    */
    iterator successor(iterator it)
    {
        return iterator(successor(it.mNode));
    }

    /*
    predecessor version with iterators.
    */
    iterator predecessor(iterator it)
    {
        return iterator(predecessor(it.mNode));
    }
public:
    /*
    Constructs an empty tree.
    */  
    BST() : mRoot(nullptr), mSize(0)
    {

    }

    /*
    Constructs a tree from given elements.
    */
    BST(const std::initializer_list<T>& init) : mRoot(nullptr), mSize(0)
    {
        for (const T& elem : init)
            insert(elem);
    }

    /*
    find (find_helper) version with iterators.
    */
    iterator find(const T& value)
    {
        return iterator(find_helper(value), this);
    }

    /*
    insert (insert_helper) version with iterators. As we let user to add more than one copy of the same value, second member of pair is always true.
    */
    std::pair<iterator, bool> insert(const T& value)
    {
        return std::make_pair(iterator(insert_helper(value), this), true);
    }

    /*
    erase (erase_helper) version with iterators.
    */
    void erase(iterator pos)
    {
        erase_helper(pos.mNode);
    }

    /*
    Returns an iterator to the first element of the BST.
    */
    iterator begin()
    {
        return iterator(min_element(mRoot), this);
    }

    /*
    Returns an iterator to the element after the last element of the BST.
    */
    iterator end()
    {
        return iterator(nullptr, this);
    }

    /*
    Checks if the container has no elements.
    */
    bool empty() const
    {
        return mSize == 0;
    }

    /*
    Returns the number of elements in the container.
    */
    size_t size() const
    {
        return mSize;
    }

    /*
    Destructs the tree.
    */
    ~BST()
    {
        delete_node_recursive(mRoot);
    }
private:
    Node* mRoot;
    size_t mSize;
};