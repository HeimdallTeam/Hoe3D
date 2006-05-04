
#ifndef _HOE_MODEL_DEF_FILE_
#define _HOE_MODEL_DEF_FILE_

#define IDMDLHEADER	('M'+('d'<<8)+('l'<<16)+(' '<<24))

#define IDMDLVER 106

#define CHUNKSRC_STREAM 0x01
#define CHUNKSRC_INDEX  0x02
#define CHUNKSRC_MATERIAL 0x03

#define MAKE_CHUNKSRC(type,nam,num) ((type << 24) | (nam << 16) | num)

// hlavicka
// seznam modelu
//  podle jmen, modely maj sources hash podle toho ktere sources potrebuji
// seznam sources
//  sources jsou identifikovatelny podle hash cisla
// modely a sources

/*	hfm_stream 
 *  (char fvf)
 *	(hfm_index)
 *	data
 */


struct hfm_stream
{
	unsigned long size_fvf;
	int numvert;
	int size;
};

#define TRIANGLELIST 4
#define TRIANGLESTRIP 5
#define TRIANGLEFAN 6

struct hfm_index
{
	int numinx;
	int type;
};

struct hfm_color
{
	float a,r,g,b;
};

// 

#define HMAT_AMBIENT	0x01
#define HMAT_DIFFUSE	0x02
#define HMAT_TEXTURE	0x08
#define HMAT_ALPHABLEND 0x10
#define HMAT_SMOOTH     0x20

struct hfm_material
{
	unsigned long texsize;
	int alphatest;
};

#endif // _HOE_MODEL_FILE_
