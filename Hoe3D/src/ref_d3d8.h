
/**
   @file    ref_d3d8.h
   @date    Mar 2006
   @version 1.0
   @brief   Hlavni soubor definice DIRECT3D8.
*/

#ifndef _HOE_D3D8_
#define _HOE_D3D8_
#define _HOE_D3D_

#include <d3d8.h>
#include "ref_base.h"

#define SetFVF SetVertexShader

#define FVF_XYZ D3DFVF_XYZ
#define FVF_XYZRHW D3DFVF_XYZRHW
#define FVF_NORMAL D3DFVF_NORMAL
#define FVF_DIFFUSE D3DFVF_DIFFUSE
#define FVF_SPECULAR D3DFVF_SPECULAR
#define FVF_TEX1 D3DFVF_TEX1
#define FVF_TEX2 D3DFVF_TEX2

#define RESERVE_PAR
typedef BYTE** D3DLOCKTYPE;

typedef IDirect3DDevice8 * SysDevice;
typedef IDirect3DTexture8 * SysTexture;
typedef IDirect3DIndexBuffer8 * SysIndexBuffer;
typedef IDirect3DVertexBuffer8 * SysVertexBuffer;

typedef D3DMATERIAL8 D3DMaterial;
typedef D3DVIEWPORT8 D3DViewport;
typedef D3DLIGHT8 D3DLight;

struct THoeInitSettings;

inline const D3DVECTOR & VECToD3D(const HoeMath::Vector3 &v)
{
	return *((D3DVECTOR*)&v);
}
/**
* @brief Trida s implementaci zakladnich vlastnosti <b>D3D8</b>
*/
class RefD3D8 : public RefBase
{
protected:
	HWND m_hWnd;  ///< Handle na okno
	uint m_Adapter; ///< Cislo adapteru
	IDirect3D8 * m_pD3D; ///< Ukazatel na DIRECT3D
	static IDirect3DDevice8 * m_Dev; ///< Device
	D3DFORMAT m_AdapterFormat; ///< Format backbufferu
	D3DCAPS8 m_Caps; ///< caps

public:
	/**
	* Konstruktor
	*/
	RefD3D8();
	/**
	* Destruktor
	*/
	virtual ~RefD3D8() {}
	/**
	* Inicializace Direct3D
	* @param his vyplnena struktura THoeInitSettings
	* @return Pokud se zdari inicializace, vraci true, jinak false.
	*/
	bool Init(THoeInitSettings * his);
	/**
	* Vraci pointer na device viz Direct3D help.
	* @return Pointer na device.
	*/
	static HOE_INLINE IDirect3DDevice8 * Device() 
	{ 
		assert(m_Dev);
		return m_Dev; 
	}
	/**
	* Zacatek sceny. viz DIRECT3D help.
	*/
	bool Begin();
	/**
	* Konec vykreslovani sceny. viz DIRECT3D help.
	*/
	void End();
	/**
	* Vymazani bufferu
	* @param target Smaze target buffer
	* @param depth Smaze depth buffer
	*/
	void ClearBuffers(bool target, bool depth);
	/**
	* Vykresleni objektu. Pocet trojuhelniku se urcuje podle velikosti bufferu.
	* @param stream Vrcholy
	* @param index Indexy
	*/
	static void DrawStdObject(class HoeStream * stream, class HoeIndex * index);
	/**
	* Vykresleni objektu s nastavenim velikosti bufferu.
	* @param stream Vrcholy
	* @param index Indexy
	* @param vert Pocet platnych vrcholu
	* @param ind Pocet indexu = pocet trojuhelniku * 3
	*/
	static void DrawStdObject(HoeStream * stream, HoeIndex * index, dword vert, dword ind);
	/**
	* Vykresleni objektu s urcenim intervalu.
	* @param stream Vrcholy
	* @param index Indexy
	* @param from Zacatek kresleni
	* @param num Pocet indexu = pocet trojuhelniku * 3
	*/
	static void DrawStdObjectFT(HoeStream * stream, HoeIndex * index, dword from, dword num);

	/**
	* Vykresleni fanu.
	* @param offset Od ktereho index uzacit
	* @param count Pocet indexu = pocet trojuhelniku + 2
	*/
	static void DrawFanObject(dword offset, dword count);
	/**
	* Vykresleni car podle vrcholu.
	* @param stream Vrcholy
	* @param lineCount Pocet car
	*/
	static void DrawLineObject(class HoeStream * stream, int lineCount);
	/**
	* Vykresleni objektu tvoreneho body.
	* @param stream Body
	* @param vertCount Pocet bodu
	*/
	static void DrawPointObject(class HoeStream * stream, int vertCount);
	/** 
	* Nastaveni aktualni modelove matice.
	* @param m Matice
	*/
	static HOE_INLINE void SetMatrix(const HoeMath::Matrix & m);
	/**
	* Zjisti zda zarizeni podporuje format textury
	* @param TextureFormat format textury
	*/
	bool IsTextureFormatOk( enum HOEFORMAT TextureFormat);
	/**
	* Podporovana verze vertex shaderu
	* @return Verze shaderu jako hiword a loword
	*/
	word HOE_INLINE GetVertexShaderVersion()
	{
		return (word)(0xffff&m_Caps.VertexShaderVersion);
	}
	/**
	* Zruseni DIRECT3D
	*/
	void Destroy();
};

/**
* Globalni funkce pro ziskani Direct3DDevice
*/
HOE_INLINE IDirect3DDevice8 * D3DDevice();

#include "ref_d3d8.inl"

#endif // _HOE_D3D8_