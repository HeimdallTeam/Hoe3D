
#include "StdAfx.h"
#include "linkout.h"
#include "linker.h"

using namespace HoeRes;

void ExportNS(Namespace& ns, HoeCore::WriteStream& str, 
			const HoeCore::Endianness& end, bool expheader = true);

LinkExport::LinkExport(const HoeCore::CString name)
	: m_name(name) 
{
}

void LinkExport::Export(Namespace& ns)
{
	HoeCore::File f;
	HoeCore::Stream& s = f;
	HoeCore::Endianness end(HoeCore::Endianness::Low);
	f.Open(m_name, HoeCore::File::hftRewrite);

	//m_obj.QSort();
	Res::MainNamespace head;
	memset(&head, 0, sizeof(head));
	head.id = le_uint32(Res::IDHRESHEADER);
	head.size_struct = be_uint16(sizeof(head));
	head.version_struct = be_uint16(Res::IDHRESVER);
	head.flags = be_uint32((uint32)end.Get());
	head.num_symbols = be_uint32(ns.GetObj().Count() + ns.GetNumNS());
	s.Write(&head, sizeof(head));
	ExportNS(ns, f, end, false);
}

bool operator < (const HoeRes::Res::Symbol& a, const HoeRes::Res::Symbol& b)
{
	return HoeCore::string::cmp(a.name,b.name) < 0;
}

bool operator > (const HoeRes::Res::Symbol& a, const HoeRes::Res::Symbol& b)
{
	return HoeCore::string::cmp(a.name,b.name) > 0;
}

void ExportNS(Namespace& ns, HoeCore::WriteStream& str, 
			const HoeCore::Endianness& end, bool expheader)
{
	// vypocitat poctet symbolu
	int numsymbols = ns.GetObj().Count() + ns.GetNumNS();
	if (expheader)
	{
		Res::Namespace head;
		memset(&head, 0, sizeof(head));
		head.id = HoeRes::Res::IDNamespace;
		head.num_symbols = end.num<uint32>(numsymbols);
		head.size_struct = end.num<uint16>(sizeof(head));
		head.version_struct = end.num<uint16>(1);
		str.Write(&head, sizeof(head));
	}
	if (numsymbols < 1)
		return;
	Res::Symbol * syms = (Res::Symbol *) str.CreateBuffer(sizeof(Res::Symbol)*numsymbols);
	memset(syms, 0, sizeof(Res::Symbol)*numsymbols);
	int nsym = 0;
	for (HoeCore::LList<Namespace>::Iterator it = ns.GetNS();it;it++)
	{
		Res::Symbol& sym = syms[nsym];
		memset(&sym,0, sizeof(sym));
		HoeCore::string::copy(sym.name, it->GetName(), sizeof(sym.name));
		sym.type = end.num<uint32>(HoeRes::Res::IDNamespace);
		sym.position = end.num<uint64>(str.Tell());
		ExportNS(*it, str, end);
		nsym++;
	}
	// Export objs
	for (HoeCore::List<Namespace::Obj>::Iterator it(ns.GetObj());it;it++)
	{
		Res::Symbol& sym = syms[nsym];
		memset(&sym,0, sizeof(sym));
		HoeCore::string::copy(sym.name, it->name, sizeof(sym.name));
		sym.type = it->type;
		sym.position = str.Tell();
		// export obj
		it->file.Flush();
		it->file.Seek(0);
		str.Write(it->file);
		nsym++;
	}
	hoe_assert(nsym == numsymbols);
	HoeCore::qsort(syms, numsymbols);
}

/*

{ "gallery:a:b", IDPicture, 1, 3445  }
{ "gallery:a:c", IDPicture, 1, 3457  }

*/
////////////////////////////////////////////////////////////
// resource
LinkRes::LinkRes(const HoeCore::CString name)
	: m_name(name) 
{
}

void ExportRes(HoeCore::String nsn, Namespace& ns,HoeCore::WriteStream& cpp)
{
	// operate obj
	for (HoeCore::List<Namespace::Obj>::Iterator i(ns.GetObj());i;i++)
	{
		HoeCore::String name = nsn;
		if (nsn.IsEmpty())
			name = i->name;
		else {
			name += ":";
			name += i->name;
		}
		HoeCore::String f;
		f.printf("{ \"%s\" },\n", name.GetPtr());
		cpp.WriteString(f);
	}
	for (HoeCore::LList<Namespace>::Iterator i(ns.GetNS());i;i++)
	{
		HoeCore::String name = nsn;
		if (nsn.IsEmpty())
			name = i->GetName();
		else {
			name += ":";
			name += i->GetName();
		}
		ExportRes(name, *i, cpp);
	}
}

void LinkRes::Export(Namespace& ns)
{
	HoeCore::File fcpp;
	HoeCore::String ncpp = m_name;
	ncpp += ".cpp";
	if (!fcpp.Open(ncpp, HoeCore::File::hftRewrite))
		return;

	fcpp.WriteString("// Auto generated file\n");
	
	ExportRes("", ns, fcpp);
}

