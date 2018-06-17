#include <iostream>
#include "MemPool.h"

MemPool::Node * MemPool::free_list[NumFreeList] = {nullptr};
char * MemPool::pool_start = nullptr;
char * MemPool::pool_end = nullptr;
MemPool::Node * MemPool::current_block = nullptr;
size_t MemPool::RoundUp(size_t n)
{
	if (n % Align)
        return (n + (Align - n % Align));
    return n;
}
size_t MemPool::FindIndex(size_t n)
{
    return RoundUp(n) / Align - 1;
}
void * MemPool::GetBlock(size_t n)
{
//    Report();
    size_t block_size = RoundUp(n);
    size_t pool_space = pool_end - pool_start;
//    std::cout << "Block Size: " << block_size << std::endl;
    if (pool_space >= block_size)
    {
//        std::cout << "Enter if block 0\n";
        
        char * temp = pool_start;
        pool_start += block_size;
//        Report();
        return reinterpret_cast<void *>(temp);
    }
    else
    {
//        std::cout << "Enter if block 1\n";
        if (pool_space)
        {
//            std::cout << "HI\n";
//            std::cout << "index: " << FindIndex(pool_space) << std::endl;
            auto & list = free_list[FindIndex(pool_space)];
            reinterpret_cast<Node *>(pool_start)->next = list;
            list = reinterpret_cast<Node *>(pool_start);
        }
        char * temp = reinterpret_cast<char*>(::operator new(AllocSize));
        std::cout << "Allocation suceeded. Memory: " << reinterpret_cast<void *>(temp) << ".\n";

        reinterpret_cast<Node *>(temp)->prev = current_block;
        current_block = reinterpret_cast<Node *>(temp)->prev;

        pool_start = temp + Align + block_size;
        pool_end = temp + AllocSize;
//        Report();
        return reinterpret_cast<Node *>(temp + Align);
    }
    
}

void * MemPool::alloc(size_t n)
{
	Node * result = nullptr;
	//if (n > ByteLimit)
		//return Allocator::allocate(n);
	if (n)
    {
        auto & free_list_use = free_list[FindIndex(n)];
        if (free_list_use == nullptr) {
            return GetBlock(n);
        }
        else {
            result = free_list_use;
            free_list[FindIndex(n)] = result->next;
        }
    }
   
//    Report();
	return result;
}

void MemPool::dealloc(void * p, size_t n)
{


	/*if (n > ByteLimit) {
		Allocator::deallocate(p, n);
		return nullptr;
	}*/
    if (n)
    {
        auto & free_list_reuse = free_list[FindIndex(n)];
        reinterpret_cast<Node *>(p)->next = free_list_reuse;
        free_list_reuse = reinterpret_cast<Node *>(p);
    }
   
//    Report();
}

MemPool::~MemPool()
{
    while (current_block)
    {
    	auto prev = current_block->prev;
    	::operator delete(current_block);
    	current_block = prev;
    }
}

void MemPool::Report()
{
    using namespace std;
//    static Node * free_list[NumFreeList];
//    // return the proper index of the list
//    // for a n-Node block
//    static size_t FindIndex(size_t n);
//    // private:
//    // memory pool level
//    static char * pool_start;
//    static char * pool_end;
//    static Node * current_block;
    cout << "Pool space: " << (size_t)(pool_end - pool_start) << endl;
//    << "Pool start: " << (void *)pool_start << endl
//    << "Pool end: " << (void *)pool_end << endl
//    << "Current block: " << (void *)current_block << endl;
    
    for (int i = 0; i < NumFreeList; i++)
    {
        cout << "free list[" << i << "]: " << free_list[i] << endl;
    }
}
