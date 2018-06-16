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
	typedef true_type 			propagate_on_container_move_assignment;
	typedef true_type 			is_always_equal;

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
	}
private:
	const size_t Align = 8;
	const size_t ByteLimit = 128;
	const size_t NodeSize = sizeof(T) + (Align - sizeof(T) % Align);
	const size_t NumFreeList = ByteLimit / NodeSize;
	
	// freelist level
	pointer free_list[NumFreeList];
	union Node
	{
		Node * next;
		value_type val;
	};

	// memory pool level
	static char * pool_start;
	static char * pool_end;
	// get an n-element blocks from the memory pool
	// if no blocks is available, throw AllocFail
	pointer GetBlock(int n);
}