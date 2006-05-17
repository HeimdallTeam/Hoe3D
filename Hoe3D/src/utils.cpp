
#include "system.h"
/*!!!*///#include <dxerr9.h>
#include "utils.h"
#include "../include/hoeinterfaces.h"
#include "hoe3d_math.h"


//////// Conzole /////////////
XHoeConsole * _Con;

void Con_Print(const char * szFormat,...)
{
	if (_Con)
	{
		static char szBuff[1024];

		va_list args;

		va_start(args, szFormat);
		vsnprintf( szBuff, 1024, szFormat, args );
		va_end(args);

		_Con->Con_Print(szBuff);
	}
}

void Con_Print(int dlevel, const char * szFormat,...)
{
	if (dlevel > 2) return;

	if (_Con)
	{
		static char szBuff[1024];

		va_list args;

		va_start(args, szFormat);
		vsnprintf( szBuff, 1024, szFormat, args );
		va_end(args);

		_Con->Con_Print(szBuff);
	}
}

void Con_Print(HoeLog * log, const char * szFormat,...)
{
	if (_Con)
	{
		static char szBuff[1024];

		va_list args;

		va_start(args, szFormat);
		vsnprintf( szBuff, 1024, szFormat, args );
		va_end(args);

		log->Log("%s",szBuff);
	}
}

void Con_Print(HoeMath::MATRIX &m)
{
	Con_Print("%f %f %f %f", m._11,m._12,m._13,m._14);
	Con_Print("%f %f %f %f", m._21,m._22,m._23,m._24);
	Con_Print("%f %f %f %f", m._31,m._32,m._33,m._34);
	Con_Print("%f %f %f %f", m._41,m._42,m._43,m._44);
}

int string_ex(const char * ex,const char * str)
{
	int i;
	for (i=0;ex[i];i++)
	{
		if (ex[i] < str[i])
			return 1;
		else if (ex[i] > str[i])
			return -1;
	}
	if (str[i] == '\0' || str[i] == ' ')
		return 0;
	else
		return 1;
}

void QuitGame(bool hard, int code)
{
#ifdef _WIN32
	if (!hard)
	{
		Con_Print("Call PostQuitMessage() with code: %d",code);
		PostQuitMessage(code);
	}
	else
#endif
	{
		Con_Print("Call exit() with code: %d",code);
		exit(code);
	}
}

void HoeLog::Log(const char * szFormat,...)
{
}

HoeLogConsole::HoeLogConsole(const char * name)
{
	strcpy(m_name,name);
}

void HoeLogConsole::Log(const char * szFormat,...)
{
	static char szBuff[1024];
	int lnsize;
	sprintf(szBuff,"%s: ",m_name);
	lnsize = strlen(szBuff);

	va_list args;

	va_start(args, szFormat);
	vsnprintf( szBuff + lnsize, 1024 - lnsize, szFormat, args );
	va_end(args);

	_Con->Con_Print(szBuff);
}




