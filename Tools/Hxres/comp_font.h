
#ifndef _HXRES_COMPILER_FONT_H_
#define _HXRES_COMPILER_FONT_H_

#include "compiler.h"

class FontCompiler : public Compiler
{
	class FontDef
	{
		struct CharDef
		{
			wchar_t ch;
			int32 size;
		};
		HoeCore::List<CharDef> m_chd;
	public:
		bool Load(const char* path);
		void Write(HoeCore::WriteStream& out);
	} m_fontdef;
public:
	FontCompiler(HoeCore::WriteStream& out) : Compiler(out) {}
	virtual bool AddProp(const HoeCore::CString name, const Value& value);
	virtual bool AddProp(const HoeCore::CString name, const Values& value);
	virtual bool Func(const HoeCore::CString name, 
                      const HoeCore::CString ret,
                      const Values& value);
	virtual bool AddObject(const Compiler* cmp);

	virtual void Done();
};

#endif //  _HXRES_COMPILER_FONT_H_


