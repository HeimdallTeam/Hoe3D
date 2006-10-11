#ifndef _HOE_CORE_H_
#define _HOE_CORE_H_

#include "hoe_types.h"

namespace HoeCore {

class ByteTileMap
{
	byte * m_map;
	uint m_width;
	uint m_height;
public:
	ByteTileMap();
	ByteTileMap(uint width, uint height);
	virtual ~ByteTileMap();
	void Create(uint width, uint height);
	HOE_INLINE byte Get(uint x, uint y) 
	{ 
		hoe_assert(m_map);
		hoe_assert(x >= 0 && x < m_width && y >= 0 && y < m_height);
		return m_map[y*m_width+x];
	}
	HOE_INLINE void Set(uint x, uint y, byte b) 
	{ 
		hoe_assert(m_map);
		hoe_assert(x >= 0 && x < m_width && y >= 0 && y < m_height);
		m_map[y*m_width+x] = b;
	}
	void Clear(byte b=0);
	bool Find(byte b, uint * x, uint * y);
	void Pisek(uint x, uint y, byte obl, byte vypln);
};

} // namespace HoeCore

#endif // _HOE_CORE_H_

