
#include "StdAfx.h"
#include "../include/hoe_core.h"
#include "../include/hoe_string.h"

////////////////////////////////
// string class
namespace HoeCore {


namespace string {

int vsnprintf(wchar_t *, size_t, const char *, va_list)
{
	return 0;
}

int vsnprintf(wchar_t *, size_t, const wchar_t *, va_list)
{
	return 0;
}

void copy(tchar *, const char *, size_t)
{
	
}

void copy(tchar *, const wchar_t *, size_t)
{
}



} // end namespace

/*const String & String::operator = (const char * s)
{
	return *this;
}*/

} // namespace HoeCore
