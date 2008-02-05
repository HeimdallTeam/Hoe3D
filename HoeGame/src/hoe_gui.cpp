
#include "StdAfx.h"
#include <hoe3d.h>
#include "../include/HoeGame/hoe_inputkeys.h"
#include "../include/HoeGame/hoe_gui.h"
#include "../include/HoeGame/hoe_engine.h"

BEGIN_HOEGAME
namespace Gui {

Item::Item()
{
	m_rect.left = 200.f;
	m_rect.top = 100.f;
	m_rect.right = 600.f;
	m_rect.bottom = 500.f;
	m_show = true;
}

void Item::Set(const tchar * prop, const HoeCore::Universal & value)
{
	if (HoeCore::string::cmp(prop,"rect")==0)
		SetRect(value);
	else if (HoeCore::string::cmp(prop,"name")==0)
		m_name = value.GetStringValue();
	else if (HoeCore::string::cmp(prop,"show")==0)
		m_show = value.GetBool();
}

void Item::SetRect(const THoeRect * rect)
{
	m_rect = *rect;
}

void Item::SetRect(const HoeCore::Universal & value)
{
	m_rect.left = value.GetPtr<float>()[0];
	m_rect.top = value.GetPtr<float>()[1];
	m_rect.right = value.GetPtr<float>()[2];
	m_rect.bottom = value.GetPtr<float>()[3];
}

TextDevice::TextDevice()
{
	shift = 0;
}

void TextDevice::_KeyDown(int k)
{
	if (!Key(k))
		return;

	switch (k)
	{
	case HK_BACK:
        Back();
		break;
	case HK_LSHIFT:
		shift |= 2;
		break;
	case HK_RSHIFT:
		shift |= 1;
		break;
	default:
		if (shift && GetKeyDesc(k)->upper)
		{
			AddChar(GetKeyDesc(k)->upper);
			return;
		}
		else if (GetKeyDesc(k)->lower)
		{
			AddChar(GetKeyDesc(k)->lower);
			return;
		}
	}
}

void TextDevice::_KeyUp(int k)
{
	switch (k)
	{
	case HK_LSHIFT:
		shift &= ~2;
		break;
	case HK_RSHIFT:
		shift &= ~1;
		break;
	}
}

StaticPicture::StaticPicture()
{
  m_pic = NULL;
}

void StaticPicture::Set(const tchar * prop, const HoeCore::Universal & value)
{
	if (HoeCore::string::cmp(prop,"picture") == 0)
	{
		// load
		HoeCore::String_s<256> buff;
		buff.printf(T("picture %s"), value.GetStringValue());
		m_pic = (IHoePicture*)GetHoeEngine()->Create(buff);
	}
	else 
		Item::Set(prop, value);
}

void StaticPicture::Draw(IHoe2D * h2d)
{
	if (m_pic)
		h2d->Blt(m_pic, &m_rect, 0);
}

/////////////////////////////////////////

void ColorRect::Set(const tchar * prop, const HoeCore::Universal & value)
{

}

void ColorRect::Draw(IHoe2D * h2d)
{
}

//////////////////////////////////////////////////////////////////

int InfoPanel::Info::comp(const void * v1,const void *v2)
{
	const Info * p1 = (Info*)v1;
	const Info * p2 = (Info*)v2;

	if (!p1->visible)
	{
		if (p2->visible)
			return 1;
		else
			return 0;
	}
	
	if (!p2->visible)
		return -1;

	if (p1->y < p2->y)
		return -1;
	else
		return 1;
}

InfoPanel::InfoPanel()
{
	m_font = NULL;
	memset(m_infos,0,sizeof(m_infos));
}

InfoPanel::~InfoPanel()
{
}

/*bool InfoPanel::Init(float min, float bottom, float left)
{
	font = (IHoeFont*)GetResMgr()->ReqResource(ID_INFO_FONT);

	stepsize = font->GetTextHeight();
	minheight = min;
	startheight = bottom - stepsize;

	return true;
}*/

void InfoPanel::Draw(IHoe2D * hoe2d)
{
	if (!m_font)
		return;
	hoe2d->SetRect(800,600);
	float t = GetHoeEngine()->SysFloatTime();

	stepsize = m_font->GetTextHeight();
	startheight = m_rect.bottom - stepsize;

	// predelat cas by se mel odecitat 

	for (int i=0;i < MAX_INFOS;i++)
	{
		if (m_infos[i].visible)
		{
			const float tt = m_infos[i].totime - t;
			unsigned long alpha;
			if (tt < 0)
			{
				m_infos[i].visible = false;
				continue;
			}
			alpha = (unsigned long)(tt * float(0x80));
			if (alpha > 0xff) alpha = 0xff;
			
			m_font->DrawText(m_rect.left,m_infos[i].y,((alpha & 0xff) << 24) | 0x00ffffff,m_infos[i].info);
		}
	}
	
	//return 0;
}

void InfoPanel::Add(const char * str)
{
	int a = -1;

	for (int i=0;i < MAX_INFOS;i++)
	{
		if (m_infos[i].visible)
		{
			// move up
			m_infos[i].y -= stepsize * 1.3f;
			if (m_infos[i].y < m_rect.top)
			{
				m_infos[i].visible = false;
			}
			else
				continue;
		}
		
		if (a == -1)
		{
			a = i;
		}
	}

	if (a == -1) // get top list
	{
		a = 0;
		for (int i=0;i < MAX_INFOS;i++)
		{
			if (m_infos[i].visible && m_infos[a].y > m_infos[i].y)
			{
				a = i;
			}
		}
	}

	assert(a != -1);

	m_infos[a].info = str;
	m_infos[a].y = startheight;
	m_infos[a].totime = GetHoeEngine()->SysFloatTime() + 5.f;
	m_infos[a].visible = true;

	//qsort(m_infos,MAX_INFOS,sizeof(Info),Info::comp);

	//GetConsole()->Con_Print(str);
}

void InfoPanel::Addf(const char * format, ...)
{
	char szBuff[512];

	va_list args;

	va_start(args, format);
#ifdef _WIN32
	_vsnprintf( szBuff, 512, format, args );
#else
	vsnprintf( szBuff, 512, format, args );
#endif
	va_end(args);

	Add(szBuff);
}

void InfoPanel::Set(const tchar *prop, const HoeCore::Universal & value)
{
	if (HoeCore::string::cmp(prop,"font")==0)
	{
		assert(GetHoeEngine());
		m_font = (IHoeFont*)GetHoeEngine()->Create(value.GetStringValue());
	}
	else
		Item::Set(prop,value);
}

////////////////////////////////////////////////
void DigiCounter::Draw(IHoe2D * d2)
{
		// draw digit
	IHoePicture * p = m_pic;//GetResMgr()->Get<IHoePicture>(ID_DIGITFONT);
	if (!p)
		return;
	const int np = 7;
	const float rl = m_rect.right;
	const float ll = m_rect.left;

	int i = m_value ? *m_value:0;
	bool sign = false;
	if (i < 0)
	{
		sign = true;
		i = -i;
	}

	const float pp = ((float)(rl-ll)) / np;
	float up = rl;
	if (i == 0)
	{
		THoeRect r = {up-pp, m_rect.top, up, m_rect.bottom};
		THoeRect r2={ 0, 0, 1*(60.0/64.0/4), 1*(60.0/64.0/3)};
		d2->Blt(p,&r, &r2);
		i = i / 10;
		up -= pp;
	}
	while (i != 0)
	{
		THoeRect r = {up-pp, m_rect.top, up, m_rect.bottom};
		int t=i%10;
		THoeRect r2={ (t%4)*(60.0f/64.0f/4), (t/4)*(60.0f/64.0f/3), (t%4+1)*(60.0f/64.0f/4), (t/4+1)*(60.0f/64.0f/3)};
		d2->Blt(p,&r,&r2);
		i = i / 10;
		up -= pp;
	}
	if (sign)
	{
		THoeRect r = {up-pp, m_rect.top, up, m_rect.bottom};
		THoeRect r2={ 2*(60.0/64.0/4), 2*(60.0/64.0/3), 3*(60.0/64.0/4), 3*(60.0/64.0/3)};
		d2->Blt(p, &r, &r2);
		i = i / 10;
		up -= pp;
	}
}

//////////////////////////////////////////////////////
Font::Font()
{ 
	m_font = NULL;
	m_text = NULL; 
	m_ax = ALeft;
	m_ay = ATop;
	m_ctrl = NULL;
	m_func = NULL;
}

Font::~Font()
{
	if (m_text)
	{
		delete [] m_text;
		m_text = NULL;
	}
}

void Font::Draw(IHoe2D *hoe2d)
{
	const tchar * text = m_text;
	// text overlap
	if (m_ctrl && m_func)
		text = (m_ctrl->*m_func)(this, text);

	if (m_font && text)
	{
		vfloat x = m_rect.left;
		vfloat y = m_rect.top;
		if (m_ax != ALeft)
		{
			// sirka
			THoeFontSize size;
			m_font->GetTextSize(text, &size);
			if (m_ax == ACenter)
				x += (m_rect.right - m_rect.left - size.width) * 0.5f; 
			else
				x = m_rect.right - size.width;
		}
		if (m_ay != ATop)
		{
			// sirka
			float height = m_font->GetTextHeight();
			if (m_ay == ACenter)
				y += (m_rect.bottom - m_rect.top - height) * 0.5f; 
			else
				y = m_rect.bottom - height;
		}
		m_font->DrawText(x, y, 0xffffffff, text);
	}
}

void Font::SetText(const tchar * ptr)
{
	if (m_text)
	{
		delete [] m_text;
		m_text = NULL;
	}
	size_t l = HoeCore::string::len(ptr);
	if (l > 0)
	{
		tchar * p = new tchar[l+1];
		memcpy(p, ptr, (l+1) * sizeof(tchar));
		m_text = p;
	}
}

void Font::Set(const tchar *prop, const HoeCore::Universal & value)
{
	if (HoeCore::string::cmp(prop,"font")==0)
	{
		m_font = (IHoeFont*)GetHoeEngine()->Create(value.GetStringValue());
	}
	else if (HoeCore::string::cmp(prop, "align_horizontal")==0)
	{
		switch (value.GetStringValue()[0])
		{
		case 'l':
			m_ax = ALeft; break;
		case 'c':
			m_ax = ACenter; break;
		case 'r':
			m_ax = ARight; break;
		};
	}
	else if (HoeCore::string::cmp(prop, "align_vertical")==0)
	{
		switch (value.GetStringValue()[0])
		{
		case 't':
			m_ay = ATop; break;
		case 'c':
			m_ay = ACenter; break;
		case 'b':
			m_ay = ABottom; break;
		};
	}
	else if (HoeCore::string::cmp(prop, "text")==0)
	{
		SetText(value.GetStringValue());
	}
	else
		Item::Set(prop,value);
}

/////////////////////////////////////////////////////////
Button::Button()
{
	m_active = false;
	m_fonttt = NULL;
	m_tt = NULL;
	m_ctrl = NULL;
	m_func = NULL;
}

void Button::Draw(IHoe2D * h2d)
{
	if (m_show)
	{
		h2d->PaintRect(&m_rect, m_active ? 0xffff0000:0x80ff0000, true);
		float widthr = (m_rect.right-m_rect.left) * 0.1f;
		float heightr = (m_rect.bottom-m_rect.top) * 0.1f;
		if (m_pic)
		{
			THoeRect r = m_rect;
			r.left+=widthr;
			r.right-=widthr;
			r.top+=heightr;
			r.bottom-=heightr;
			h2d->Blt(m_pic,&r,0);
		}
	}
}

void Button::Draw2(IHoe2D * h2d)
{
	if (m_show && m_pic && m_fonttt && m_tt && m_active)
	{
		// do leveho horniho nebo praveho dolniho
		THoeFontSize size;
		m_fonttt->GetTextSize(m_tt, &size);
		m_fonttt->DrawText( m_rect.left-size.width, m_rect.top-20, 0xffffffff, 
              m_tt );
		//m_fonttt->DrawText( m_rect.right, m_rect.bottom, 0xffffffff, 
        //      m_tt );
	}
}

void Button::Move(const vfloat x, const vfloat y, bool & act)
{
	if (!act && m_show && m_rect.left <= x && m_rect.right >= x && m_rect.top <= y && m_rect.bottom >= y)
	{
		if (!m_active)
			OnActivate(true);
		m_active = act = true;
	}
	else
	{
		if (m_active)
			OnActivate(false);
		m_active = act = false;
	}
}

bool Button::Click(const vfloat x, const vfloat y)
{
	if (m_show && m_rect.left <= x && m_rect.right >= x && m_rect.top <= y && m_rect.bottom >= y)
	{
		OnClick();
		return true;
	}
	return false;
}

void Button::OnClick()
{
	if (m_ctrl && m_func)
		(m_ctrl->*m_func)(this);
}

void Button::OnActivate(bool active)
{
}

void Button::SetToolTipFont(IHoeFont * fnt)
{
	m_fonttt = fnt;
}
void Button::SetToolTip(const tchar * tt)
{
	m_tt = tt;
}

void Button::Set(const tchar * prop, const HoeCore::Universal & value)
{
	if (HoeCore::string::cmp(prop,"tooltipfont") == 0)
	{
		// load
		SetToolTipFont((IHoeFont*)GetHoeEngine()->Create(value.GetStringValue()));
	}
	else 
		StaticPicture::Set(prop, value);
}

////////////////////////////////////
void ButtonUsr::Set(const tchar * prop, const HoeCore::Universal & value)
{
	if (HoeCore::string::cmp(prop,"picture_act") == 0)
	{
		// load
		HoeCore::String_s<256> buff;
		buff.printf(T("picture %s"), (const tchar *)value);
		m_pic_active = (IHoePicture*)GetHoeEngine()->Create(buff);
	}
	else 
		Button::Set(prop, value);
}

void ButtonUsr::Draw(IHoe2D * h2d)
{
	if (m_show)
	{
		if (m_pic && m_pic_active)
			h2d->Blt(m_active ? m_pic_active:m_pic, &m_rect, 0);
	}
}

} // namespace Gui
END_HOEGAME

