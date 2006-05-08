
/**
   @file    ref_opengl_ext.h
   @date    May 2006
   @version 1.0
   @brief   Rozsireni OpenGL
*/

#ifndef _HOE_OPENGL_EXT_H_
#define _HOE_OPENGL_EXT_H_

#include "glext.h"

typedef void (*GLPROCEDURE)(void);

namespace GLExt { 

struct Extension
{
	bool supported;

	Extension();
	inline bool IsSupported() { return supported; }
	/**
	* Test na pritomnost rozsireni
	* @param ext_name Jmeno extension
	* @param extensions String extensions. Pokud je NULL funkce si zjisti rozsireni z ovladece OpenGL
	*/
	static bool TestExt(const char * ext_name,const char * extensions = NULL);
	/**
	* Vypise opengl rozisreni do konzole
	*/
	static void PrintGlExt();

	static GLPROCEDURE GetProc(const char * name);
};

struct CompressTextures : public Extension
{
	void Check();
};

class VertexBuffer : public Extension
{
public:
	void Check();
	PFNGLGENBUFFERSARBPROC glGenBuffersARB;// Generov�n� VBO jm�na
	PFNGLBINDBUFFERARBPROC glBindBufferARB;// Zvolen� VBO bufferu
	PFNGLBUFFERDATAARBPROC glBufferDataARB;// Nahr�v�n� dat VBO
	PFNGLDELETEBUFFERSARBPROC glDeleteBuffersARB;// Maz�n� VBO
};

}

#endif // _HOE_OPENGL_EXT_H_

