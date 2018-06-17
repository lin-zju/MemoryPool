#include <iostream>
#include "MemPool.h"
MemPool::Node * MemPool::free_list[NumFreeList] = {nullptr};
char * MemPool::pool_start = nullptr;
char * MemPool::pool_end = nullptr;
size_t MemPool::RoundUp(size_t n)
{
	if (n % Align)
        return (n + (Align - n % Align));
    return n;
}
size_t MemPool::FindIndex(size_t n)
{
    return RoundUp(n) / Align;
}
void * MemPool::GetBlock(size_t n)
{
    size_t block_size = RoundUp(n);
    size_t pool_space = pool_end - pool_start;
    std::cout << "Block Size: " << block_size << std::endl
              << "Pool Space: " << pool_space << std::endl;
    if (pool_space >= block_size)
    {
        std::cout << "Enter if block 0\n";
        
        char * temp = pool_start;
        pool_start += block_size;
        return reinterpret_cast<void *>(temp);
    }
    else
    {
        std::cout << "Enter if block 1\n";
        if (pool_space)
        {
            auto list = free_list[FindIndex(pool_space)];
            reinterpret_cast<Node *>(pool_start)->next = list;
            list = reinterpret_cast<Node *>(pool_start);
        }
        char * temp = reinterpret_cast<char*>(::operator new(AllocSize));
        pool_start = temp + block_size;
        pool_end = temp + AllocSize;
        return reinterpret_cast<Node *>(temp);
    }
}

void * MemPool::alloc(size_t n)
{
	Node * result;
	Node * free_list_use;
	//if (n > ByteLimit)
		//return Allocator::allocate(n);
	
	free_list_use = free_list + FindIndex(n);
	if (free_list_use == nullptr) {
		return GetBlock(n);
	}
	else {
		result = *free_list_use;
		free_list[FindIndex(n)] = result->next;
	}
	return result;
}

void MemPool::dealloc(void * p, size_t n)
{
	Node * free_list_reuse;

	/*if (n > ByteLimit) {
		Allocator::deallocate(p, n);
		return nullptr;
	}*/
	free_list_reuse = free_list + FindIndex(n);
	p->next = free_list_reuse;
	*free_list_reuse = p;
	return free_list_reuse;
}

MemPool::~MemPool()
{
    ::operator delete(pool_start);
    for (auto i = Align; i <= ByteLimit; i += Align)
    {
        auto p = free_list[i];
        while (p)
        {
            auto q = p->next;
            ::operator delete(p);
            p = q;
        }
    }
}
