
#ifndef _OBJECT_CONTROLLER_H_
#define _OBJECT_CONTROLLER_H_

#include "../include/hoeobject.h"
#include "utils.h"
#include "map_utils.h"

class HoeModel;
class HoeScene;

class ObjectController : public IHoeObjectController
{
	MapOrientation pos;
	float m_frame;
	dword m_overcolor;
	dword flags;
public:
	XHoeObject * object;
	HoeModel * model;
	HoeMath::Vector3v m_scale;
	struct TSubObjectPtr
	{
		THoeSubObject::Type type;
		THoeSubObject * ptr;
	};
	HoeCore::Set<TSubObjectPtr> m_adv; // seznam dalsich podobjektu a efektu

	ObjectController();
	bool IsValid() { return (object != NULL); }
	bool operator == (const XHoeObject * obj) const
	{
		return object == obj;
	}

	bool Create(XHoeObject * obj);
	void Render(const HoeScene * scene);
	//inline bool IsShow() { return flags & 0x1; }
	inline dword GetFlags() { return flags; }
	inline XHoeObject * GetObject() { return object; }
	inline const HoeMath::Vector3v & GetVectorPosition() { return pos.xyz; }

	virtual void HOEAPI SetModel(IHoeModel * model);
	virtual IHoeModel * HOEAPI GetModel();
	virtual bool HOEAPI LoadModel(const tchar * cmd);
	virtual void HOEAPI Unregister();

	virtual void HOEAPI SetPosition(const HoeMath::Vector3v &pos);
	virtual void HOEAPI SetOrientation(const vfloat x, const vfloat y, const vfloat z, const vfloat angle);
	virtual const HoeMath::Vector3v & HOEAPI GetPosition() const;
	virtual void HOEAPI GetOrientation(vfloat *x, vfloat *y, vfloat *z, vfloat *angle);
	virtual void HOEAPI SetScale(const HoeMath::Vector3v &scale);
	virtual const HoeMath::Vector3v & HOEAPI GetScale() const;
	/** @see IHoeObjectController::LinkSubObject */
	virtual void HOEAPI Link(THoeSubObject::Type type, THoeSubObject * obj);
	/** @see IHoeObjectController::UnlinkSubObject */
	virtual void HOEAPI Unlink(THoeSubObject * obj);

	virtual void HOEAPI SetFlags(unsigned long flags);
	virtual void HOEAPI UnsetFlags(unsigned long flags);

	virtual void HOEAPI SetAnimationTime(float t) { m_frame = t; }
	virtual void HOEAPI SetOverColor(dword color) { m_overcolor = color; }
};

#endif // _OBJECT_CONTROLLER_H_

