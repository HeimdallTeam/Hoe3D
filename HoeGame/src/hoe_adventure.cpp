
#include "StdAfx.h"
#include "../include/hoe_adventure.h"
#include "../include/hoe_engine.h"
#include "../include/hoe_structfile.h"

BEGIN_HOEGAME
namespace Adventure {

// klikaci objekty
// staci zatim jen ctverce
// - zmena rozmeru
// zmena textury
// animovana textura

Kulission::Kulission(IHoeScene * scn) : BaseObject(scn)
{
	char cmd[1000];
	sprintf(cmd, "generate model plane 1");
	m_model = (IHoeModel*)HoeGame::GetHoeEngine()->Create(cmd);
	if (!m_model)
		return;
	scn->RegisterObject(this);
	this->SetModel(m_model);
	this->Show(true);
}
	
Kulission::~Kulission()
{
}

void Kulission::SetSize(float width, float height)
{
	this->GetCtrl()->SetScale(HoeMath::Vector3(width, height, 1));
}

void Kulission::SetTexture(const char * texname)
{
	IHoeModelModifier * mf = m_model->GetModifier();
	mf->SetMaterialTexture(0, texname);
	// remap
	mf->Release();
	
}

void Kulission::SetOverlap(int i)
{
	IHoeModelModifier * mf = m_model->GetModifier();
	THoeRect s = { 1/8.f * i, 0, 1/8.f * (i+1), 1};
	mf->SetTextureOverlap(0, s);
	// remap
	mf->Release();
	
}

Theatre::Theatre()
{
	m_scene = NULL;
}

Theatre::~Theatre() 
{
	//if (m_scene)
	//	m_scene->Delete();
}



bool Theatre::Create()
{	
	m_scene = HoeGame::GetHoeEngine()->CreateScene(HOETS_NORMAL);
	// vytvorit kameru
	if (!m_camera.Init(m_scene))
		return false;
	m_camera.SetPosition(0,0,-140);
	//m_camera.SetDistance(100);

	// theatre camera
	// kamera ktera nejde rotovat, ukazuje jen do jednoho smeru
	// omezena pro pohyb v kvadru
	// dopredu dozadu
	// doleva, nahoru, dolu

   //IHoeModel * mod2 = (IHoeModel*)GetEngine()->Create("model angel file:'angel.txt' -dump");
   //IHoeModel * mod2 = (IHoeModel*)HoeGame::GetHoeEngine()->Create("model angel");

	//m_camera.SetDistance(300);
   if (HoeGetInput(HoeGame::GetHoeEngine()))
		HoeGetInput(HoeGame::GetHoeEngine())->RegisterKeyboard(&m_camera);
	return true;
}

bool Theatre::Load(const char * fname)
{
	// nacteni kulis ze souboru
	HoeGame::ObjectFileParser parser;
	HoeCore::String_s<1024> err;

	if (!parser.Open(fname))
		return false;

	while (parser.ParseObject())
	{
		const Property * p;
		// vytvorit novou kulisu
		Kulission * k = new Kulission(m_scene);
		while (p = parser.GetNextProperty())
		{
			//gui->Set(p->GetName(), p->GetStringValue());
			if (strcmp(p->GetName(),"position") == 0)
			{
				
				k->SetPosition(p->GetValue().GetPtr<float>()[0],
					p->GetValue().GetPtr<float>()[1],
					p->GetValue().GetPtr<float>()[2]);
			}
			else if (strcmp(p->GetName(),"size") == 0)
			{
				k->SetSize(p->GetValue().GetPtr<float>()[0],
					p->GetValue().GetPtr<float>()[1]);
			}
			else if (strcmp(p->GetName(),"texture") == 0)
			{
				k->SetTexture(p->GetValue().GetStringValue());
			}
		}
		if (parser.GetLastError())
		{
			err = parser.GetLastError();
			goto loaderror;
		}
	}
	if (parser.GetLastError() == NULL)
		return true;
	else
		err = parser.GetLastError();
loaderror:
	BaseConsole::Printf("%s(%d): error: %s", fname, parser.GetLineNum(), (const char*)err);
	return false;
}

}; // Adventure
END_HOEGAME

