@page _PAGE_Main_IndexTree Index Tree

Entries in the index tree are structured both in a tree structure and in an
index-based vector.

Each entry within the tree is uniquely identified by its key. This key consists of
the first letter for the type of the entry (L for leave, B for branch, R for root),
the path to the entry and the actual name of the entry.

If an entry is added, it is also entered in a free space in a vector and thus
receives a unique index value within the tree. Access to the entry using this index
value is faster than using the key. As long as the entry is not removed from the tree,
it retains this index value. In other words, if entries are removed from the tree,
all other entries retain their place within the vector. If the removed entry was not
at the end of the vector, the element in the vector is marked as free and transferred
to a list of free entries. Only when the last entry of the vector is removed, i.e. the
entry with the currently highest possible index value, is the size of the vector and
the list of free entries corrected.

The class is particularly suitable for those applications in which the content of the
tree is usually not changed at runtime and the individual entries must be accessed
quickly. This is the case, for example, with the Trace Admin objects, in which the
tree is only expanded but never reduced after program start, since entries from this
tree are not removed at runtime, but only the reference counter is updated.

Other applications such as in a database in which a large number of objects
(groups, enumerations, parameters, tables) are to be managed, with the objects generally
being created only once when the program is started and then no longer being removed.

The use of the index tree generally makes sense if it is not only important to be able
to access the index value as quickly as possible, but also if no (or hardly) entries
are removed from it at runtime. It is also used for the test library or the structuring
of physical units.

If the tree can be changed from different threads at runtime, access to the tree must be
serialized and protected via a mutex. If two threads make an entry in the tree at the
same time, this could otherwise lead to inconsistencies within the tree. A flag in the
structure of the index tree allows this mutex to be created optionally.

If the tree is accessed from multiple threads and a thread requests that no other thread
make changes to the tree during the access (e.g. when iterating through the tree), the
tree can be locked for this thread as long as necessary ( however only if the mutex was
also created). Of course, it must then be ensured that the lock is then removed again.

If the tree is accessed from multiple threads, but it is a tree that is only set up once
at runtime, after which it is no longer changed but only accessed for reading, access
don't have to be protected via a mutex and there is no need to create the mutex.
