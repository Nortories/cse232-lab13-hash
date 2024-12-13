/***********************************************************************
 * Header:
 *    HASH
 * Summary:
 *    Our custom implementation of std::unordered_Set
 *      __       ____       ____         __
 *     /  |    .'    '.   .'    '.   _  / /
 *     `| |   |  .--.  | |  .--.  | (_)/ /
 *      | |   | |    | | | |    | |   / / _
 *     _| |_  |  `--'  | |  `--'  |  / / (_)
 *    |_____|  '.____.'   '.____.'  /_/
 *
 *    This will contain the class definition of:
 *        unordered_set           : A class that represents a hash
 *        unordered_set::iterator : An interator through hash
 * Author
 *    Joshua and Brooklyn
 ************************************************************************/

#pragma once

#include "list.h"     // because this->buckets[0] is a list
#include <memory>     // for std::allocator
#include <functional> // for std::hash
#include <cmath>      // for std::ceil
   

class TestHash;             // forward declaration for Hash unit tests

namespace custom
{
/************************************************
 * UNORDERED SET
 * A set implemented as a hash
 ************************************************/
template <typename T>
class unordered_set
{
   friend class ::TestHash;   // give unit tests access to the privates
public:
   //
   // Construct
   //
   unordered_set() : numElements(0)
   {
   }
   unordered_set(unordered_set&  rhs) // copy construct
   {
      *this = rhs;
   }
   unordered_set(unordered_set&& rhs) // move construct 
   {
      *this = std::move(rhs);
   }

   template <class Iterator>
   unordered_set(Iterator first, Iterator last) : numElements(0) // iterator constructor 
   {
      while (first != last)
      {
         insert(*first);
         ++first;
      }
   }

   //
   // Assign
   //
   unordered_set& operator=(unordered_set& rhs)
   {
      numElements = rhs.numElements;

      // copy assign each element from rhs 
      for (size_t i = 0; i < 10; i++)
         buckets[i] = rhs.buckets[i];

      return *this;
   }
   unordered_set& operator=(unordered_set&& rhs)
   {
      numElements = 0;
      std::swap(numElements, rhs.numElements); // now rhs.numElements = 0 

      // move assign each element from rhs 
      for (size_t i = 0; i < 10; i++)
         buckets[i] = std::move(rhs.buckets[i]);

      return *this;
   }
   unordered_set& operator=(const std::initializer_list<T>& il)
   {
      clear(); // start from scratch 
      reserve(il.size); // allocate space 

      // insert each element in the given list 
      for (auto & i : il)
         insert(i);

      return *this;
   }
   void swap(unordered_set& rhs)
   {
      std::swap(numElements, rhs.numElements);

      // swap buckets 
      for (size_t i = 0; i < 10; i++)
         std::swap(buckets[i], rhs.buckets[i]);
   }

   

   // 
   // Iterator
   //
   class iterator;
   class local_iterator;
   iterator begin()
   {
      // look for first non empty
      for (size_t i = 0; i < 10; i++)
      {
         if (!buckets[i].empty())
         {
            // return begin()
            return iterator(
               &buckets[i],       // list<T>* pBucket
               &buckets[10],       // list<T>* pBucketEnd
               buckets[i].begin() // list<T>::iterator itList
            );
         }
      }
      // return end() if all buckets are empty
      return end();
   }
   iterator end()
   {
      return iterator(&buckets[10], &buckets[10], buckets[0].end());
   }
   local_iterator begin(size_t iBucket)
   {
      return local_iterator(buckets[iBucket].begin());
   }
   local_iterator end(size_t iBucket)
   {
      return local_iterator(buckets[iBucket].end());
   }

   //
   // Access
   //
   size_t bucket(const T& t)
   {
       // calculate the index of the bucket for the element t 
       // hash t then % the number of buckets 
      return std::hash<T>{}(t) % bucket_count();
   }

   iterator find(const T& t);

   //   
   // Insert
   //
   custom::pair<iterator, bool> insert(const T& t); // insert 1 element 
   void insert(const std::initializer_list<T> & il); // insert a list 


   // 
   // Remove
   //
   void clear() noexcept
   {
       // clear each bucket 
      for (size_t i = 0; i < bucket_count(); ++i)
         buckets[i].clear();

      // no more elements 
      numElements = 0;
   }

   iterator erase(const T& t);

   //
   // Status
   //
   size_t size() const 
   { 
      return numElements; 
   }
   bool empty() const 
   { 
      return numElements == 0; 
   }
   size_t bucket_count() const 
   { 
      return 10; // see below, there are 10 buckets 
   }
   size_t bucket_size(size_t i) const
   {
       // size of the specified bucket 
      return buckets[i].size();
   }

private:

   custom::list<T> buckets [10];   // exactly 10 buckets
   int numElements;                // number of elements in the Hash
};


/************************************************
 * UNORDERED SET ITERATOR
 * Iterator for an unordered set
 ************************************************/
template <typename T>
class unordered_set <T> ::iterator
{
   friend class ::TestHash;   // give unit tests access to the privates
   template <class TT>
   friend class custom::unordered_set;
public:
   // 
   // Construct
   //
   iterator() : pBucket(nullptr), pBucketEnd(nullptr), itList()
   {  
   }
   iterator(typename custom::list<T>* pBucket,
            typename custom::list<T>* pBucketEnd,
            typename custom::list<T>::iterator itList) : pBucket(pBucket), pBucketEnd(pBucketEnd), itList(itList)
   {
   }
   iterator(const iterator& rhs) : pBucket(rhs.pBucket), pBucketEnd(rhs.pBucketEnd), itList(rhs.itList)
   { 
   }

   //
   // Assign
   //
   iterator& operator = (const iterator& rhs)
   {
      if (this != &rhs)
      {
         pBucket = rhs.pBucket;
         pBucketEnd = rhs.pBucketEnd;
         itList = rhs.itList;
      }
      return *this;
   }

   //
   // Compare
   //
   bool operator != (const iterator& rhs) const 
   { 
       // not equals (returns true if anything is different) 
      return pBucket != rhs.pBucket || itList != rhs.itList; 
   }
   bool operator == (const iterator& rhs) const 
   { 
       // equals (returns true only if everything is same) 
      return pBucket == rhs.pBucket && itList == rhs.itList; 
   }

   // 
   // Access
   //
   T& operator * ()
   {
      return *itList; // pointer 
   }

   //
   // Arithmetic
   //
   iterator& operator ++ ();
   iterator operator ++ (int postfix)
   {
      auto tmp = *this;
      itList++; // increment by 1 
      return tmp; // return state before it was incremented 
   }

private:
   custom::list<T> *pBucket;
   custom::list<T> *pBucketEnd;
   typename list<T>::iterator itList;
};


/************************************************
 * UNORDERED SET LOCAL ITERATOR
 * Iterator for a single bucket in an unordered set
 ************************************************/
template <typename T>
class unordered_set <T> ::local_iterator
{
   friend class ::TestHash;   // give unit tests access to the privates

   template <class TT>
   friend class custom::unordered_set;
public:
   // 
   // Construct
   //
   local_iterator() : itList()
   {
   }
   local_iterator(const typename custom::list<T>::iterator& itList) : itList(itList)
   {
   }
   local_iterator(const local_iterator& rhs) : itList(rhs.itList)
   { 
   }

   //
   // Assign
   //
   local_iterator& operator=(const local_iterator& rhs)
   {
      if (this != &rhs)
      {
         itList = rhs.itList;
      }
      return *this;
   }

   //
   // Compare
   //
   bool operator!=(const local_iterator& rhs) const
   {
      return itList != rhs.itList; //not equals 
   }

   bool operator==(const local_iterator& rhs) const
   {
      return itList == rhs.itList;//equals 
   }

   //
   // Access
   //
   T& operator*()
   {
      return *itList;
   }

   //
   // Arithmetic
   //
   local_iterator& operator++()
   {
      ++itList; 
      // prefix operator returns new value after incrementing 
      return *this;
   }

   local_iterator operator++(int postfix)
   {
      local_iterator temp = *this;
      ++(*this); // increment by 1 
      return temp; // return value before incrementing 
   }

private:
   typename custom::list<T>::iterator itList;
};


/*****************************************
 * UNORDERED SET :: ERASE
 * Remove one element from the unordered set
 ****************************************/
template <typename T>
typename unordered_set <T> ::iterator unordered_set<T>::erase(const T& t)
{
    size_t bucketIndex = bucket(t); // calculate bucket index for the element 

    // go thru every element in the bucket 
    for (auto it = buckets[bucketIndex].begin(); it != buckets[bucketIndex].end(); ++it)
    {
        if (*it == t)
        {
            it = buckets[bucketIndex].erase(it); // erase the element if you find it 
            numElements--; // then there is 1 less element 

            // If bucket is now empty, move to the next valid bucket
            if (it == buckets[bucketIndex].end())
            {
                for (++bucketIndex; bucketIndex < bucket_count(); ++bucketIndex)
                {
                    if (!buckets[bucketIndex].empty())
                    {
                        // If the bucket is not empty, return an iterator 
                        return iterator(&buckets[bucketIndex], &buckets[bucket_count()], buckets[bucketIndex].begin());
                    }
                }
                return end(); // Return end if no more valid buckets
            }
            // if bucket is not empty, return an iterator 
            return iterator(&buckets[bucketIndex], &buckets[bucket_count()], it);
        }
    }
    return end(); // Element not found
}

/*****************************************
 * UNORDERED SET :: INSERT
 * Insert one element into the hash
 ****************************************/
template <typename T>
custom::pair<typename custom::unordered_set<T>::iterator, bool> unordered_set<T>::insert(const T& t)
{
size_t bucketIndex = bucket(t); // Calculate bucket using the hash function

    for (const T& value : buckets[bucketIndex])
    {
        if (value == t)
        {
            // Return an iterator pointing to the existing value
            return custom::pair<iterator, bool>(
                iterator(&buckets[bucketIndex], &buckets[bucket_count()], buckets[bucketIndex].begin()), false);
        }
    }
    // Add the value and update numElements
    buckets[bucketIndex].push_back(t);
    numElements++;
    
    // Return a pair with iterator for the new value, and bool true because inserted new element 
    return custom::pair<iterator, bool>(
        iterator(&buckets[bucketIndex], &buckets[bucket_count()], buckets[bucketIndex].begin()), true);
}
template <typename T>
void unordered_set<T>::insert(const std::initializer_list<T> & il)
{
}

/*****************************************
 * UNORDERED SET :: FIND
 * Find an element in an unordered set
 ****************************************/
template <typename T>
typename unordered_set<T>::iterator unordered_set<T>::find(const T& t)
{
    size_t bucketIndex = bucket(t); // calculate bucket index for the element 

    // go thru every element in the bucket 
    for (auto it = buckets[bucketIndex].begin(); it != buckets[bucketIndex].end(); ++it)
    {
        if (*it == t)
        {
            // return iterator for the element 
            return iterator(&buckets[bucketIndex], &buckets[bucket_count()], it);
        }
    }
    return end(); // Return end if not found
}


/*****************************************
 * UNORDERED SET :: ITERATOR :: INCREMENT
 * Advance by one element in an unordered set
 ****************************************/
template <typename T>
typename unordered_set<T>::iterator& unordered_set<T>::iterator::operator++()
{
    if (itList != pBucket->end())
    {
        ++itList; // Advance within the current bucket
    }
    if (itList == pBucket->end()) // If end of the current bucket
    {
        do
        {
            ++pBucket; // Move to the next bucket
        } while (pBucket != pBucketEnd && pBucket->empty());
        if (pBucket != pBucketEnd)
        {
            itList = pBucket->begin(); // Set iterator to the first element of the next bucket
        }
    }
    return *this;
}

/*****************************************
 * SWAP
 * Stand-alone unordered set swap
 ****************************************/
template <typename T>
void swap(unordered_set<T>& lhs, unordered_set<T>& rhs)
{
   lhs.swap(rhs); // swappy swap 
}

}
