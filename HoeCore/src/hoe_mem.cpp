
#include "StdAfx.h"
#include "../include/HoeCore/hoe_mem.h"

void * operator new(size_t s, HoeCore::MemoryPool& pool)
{
	return pool.GetMem(s, sizeof(void*)); // todo default
}

void * operator new[](size_t s, HoeCore::MemoryPool& pool)
{
	return pool.GetMem(s, sizeof(void*));
}

void operator delete( void* , HoeCore::MemoryPool& pool )
{
	/* empty operator */
}

////////////////////////////////////////////////////

void HoeCore::CrossMemMove(void * dest, void * src, size_t size)
{
	// rozhoduje smer odkud a kam
	byte* d = reinterpret_cast<byte*>(dest);
	byte* s = reinterpret_cast<byte*>(src);
	if (d < s)
	{
		// musi projet popredu, optimalizace by byla kdyby memcpy kopirovala vzdy odpredu
		size_t di = s - d;
		for (size_t i=0;i < size;i+=di)
		{
			memcpy(d+i, s+i, size < (di+i) ? size-i:di);
		}
	}
	else if (d > s)
	{
		int di = d - s;
		for (int i=size-di;i >= 0;i-=di)
		{
			memcpy(d+i, s+i, i > di ? di:di-i);
		}
	}
}

/////////////////////////////////////////////////
HoeCore::Buffer::Buffer() : m_buff(0), m_alloc(0)
{
}

HoeCore::Buffer::~Buffer()
{
	Free();
}

void * HoeCore::Buffer::GetPtr(size_t num)
{
	if (m_buff && num <= m_alloc)
		return m_buff;
	if (m_buff) Free();
	return m_buff = malloc(m_alloc = num);
}

void HoeCore::Buffer::Free()
{
	if (m_buff)
	{
		free(m_buff);
		m_buff = 0;
		m_alloc = 0;
	}
}

/////////////////////////////////////////////////
// Pools
void * HoeCore::MemoryPool::PoolItem::GetMem(size_t s, size_t aligment)
{
	// aligment
	if (s+aligment-1 > (max-size)) return NULL;
	if (aligment > 1) // find aligment
	{
		while (size % aligment != 0) size++;
	}
	void * ret = base + size;
	size += s;
	return ret;
}

void * HoeCore::MemoryPool::PoolItem::Clone(const void * p, size_t s, size_t aligment)
{
	void * pp = GetMem(s, aligment);
	memcpy(pp, p, s);
	return pp;
}

HoeCore::MemoryPool::PoolItem * HoeCore::MemoryPool::CreateNew(size_t size)
{
	PoolItem pi = {0};
	pi.max = size + sizeof(PoolItem);
	pi.base = new char[pi.max];
	return reinterpret_cast<PoolItem*>(pi.Clone(&pi, sizeof(PoolItem), sizeof(PoolItem*)));
}

HoeCore::MemoryPool::PoolItem * HoeCore::MemoryPool::FindFree(size_t size)
{
	PoolItem * b = m_pool;
	while (b)
	{
		if (b->GetAvail() >= size)
			return b;
		b = b->next;
	}
	return NULL;
}
void HoeCore::MemoryPool::Free()
{
	while (m_pool)
	{
		PoolItem * next = m_pool->next;
		delete [] m_pool->base;
		m_pool = next;
	}
}

HoeCore::MemoryPool::MemoryPool() 
{ 
	m_pool = NULL; 
}

HoeCore::MemoryPool::~MemoryPool()
{
	Free();
}

void * HoeCore::MemoryPool::GetMem(size_t s, size_t aligment)
{
	PoolItem * b = FindFree(s+aligment-1);
	if (b) { return b->GetMem(s,aligment); }
	// vytvorit
	b = CreateNew(s > 10000 ? s*2:10000);
	b->next = m_pool;
	m_pool = b;
	return b->GetMem(s,aligment);
}

void * HoeCore::MemoryPool::Clone(const void * p, size_t s, size_t aligment)
{
	void * pp = GetMem(s, aligment);
	memcpy(pp, p, s);
	return pp;
}

void HoeCore::MemoryPool::GetStats(HoeCore::MemoryPool::Stats* stat)
{
	memset(stat, 0, sizeof(Stats));
	PoolItem* pi = m_pool;
	while (pi)
	{
		stat->numpools++;
		stat->avail += pi->GetAvail();
		stat->used += pi->size;
		pi = pi->next;
	}
}





