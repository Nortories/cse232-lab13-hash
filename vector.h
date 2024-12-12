/***********************************************************************
 * Header:
 *    VECTOR
 * Summary:
 *    Our custom implementation of std::vector
 *    __    ___     ___    _   __
 *   /_ |  / _ \   / _ \  (_) / /
 *    | | | | | | | | | |    / / 
 *    | | | | | | | | | |   / /  
 *    | | | |_| | | |_| |  / / _ 
 *    |_|  \___/   \___/  /_/ (_)
 *                             
 *    This will contain the class definition of:
 *        vector                 : A class that represents a Vector
 *        vector::iterator       : An iterator through Vector
 * Author
 *    Br. Helfrich
 ************************************************************************/

#pragma once

#include <cassert>  // because I am paranoid
#include <new>      // std::bad_alloc
#include <memory>   // for std::allocator

class TestVector; // forward declaration for unit tests
class TestStack;
class TestPQueue;
class TestHash;

namespace custom
{

/*****************************************
 * VECTOR
 * Just like the std :: vector <T> class
 ****************************************/
template <typename T>
class vector
{
   friend class ::TestVector; // give unit tests access to the privates
   friend class ::TestStack;
   friend class ::TestPQueue;
   friend class ::TestHash; 
public:

   //
   // Construct
   //

   vector() : data(nullptr), numElements(0), numCapacity(0) {}
   vector(size_t numElements                );
   vector(size_t numElements, const T & t   );
   vector(const std::initializer_list<T>& l );
   vector(const vector &  rhs);
   vector(      vector && rhs);
  ~vector();

   //
   // Assign
   //

   void swap(vector& rhs)
   {
      std::swap(data, rhs.data);
      std::swap(numElements, rhs.numElements);
      std::swap(numCapacity, rhs.numCapacity);
   }
   vector & operator = (const vector & rhs);
   vector & operator = (vector&& rhs);

   // 
   // Iterator
   // 

   class iterator;
   iterator       begin() { return iterator(data);               }
   iterator       end()   { return iterator(data + numElements); }

   //
   // Access
   //

         T& operator [] (size_t index);
   const T& operator [] (size_t index) const;
         T& front();
   const T& front() const;
         T& back();
   const T& back() const;

   //
   // Insert
   //

   void push_back(const T& t);
   void push_back(T&& t);
   void reserve(size_t newCapacity);
   void resize(size_t newElements);
   void resize(size_t newElements, const T& t);

   //
   // Remove
   //

   void clear()
   {
      numElements = 0;
   }
   void pop_back()
   {
      if (numElements)
         --numElements;
   }
   void shrink_to_fit();

   // 
   // Status
   //

   size_t  size()          const { return numElements;}
   size_t  capacity()      const { return numCapacity;}
   bool empty()            const { return numElements == 0;}

private:

   T *  data;             // user data, a dynamically-allocated array
   size_t  numCapacity;   // the capacity of the array
   size_t  numElements;   // the number of items currently used
};

/**************************************************
 * VECTOR ITERATOR
 * An iterator through vector.  You only need to
 * support the following:
 *   1. Constructors (default and copy)
 *   2. Not equals operator
 *   3. Increment (prefix and postfix)
 *   4. Dereference
 * This particular iterator is a bi-directional meaning
 * that ++ and -- both work.  Not all iterators are that way.
 *************************************************/
template <typename T>
class vector <T> ::iterator
{
   friend class ::TestVector; // give unit tests access to the privates
   friend class ::TestStack;
   friend class ::TestPQueue;
   friend class ::TestHash;
public:
   // constructors, destructors, and assignment operator
   iterator() : p(nullptr)              {                     }
   iterator(T* p) : p(p)                {                     }
   iterator(const iterator& rhs)        { *this = rhs;        }
   iterator(size_t index, vector<T>& v) { p = v.data + index; }
   iterator& operator = (const iterator& rhs)
   {
      this->p = rhs.p;
      return *this;
   }

   // equals, not equals operator
   bool operator != (const iterator& rhs) const { return rhs.p != this->p; }
   bool operator == (const iterator& rhs) const { return rhs.p == this->p; }

   // dereference operator
   T& operator * ()
   {
      if (p)
         return *p;
      else
         throw "ERROR: Trying to dereference a NULL pointer";
   }

   // prefix increment
   iterator& operator ++ ()
   {
      p++;
      return *this;
   }

   // postfix increment
   iterator operator ++ (int postfix)
   {
      iterator tmp(*this);
      p++;
      return *this;
   }

   // prefix decrement
   iterator& operator -- ()
   {
      p--;
      return *this;
   }

   // postfix decrement
   iterator operator -- (int postfix)
   {
      iterator tmp(*this);
      p--;
      return *this;
   }

private:
   T* p;
};

/*****************************************
 * VECTOR :: NON-DEFAULT constructors
 * non-default constructor: set the number of elements,
 * construct each element, and copy the values over
 ****************************************/
template <typename T>
vector <T> :: vector(size_t num, const T & t) :
data(nullptr), numElements(0), numCapacity(0)
{
   // do nothing if there is nothing to do
   if (num > 0)
   {
      // allocate memory
      data = new T[num];
      numCapacity = num;
      numElements = num;

      // copy the value
      for (size_t i = size_t(0); i < num; i++)
         data[i] = t;
   }

}

/*****************************************
 * VECTOR :: INITIALIZATION LIST constructors
 * Create a vector with an initialization list.
 ****************************************/
template <typename T>
vector <T> :: vector(const std::initializer_list<T> & l) :
      data(nullptr), numElements(0), numCapacity(0)
{
   if (l.size())
   {
      // allocate memory
      data = new T[l.size()];

      // copy the value
      size_t i = size_t(0);
      for (auto &item : l)
         data[i++] = item;
      numElements = l.size();
      numCapacity = l.size();
   }
}

/*****************************************
 * VECTOR :: NON-DEFAULT constructors
 * non-default constructor: set the number of elements,
 * construct each element, and copy the values over
 ****************************************/
template <typename T>
vector <T> :: vector(size_t num):
      data(nullptr), numElements(0), numCapacity(0)
{
   // do nothing if there is nothing to do
   if (num > size_t(0))
   {
      numElements = num;
      numCapacity = num;
      data = new T[num];
      for (size_t i = size_t(0); i < num; i++)
         data[i] = T();
   }
}

/*****************************************
 * VECTOR :: COPY CONSTRUCTOR
 * Allocate the space for numElements and
 * call the copy constructor on each element
 ****************************************/
template <typename T>
vector <T> :: vector (const vector & rhs) : data(nullptr), numElements(0), numCapacity(0)
{
   *this = rhs;
}

/*****************************************
 * VECTOR :: MOVE CONSTRUCTOR
 * Steal the values from the RHS and set it to zero.
 ****************************************/
template <typename T>
vector <T> :: vector (vector && rhs) : data(nullptr), numElements(0), numCapacity(0)
{
   *this = std::move(rhs);
}

/*****************************************
 * VECTOR :: DESTRUCTOR
 * Call the destructor for each element from 0..numElements
 * and then free the memory
 ****************************************/
template <typename T>
vector <T> :: ~vector()
{
   if (numCapacity > 0)
   {
      assert(nullptr != data);
      delete [] data;
   }
}

/***************************************
 * VECTOR :: RESIZE
 * This method will adjust the size to newElements.
 * This will either grow or shrink newElements.
 *     INPUT  : newCapacity the size of the new buffer
 *     OUTPUT :
 **************************************/
template <typename T>
void vector <T> :: resize(size_t newElements)
{
   assert(newElements >= 0);

   // grow as necessary
   if (newElements > numElements)
   {
      // increase capacity as necessary
      if (newElements > numCapacity)
         reserve(newElements);

      // now fill the new slots with the default T
      for (size_t i = numElements; i < newElements; i++)
         data[i] = T();
   }

   // if we have made it this far, adjust the number of elements
   numElements = newElements;

}

template <typename T>
void vector <T> :: resize(size_t newElements, const T & t)
{
   assert(newElements >= 0);

   // grow as necessary
   if (newElements > numElements)
   {
    // increase capacity as necessary
    if (newElements > numCapacity)
       reserve(newElements);

    // now fill the new slots with the default T
    for (size_t i = numElements; i < newElements; i++)
       data[i] = t;
   }

   // if we have made it this far, adjust the number of elements
   numElements = newElements;
}

/***************************************
 * VECTOR :: RESERVE
 * This method will grow the current buffer
 * to newCapacity.  It will also copy all
 * the data from the old buffer into the new
 *     INPUT  : newCapacity the size of the new buffer
 *     OUTPUT :
 **************************************/
template <typename T>
void vector <T> :: reserve(size_t newCapacity)
{
   // do nothing if we are already big enough
   if (newCapacity <= numCapacity)
      return;
   assert(newCapacity > 0 && newCapacity > numCapacity);

   // allocate the new array
   T* pNew = new T[newCapacity];

   // copy over the data from the old array
   for (size_t i = 0; i < numElements; i++)
      pNew[i] = std::move(data[i]);

   delete[] data;

   data = pNew;
   numCapacity = newCapacity;

}

/***************************************
 * VECTOR :: SHRINK TO FIT
 * Get rid of any extra capacity
 *     INPUT  :
 *     OUTPUT :
 **************************************/
template <typename T>
void vector <T> :: shrink_to_fit()
{
   // do nothing if we have no space
   if (numCapacity == numElements)
      return;

   // allocate the new array
   T * pNew;
   if (numElements != 0)
   {
      pNew = new T[numElements];
      for (size_t i = 0; i < numElements; i++)
         pNew[i] = data[i];
   }
   else
      pNew = nullptr;


   // delete the old and assign the new
   if (nullptr != data)
   {
      delete[] data;
   }
   data = pNew;
   numCapacity = numElements;
}



/*****************************************
 * VECTOR :: SUBSCRIPT
 * Read-Write access
 ****************************************/
template <typename T>
T & vector <T> :: operator [] (size_t index)
{
   // sanity check. Note that we do not do error-checking with []
   assert (index >= 0 && index < numElements);
   return data[index];    // return by-reference

}

/******************************************
 * VECTOR :: SUBSCRIPT
 * Read-Write access
 *****************************************/
template <typename T>
const T & vector <T> :: operator [] (size_t index) const
{
   // sanity check
   assert (index >= 0 && index < numElements);
   return data[index];    // return const by-reference
}

/*****************************************
 * VECTOR :: FRONT
 * Read-Write access
 ****************************************/
template <typename T>
T & vector <T> :: front ()
{
   // sanity check. Note that we do not do error-checking with front
   assert(numElements > 0);
   return data[0];    // return by-reference
}

/******************************************
 * VECTOR :: FRONT
 * Read-Write access
 *****************************************/
template <typename T>
const T & vector <T> :: front () const
{
   // sanity check
   assert(numElements > 0);
   return data[0];    // return const by-reference
}

/*****************************************
 * VECTOR :: FRONT
 * Read-Write access
 ****************************************/
template <typename T>
T & vector <T> :: back()
{
   // sanity check. Note that we do not do error-checking with back
   assert(numElements > 0);
   return data[numElements - 1];    // return by-reference
}

/******************************************
 * VECTOR :: FRONT
 * Read-Write access
 *****************************************/
template <typename T>
const T & vector <T> :: back() const
{
   // sanity check
   assert(numElements > 0);
   return data[numElements - 1];    // return const by-reference
}

/***************************************
 * VECTOR :: PUSH BACK
 * This method will add the element 't' to the
 * end of the current buffer.  It will also grow
 * the buffer as needed to accomodate the new element
 *     INPUT  : 't' the new element to be added
 *     OUTPUT : *this
 **************************************/
template <typename T>
void vector <T> :: push_back (const T & t)
{
   assert(numElements <= numCapacity);

   // grow if necessary
   if (numCapacity == 0)
      reserve(1);
   else if (numElements == numCapacity)
      reserve(numCapacity * 2);   // could throw ERROR: Unable to allocate ...
   assert(numElements < numCapacity);

   // actually add on to the end of the list
   data[numElements++] = t;
}

template <typename T>
void vector <T> ::push_back(T && t)
{
   assert(numElements <= numCapacity);

   // grow if necessary
   if (numCapacity == 0)
      reserve(1);
   else if (numElements == numCapacity)
      reserve(numCapacity * 2);   // could throw ERROR: Unable to allocate ...
   assert(numElements < numCapacity);

   // actually add on to the end of the list
   data[numElements++] = std::move(t);
}


/***************************************
 * VECTOR :: ASSIGNMENT
 * This operator will copy the contents of the
 * rhs onto *this, growing the buffer as needed
 *     INPUT  : rhs the vector to copy from
 *     OUTPUT : *this
 **************************************/
template <typename T>
vector <T> & vector <T> :: operator = (const vector & rhs)
{
   // clear out the old data
   clear();

   // ensure we have sufficient size
   if (rhs.size() > numElements)
      reserve(rhs.size());

   // copy over the elements from the initialization list
   for (size_t i = size_t(0); i < rhs.size(); i++)
      data[i] = rhs.data[i];
   numElements = rhs.numElements;

   // return self
   return *this;
}
template <typename T>
vector <T>& vector <T> :: operator = (vector&& rhs)
{
   clear();
   shrink_to_fit();
   swap(rhs);
   return *this;
}



} // namespace custom
