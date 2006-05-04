
#include "StdAfx.h"
#include "model_app.h"

const char * g_TutorialName = "models";

int _hoemain(HOE_INSTANCE instance, HoeGame::Console * con)
{
	ModelApp app(instance,con);

	if (!app.InitTutorial(HOETS_NORMAL))
	{
		app.HandleError();
		return 0;
	}

	app.LoadScene();

	app.Run();

	app.Destroy();

	return 0;
}

/////////////////////////////////////////////////////////////////

ModelApp::ModelApp(HOE_INSTANCE instance, HoeGame::Console * con) : HoeTutorial(instance, con)
{
}

bool ModelApp::LoadScene()
{

	GetFS()->AddResourceFile("../data/jackolan.hm");
	GetFS()->AddResourceFile("../data/angel.hm");
	//GetEngine()->AddResource("../data/konvice.hm");
	//HoeGetRef(GetEngine())->SetBackgroundColor(0xfff00a79);
	HoeGetRef(GetEngine())->SetBackgroundColor(0xff000000);

	if (!m_view.Init(GetEngine()->GetActiveScene()))
		return false;

	m_view.SetTargetPosition(0,0,0);
	m_view.SetAngle(3.1f);
	m_view.SetArcAngle(-0.16f);
	m_view.SetDistance(150);
	HoeGetInput(GetEngine())->RegisterKeyboard(&m_view);
	HoeGetInput(GetEngine())->RegisterMouse(IHoeInput::MT_Foreground, &m_view);

	for (int i=0;i < 8;i++)
		m_l[i] = GetEngine()->GetActiveScene()->CreateLight();
	m_l[ 0]->SetColor( 1.0f, 0.0f, 0.0f);
	m_l[ 1]->SetColor( 0.0f, 1.0f, 0.0f);
	m_l[ 2]->SetColor( 0.0f, 0.0f, 1.0f);
	m_l[ 3]->SetColor( 0.5f, 0.5f, 0.0f);
	m_l[ 4]->SetColor( 0.5f, 0.0f, 0.5f);
	m_l[ 5]->SetColor( 0.0f, 0.5f, 0.5f);
	m_l[ 6]->SetColor( 0.3f, 0.3f, 0.3f);
	m_l[ 7]->SetColor( 1.0f, 1.0f, 1.0f);
	//m_l[ 7]->SetColor( 0.6f, 0.4f, 0.0f);
	/*m_l[ 8]->SetColor( 0.6f, 0.0f, 0.4f);
	m_l[ 9]->SetColor( 0.4f, 0.6f, 0.0f);
	m_l[10]->SetColor( 0.0f, 0.6f, 0.4f);
	m_l[11]->SetColor( 0.4f, 0.0f, 0.6f);
	m_l[12]->SetColor( 0.0f, 0.4f, 0.6f);
	m_l[13]->SetColor( 0.6f, 0.2f, 0.2f);
	m_l[14]->SetColor( 0.2f, 0.6f, 0.2f);
	m_l[15]->SetColor( 0.2f, 0.2f, 0.6f);
	m_l[16]->SetColor( 0.8f, 0.1f, 0.1f);*/

	IHoeModel * mod1 = (IHoeModel*)GetEngine()->Create("model jackolan");
	if (!mod1)
		return false;
	IHoeModel * mod2 = (IHoeModel*)GetEngine()->Create("model angel");
	if (!mod2)
		return false;

	const float leng = 10.f;
	const float st = 1.f;

	/*for (float a=0.f;a < (6.28f - st);a += st)
	{
		CreateObj(mod, sinf(a) * leng, cosf(a) * leng);
	}*/
	CreateObj(mod2, 0, 0);
	//CreateObj(mod1, 30, -10);
	//CreateObj(mod1, -30, -10);

	//GetEngine()->exec("hidefps");

	return true;

}
#ifndef PI180
#define PI180	0.0174532925199432957692369076848861f	// pi / 180
#endif
struct Vec
{
	float x,y,z;
	void RotY( const float angle)
	{
		float s = (float) sin( angle * PI180);
		float c = (float) cos( angle * PI180);
		float X=x;
		x =  x*c + z*s;
		z = -X*s + z*c;
	}
	void RotZ( const float angle)
	{
		float s = (float) sin( angle * PI180);
		float c = (float) cos( angle * PI180);
		float X=x;
		x =  x*c - y*s;
		y =  X*s + y*c;
	} 
};
void ModelApp::OnUpdate(float timeframe)
{
	static float rotz = 0;
	static float roty = 0;
	rotz+=90.f*timeframe;
	roty+=10.f*timeframe;
	const float l = 100.f;
	for(int i=0; i<8; i++)
	{
		Vec a;
		a.x = 2.0f; a.y = 0; a.z = 0;
		if(i<4)
		{
			a.RotZ(rotz+(float)i*360.f/(float)4.f);
			a.y *= 0.5;
			a.RotZ(45.f);
			a.RotY(roty);
		}
		else
		{
			a.RotZ(rotz+(float)i*360.f/(float)4.f+45.f);
			a.y *= 0.5f;
			a.RotZ(-45.f);
			a.RotY(roty+90.f);
		}
		m_l[i]->SetPosition(a.x*l, a.y*l, a.z*l);
	}

}

bool ModelApp::CreateObj(IHoeModel *m, float x, float y)
{
	Model * obj = new Model;
	GetEngine()->GetActiveScene()->RegisterObject(obj);
	obj->SetModel(m);
	//obj->SetOrientation(0,1,0,0.3f);
	obj->SetPosition(x,0.f,y);
	obj->Show(true);
	return true;
}
