/**
   This file is part of libavrc++.

   Copyright (c) 2014, netWorms, All rights reserved.

   libavrc++ is  free software: you can  redistribute it and/or modify  it under
   the terms of the  GNU Lesser General Public License as  published by the Free
   Software Foundation, either version 3 of the License, or (at your option) any
   later version.

   libavrc++ is distributed in the hope that  it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
   A PARTICULAR  PURPOSE.  See the  GNU Lesser  General Public License  for more
   details.

   You should  have received  a copy  of the GNU  Lesser General  Public License
   along with libavrc++.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef __QUEUE_HH__
#define __QUEUE_HH__

#include <stdint.h>

template<typename T, typename size_type, size_type size>
class Queue {
public:
  Queue() :  p(0), n(0) {};

  /// insert a new element at the end
  inline void push(T val) {
    if(n < size) {
      buffer[(p + n) % size] = val;
      ++n;
    }
  }

  /// access and remove the element at the beginning
  inline T pop() {
    T val = T();
    if(n != 0) {
      val = buffer[p];
      p = (p + 1) % size;
      --n;
    }
    return val;
  }

  /// access the element at the beginning
  inline T peek() {
    if(isEmpty()) return T();
    else return buffer[p];
  }

  /// check if the queue is empty
  inline bool isEmpty() { return (n == 0); }
  /// check if the queue is full
  inline bool isFull() { return (n == size); }

  /// specifies how many elements are in the queue
  inline size_type used() { return n; }

  /// empty the queue
  inline void flush() { n = p = 0; }
private:
  size_type p, n;
  T buffer[size];
};

#endif // __QUEUE_HH__



/*
 *            p
 *   ___ ___ ___ ___ ___ ___ ___ ___ ___
 *  |   |   | 0 | 1 | 2 |...|n-1|   |   |
 *  |___|___|___|___|___|___|___|___|___|
 *
 *  |------------------------ size -----------------------------|
 */
