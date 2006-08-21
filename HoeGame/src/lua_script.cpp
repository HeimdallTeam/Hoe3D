
#include "StdAfx.h"

extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

// kompatibilita se starsi versi luy
#ifndef LUA_VERSION_NUM
#define LUA_VERSION_NUM 500
#endif

#include "../include/lua_script.h"
#include "../include/hoe_resource_mgr.h"
#include "../include/hoe_lang.h"
#include "../include/hoe_fs.h"

BEGIN_HOEGAME

LuaScript * LuaScript::_this = NULL;

LuaThread::LuaThread()
{
	m_L = NULL;
}

bool LuaThread::Next()
{
	return lua_resume(m_L,0) == 0;
}

void LuaThread::run(const char *fn)
{
	lua_getglobal(m_L, fn);
	lua_pcall(m_L,0,0,0);
}

//////////////////////////////////////////////////////

LuaParam::LuaParam(lua_State * L)
{
	m_L = L;
	nump = 0;
}

bool LuaParam::CheckPar(int num, const char * par) const
{
	int i = 0;
	if (lua_gettop(m_L) != num)
		return false;

	while (par && *par)
	{
		i--;
		switch (*par)
		{
		case 'p':
			if (!lua_islightuserdata(m_L,i))
				return false;
			break;
		case 'n':
			if (!lua_isnumber(m_L,i))
				return false;			
			break;
		case 's':
			if (!lua_isstring(m_L,i))
				return false;
			break;
		case 'b':
		case '*':
			break;
		default:
			return false;
		}
		par++;
	}
	return true;
}

bool LuaParam::CheckPar(int num, const char * par, const char * func)
{
	if (!CheckPar(num, par))
	{
		Error("incorrect argument to function `%s'", func);
		return false;
	}
	return true;
}

int LuaParam::GetNum(int pos) const
{
	return (int)lua_tonumber(m_L, pos);
}

double LuaParam::GetDouble(int pos) const
{
	return lua_tonumber(m_L, pos);
}

const char * LuaParam::GetString(int pos) const
{
	return lua_tostring(m_L, pos);
}

bool LuaParam::GetBool(int pos) const
{
	return (lua_toboolean(m_L,pos) != 0);
}

void * LuaParam::GetPointer(int pos) const
{
	return lua_touserdata(m_L,pos);
}

void LuaParam::SaveString(const char *str)
{
	lua_pushstring(m_L,str);
	nump++;
}

void LuaParam::PushNum(int num)
{
	lua_pushnumber(m_L, num);
	nump++;
}

void LuaParam::PushPointer(void *p)
{
	lua_pushlightuserdata(m_L, p);
	nump++;
}

int LuaParam::GetNumParam() const
{
	return lua_gettop(m_L);
}

bool LuaParam::IsString(int pos) const
{
	return lua_isstring(m_L,pos) != 0;
}

bool LuaParam::IsNum(int pos) const
{
	return lua_isnumber(m_L,pos) != 0;
}

bool LuaParam::IsNil(int pos) const
{
	return lua_isnil(m_L, pos) != 0;
}

bool LuaParam::IsTable(int pos) const
{
	return lua_istable(m_L, pos) != 0;
}

void LuaParam::PushTable()
{
	lua_newtable(m_L);
	nump++;
}

void LuaParam::SetTableInteger(const char *par, int data, int tab)
{
#if LUA_VERSION_NUM >= 501
	lua_pushinteger(m_L, data);
	lua_setfield(m_L,tab,par);
#else
	assert(!"tato funkce neni dostupna ve verzi Lua 5.0");
#endif
}

int LuaParam::GetTableInteger(const char *par, int tab) const
{
#if LUA_VERSION_NUM >= 501
	lua_getfield(m_L,tab,par);
#else
	assert(!"tato funkce neni dostupna ve verzi Lua 5.0");
#endif
	int i = (int)lua_tonumber(m_L,-1);
	lua_pop(m_L, 1);
	return i;
}

void LuaParam::SetTableFloat(const char *par, float data, int tab)
{
#if LUA_VERSION_NUM >= 501
	lua_pushnumber(m_L, data);
	lua_setfield(m_L,tab,par);
#else
	assert(!"tato funkce neni dostupna ve verzi Lua 5.0");
#endif
}


float LuaParam::GetTableFloat(const char *par, int tab) const
{
#if LUA_VERSION_NUM >= 501
	lua_getfield(m_L,tab,par);
#else
	assert(!"tato funkce neni dostupna ve verzi Lua 5.0");
#endif
	float i = (float)lua_tonumber(m_L,-1);
	lua_pop(m_L, 1);
	return i;
}

void LuaParam::SetTablePointer(const char *par, void * data, int tab)
{
#if LUA_VERSION_NUM >= 501
	lua_pushlightuserdata(m_L, data);
	lua_setfield(m_L,tab,par);
#else
	assert(!"tato funkce neni dostupna ve verzi Lua 5.0");
#endif
}

void * LuaParam::GetTablePointer(const char *par, int tab) const
{
#if LUA_VERSION_NUM >= 501
	lua_getfield(m_L,tab,par);
#else
	assert(!"tato funkce neni dostupna ve verzi Lua 5.0");
#endif
	void * p = lua_touserdata(m_L,-1);
	lua_pop(m_L, 1);
	return p;
}

void LuaParam::Pop(int num)
{
	lua_pop(m_L, num);
}

void LuaParam::Error(const char * szFormat, ...)
{
	va_list args;
	va_start(args, szFormat);
	lua_pushvfstring(m_L, szFormat, args);
	va_end(args);
	lua_error(m_L);
}

//////////////////////////////////////////////////////
LuaFunc::LuaFunc(LuaScript * scr, const char * funcname) : LuaParam(scr->GetLua())
{
	lua_getglobal(m_L, funcname);
}

bool LuaFunc::Run(int nres)
{
	//lua_Debug ar;
	lua_call(m_L,nump,nres);
	/*switch (err)
	{
	case LUA_ERRRUN:
	case LUA_ERRMEM:
	case LUA_ERRERR:
		//lua_getstack(m_L,1,&ar);
		Console::Printf("%s", lua_tostring(m_L, -1));
		exit(1);
		break;
	};*/
	return false;
}

//////////////////////////////////////////////////////


LuaScript::LuaScript(BaseConsole * c)
{
	assert(_this == NULL);
	_this = this;
	con = c;
	engine = NULL;
	resources = NULL;
	lang = NULL;
}

bool LuaScript::Init()
{
	con->Con_Print("Lua init.."); 
	/* initialize Lua */
	m_L = lua_open();
	//lua_atpanic(L, print);
	/* load Lua base libraries */
	//luaL_opentests(m_L); 

	//lua_baselibopen(m_L);
	//lua_tablibopen(m_L);
	//lua_iolibopen(m_L);
	//lua_strlibopen(m_L);
	//lua_mathlibopen(m_L);
	//lua_dblibopen(m_L); 
	
	lua_register(m_L,"print",LuaScript::print);
	lua_register(m_L,"sleep",LuaScript::sleep);
	lua_atpanic(m_L,LuaScript::error);

	con->Con_Print("$Lua: " LUA_VERSION " " LUA_COPYRIGHT " $");
	con->Con_Print("$Authors: " LUA_AUTHORS " $");
    con->Con_Print("$URL: www.lua.org $");
	//con->Printf("Stack: %dKb", lua_getgcthreshold (m_L));

	return true;
}

void LuaScript::Close()
{
	if (m_L)
		lua_close(m_L);
	m_L = NULL;
}

void LuaScript::func(const char * name)
{
	LuaFunc f(this, name);
	f.Run(0);
}

bool LuaFile::Open(const char * filename)
{
	m_act = m_bufflength = 0;
	m_f = fopen(filename,"r");
	return (m_f != NULL);
}

void LuaFile::Close()
{
	fclose(m_f);
}

int LuaFile::GetNextChar()
{
	if (m_act < m_bufflength)
	{
		return m_buff[m_act++];
	}
	else
	{
		if (feof(m_f))
			return -1;

		m_bufflength = fread(m_buff, 1, sizeof(m_buff), m_f);
		if (m_bufflength > 0)
		{
			m_act=1;
			return m_buff[0];
		}
		else
			return -1;
	}
}

LuaPreprocess::LuaPreprocess(lua_State*L)
{
	m_L = L;
	m_constans = NULL;
	m_lerror = "success";
}

const char * LuaPreprocess::StdCopy(size_t *size)
{
	m_buff.Reset();
	while (!m_buff.IsFull())
	{
		char ic = m_f.GetNextChar();
		if (ic == -1)
			break;
		else
            m_buff.Add(ic);
	}
	*size = m_buff.GetSize();
	if (*size > 0)
        return m_buff;
	else
		return NULL;
}

#define IS_WORDCHAR(c) ((c>='a' && c<='z') || (c>='A' && c<='Z') || (c>='0' && c<='9') || c=='_')

const char * LuaPreprocess::IDfilter(size_t *size){

	char ic;

	m_buff.Reset();
	LuaTextBuffer w;

	while(1){
        
		w.Reset();

		// preskoceni neprazdnych znaku az do pismene nebo do konce radky
		do
		{
			if (m_buff.IsFull())
				goto returnbuff;

			ic=m_f.GetNextChar();
			if (ic == -1)
				goto returnbuff;

			if (IS_WORDCHAR(ic))
				w.Add(ic);
			else
                m_buff.Add(ic);

		} while (!IS_WORDCHAR(ic));

		if (ic == -1)
			goto returnbuff;

		// vyjmout zbytek slova (max 200 znaku)
		while (w.GetSize() < 200)
		{
            ic=m_f.GetNextChar();
			if (IS_WORDCHAR(ic))
				w.Add(ic);
			else
				break;
		}

		int i=0;

		for(i=0;m_constans[i].String!=NULL;i++){
			if(strlen(m_constans[i].String) == w.GetSize() && strncmp(m_constans[i].String,w,w.GetSize()) == 0) break;
		}

		if (m_constans[i].String!=NULL)
		{
			m_buff.WriteID(m_constans[i].ID);
		}
		else
		{
			m_buff.Add(w);
		}

		if (ic!=-1)
            m_buff.Add(ic);
		else
			break;
	}

returnbuff:
	*size = m_buff.GetSize();
	if (*size > 0)
        return m_buff;
	else
		return NULL;
}


const char *LuaPreprocess::getF(lua_State *L, void *ud, size_t *size)
{
	const char * buff;
	LuaPreprocess *lp = reinterpret_cast<LuaPreprocess *>(ud);
	if (lp->m_constans)
        buff = lp->IDfilter(size);
	else
		buff = lp->StdCopy(size);

	if (lp->m_foutdbg)
		fwrite(buff,1,*size,lp->m_foutdbg);
	return buff;

}

bool LuaPreprocess::LoadFile(const char *filename)
{
	char filestr[1024];
	m_foutdbg = NULL;
#ifdef _DEBUG
	sprintf(filestr, "%s.dbg", filename);
	m_foutdbg = fopen(filestr, "w");
#endif

	sprintf(filestr, "@%s", filename);
	//if (luaL_loadfile(m_L, fn) == LUA_ERRFILE)
	//	return false;

 // LoadF lf;
	int status;
 // int c;
 // int fnameindex = lua_gettop(L) + 1;  /* index of filename on the stack */
 //   lua_pushfstring(L, "@%s", filename);
	if (!m_f.Open(filename))
	{
		lua_pushfstring(m_L, "Failed open script '%s'", filename);
		m_lerror = lua_tostring(m_L, -1);
		return false;
	}

 // if (lf.f == NULL) return errfile(L, fnameindex);  /* unable to open file */
 // c = ungetc(getc(lf.f), lf.f);
 // if (!(isspace(c) || isprint(c)) && lf.f != stdin) {  /* binary file? */
 //   fclose(lf.f);
 //   lf.f = fopen(filename, "rb");  /* reopen in binary mode */
 //   if (lf.f == NULL) return errfile(L, fnameindex); /* unable to reopen file */
 // }
	status = lua_load(m_L, getF, this, filestr);
	m_f.Close();

	if (status)
	{
		m_lerror = lua_tostring(m_L, -1);
		return false;
	}
	else
	{
		return true; 
	}

	if (m_foutdbg)
		fclose(m_foutdbg);
	m_foutdbg = NULL;
 
}

bool LuaScript::Load(const char * fn, LuaPreprocess::IDConst * csts)
{
	LuaPreprocess lp(m_L);
	lp.SetConstans(csts);
	
	if (!lp.LoadFile(fn))
	{
		con->Printf("Lua error: %s", lp.GetLastError());
		return false;
	}

	lua_pcall(m_L,0,0,0);

	return true;
}

void LuaScript::AddFunc(const char * funcname, int (*func)(lua_State*))
{
	lua_register(m_L,funcname,func);
};

bool LuaScript::Connect(FileSystem * fs)
{
	filesystem = fs;
	lua_register(m_L,"AddFile",LuaScript::loadfile);
	return true;
}

bool LuaScript::Connect(IHoe3DEngine *eng)
{
	engine = eng;
	lua_register(m_L,"hoe",LuaScript::hoe);
	return true;
}

bool LuaScript::Connect(ResourceMgr *res)
{
	resources = res;
	lua_register(m_L,"LoadResource",LuaScript::loadresource);
	return true;
}

bool LuaScript::Connect(Lang * l)
{
	lang = l;
	lua_register(m_L,"LoadLang",LuaScript::loadlang);
	lua_register(m_L,"AddLang",LuaScript::addlang);
	lua_register(m_L,"GetLang",LuaScript::getlang);
	return true;
}

// user functions
int LuaScript::error(lua_State * L)
{
	int n=lua_gettop(L);
	int i;
	static char buff[512];
	static char mbuff[256];
	buff[0] = '\0';
	for (i=1; i<=n; i++)
	{
		if (lua_isstring(L,i))
			sprintf(mbuff,"%s",lua_tostring(L,i));
		else if (lua_isnil(L,i))
			sprintf(mbuff,"%s","nil");
		else if (lua_isboolean(L,i))
			sprintf(mbuff,"%s",lua_toboolean(L,i) ? "true" : "false");
		else
			sprintf(mbuff,"%s:%p",lua_typename(L,lua_type(L,i)),lua_topointer(L,i));
		strcat(buff,mbuff);
	}
	GetInstance()->con->Con_Print(buff);
	return 0;
}

int LuaScript::print(lua_State * L)
{
	int n=lua_gettop(L);
	int i;
	static char buff[512];
	static char mbuff[256];
	buff[0] = '\0';
	for (i=1; i<=n; i++)
	{
		if (lua_isstring(L,i))
			sprintf(mbuff,"%s",lua_tostring(L,i));
		else if (lua_isnil(L,i))
			sprintf(mbuff,"%s","nil");
		else if (lua_isboolean(L,i))
			sprintf(mbuff,"%s",lua_toboolean(L,i) ? "true" : "false");
		else
			sprintf(mbuff,"%s:%p",lua_typename(L,lua_type(L,i)),lua_topointer(L,i));
		strcat(buff,mbuff);
	}
	GetInstance()->con->Con_Print(buff);
	return 0;
}

int LuaScript::sleep(lua_State * L)
{
	return lua_yield(L,0);
}

int LuaScript::loadfile(lua_State * L)
{
	LuaParam par(L);

	const char * fname;

	if (!par.CheckPar(1,"s"))
	{
		/*
		lua_pushstring(L, "incorrect argument to function `average'");
        lua_error(L);
		*/

		GetInstance()->con->Con_Print("Lua error: Bad param in function LoadFile");
		return 0;
	}

	fname = lua_tostring(L,-1);

	if (!GetInstance()->filesystem->AddResourceFile(fname))
		GetInstance()->con->Printf("WARNING: file '%s' not exist.",fname);

	return 0;
}

int LuaScript::hoe(lua_State * L)
{
	LuaParam par(L);

	const char * cmd;

	if (!par.CheckPar(1,"s"))
	{
		/*
		lua_pushstring(L, "incorrect argument to function `average'");
        lua_error(L);
		*/

		GetInstance()->con->Con_Print("Lua error: Bad param in function Exec");
		return 0;
	}

	cmd = lua_tostring(L,-1);

	GetInstance()->engine->exec(cmd);

	return 0;
}

int LuaScript::loadresource(lua_State * L)
{
	LuaParam par(L);

	if (!par.CheckPar(2,"sn"))
	{
		GetInstance()->con->Con_Print("Lua error: Bad param in function AddResource");
		return 0;
	}

	if (!GetInstance()->resources->AddResource(par.GetNum(-2), par.GetString(-1)))
	{
		GetInstance()->con->Printf("WARNING: cmd '%s' failed.",lua_tostring(L,-1));
	}

	return 0;
} 

int LuaScript::loadlang(lua_State * L)
{
	LuaParam par(L);

	if (!par.CheckPar(1,"s"))
	{
		GetInstance()->con->Con_Print("Lua error: Bad param in function LoadLang");
		return 0;
	}

	if (!GetInstance()->lang->Load(par.GetString(-1),GetInstance()->con))
	{
		GetInstance()->con->Printf("WARNING: Load lang file '%s' failed.",par.GetString(-1));
	}

	return 0;
} 

int LuaScript::addlang(lua_State * L)
{
	LuaParam par(L);

	if (!par.CheckPar(2,"sn"))
	{
		GetInstance()->con->Con_Print("Lua error: Bad param in function AddLang");
		return 0;
	}

	GetInstance()->lang->Add(par.GetNum(-2), par.GetString(-1));

	return 0;
} 

int LuaScript::getlang(lua_State * L)
{
	LuaParam par(L);

	if (!par.CheckPar(1,"n"))
	{
		GetInstance()->con->Con_Print("Lua error: Bad param in function GetLang");
		return 0;
	}

	par.SaveString(GetInstance()->lang->GetString(par.GetNum(-1)));

	return 1;
} 


END_HOEGAME











