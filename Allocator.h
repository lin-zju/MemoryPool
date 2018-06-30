#ifndef ALLOCATOR_H_
#define ALLOCATOR_H_

#include "MemPool.h"
#include <iostream>
#include <new>

extern int chunk_allocation_count;
static MemPool pool;
template <typename T>
class Allocator
{
public:
	// type definitions
	typedef void 				_Not_user_specialized;
	typedef T 					value_type;
	typedef value_type * 		pointer;
	typedef const value_type * 	const_pointer;
	typedef value_type & 		reference;
	typedef const value_type & 	const_reference;
	typedef size_t 				size_type;
	typedef ptrdiff_t 			difference_type;
    typedef std::true_type 			propagate_on_container_move_assignment;
    typedef std::true_type 			is_always_equal;

	// methods
	pointer address(reference x) const noexcept { return &x; }
	const_pointer address(const_reference x) const noexcept { return &x; }
	void deallocate(pointer p, size_type n);
	pointer allocate(size_type n);
	template <class U, class... Args>
	void construct(U * p, Args &&... args );
	template <class U>
	void destroy(U * p);
};


template <typename T>
void Allocator<T>::deallocate(pointer p, size_type n)
{
//    std::cout << "???\n";
	if (n * sizeof(T) <= MemPool::ByteLimit)
		pool.dealloc(p, n * sizeof(T));
	else
		::operator delete(p);
}

template <typename T>
typename Allocator<T>::pointer Allocator<T>::allocate(size_type n)
{
	if (n * sizeof(T) <= MemPool::ByteLimit)
        return reinterpret_cast<pointer>(pool.alloc(n * sizeof(T)));
	else
    {
        chunk_allocation_count++;
        auto temp = reinterpret_cast<pointer>(::operator new(n * sizeof(T)));
//        std::cout << "Chunk allocation: " << n * sizeof(T) << " bytes.\n" << std::endl;
//        std::cout << "Count: " << chunk_allocation_count++ << std::endl;
        return temp;
    }
}

template <typename T>
template <class U, class... Args>
void Allocator<T>::construct(U * p, Args &&... args )
{
	new (p) T(args...);
}

template <typename T>
template <class U>
void Allocator<T>::destroy(U * p)
{
	p->~T();
}

#endif
