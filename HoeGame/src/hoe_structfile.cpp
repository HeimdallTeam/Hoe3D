
#include "StdAfx.h"
#include "../include/hoe_structfile.h"
#include <locale.h>

void yyobjectrestart ( FILE *input_file );
int yyobjectlex();
extern unsigned char *yyobjecttext;
extern int yyobjectleng;
extern int yyobjectlineno;

BEGIN_HOEGAME

static int lex()
{
	int type = yyobjectlex();
	while (type == '\n')
		type = yyobjectlex();
	return type;
}

static int lexn()
{
	return yyobjectlex();
}

ObjectFileParser::ObjectFileParser()
{
	m_f = NULL;
}

ObjectFileParser::~ObjectFileParser()
{
	Close();
}

bool ObjectFileParser::Open(const char *fname)
{
	setlocale(LC_NUMERIC, "C");

	FILE * m_f = fopen(fname, "rt");
	if (!m_f)
		return false;
	yyobjectrestart(m_f);
	return true;
}

void ObjectFileParser::Close()
{
	if (m_f)
		fclose(m_f);
	m_f = NULL;
}

bool ObjectFileParser::ParseObject()
{
	int type = 0;
	if ((type=lex()) == 257)
	{
		// parse jmena
		m_objtype = (char*)yyobjecttext;
		// '{'
		type=lex();
		if (type != '{')
		{
			m_error = "syntax error, `{' requied";
			return false;
		}
		return true;
	}
	if (type != 0)
	{
		m_error = "syntax error on end of file";
	}
	return false;
}

const Property * ObjectFileParser::GetNextProperty()
{
	int type = 0;
	// jmeno
	if ((type=lex()) == 257)
	{
		m_property.SetName((char*)yyobjecttext);
		if (lexn() != '=')
		{
			m_error = "syntax error, `=' requied";
			return NULL;
		}
		// value
		switch (type = lexn())
		{
		case 257:
		case 258:
			m_property.Set((char*)yyobjecttext);
			break;
		case '(': // vector
			if (!ScanVector())
				return NULL;
			break;
		case '\n':
			m_error = "syntax error, empty value";
			return NULL;
		default:
			m_error.printf("syntax error, unknown character %c", type);
			return NULL;
		};
		if (lexn() != '\n')
		{
			m_error = "syntax error, new line requied";
			return NULL;
		}
		// 
		return &m_property;
	}
	if (type != '}')
	{
		m_error = "syntax error, `}' requied";
	}
	return NULL;
}


int ObjectFileParser::GetLineNum()
{
	return yyobjectlineno;
}

bool ObjectFileParser::ScanVector()
{
	int type;
	float vec[100];
	int s = 0;
	while (1)
	{
		if (s >= 100)
		{
			m_error = "vector is too big";
			return false;
		}
		// cislo
		type = lexn();
		switch (type)
		{
		case 259:
		case 260:
			vec[s++] = (float)atof((char*)yyobjecttext);
			break;
		default:
			m_error = "vector is only for numbers";
			return false;
		}
		type = lexn();
		if (type == ')')
		{
			m_property.Set(vec, s);
			return true;
		}
		if (type != ',')
		{
			m_error = "syntax error, `,' or `)' requied";
			return false;
		}
	}
}

///////////////////////////////////////////////////////
// Writer
ObjectFileWriter::ObjectFileWriter()
{
	m_file = NULL;
}

ObjectFileWriter::~ObjectFileWriter()
{
	Close();
}


bool ObjectFileWriter::Open(const char * fname)
{
	setlocale(LC_NUMERIC, "C");

	m_file = fopen(fname , "wt");
	if (!m_file)
		return false;

	m_level = 0;
	return true;
}

void ObjectFileWriter::Close()
{
	if (m_file)
		fclose(m_file);
	m_file = NULL;
}

void ObjectFileWriter::BeginObject(const char * objname)
{
	WriteLevel();
	fprintf(m_file, "%s\n", objname);
	WriteLevel();
	fprintf(m_file, "{\n");
	m_level++;
}

void ObjectFileWriter::EndObject()
{
	if (m_level > 0)
		m_level--;
	WriteLevel();
	fprintf(m_file, "}\n");
}

void ObjectFileWriter::AddString(const char * name, const char * str)
{
	WriteLevel();
	fprintf(m_file, "%s = \"%s\"\n", name, str);
}

void ObjectFileWriter::AddFloat(const char * name, float f)
{
	WriteLevel();
	fprintf(m_file, "%s = %f\n", name, f);
}

void ObjectFileWriter::AddBool(const char * name, bool b)
{
	WriteLevel();
	if (b)
		fprintf(m_file, "%s = true", name);
	else
		fprintf(m_file, "%s = false\n", name);
}

void ObjectFileWriter::AddHex(const char * name, unsigned long dw)
{
	WriteLevel();
	fprintf(m_file, "%s = %x\n", name, dw);
}

void ObjectFileWriter::AddVector(const char * name, int num, ...)
{
	WriteLevel();
	fprintf(m_file, "%s = (",name);

	va_list args;

	va_start(args, name);
	bool comma = false;
	while (num > 0)
	{
		register float f = va_arg( args, float);
		if (comma)
			fprintf(m_file, ", %f", f);
		else
		{
			fprintf(m_file, "%f", f);
			comma = true;
		}
		num--;
	}
	va_end(args);
	fprintf(m_file, ")\n");

}

void ObjectFileWriter::WriteLevel()
{
	for (int l=0;l < m_level;l += 10)
		fwrite("\t\t\t\t\t\t\t\t\t\t", 1, 
			(m_level-l) > 10 ? 10:(m_level-l), m_file);
}

END_HOEGAME


