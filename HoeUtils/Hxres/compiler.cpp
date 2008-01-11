
#include "StdAfx.h"
#include "compiler.h"
#include "error.h"

using namespace HoeCore;

Compiler * Compiler::Create(HoeCore::String&, int type, Stream& s)
{
    switch (type)
    {
	case ERT_Picture:
		return new PictureCompiler(s);
	case ERT_Stream:
		return new StreamCompiler(s);
    default:
		hoe_assert(!"Unknown compiler type.");
        return NULL;
    };
}

//////////////////////////////////////////////////////////
// Compilers
bool CheckArg(const CString name, const Universal& value, Universal::Type type, bool th=true)
{
	if (value.GetType() != type)
	{
		if (th)
		{
			throw ConvertError(name, 
				value.GetTypeName(),
				Universal::GetTypeName(type));
		}
		return false;
	}
	return true;
}

// picture
bool PictureCompiler::AddProp(const CString name, const Universal& value)
{
	if (name == "File")
	{
		CheckArg(name, value, Universal::TypeString);
        im.SetSource(value.GetStringValue());
		return true;
	}
	else
		throw UnknownError(name, "attribute");
	return false;
}

bool PictureCompiler::AddProp(const HoeCore::CString name, const VectorUniversal& value)
{
	return false;
}

void PictureCompiler::Done()
{
	HoeRes::Res::HeadResource head;
	head.id = 0;
	head.size_struct = sizeof(head);
	head.version_struct = 1;
	m_out.Write(&head, sizeof(head));
}

bool PictureCompiler::Func(const HoeCore::CString name, const VectorUniversal& value)
{
    printf("Running func %s width %d arguments.\n", (const tchar*)name, value.Count());
	return false;
}

/////
File::File()
{
	f = NULL;
}

File::~File()
{
	if (f) fclose(f);
}

void File::Open(const HoeCore::CString s)
{
	f = fopen(s, "wb");
}

void File::Write(void* ptr, size_t size)
{
	if (f) fwrite(ptr, size, 1, f);
}





