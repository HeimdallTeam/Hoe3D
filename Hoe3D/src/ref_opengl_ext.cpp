
// ciste opengl zdrojak

#include "system.h"
#include "utils.h"
#include "ref_opengl.h"
#include "glext.h"

PFNGLPROGRAMLOCALPARAMETER4FVARBPROC glProgramLocalParameter4fvARB = NULL;
PFNGLGENPROGRAMSARBPROC glGenProgramsARB = NULL;
PFNGLBINDPROGRAMARBPROC glBindProgramARB = NULL;
PFNGLPROGRAMSTRINGARBPROC glProgramStringARB = NULL;

PFNGLGENBUFFERSARBPROC glGenBuffersARB = NULL;// Generov�n� VBO jm�na
PFNGLBINDBUFFERARBPROC glBindBufferARB = NULL;// Zvolen� VBO bufferu
PFNGLBUFFERDATAARBPROC glBufferDataARB = NULL;// Nahr�v�n� dat VBO
PFNGLDELETEBUFFERSARBPROC glDeleteBuffersARB = NULL;// Maz�n� VBO

//////////////////////////////////

bool RefOpenGL::TestExt(const char * ext_name,const char * extensions)
{
	const char * pe;
	if (extensions)
		pe = extensions;
	else
		pe = (const char *) glGetString(GL_EXTENSIONS);

	const char * en = ext_name;
	while (*pe)
	{
		while (*pe && *pe != ' ' && *en)
		{
			if (*pe != *en)
				break;
			pe++;
			en++;
		}
		if ((*pe == ' ' || *pe == '\0') && *en == '\0')
			return true;
		en = ext_name; // reset ukazatele..
		// odskoci si na zacatek dalsi
        while (*pe && *pe != ' ') pe++;
		if (*pe == '\0')
			break;
		else
			*pe++;
	}
	return false;
}

void RefOpenGL::PrintGlExt()
{
	//
	char buff[256];
	char * pe = (char *) glGetString(GL_EXTENSIONS);
	char * pb = buff;
    while (*pe)
	{
		if (*pe == ' ')
		{
			pe++;
			*pb = '\0';
			Con_Print(buff);
			pb = buff;
		}
		else
		{
			*pb = *pe;
			pb++;
			pe++;
		}
	}

}

GLPROCEDURE RefOpenGL::GetProc(const char * name)
{
#ifdef _WIN32
	return (GLPROCEDURE)wglGetProcAddress(name);
#endif
#ifdef _LINUX
	return glXGetProcAddressARB((GLubyte*)name);
#endif
}


void RefOpenGL::LoadExtensions()
{
	memset(&ext, 0, sizeof(ext));
	// Compressed textures
	ext.comp = TestExt("GL_ARB_texture_compression");
	if (ext.comp)
		Con_Print("Use extension: GL_ARB_texture_compression");
	/*"GL_3DFX_texture_compression_FXT1";
	"GL_EXT_texture_compression_s3tc";
	"GL_S3_s3tc";
	GLint NumFormat = 0;
	GLint *Formats = NULL;
	glGetIntegerv(GL_NUM_COMPRESSED_TEXTURE_FORMATS_ARB, &NumFormat);
	NumFormat;*/
	
	// Vertex buffer
	ext.vb = TestExt("GL_ARB_vertex_buffer_object");
	if (ext.vb)
	{
		Con_Print("Use extension: GL_ARB_vertex_buffer_object");
		glGenBuffersARB = (PFNGLGENBUFFERSARBPROC) GetProc("glGenBuffersARB");
		glBindBufferARB = (PFNGLBINDBUFFERARBPROC) GetProc("glBindBufferARB");
		glBufferDataARB = (PFNGLBUFFERDATAARBPROC) GetProc("glBufferDataARB");
		glDeleteBuffersARB = (PFNGLDELETEBUFFERSARBPROC) GetProc("glDeleteBuffersARB");
	}
	else
	{
		glGenBuffersARB = NULL;// Generov�n� VBO jm�na
		glBindBufferARB = NULL;// Zvolen� VBO bufferu
		glBufferDataARB = NULL;// Nahr�v�n� dat VBO
		glDeleteBuffersARB = NULL;// Maz�n� VBO
	}
	// Vertex shader
	ext.vs = TestExt("GL_ARB_vertex_program");
	if (ext.vs)
	{
		Con_Print("Use extension: GL_ARB_vertex_program");
		glProgramLocalParameter4fvARB = (PFNGLPROGRAMLOCALPARAMETER4FVARBPROC) GetProc("glVertexAttrib4fvARB");
		assert(glProgramLocalParameter4fvARB);
		glGenProgramsARB = (PFNGLGENPROGRAMSARBPROC) GetProc("glGenProgramsARB");
		assert(glGenProgramsARB);
		glBindProgramARB = (PFNGLBINDPROGRAMARBPROC) GetProc("glBindProgramARB");
		assert(glBindProgramARB);
		glProgramStringARB = (PFNGLPROGRAMSTRINGARBPROC) GetProc("glProgramStringARB");
		assert(glProgramStringARB);
	}
	else
	{
		glProgramLocalParameter4fvARB = NULL;
		glGenProgramsARB = NULL;
		glBindProgramARB = NULL;
		glProgramStringARB = NULL;
	}
}


