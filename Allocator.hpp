#include <new>
template <typename T>
size_t Allocator<T>::FindIndex(size_t n)
{
	return (n * NodeSize) / Align;
}

template <typename T>
typename Allocator<T>::Node * Allocator<T>::GetBlock(int n)
{
	size_t block_size = n * NodeSize;
	if (pool_end - pool_start >= block_size)
	{
		auto temp = pool_start;
		pool_start += block_size;
		return temp;
	}
	else
	{
		// collect left space into the proper list
		auto list = free_list[FindIndex(pool_end - pool_start)];
		reinterpret_cast<Node *>(pool_start)->next = list;
		list = pool_start;

        auto temp = ::operator new(AllocSize);
		pool_start = reinterpret_cast<char *>(temp) + block_size;
		pool_end = reinterpret_cast<char *>(temp) + AllocSize;
		return temp;
	}
}
