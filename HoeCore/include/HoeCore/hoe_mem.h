
#ifndef _HOE_CORE_MEM_H_
#define _HOE_CORE_MEM_H_

namespace HoeCore {

/**
* Bezpecne presunuti pameti, 
* vyhodne pokud se zdrojove a cilove casti prekryvaji
*/
void CrossMemMove(void * dest, void * src, size_t size);

template<typename TYPE> void MemSwitch(TYPE* a, TYPE* b,  const TYPE ** ctrl = NULL)
{
	unsigned char buff[sizeof(TYPE)];
	memcpy(buff, a, sizeof(TYPE));
	memcpy(a, b, sizeof(TYPE));
	memcpy(b, buff, sizeof(TYPE));
	if (ctrl)
	{
		if (a == *ctrl)
			*ctrl = b;
		else if (b == *ctrl)
			*ctrl = a;
	}
}

/** Optimalizator pro alokovani malych objektu */
class MemoryPool
{
	struct PoolItem
	{
		PoolItem * next;
		char * base;
		size_t size;
		size_t max;
		inline size_t GetAvail() { return max - size; }
		void * GetMem(size_t s, size_t aligment);
		void * Clone(const void * p, size_t s, size_t aligment);
	};
	PoolItem * m_pool;
	PoolItem * CreateNew(size_t size);
	PoolItem * FindFree(size_t size);
public:
	struct Stats
	{
		size_t numpools;
		size_t avail;
		size_t used;
	};

	MemoryPool();
	~MemoryPool();
	/** Prirazeni pameti z poolu */
	void * GetMem(size_t s, size_t aligment=1);
	/** Zkopirovani pameti do poolu */
	void * Clone(const void * p, size_t s, size_t aligment=1);
	/** Uvolneni cele pameti */
	void Free();
	/** Statistiky */
	void GetStats(Stats* stat);

};

} // namespace HoeCore

void * operator new(size_t, HoeCore::MemoryPool& pool);
//void * operator new(size_t, void *);
void * operator new[](size_t, HoeCore::MemoryPool& pool);
//void * operator new[](size_t, void *);
// empty delete operators
void operator delete( void* , HoeCore::MemoryPool& pool );

#if 0 
inline void * operator new(size_t s, void* ptr)
{
	return ptr;
}
inline void * operator new[](size_t s, void* ptr)
{
	return ptr;
}
inline void operator delete( void* , void* )
{
}
#endif

#endif // _HOE_CORE_MEM_H_
