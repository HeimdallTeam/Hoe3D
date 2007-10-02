
#include "StdAfx.h"
#include "../include/hoe_types.h"
#include "../include/hoe_log.h"
#include "../../HoeCore/include/hoe_string.h"

void HoeLog::Log(const char * szFormat,...)
{
}

////////////////////////////////////

HoeLogConsole::HoeLogConsole(const tchar * name)
{
	HoeCore::string::copy(m_name,name,sizeof(m_name)/sizeof(tchar));
}

void HoeLogConsole::Log(const char * szFormat,...)
{
	static char szBuff[1024];
	size_t lnsize;
	sprintf(szBuff,"%s: ",m_name);
	lnsize = strlen(szBuff);

	va_list args;

	va_start(args, szFormat);
	HoeCore::string::vsnprintf( szBuff + lnsize, 1024 - lnsize, szFormat, args );
	va_end(args);

	Con_Print(szBuff);
}

///////////////////////////////////////
HoeLogFile::HoeLogFile(const tchar * name)
{
	m_file = _wfopen(name,T("wt")); 
}

HoeLogFile::~HoeLogFile()
{
	if (m_file)
		fclose(m_file);
}

void HoeLogFile::Log(const char * szFormat,...)
{
	static char szBuff[1024];
	va_list args;

	va_start(args, szFormat);
	HoeCore::string::vsnprintf( szBuff, 1023, szFormat, args );
	va_end(args);

	if (m_file)
	{
		fputs(szBuff, m_file);
		fflush(m_file);
	}
}





