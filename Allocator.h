#include <exception>

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
	pointer address(reference x) const noexcept;
	const_pointer address(const_reference x) const noexcept;
	void deallocate(pointer p, size_type n);
	pointer allocate(size_type n);
	template <class U, class... Args>
	void construct(U * p, Args &&... args );
	template <class U>
	void destroy(U * p);

	class AllocFail : public std::exception
	{
	public:
		const char * what() const noexcept
			{ return "no memory available!\n";} 
    };
//private:
	static const size_t Align = 8;
	static const size_t ByteLimit = 128;
	// round up Node size
	static const size_t NodeSize = sizeof(T) + (Align - sizeof(T) % Align);
	static const size_t NumFreeList = ByteLimit / NodeSize;
	const static size_t AllocSize;

	// freelist level
	union Node
	{
		Node * next;
		value_type val;
	};
	static Node * free_list[NumFreeList];
	// return the proper index of the list 
	// for a n-Node block
	static size_t FindIndex(size_t n);

	// memory pool level
	static char * pool_start;
	static char * pool_end;
	// get an n-Node blocks from the memory pool
	// if no blocks is available, throw AllocFail
	Node * GetBlock(int n);
};


template <typename T>
char * Allocator<T>::pool_start = nullptr;
template <typename T>
char * Allocator<T>::pool_end = nullptr;
template <typename T>
typename Allocator<T>::Node * free_list[Allocator<T>::NumFreeList] = {nullptr};
#include "Allocator.hpp"
