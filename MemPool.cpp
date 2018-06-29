#include <iostream>
#include "MemPool.h"
// bookkeeping
int pool_allocation_count;
int chunk_allocation_count;

//size_t MemPool::AllocSize =  (MemPool::ByteLimit << 2) + sizeof(Node *);
size_t MemPool::AllocSize =  (1 << 27) + sizeof(Node *);

MemPool::Node * MemPool::free_list[NumFreeList] = {nullptr};
char * MemPool::pool_start = nullptr;
char * MemPool::pool_end = nullptr;
MemPool::Node * MemPool::current_block = nullptr;


MemPool::MemPool()
{
    pool_start = reinterpret_cast<char*>(::operator new(AllocSize));
    reinterpret_cast<Node *>(pool_start)->prev = current_block;
    current_block = reinterpret_cast<Node *>(pool_start);
    
    pool_start += sizeof(Node *);
    pool_end = pool_start + AllocSize;
   
}
size_t MemPool::RoundUp(size_t n)
{
//    if (n % Align)
//        return (n + (Align - n % Align));
    int m = Align;
    while (m < n)
        m <<= 1;
    return m;
}
size_t MemPool::FindIndex(size_t n)
{
    int count = 0;
    
    int m = Align;
    while (m < n)
    {
        m <<= 1;
        count++;
    }
//    std::cout << "Size: " << n << " List: " << count << std::endl;
    return count;
}
void * MemPool::GetBlock(size_t n)
{
//    Report();
    size_t block_size = RoundUp(n);
//    std::cout << block_size << std::endl;
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
//        std::cout << "Memory pool allocation: " << n << " bytes\n";
//        std::cout << "Enter if block 1\n";
        pool_allocation_count++;
//        if (pool_space)
//        {
////            std::cout << "HI\n";
////            std::cout << "index: " << FindIndex(pool_space) << std::endl;
//            auto & list = free_list[FindIndex(pool_space)];
//            reinterpret_cast<Node *>(pool_start)->next = list;
//            list = reinterpret_cast<Node *>(pool_start);
//        }
        char * temp = reinterpret_cast<char*>(::operator new(AllocSize));
        std::cout << "Allocation suceeded. Memory: " << reinterpret_cast<void *>(temp) << ".\n";

        reinterpret_cast<Node *>(temp)->prev = current_block;
        current_block = reinterpret_cast<Node *>(temp);

        pool_start = temp + sizeof(Node *) + block_size;
        pool_end = temp + AllocSize;
//        Report();
//        std::cout << "Break if\n";
        AllocSize = AllocSize << 1;
//        std::cout << "AllocSize: " << AllocSize << std::endl;
        return reinterpret_cast<void *>(temp + sizeof(Node *));
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
//            std::cout << "Reusing Size: " << n << "\n";
//            std::cout << "Reusing "
            result = free_list_use;
            free_list_use = result->next;
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
//        std::cout << "serious!\n";
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
