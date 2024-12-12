/***********************************************************************
 * Header:
 *    LIST
 * Summary:
 *    Our custom implementation of std::list
 *      __       ____       ____         __
 *     /  |    .'    '.   .'    '.   _  / /
 *     `| |   |  .--.  | |  .--.  | (_)/ /
 *      | |   | |    | | | |    | |   / / _
 *     _| |_  |  `--'  | |  `--'  |  / / (_)
 *    |_____|  '.____.'   '.____.'  /_/
 *
 *
 *    This will contain the class definition of:
 *        List         : A class that represents a List
 *        ListIterator : An iterator through List
 * Author
 *    Joshua and Brooklyn 
 ************************************************************************/

#pragma once
#include <cassert>     // for ASSERT
#include <iostream>    // for nullptr
#include <new>         // std::bad_alloc
#include <memory>      // for std::allocator
 
class TestList;        // forward declaration for unit tests
class TestHash;        // to be used later

namespace custom
{

/**************************************************
 * LIST
 * Just like std::list
 **************************************************/
template <typename T>
class list
{
   friend class ::TestList; // give unit tests access to the privates
   friend class ::TestHash;
   friend void swap(list& lhs, list& rhs);
public:  
   // 
   // Construct
   //

   list();
   list(list <T> & rhs);
   list(list <T>&& rhs);
   list(size_t num, const T & t);
   list(size_t num);
   list(const std::initializer_list<T>& il);
   template <class Iterator>
   list(Iterator first, Iterator last);
  ~list() 
   {
   }

   // 
   // Assign
   //

   list <T> & operator = (list &  rhs);
   list <T> & operator = (list && rhs);
   list <T> & operator = (const std::initializer_list<T>& il);
   void swap(list <T>& rhs);

   //
   // Iterator
   //

   class  iterator;
   iterator begin() { return iterator(pHead); }
   iterator rbegin() { return iterator(pTail); }
   iterator end() { return iterator(nullptr); }

   //
   // Access
   //

   T& front();
   T& back();

   //
   // Insert
   //

   void push_front(const T&  data);
   void push_front(      T&& data);
   void push_back (const T&  data);
   void push_back (      T&& data);
   iterator insert(iterator it, const T& data);
   iterator insert(iterator it, T&& data);

   //
   // Remove
   //

   void pop_back();
   void pop_front();
   void clear();
   iterator erase(const iterator& it);

   // 
   // Status
   //

   bool empty()  const { return numElements == 0; }
   size_t size() const { return numElements; }

private:
    // nested linked list class
    class Node;

    // member variables
    size_t numElements; // though we could count, it is faster to keep a variable
    Node* pHead;    // pointer to the beginning of the list
    Node* pTail;    // pointer to the ending of the list
};


/*************************************************
 * NODE
 * the node class.  Since we do not validate any
 * of the setters, there is no point in making them
 * private.  This is the case because only the
 * List class can make validation decisions
 *************************************************/
template <typename T>
class list <T> :: Node
{
public:
   //
   // Construct
   //
   Node()  
   {
      pNext = pPrev = nullptr;
   }
   Node(const T &  data)  
   {
      pNext = pPrev = nullptr;
      this->data = data; //
   }
   Node(      T && data)  
   {
      pNext = pPrev = nullptr;
      this->data = std::move(data); 
   }

   //
   // Data
   //

   T data;                 // user data
   Node * pNext;       // pointer to next node
   Node * pPrev;       // pointer to previous node
};

/*************************************************
 * LIST ITERATOR
 * Iterate through a List, non-constant version
 ************************************************/
template <typename T>
class list <T> :: iterator
{
   friend class ::TestList; // give unit tests access to the privates
   friend class ::TestHash;
   template <typename TT>
   friend class custom::list;
public:
   // constructors, destructors, and assignment operator
   iterator() 
   {
      //p = new typename list <T> ::Node;
	   p = nullptr;
   }
   iterator(Node * p) 
   {
      //p = new typename list <T> ::Node;
	   this->p = p;
   }
   iterator(const iterator  & rhs) 
   {
      //p = new typename list <T> ::Node;
	   p = rhs.p;
   }
   iterator & operator = (const iterator & rhs)
   {
	   this->p = rhs.p; //
      return *this;
   }
   
   // equals, not equals operator
   bool operator == (const iterator& rhs) const {/* return true;*/ return p == rhs.p; }
   bool operator != (const iterator & rhs) const { /*return true;*/ return p != rhs.p;
   }

   // dereference operator, fetch a node
   T & operator * ()
   {
      //return *(new T);
       return p->data;
   }

   // postfix increment
   iterator operator ++ (int postfix)
   {
      //return *this;
       iterator temp(*this);
       p = p->pNext;
       return temp;
   }

   // prefix increment
   iterator & operator ++ ()
   {
      //return *this;
       p = p->pNext;
       return *this;
   }
   
   // postfix decrement
   iterator operator -- (int postfix)
   {
      //return *this;
       iterator temp(*this);
       p = p->pPrev;
       return temp;
   }

   // prefix decrement
   iterator & operator -- ()
   {
      //return *this;
       p = p->pPrev;
       return *this;
   } 

   // two friends who need to access p directly
   friend iterator list <T> :: insert(iterator it, const T &  data);
   friend iterator list <T> :: insert(iterator it,       T && data);
   friend iterator list <T> :: erase(const iterator & it);

private:

   typename list <T> :: Node * p;
};

/*****************************************
 * LIST :: NON-DEFAULT constructors
 * Create a list initialized to a value
 ****************************************/
template <typename T>
list <T> ::list(size_t num, const T & t) : numElements(0), pHead(nullptr), pTail(nullptr) // : numElements(0), pHead(nullptr), pTail(nullptr)
{
   //numElements = 99;
   //pHead = pTail = new list <T> ::Node();
    for (size_t i = 0; i < num; ++i)
        push_back(t);
}

/*****************************************
 * LIST :: ITERATOR constructors
 * Create a list initialized to a set of values
 ****************************************/
template <typename T>
template <class Iterator>
list <T> ::list(Iterator first, Iterator last) : numElements(0), pHead(nullptr), pTail(nullptr) // : numElements(0), pHead(nullptr), pTail(nullptr)
{
   /*numElements = 99;
   pHead = pTail = new list <T> ::Node();*/
   for (auto it = first; it != last; ++it)
       push_back(*it);
}

/*****************************************
 * LIST :: INITIALIZER constructors
 * Create a list initialized to a set of values
 ****************************************/
template <typename T>
list <T> ::list(const std::initializer_list<T>& il) : numElements(0), pHead(nullptr), pTail(nullptr) // : numElements(0), pHead(nullptr), pTail(nullptr)
{
   /*numElements = 99;
   pHead = pTail = new list <T> ::Node();*/
    for (const auto& item : il)
        push_back(item);
}

/*****************************************
 * LIST :: NON-DEFAULT constructors
 * Create a list initialized to a value
 ****************************************/
template <typename T>
list <T> ::list(size_t num) : numElements(0), pHead(nullptr), pTail(nullptr) // : numElements(0), pHead(nullptr), pTail(nullptr)
{
   /*numElements = 99;
   pHead = pTail = new list <T> ::Node();*/
    for (size_t i = 0; i < num; ++i)
        push_back(T());
}

/*****************************************
 * LIST :: DEFAULT constructors
 ****************************************/
template <typename T>
list <T> ::list() : numElements(0), pHead(nullptr), pTail(nullptr) // : numElements(0), pHead(nullptr), pTail(nullptr)
{
   /*numElements = 99;
   pHead = pTail = new list <T> ::Node();*/
}

/*****************************************
 * LIST :: COPY constructors
 ****************************************/
template <typename T>
list <T> ::list(list& rhs) : numElements(0), pHead(nullptr), pTail(nullptr) // : numElements(0), pHead(nullptr), pTail(nullptr)
{
   /*numElements = 99;
   pHead = pTail = new list <T> ::Node();*/
    for (auto it = rhs.begin(); it != rhs.end(); ++it)
        push_back(*it);
}

/*****************************************
 * LIST :: MOVE constructors
 * Steal the values from the RHS
 ****************************************/
template <typename T>
list <T> ::list(list <T>&& rhs) : numElements(0), pHead(nullptr), pTail(nullptr) // : numElements(0), pHead(nullptr), pTail(nullptr)
{
    // Same as copy constructor 
    for (auto it = rhs.begin(); it != rhs.end(); ++it)
        push_back(*it);

    // Make sure all variables are set 
    pHead = rhs.pHead;
    pTail = rhs.pTail;
    rhs.numElements = 0;
    rhs.pHead = nullptr;
    rhs.pTail = nullptr;
}

/**********************************************
 * LIST :: assignment operator - MOVE
 * Copy one list onto another
 *     INPUT  : a list to be moved
 *     OUTPUT :
 *     COST   : O(n) with respect to the size of the LHS 
 *********************************************/
template <typename T>
list <T>& list <T> :: operator = (list <T> && rhs)
{
    clear();                            //
    numElements = rhs.numElements;      //
    pHead = rhs.pHead;                  //
	pTail = rhs.pTail;  		        //

	rhs.numElements = 0;				//  
	rhs.pHead = rhs.pTail = nullptr;    //
   return *this;
}

/**********************************************
 * LIST :: assignment operator
 * Copy one list onto another
 *     INPUT  : a list to be copied
 *     OUTPUT :
 *     COST   : O(n) with respect to the number of nodes
 *********************************************/
template <typename T>
list <T> & list <T> :: operator = (list <T> & rhs)
{
    if (this != &rhs) //
    {
		clear();      //
		for (auto it = rhs.begin(); it != rhs.end(); ++it)  //
			push_back(*it);  //
    }
   return *this;
}

/**********************************************
 * LIST :: assignment operator
 * Copy one list onto another
 *     INPUT  : a list to be copied
 *     OUTPUT :
 *     COST   : O(n) with respect to the number of nodes
 *********************************************/
template <typename T>
list <T>& list <T> :: operator = (const std::initializer_list<T>& rhs)
{
    clear();        //
    for (const auto& item : rhs)  //
		push_back(item);  //
	return *this;  
}

/**********************************************
 * LIST :: CLEAR
 * Remove all the items currently in the linked list
 *     INPUT  :
 *     OUTPUT :
 *     COST   : O(n) with respect to the number of nodes
 *********************************************/
template <typename T>
void list <T> :: clear()
{
	while (!empty()) //
		pop_front();  //
}

/*********************************************
 * LIST :: PUSH BACK
 * add an item to the end of the list
 *    INPUT  : data to be added to the list
 *    OUTPUT :
 *    COST   : O(1)
 *********************************************/
template <typename T>
void list <T> ::push_back(const T& data)
{
    Node* newNode = new Node(data);
    if (pTail == nullptr)
        pHead = pTail = newNode;
    else
    {
        pTail->pNext = newNode;
        newNode->pPrev = pTail;
        pTail = newNode;
    }
    ++numElements;
}

template <typename T>
void list <T> ::push_back(T&& data)
{
    Node* newNode = new Node(std::move(data));
    if (pTail == nullptr)
        pHead = pTail = newNode;
    else
    {
        pTail->pNext = newNode;
        newNode->pPrev = pTail;
        pTail = newNode;
    }
    ++numElements;
}

/*********************************************
 * LIST :: PUSH FRONT
 * add an item to the head of the list
 *     INPUT  : data to be added to the list
 *     OUTPUT :
 *     COST   : O(1)
 *********************************************/
template <typename T>
void list <T> ::push_front(const T& data)
{
    Node* newNode = new Node(data);
    if (pHead == nullptr)
        pHead = pTail = newNode;
    else
    {
        pHead->pPrev = newNode;
        newNode->pNext = pHead;
        pHead = newNode;
    }
    ++numElements;
}

template <typename T>
void list <T> ::push_front(T&& data)
{
    Node* newNode = new Node(std::move(data));
    if (pHead == nullptr)
        pHead = pTail = newNode;
    else
    {
        pHead->pPrev = newNode;
        newNode->pNext = pHead;
        pHead = newNode;
    }
    ++numElements;
}


/*********************************************
 * LIST :: POP BACK
 * remove an item from the end of the list
 *    INPUT  : 
 *    OUTPUT :
 *    COST   : O(1)
 *********************************************/
template <typename T>
void list <T> ::pop_back()
{
    if (pTail == nullptr)
        return;

    Node* toDelete = pTail;
    if (pTail == pHead)
        pHead = pTail = nullptr;
    else
    {
        pTail = pTail->pPrev;
        pTail->pNext = nullptr;
    }
    delete toDelete;
    --numElements;
}

/*********************************************
 * LIST :: POP FRONT
 * remove an item from the front of the list
 *    INPUT  :
 *    OUTPUT :
 *    COST   : O(1)
 *********************************************/
template <typename T>
void list <T> ::pop_front()
{
    if (pHead == nullptr)
        return;

    Node* toDelete = pHead;
    if (pHead == pTail)
        pHead = pTail = nullptr;
    else
    {
        pHead = pHead->pNext;
        pHead->pPrev = nullptr;
    }
    delete toDelete;
    --numElements;
}

/*********************************************
 * LIST :: FRONT
 * retrieves the first element in the list
 *     INPUT  : 
 *     OUTPUT : data to be displayed
 *     COST   : O(1)
 *********************************************/
template <typename T>
T & list <T> :: front()
{
	if (pHead == nullptr)
		throw "ERROR: unable to access data from an empty list"; //to match test case
	return pHead->data;
}

/*********************************************
 * LIST :: BACK
 * retrieves the last element in the list
 *     INPUT  : 
 *     OUTPUT : data to be displayed
 *     COST   : O(1)
 *********************************************/
template <typename T>
T & list <T> :: back()
{
    if (pTail == nullptr)
        throw "ERROR: unable to access data from an empty list"; //to match test case
    return pTail->data;
}

/******************************************
 * LIST :: REMOVE
 * remove an item from the middle of the list
 *     INPUT  : an iterator to the item being removed
 *     OUTPUT : iterator to the new location 
 *     COST   : O(1)
 ******************************************/
template <typename T>
typename list <T> :: iterator  list <T> :: erase(const list <T> :: iterator & it)
{
    if (it.p == nullptr)
        return end();

    Node* toDelete = it.p;

    if (toDelete == pHead)  
    {
        pop_front();
		return begin();  // Return new head if the first element being deleted
    }
	else if (toDelete == pTail)
    {
        pop_back();
        return end();  // Return end() else if the last element is being deleted
    }
	else  // otherwise the element is in the middle so delete and adjust pNext and pPrev pointers
    {
        toDelete->pPrev->pNext = toDelete->pNext;
        toDelete->pNext->pPrev = toDelete->pPrev;
        Node* nextNode = toDelete->pNext;
		delete toDelete; // dont forget to delete the node
		--numElements; // just deleted a node so decrement the count
        return iterator(nextNode); 
    }
}

/******************************************
 * LIST :: INSERT
 * add an item to the middle of the list
 *     INPUT  : data to be added to the list
 *              an iterator to the location where it is to be inserted
 *     OUTPUT : iterator to the new item
 *     COST   : O(1)
 ******************************************/
template <typename T>
typename list <T> :: iterator list <T> :: insert(list <T> :: iterator it,
                                                 const T & data) 
{
    if (it.p == nullptr)
    {
        push_back(data);
        return iterator(pTail);
    }
    else if (it.p == pHead)
    {
        push_front(data);
        return iterator(pHead);
    }

    Node* newNode = new Node(data);
    newNode->pPrev = it.p->pPrev;
    newNode->pNext = it.p;
    it.p->pPrev->pNext = newNode;
    it.p->pPrev = newNode;

    ++numElements;
    return iterator(newNode);
}

template <typename T>
typename list <T> :: iterator list <T> :: insert(list <T> :: iterator it,
   T && data)
{
    if (it.p == nullptr)
    {
        push_back(std::move(data));
        return iterator(pTail);
    }
    else if (it.p == pHead)
    {
        push_front(std::move(data));
        return iterator(pHead);
    }

    Node* newNode = new Node(std::move(data));
    newNode->pPrev = it.p->pPrev;
    newNode->pNext = it.p;
    it.p->pPrev->pNext = newNode;
    it.p->pPrev = newNode;

    ++numElements;
    return iterator(newNode);
}

/**********************************************
 * LIST :: assignment operator - MOVE
 * Copy one list onto another
 *     INPUT  : a list to be moved
 *     OUTPUT :
 *     COST   : O(n) with respect to the size of the LHS
 *********************************************/
template <typename T>
void swap(list <T> & lhs, list <T> & rhs)
{
	list <T> temp(std::move(lhs));
	lhs = std::move(rhs);
	rhs = std::move(temp);
}

template <typename T>
void list<T>::swap(list <T>& rhs)
{
    list <T> temp(std::move(*this));
    *this = std::move(rhs);
    rhs = std::move(temp);
}

//#endif
}; // namespace custom
