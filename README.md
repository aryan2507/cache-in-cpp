# cache-in-cpp
Implementation of Cache in C++

The cache is a doubly linked list of nodes, which contain the key-value pair.
Internally the cache uses an unordered map to allow fast searching of the node in question using its key. This increases the speed as O(N) traversal is avoided.
The cache can store any type of object required. 
The cache can be indexed using any type of object.
This is implemented using templates. 

You can choose from three different schemes-
1. LRU
2. FIFO
3. LIFO
Other schemes can be implemented using the already constructed
using the functions implemented in the Cache class.

To implement a new scheme, you need to fill two functions:
1. Used():  what to do if an entry is accessed.
2. InsertNode(): how to insert a new node in it.
3. Evict(): how to evict an entry in case the cache is filled.
Some other functions that can be used to implement the above functions are:
1. IsolateNode: Remove a node from the cache.
2. InsertBegin and InsertLast
3. DeleteBegin and DeleteLast
4. DeleteNode: Delete a node and free its memory. Shouldn't be used without using IsolateNode() first.

The cache is made thread safe using reader-writer locks.

The cache provides you with the following functions:
1. Query(): checks the cache for an element with the given key. If present, it returns a pointer to the value present in the cache, otherwise a nullptr is returned.
2. Insert(): inserts the given key-value pair in the cache. It may evict some elements from the cache if no space is present.
3. IsPresent(): checks the cache for an element with the given key, returns true if present and false otherwise.
















Disclaimer: The code present in this repo should not be used without prior permission.
