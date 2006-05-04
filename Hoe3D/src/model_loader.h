
#ifndef _MODEL_LOADER_H_
#define _MODEL_LOADER_H_

#include "../include/hoefs.h"

class FileSystem;
class XHoeFile;
class MaterialSystem;
class HoeModel;
class HoeStream;
class HoeIndex;
struct hfm_chunk;
struct tChunk;

class ModelLoader
{
	HoeFileReader m_reader;

	HoeLog * m_log;

	HoeModel * LoadModel();
public:
	ModelLoader(HoeLog * log = NULL);
	HoeModel * LoadModel(const char * name);

	HoeStream * GetStream(int id);
	HoeIndex * GetIndex(int id);
	HoeMaterial * GetMaterial(int id);
};


#endif // _MODEL_LOADER_H_

