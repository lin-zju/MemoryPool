#include <new>
class MemPool
{
public:
	static void * alloc(size_t n);
	static void * dealloc(char * p, size_t n);
    ~MemPool();
// private:
    union Node
    {
        Node * next;
        char byte;
        // no visible to free_list
        Node * prev;
    };
	static const size_t Align = sizeof(Node *);
	static const size_t ByteLimit = 128;
	static const size_t NumFreeList = ByteLimit / Align;
	// AllocSize must be larger than Align + ByteLimit
    static const size_t AllocSize = 256;
	static size_t RoundUp(size_t n);

// private:
	// freelist level
	
	static Node * free_list[NumFreeList];
	// return the proper index of the list 
	// for a n-Node block
	static size_t FindIndex(size_t n);
// private:
	// memory pool level
	static char * pool_start;
	static char * pool_end;
	static Node * current_block;
	// get an n-Node blocks from the memory pool
	// if no blocks is available, throw AllocFail
	void * GetBlock(size_t n);
    void Report();
};


