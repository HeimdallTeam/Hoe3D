
#ifndef _HOE_UTILS_H_
#define _HOE_UTILS_H_

namespace HoeUtils {

#ifdef _WIN32
class FindFile
{
	intptr_t m_handle; 
	struct _finddata_t m_fileinfo;
public:
	FindFile(const HoeCore::CString mask);
	~FindFile();
	bool IsValid();
	void Next();
	operator bool ();
	FindFile& operator ++ ();

	// bbb
	struct _finddata_t& Get() { return m_fileinfo; }
};
#else
class FindFile
{
public:
	FindFile(const HoeCore::CString mask);
	~FindFile();
	bool IsValid();
	void Next();
	operator bool ();
	FindFile& operator ++ ();

	// bbb
	//struct _finddata_t& Get() { return m_fileinfo; }
    const HoeCore::CString GetName();
};
#endif

const HoeCore::String GetFileName(const HoeCore::CString path, bool withex = true);

} // namespace HoeUtils

#endif // _HOE_UTILS_H_
