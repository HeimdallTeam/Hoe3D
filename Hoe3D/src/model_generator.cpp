
#include "StdAfx.h"
#include "shared.h"
#include "utils.h"
#include "ref.h"
#include "hoe_model.h"
#include "hoe_index.h"
#include "hoe_stream.h"
#include "model_generator.h"

struct BoxVertex
{
	HoeMath::VECTOR3 pos;
	HoeMath::VECTOR3 n;
	HoeMath::VECTOR2 tex;
};

ModelGenerator::ModelGenerator(HoeLog *log, int flags)
{
	m_log = log;
	m_flags = flags;
}

/*
 0 1
 2 3
 4 5
 6 7 

 0 1 2 1 3 2
*/
#define SIDE(a,b,c,d) w[0]=a;w[1]=b;w[2]=c;w[3]=b;w[4]=d;w[5]=c;

HoeModel * ModelGenerator::GenBox(float size)
{
	int i;
	HoeModel * mod = new HoeModel(1,1,1);
	HoeStream * stream = new HoeStream();
	HoeIndex * index = new HoeIndex();
	// 8 vertexu a 
	LOG(m_log)->Log("Generating box of size %f", size);
	stream->Create(8, "pn2", sizeof(BoxVertex) * 8);
	byte * l = stream->Lock();
	for (i=0;i < 8;i++)
	{
		BoxVertex * bv = reinterpret_cast<BoxVertex*>(l);
		bv->n.x = i%2 ? 1.f:-1.f;
		bv->n.y = i > 3 ? 1.f:-1.f;
		bv->n.z = (i/2)%2 ? 1.f:-1.f;
		bv->pos = bv->n * (size / 2);
		bv->n.Normalize();
		bv->tex.x = i%2 ? 0.f:1.f;
		bv->tex.y = (i/2)%2 ? 0.f:1.f;
		l += sizeof(BoxVertex);
	}
	if (m_log && m_flags)
		stream->Dump(m_log);
	stream->Unlock();
	mod->AddDefStream(stream);
	
	index->Create(36);
	word * w = index->Lock();
	SIDE(0,1,2,3);w+=6;
	SIDE(5,4,7,6);w+=6;
	SIDE(3,1,7,5);w+=6;
	SIDE(0,2,4,6);w+=6;
	SIDE(2,3,6,7);w+=6;
	SIDE(1,0,5,4);w+=6;
	if (m_log && m_flags)
		index->Dump(m_log);
	index->Unlock();
	mod->AddDefIndex(index);

	HoeMaterial * mat = new HoeMaterial();
	mat->SetColor(HoeMaterial::Diffuse, HoeMaterialColor(1,1,1,1));
	//mat->SetLightReag(false);
	mod->AddDefMaterial(mat);
	return mod;
}

HoeModel * ModelGenerator::GenSphere(float size, int res)
{
	HoeModel * mod = new HoeModel(1,1,1);
	HoeStream * stream = new HoeStream();
	HoeIndex * index = new HoeIndex();
	// 8 vertexu a 
	LOG(m_log)->Log("Generating sphere of size %f and res: %d", size);
	stream->Create(8, "pn", sizeof(BoxVertex) * 8);
	byte * l = stream->Lock();
	// 
	/*for (i=0;i < 8;i++)
	{
		BoxVertex * bv = reinterpret_cast<BoxVertex*>(l);
		bv->n.x = i%2 ? 1.f:-1.f;
		bv->n.y = i > 3 ? 1.f:-1.f;
		bv->n.z = (i/2)%2 ? 1.f:-1.f;
		bv->pos = bv->n * (size / 2);
		bv->n.Normalize();
		l += sizeof(BoxVertex);
	}*/
	if (m_log && m_flags)
		stream->Dump(m_log);
	stream->Unlock();
	mod->AddDefStream(stream);
	
	index->Create(36);
	word * w = index->Lock();
	/**/
	if (m_log && m_flags)
		index->Dump(m_log);
	index->Unlock();
	mod->AddDefIndex(index);

	HoeMaterial * mat = new HoeMaterial();
	mat->SetColor(HoeMaterial::Diffuse, HoeMaterialColor(1,1,1,1));
	//mat->SetLightReag(false);
	mod->AddDefMaterial(mat);
	return mod;
}
