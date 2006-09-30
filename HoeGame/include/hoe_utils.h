
#ifndef _HOE_GAME_UTILS_H_
#define _HOE_GAME_UTILS_H_

#include "hoe_game.h"

BEGIN_HOEGAME

bool SetRootDir(const char * dir);
const char * GetBaseDir(const char * path);

void RemoveEndLine(char * line);

bool SetRootFromExe(const char * path);
#ifdef _WIN32
bool SetRootFromInstance(HINSTANCE hInstance);
#endif

#ifdef _WIN32
#define BEGIN_TRY __try {
#define END_TRY(code)  } __except (HoeGame::ExpFilter(GetExceptionInformation(), GetExceptionCode())) \
{ \
	code ; \
} 
LONG WINAPI ExpFilter(EXCEPTION_POINTERS* pExp, DWORD dwExpCode);
#else
#define BEGIN_TRY
#define END_TRY(code)
#endif

/**
* Trida udrzujici mnozinu objektu
*/
template<class C> class PtrSet
{
protected:
	uint m_count;
	uint m_size;
	C * m_ptr;
	void Resize(uint num)
	{
		if (!num) return;
		m_ptr = (C*)realloc(m_ptr, num * sizeof(C));
		assert(m_ptr); /*!!!*/
		m_size = num;
	}
public:
	PtrSet()
	{
		m_count = 0;m_size = 0; m_ptr = NULL;
	}
	PtrSet(uint initnum)
	{
		m_count = 0;m_size = 0; m_ptr = NULL;
		Resize(initnum);
	}
	~PtrSet()
	{
		if (m_ptr) free(m_ptr);
	}
	void Add(C c)
	{
		if (m_size == m_count)
			Resize(m_size + (m_size/5>=1 ? m_size/5:1));
		m_ptr[m_count] = c;m_count++;
	}
	C & Get(uint n)
	{
		assert(n < m_count);
		return m_ptr[n];
	}
	C & operator [] (const int index)
	{
		return Get(index);
	}
	/** Odebrani vsech stejnych objektu */
	void Remove(C c)
	{
		assert(m_ptr);
		for (uint i=0;i < m_count;)
		{
			if (m_ptr[i] == c)
			{
				RemoveIndex(i);
			}
			else
				i++;
		}
	}
	void RemoveIndex(int index)
	{
		if (index < (m_count-1))
			m_ptr[index] = m_ptr[m_count-1];
		m_count--;
	}
	uint Count() { return m_count; }
	void SetCount(int count) { m_count=count; }
};

template <class C> class PtrList : public PtrSet<C>
{
};

END_HOEGAME

#endif // _HOE_GAME_UTILS_H_

