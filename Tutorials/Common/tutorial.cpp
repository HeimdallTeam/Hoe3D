
#include "tutorial.h"

using namespace HoeGame;

HoeTutorial::HoeTutorial(HOE_INSTANCE instance, HoeGame::Console * con) : HoeApp(instance, con)
{
}

bool HoeTutorial::InitTutorial(HOE_TYPE_SCENE ts)
{
	if (!Init(GetTitle(), HOESDK_VERSION))
		return false;

	GetEngine()->SetActiveScene(GetEngine()->CreateScene(ts));
	HoeGetInfo(GetEngine())->LoadFont(T("../data/font.ttf"),12);

	return true;
}

int _hoemain(HOE_INSTANCE instance, HoeGame::Console * con);

static const char * GetLogname()
{
	static char logname[512];
	sprintf(logname,"%s.log",g_TutorialName);
	return logname;
}

#ifdef _WIN32
int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPWSTR lpCmdLine,int nCmdShow)
{
	/*SetRootFromInstance(hInstance);

	Console con;
	con.SetFileLogging(GetLogname());
	//ConfigVars config;
	//config.Load("tutorials.conf");

	return _hoemain(hInstance,&con);*/
	return 0;
}

#else

int main(int argc,char * argv[])
{
    // SetRootFromExe(argv[0]);

	Console con;
        con.SetFileLogging(GetLogname());
	ConfigVars config;
	config.Load("tutorials.conf");

	return _hoemain(NULL,&con);
}

#endif


