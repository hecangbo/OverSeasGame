#ifndef _MEMORY_POOL_H_
#define _MEMORY_POOL_H_

class MemoryPool
{
public:
	enum { EXPANSION_SIZE = 1};

	MemoryPool(int aItemSize, int nItemCount = EXPANSION_SIZE)
		: m_ItemSize(aItemSize)
	{
		ExpandFreeList(nItemCount);
	}

	~MemoryPool()
	{
		//free all memory in the list
		MemoryPool* pNext = 0;
		for(pNext = m_pFreeList; pNext != 0; pNext = m_pFreeList)
		{
			m_pFreeList = m_pFreeList->m_pFreeList;
			delete [](char*)pNext;
		}
	}

	void* Alloc()
	{
		if(m_pFreeList == 0)
		{
			ExpandFreeList();
		}

		//get free memory from head
		MemoryPool* pHead = m_pFreeList;
		m_pFreeList = m_pFreeList->m_pFreeList;
		return pHead;
	}

	void Free(void* p)
	{
		//push the free memory back to list
		MemoryPool* pHead = static_cast<MemoryPool*>(p);
		pHead->m_pFreeList = m_pFreeList;
		m_pFreeList = pHead;
	}

	int GetBufferSize(){return m_ItemSize;}

protected:
	//allocate memory and push to the list
	void ExpandFreeList(int nItemCount = EXPANSION_SIZE)
	{
		unsigned int nSize = m_ItemSize > sizeof(MemoryPool*) ? m_ItemSize : sizeof(MemoryPool*);
		MemoryPool* pLastItem = static_cast<MemoryPool*>(static_cast<void*>(new char[nSize]));
		m_pFreeList = pLastItem;
		for(int i = 0; i < nItemCount - 1; ++i)
		{
			pLastItem->m_pFreeList = static_cast<MemoryPool*>(static_cast<void*>(new char[nSize]));
			pLastItem = pLastItem->m_pFreeList;
		}

		pLastItem->m_pFreeList = 0;
	}

private:
	int m_ItemSize;
	MemoryPool* m_pFreeList;
};

#endif