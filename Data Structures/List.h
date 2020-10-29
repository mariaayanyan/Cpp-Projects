#include <cassert>
#include <algorithm>

template <typename T>
class List {
private:
    /*
    Class for representing each element in the container.
    */
    struct Node {
        T mData;
        Node* mNext;
        Node* mPrev;

        /*
        Constructor for Node.
        */
        Node(const T& value = {}, Node* next = nullptr, Node* prev = nullptr) : mData(value), mNext(next), mPrev(prev)
        {

        }
    };

    /*
    Class for representing containers iterator.
    */
    struct List_iterator {
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = T;
        using difference_type = int;
        using pointer = T*;
        using reference = T&;

        /*
        Constructor for iterator.
        */
        List_iterator(Node* node = nullptr) : mNode(node)
        {

        }

        /*
        Dereference operator overloading. Returns underlying objects data.
        If object is not valid throw assertion.
        */
        T& operator*()
        {
            assert(mNode != nullptr);
            return mNode->mData;
        }

        /*
        Assignment operator overloading. Change underlying object to the rhs object.
        */
        List_iterator& operator=(const List_iterator& rhs)
        {
            mNode = rhs.mNode;
        }

        /*
        Check if underlying objects are same.
        */
        bool operator==(const List_iterator& rhs) const
        {
            return mNode == rhs.mNode;
        }

        /*
        Check if underlying objects are not same.
        */
        bool operator!=(const List_iterator& rhs) const
        {
            return mNode != rhs.mNode;
        }

        /*
        Preincrement operator overloading. After this iterator should point to its next element. If object is not valid throw assertion.
        Return reference to target (*this) iterator.
        */
        List_iterator& operator++()
        {
            assert(mNode != nullptr);
            mNode = mNode->mNext;
            return *this;
        }

        /*
        Postincrement operator overloading. After this iterator should point to its next element. If object is not valid throw assertion.
        Return targets value before this operation.
        */
        List_iterator operator++(int)
        {
            assert(mNode != nullptr);
            List_iterator tmp = *this;
            mNode = mNode->mNext;
            return tmp;
        }

        /*
        Predecrement operator overloading. After this iterator should point to its previous element. If object is not valid throw assertion.
        Return reference to target (*this) iterator.
        */
        List_iterator& operator--()
        {
            assert(mNode != nullptr);
            mNode = mNode->mPrev;
            return *this;
        }

        /*
        Postdecrement operator overloading. After this iterator should point to its previous element. If object is not valid throw assertion.
        Return targets value before this operation.
        */
        List_iterator operator--(int)
        {
            assert(mNode != nullptr);
            List_iterator tmp = *this;
            mNode = mNode->mPrev;
            return tmp;
        }

        // data members
        Node* mNode;
    };

    typedef List_iterator iterator;
private:
    Node* mHead;
    Node* mTail;
    std::size_t mSize;

    /*
    Check if given position iterator valid. To be used for bounding checks in insert and erase.
    */
    bool is_position_valid(iterator pos)
    {
        for (iterator it = begin(); it != end(); ++it)
            if (it == pos)
                return true;
        return false;
    }
public:
    /*
    Default constructor for List.
    */
    List() : mHead(nullptr), mTail(nullptr), mSize(0)
    {

    }

    /*
    Constructor for List.
    */
    List(std::size_t count, const T& value = {}) : mHead(nullptr), mTail(nullptr), mSize(count)
    {
        if (count == 0)
            return;
        
        mHead = new Node(value);
        mTail = mHead;
       
        for (std::size_t i = 1; i < count; ++i)
        {
            Node* newNode = new Node(value, nullptr, mTail);
            mTail->mNext = newNode;
            mTail = newNode;
        }
    }

    /*
    Appends the given element value to the end of the container.
    */
    void push_back(const T& elem)
    {
        Node* newNode = new Node(elem, nullptr, mTail);

        if (empty())
        {
            mHead = mTail = newNode;
            ++mSize;
            return;
        }

        mTail->mNext = newNode;
        mTail = newNode;
        ++mSize;
    }

    /*
    Removes the last element of the container.
    If user tries to call pop_back on an empty container throw assertion.
    */
    void pop_back()
    {
        assert(mSize);
        --mSize;
        if (mSize == 0)
        {
            delete mTail;
            mHead = mTail = nullptr;
            return;
        }

        mTail = mTail->mPrev;
        delete mTail->mNext;
        mTail->mNext = nullptr;
    }

    /*
    Prepends the given element value to the beginning of the container.
    */
    void push_front(const T& elem)
    {
        Node* newNode = new Node(elem, mHead);
        
        if (empty())
        {
            mHead = mTail = newNode;
            ++mSize;
            return;
        }

        mHead->mPrev = newNode;
        mHead = newNode;
        ++mSize;
    }

    /*
    Removes the first element of the container.
    If user tries to call pop_front on an empty container throw assertion.
    */
    void pop_front()
    {
        assert(mSize);
        --mSize;
        if (mSize == 0)
        {
            delete mHead;
            mHead = mTail = nullptr;
            return;
        }

        mHead = mHead->mNext;
        delete mHead->mPrev;
        mHead->mPrev = nullptr;
    }

    /*
    Resizes the container to contain count elements.
    If the current size is greater than count, the container is reduced to its first count elements.
    If the current size is less than count, additional default-inserted elements are appended.
    */
    void resize(std::size_t count)
    {
        if (mSize <= count)
            while (mSize != count)
                push_back(T());
        else
            while (mSize != count)
                pop_back();
    }

    /*
    Erases all elements from the container. After this call, size() returns zero.
    */
    void clear()
    {
        while(mSize)
            pop_back();
    }

    /*
    Removes all elements satisfying specific criteria.
    The first version removes all elements that are equal to value, the second version removes all elements for which predicate p returns true.
    */
    std::size_t remove(const T& value)
    {
        std::size_t count = 0;
        for (iterator it = begin(); it != end();)
        {
            if (*it == value)
            {
                ++count;
                iterator tmp = it;
                ++it;
                erase(tmp);
            }
            else
                ++it;
        }
        return count;
    }

    template <typename UnaryPredicat>
    std::size_t remove_if(UnaryPredicat p)
    {
        std::size_t count = 0;
        for (iterator it = begin(); it != end();)
        {
            if (p(*it))
            {
                ++count;
                iterator tmp = it;
                ++it;
                erase(tmp);
            }
            else
                ++it;
        }
        return count;
    }

    /*
    Returns an iterator to the first element of the list.
    */
    iterator begin() const
    {
        return iterator(mHead);
    }

    /*
    Returns an iterator to the element following the last element of the list.
    */
    iterator end() const
    {
        return iterator(nullptr);
    }

    /*
    Inserts element before pos in the container. pos may be the end() iterator.
    If pos is not valid throw assertion.
    */
    void insert(iterator pos, const T& value = {})
    {
        assert(is_position_valid(pos) || pos == end());
        
        if (pos == end())
        {
            push_back(value);
            return;
        }

        if (pos == begin())
        {
            push_front(value);
            return;
        }

        ++mSize;
        Node* next = pos.mNode;
        Node* prev = pos.mNode->mPrev;
        Node* newNode = new Node(value, next, prev);

        next->mPrev = newNode;
        prev->mNext = newNode;
    }


    /*
    Removes the element at pos. If pos is not valid throw assertion.
    */
    void erase(iterator pos)
    {
        assert(is_position_valid(pos));
        
        if (pos == begin())
        {
            pop_front();
            return;
        }

        if (pos.mNode == mTail)
        {
            pop_back();
            return;
        }

        --mSize;
        Node* next = pos.mNode->mNext;
        Node* prev = pos.mNode->mPrev;
        
        next->mPrev = prev;
        prev->mNext = next;
    
        delete pos.mNode; 
    }

    /*
    Returns reference to the first element in the container.
    If user tries to call front on an empty container throw assertion.
    */
    T& front()
    {
        assert(mSize);
        return mHead->mData;
    }
    const T& front() const
    {
        assert(mSize);
        return mHead->mData;
    }

    /*
    Returns reference to the last element in the container.
    If user tries to call back on an empty container throw assertion.
    */
    T& back()
    {
        assert(mSize);
        return mTail->mData;
    }
    const T& back() const
    {
        assert(mSize);
        return mTail->mData;
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
    std::size_t size() const
    {
        return mSize;
    }

    /*
    Destructs the list.
    */
    ~List()
    {
        clear();
    }
};