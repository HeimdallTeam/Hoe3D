
#include "system.h"
#include "shared.h"
#include "utils.h"
#include "ref.h"
#include "states.h"
#include "gridsurface.h"
#include "texture_system.h"
#include "hoe_texture.h"
#include "hoe_time.h"

struct VecPDT
{
		HoeMath::VECTOR3 pos;
		dword color;
		HoeMath::VECTOR2 tex;
};

// D3DTOP_BLENDTEXTUREALPHA current textura -- alpha current
// rozdelit to na skupiny mrizek
// kazda skupina mrizek svoje rozdeleni podle viditelnosti
// vykresleni podle povrchu (kazdy povrch, svoje quadtree)
// 



//////////////////////////////////////////////////////////
// GridSurfaceType
void TGridSurfaceType::Setup()
{
#ifdef _HOE_D3D_

		D3DDevice()->SetTexture(0, tex1->GetTexture());
		D3DDevice()->SetTexture(1, tex2->GetTexture());
		D3DDevice()->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE); 
		D3DDevice()->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_CURRENT); 
		D3DDevice()->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TEXTURE); 
		D3DDevice()->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_BLENDTEXTUREALPHA); 
		D3DDevice()->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT); 
		D3DDevice()->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE); 
		D3DDevice()->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 0);
#if 0
		D3DDevice()->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		D3DDevice()->SetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		D3DDevice()->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		D3DDevice()->SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
#endif

#endif
#ifdef _HOE_OPENGL_
		glActiveTextureARB(GL_TEXTURE0_ARB);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, tex1->GetTexture());
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

		glActiveTextureARB(GL_TEXTURE1_ARB);
		glEnable(GL_TEXTURE_2D);

		// Here we turn on the COMBINE properties and increase our RGB
		// gamma for the detail texture. 2 seems to work just right.
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_ARB);
		glTexEnvi(GL_TEXTURE_ENV,GL_COMBINE_RGB_ARB,	GL_INTERPOLATE_ARB);
		glTexEnvi(GL_TEXTURE_ENV,GL_SOURCE2_RGB_ARB, GL_TEXTURE);
		glTexEnvi(GL_TEXTURE_ENV,GL_OPERAND2_RGB_ARB, GL_SRC_ALPHA);

		// Bind the detail texture
		glBindTexture(GL_TEXTURE_2D, tex2->GetTexture());
		glDisable(GL_LIGHTING);
		checkgl("multitexture");
#endif
}

//////////////////////////////////////////////////////////
void TGridSurfaceTreeItem::DeleteSub()
{
	if (q1)
	{
		q1->DeleteSub();
		delete q1;
		q1 = NULL;
	}
	if (q2)
	{
		q2->DeleteSub();
		delete q2;
		q2 = NULL;
	}
	if (q3)
	{
		q3->DeleteSub();
		delete q3;
		q3 = NULL;
	}
	if (q4)
	{
		q4->DeleteSub();
		delete q4;
		q4 = NULL;
	}
}


//////////////////////////////////////////////////////////
GridSurface::GridSurface()
{
	m_worldpos.Identity();
	m_loaded = false;
	m_sizeX = m_sizeY = 10.f;
	m_grids = NULL;
	m_gst_first = NULL;
	m_wire = false;
	memset(m_textures, 0, sizeof m_textures);
}

TGridSurfaceTreeItem * GridSurface::CreateQuadTree(dword * gr, uint ngr, uint minx, uint maxx, uint miny, uint maxy)
{

	// najdrive rozdelit na kvadranty
	// spocitat v kterem kvadrantu je kolik ctvercu
	// pustit subtree na kazdy vyznamny kvadrant
	// mel by dostat strukturu informaci
	// minimalni vyska kvadrantu
	// velikost kvadrantu
	// 
	// nakonec spocitat sam sebe
	//if (maxx-minx < neco && maxy-miny  < neco)
	{
		dword start = m_multi.GetPos();
		// pridat gridy a pocitat velikost
		// preneseni gridu do streamu
		// nalezeni vsech pusobnych gridu
		for (uint i=0;i < ngr;i++)
		{
			uint x = hiword(gr[i]);
			uint y = loword(gr[i]);
			if (x < minx || x > maxx || y < miny || y > maxy)
				continue;

			// ulozit grid
			VecPDT * pv = (VecPDT*)m_multi.GetLockedVertices();
			const float vx = m_sizeX / (m_width+1);
			const float vy = m_sizeY / (m_height+1);
			const HoeMath::VECTOR2 s[4] = { HoeMath::VECTOR2(0,0),
											HoeMath::VECTOR2(1/8.f,0),
											HoeMath::VECTOR2(0,1/4.f),
											HoeMath::VECTOR2(1/8.f,1/4.f)};
			pv[0].pos = HoeMath::VECTOR3((x+0) * vx - (m_sizeX*0.5f), 0, (y+0) * vy - (m_sizeY*0.5f));
			pv[1].pos = HoeMath::VECTOR3((x+1) * vx - (m_sizeX*0.5f), 0, (y+0) * vy - (m_sizeY*0.5f));
			pv[2].pos = HoeMath::VECTOR3((x+1) * vx - (m_sizeX*0.5f), 0, (y+1) * vy - (m_sizeY*0.5f));
			pv[3].pos = HoeMath::VECTOR3((x+0) * vx - (m_sizeX*0.5f), 0, (y+1) * vy - (m_sizeY*0.5f));
			for (int i=0;i < 4;i++) {
			// levo nahore
				int sup = (m_grids[y*m_width+x].ori1+i)%4;
				pv->color = 0xffffff00;
				// orientace
				// vypocitat souradnice podle map
				pv->tex = s[sup] + HoeMath::VECTOR2(m_grids[y*m_width+x].x1/8.f,m_grids[y*m_width+x].y1/4.f);
				pv++;
			}

			m_multi.AddIndex(0);
			m_multi.AddIndex(1);
			m_multi.AddIndex(3);
			m_multi.AddIndex(1);
			m_multi.AddIndex(2);
			m_multi.AddIndex(3);
			
			m_multi.SkipVertices(4);
			/*const float px = x*m_sizeX/hx-m_sizeX/2;
			const float py = y*m_sizeY/hy-m_sizeY/2;
			const float l = sqrtf((0-px)*(0-px)+(0-py)*(0-py));
			pv->pos = HoeMath::VECTOR3(px,m_heights.getHeightAt(x,y),py);
			pv->tex = HoeMath::VECTOR2((x % 2) * (1/8.f) + (7/8.f),(y % 2) * (1/4.f) + (2/4.f));

			// compute color
			//if (l > 100.f)
				pv->color = 0xffffffff;
			//else
			//{
			//	byte c = byte(0xff * l/100.f);
			//	pv->color = (c << 8 | c) | 0xffff0000;
			//}
			pv++;
			}*/
		}

		dword end = m_multi.GetPos();
		if (end-start == 0)
			return NULL;
		// posledni hotovy
		TGridSurfaceTreeItem * i = new TGridSurfaceTreeItem();
		i->q1 = i->q2 = i->q3 = i->q4 = NULL;
		// pridat vsechny vrcholy 
		i->start = start;
		i->end = end;
		// pridat velikost
		return i;
	}

	// nastavit vsechny na grid

	// pokud je jen jeden tak vratit ten jeden
	// pokud je vice, vytvorit strom

	// spocitat udaje

	return NULL;
}


void GridSurface::Load()
{
	if (!m_grids)
		return;

	// odstraneni predesleho
	Unload();
	// podle urovne rozdeleni do skupin (meni se jen jedna skupina),
	// zase na druhou stranu pomalejsi render (pro editor)

	// na zacatku grid a nekolik textur
	// predelat do streamu, indexu a matrose
	// postupne vyplnovat pole gridu, dokud nebude cele predelane
	// vytvoreni pole gridu
	bool * pg = new bool[m_width*m_height];
	memset(pg, 0,m_width*m_height * sizeof(byte));

	dword * gl = new dword[m_width*m_height];
	// multistream create
	// dobre by bylo spocitat kolik budevrcholu (to je mozne, protoze se tam musi premistit vsichny)
	if (!m_multi.Begin(m_width*m_height*4, m_width*m_height*6, "pdt", sizeof(VecPDT)))
		return; /*!!!*/

	for (uint y=0;y < m_height;y++)
	{
		for (uint x=0;x < m_width;x++)
		{
			if (pg[y*m_width+x])
				continue; // preskoceni zpracovanych

			// nalezeni prvniho gridu a sestaveni surface
			TGridDesc * type = &m_grids[y*m_width+x];
			TGridSurfaceType * nt = new TGridSurfaceType();
			
			// nastaveni textur
			nt->tex1 = m_textures[type->tex1].tex;
			//GetTextureSystem()->GetTexture("trava");
			nt->tex2 = m_textures[type->tex2].tex;
			//GetTextureSystem()->GetTexture("strom_war3");
			assert(nt->tex1 || nt->tex2);


			uint numgrids = 1;
			uint maxx,maxy,minx,miny;
			maxx = minx = x;
			maxy = miny = y;
			gl[0] = makedword(x,y);
			pg[y*m_width+x] = true;
			// pridani prvniho
			// hledani stejnych poli
			for (uint yy=y;yy < m_height;yy++)
			{
				for (uint xx=0;xx < m_width;xx++)
				{
					if (!pg[yy*m_width+xx] && m_grids[yy*m_width+xx].tex1 == type->tex1
						&& m_grids[yy*m_width+xx].tex2 == type->tex2)
					{
						// pokud stejne tak pridat
						gl[numgrids] = makedword(xx,yy);
						pg[yy*m_width+xx] = true;
						if (xx < minx) minx = xx;
						if (xx > maxx) maxx = xx;
						if (yy < miny) miny = yy;
						if (yy > maxy) maxy = yy;
						numgrids++;
					}
				}
			}
			// sestaveni opt. stromu
			// rozdeleni na oblasti
			// vyteckovat
			// kazdy grid se nakopiruje do streamu (neresit zda jsou vedle sebe, jen uvnitr gridu)
			nt->root = CreateQuadTree(gl, numgrids, minx, maxx, miny, maxy);
			// pridani do seznamu povrchu
			TGridSurfaceType ** nt2 = &m_gst_first;
			while (*nt2) nt2 = &((*nt2)->next);
			*nt2 = nt;
			nt->next = NULL;

		}
	}

	m_multi.End();

	delete [] pg;
	delete [] gl;
	m_loaded = true;

}

void GridSurface::Unload()
{
	TGridSurfaceType * gst = m_gst_first;
	while (gst)
	{
		TGridSurfaceType * next = gst->next;
		if (gst->root)
		{
			gst->root->DeleteSub();
			delete gst->root;
		}
		delete gst;
		gst = next;
	}
	m_gst_first = NULL;
	m_loaded = false;
}

void GridSurface::Render()
{
	// vykreslovat jenom celky v polickach

	if (m_loaded && m_gst_first)
	{
		/*static TimeMeter tm("Load terrain",true); 
		tm.Begin();
		Load();
		tm.End();*/
		// wireframe
		Ref::SetMatrix(m_worldpos);
		GetHoeStates()->SetupMap();
		if (m_wire)
			GetHoeStates()->StartWireframe();
		//m_mat.Setup();

		// toto by mohlo byt i v predrenderingu
		TGridSurfaceType * gst = m_gst_first;
		while (gst)
		{
			gst->Setup();
			// renderovani multistreamu pres grid tree

			m_multi.Render();
			gst = gst->next;
		}
#ifdef _HOE_OPENGL_
		glDisable(GL_TEXTURE_2D);
		glActiveTextureARB(GL_TEXTURE0_ARB);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
#endif
		// wireframe
		if (m_wire)
			GetHoeStates()->EndWireframe();
	}
}

void HOEAPI GridSurface::SetPosCenter(float x, float y, float z)
{
	m_worldpos.Translate(x,z,y);
}

void HOEAPI GridSurface::SetTexture(int slot, const char *texname, int width, int height)
{
	assert(slot >= 0 && slot < MaxTextureSlots);
	if (texname)
	{
		m_textures[slot].tex = GetTextureSystem()->GetTexture(texname);
		assert(width > 0 && height > 0);
		m_textures[slot].nx = width;
		m_textures[slot].ny = height;
	}
	else
	{
		m_textures[slot].tex = NULL;
	}
}

void HOEAPI GridSurface::Create(float sizeX, float sizeY, int resX,int resY)
{
	m_sizeX = sizeX;
	m_sizeY = sizeY;
	//m_heights.generatePlaneMap( resX, resY, 0.f);
	SAFE_DELETE(m_grids);
	m_width = (size_t)resX;
	m_height = (size_t)resY;
	m_grids = new TGridDesc[m_width*m_height];
	for (size_t i=0;i < m_width * m_height;i++)
	{
		memset(&m_grids[i], 0, sizeof TGridDesc);
		m_grids[i].tex1 = m_grids[i].tex2 = 0x0;
		m_grids[i].x1 = rand() % 4 + 4;
		m_grids[i].y1 = rand() % 4;

	}
	Load();
}

void HOEAPI GridSurface::SetGridDesc(int x, int y, IHoeEnv::GridSurface::TGridDesc *desc)
{
	assert(m_grids);
	assert(x >= 0 && (uint)x < m_width);
	assert(y >= 0 && (uint)y < m_height);
	memcpy(&m_grids[m_width*y+x], desc, sizeof IHoeEnv::GridSurface::TGridDesc);
	Load();
}

void HOEAPI GridSurface::GetGridDesc(int x, int y, IHoeEnv::GridSurface::TGridDesc *desc)
{
	assert(m_grids);
	assert(x >= 0 && (uint)x < m_width);
	assert(y >= 0 && (uint)y < m_height);
	memcpy(desc, &m_grids[m_width*y+x], sizeof IHoeEnv::GridSurface::TGridDesc);

}

void HOEAPI GridSurface::ShowWireframe(bool show)
{
	m_wire = show;
}

void HOEAPI GridSurface::Dump(XHoeStreamWrite * stream)
{
	// write version
	assert(m_grids);
	stream->Write<uint>(le_uint(1));
	// size
	stream->Write<uint>(le_uint(m_width));
	stream->Write<uint>(le_uint(m_height));
	stream->Write<float>(le_float(m_sizeX));
	stream->Write<float>(le_float(m_sizeY));
	// struktura pro data
	// pak jednotlive gridy
	stream->Write(m_grids, sizeof(TGridDesc) * m_height * m_width);
	// hotovo
}

void HOEAPI GridSurface::LoadDump(XHoeStreamRead * stream)
{
	assert(le_uint(stream->Read<uint>())==1);
	// size
	m_width=le_uint(stream->Read<uint>());
	m_height=le_uint(stream->Read<uint>());
	m_sizeX=le_float(stream->Read<float>());
	m_sizeY=le_float(stream->Read<float>());
	// struktura pro data
	// pak jednotlive gridy
	SAFE_DELETE_ARRAY(m_grids);
	m_grids = new TGridDesc[m_height * m_width];
	stream->Read(m_grids, sizeof(TGridDesc) * m_height * m_width);
	// hotovo
	Load();
}



