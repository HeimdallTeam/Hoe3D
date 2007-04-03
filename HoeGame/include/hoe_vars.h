
#ifndef _HOE_GAME_VARS_H_
#define _HOE_GAME_VARS_H_

#include "hoe_game.h"

BEGIN_HOEGAME

class CVarValue : public THoeVarValue
{
public:
	CVarValue() { i = 0; }
	CVarValue(float _f) { f = _f; }
};

class CVar : protected THoeVar
{
protected:
	static THoeVar * staticVars;
	static char lastError[1024];

	void Register();
	void Init(const char * name, int flags);
	void SetString(const char * str);
	bool ParseIndex(const char * idf, int &pos, int &flags, const THoeVarIndex *& ix) const;
public:
	CVar(const char * name, bool value, int flags=0);
	CVar(const char * name, int value, int flags=0);
	CVar(const char * name, float value, int flags=0);
	CVar(const char * name, const char * value, int flags=0);
	CVar(const char * name, const THoeVarIndex * s, THoeVarValue * val, size_t size, int flags=0);
	~CVar();
	const char * GetName() const { return name; }
	const char * GetStringValue();
	bool GetBool() const { return value.b; }
	int GetInt() const { return value.i; }
	float GetFloat() const { return value.f; }
	const char * GetString() const { return value.str; }

	bool GetBool(int i) const { return vars[i].b; }
	int GetInt(int i) const { return vars[i].i; }
	float GetFloat(int i) const { return vars[i].f; }
	const char * GetString(int i) const { return vars[i].str; }

	// structured types

	bool IsInt() const { return (flags & TVAR_TYPE) == TVAR_INTEGER; }
	bool IsFloat() const { return (flags & TVAR_TYPE) == TVAR_FLOAT; } 
	bool IsModified() const { return (flags & TVAR_MODIFIED) != 0; }
	bool IsArray() const { return (flags & TVAR_TYPE) == TVAR_ARRAY; } 
	bool IsStruct() const { return (flags & TVAR_TYPE) == TVAR_STRUCT; } 

	bool Set(int i);
	bool Set(float f);
	bool Set(bool b);
	bool Set(const char * str);

	bool SetStructItem(int i, int idf, int flags=TVAR_INTEGER);
	bool SetStructItem(float f, int idf, int flags=TVAR_FLOAT);
	bool SetStructItem(bool b, int idf, int flags=TVAR_BOOL);
	bool SetStructItem(const char * str, int idf, int flags=TVAR_STR);

	// statics
	static CVar * GetVar(const char * name);
	static CVar * GetFullVar(const char * path, int &pos, int &flags, const THoeVarIndex *& ix);
	static bool SetVarValue(const char * path, const char * vs);
	static bool SetVarValue(const char * path, float vf);

	static const char * GetLastError() { return lastError; }

	LUA_FUNCTION(l_setvar);
	LUA_FUNCTION(l_getvar);

	static int c_printvar(int argc, const char * argv[], void * param);
	static int c_printallvars(int argc, const char * argv[], void * param);
	static int c_setvar(int argc, const char * argv[], void * param);
};

END_HOEGAME

#endif // _HOE_GAME_VARS_H_

