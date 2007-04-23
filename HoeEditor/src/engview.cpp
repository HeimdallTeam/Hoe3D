
#include "StdAfx.h"
#include "../include/he/app.h"
#include "../include/he/editor.h"
#include "../include/he/engview.h"
#include "../../HoeGame/include/hoe_utils.h"
#include "../../HoeGame/include/hoe_engine.h"

namespace HoeEditor {

// event handler
EngineEventHandler * EngineEventHandler::_this = NULL;

EngineEventHandler::EngineEventHandler()
{
	assert(_this == NULL);
	_this = this;
}

EngineEventHandler::~EngineEventHandler()
{
	assert(_this == this);
	_this = NULL;
}

BEGIN_EVENT_TABLE(EngineView, wxWindow)
	EVT_PAINT(EngineView::OnPaint)
	EVT_MOTION(EngineView::OnMouseMove)
	EVT_MOUSEWHEEL(EngineView::OnMouseWheel)
	EVT_LEFT_DOWN(EngineView::OnMouseDown)
	EVT_LEFT_UP(EngineView::OnMouseUp)
	EVT_RIGHT_DOWN(EngineView::OnMouseDown)
	EVT_RIGHT_UP(EngineView::OnMouseUp)
	EVT_KEY_DOWN(EngineView::OnKeyDown)
	EVT_KEY_UP(EngineView::OnKeyUp)
	EVT_CHAR(EngineView::OnChar)
	EVT_ENTER_WINDOW( EngineView::OnEnterWindow )
	EVT_LEAVE_WINDOW( EngineView::OnLeaveWindow )
	EVT_SET_FOCUS(EngineView::OnSetFocus)
	EVT_KILL_FOCUS( EngineView::OnKillFocus ) 
	EVT_SIZE(EngineView::OnSize)
	EVT_SIZING(EngineView::OnSizing)
END_EVENT_TABLE()

EngineView * EngineView::g_shared = NULL;

EngineView::EngineView()
{
	assert(g_shared == NULL);
	g_shared = this;
#ifndef HOE_STATIC_ENGINE
	m_dllpath[0] = '\0';
#endif
	m_loaded = false;
	SetBackgroundColour(wxColor(57,67,140));
}

EngineView::~EngineView()
{
	assert(g_shared == this);
	g_shared = NULL;
}

#ifndef HOE_STATIC_ENGINE
void EngineView::SetEnginePath(const char * path)
{
	strncpy(m_dllpath, path, sizeof(m_dllpath) - 1);
}
#endif

#ifdef HOE_STATIC_ENGINE
bool EngineView::InitStatic(XHoeFS * hfs)
{
	wxLogMessage("* Load Engine *");
	this->m_engine = CreateHoeEngine(HOESDK_VERSION, App::Get()->GetConsole(),hfs,NULL,0,HOEF_NOINPUT);
#else
bool EngineView::Init(XHoeFS * hfs, int sdkver)
{
	bool ret;
	BEGIN_TRY
		ret = InitUntry(hfs, sdkver);
	END_TRY(ret = false);
#ifndef HOE_STATIC_ENGINE
	if (!ret)
		m_lib.Unload();
#endif
	return ret;
}

bool EngineView::InitUntry(XHoeFS * hfs, int sdkver)
{
	Unload();
	if (sdkver != HOESDK_VERSION)
	{
		wxLogMessage("Bad version for HoeEditor interface (req: %.2f editor: %.2f)",sdkver * 0.01f,HOESDK_VERSION * 0.01f);
		return false;
	}

	assert(strlen(m_dllpath) > 0);
	
	HOE_FUNCCREATE GetEngineInterface;
	
	if (!m_lib.Load(m_dllpath))
	{
		wxLogMessage(_("Failed load library: %s"),m_dllpath);
		return false;
	}

#ifdef _LINUX
    const char * engsym = "CreateHoeEngine";
#else
    const char * engsym = "_CreateHoeEngine@24";
#endif
	GetEngineInterface = (HOE_FUNCCREATE)m_lib.GetSymbol(engsym);
	if (!GetEngineInterface)
	{
		wxLogMessage(_("Find symbol GetEngineInterface failed."));
		return false;
	}

	wxLogMessage("* Load Engine from %s *",m_dllpath);
	HoeGame::g_hoeengine = GetEngineInterface(HOESDK_VERSION,App::Get()->GetConsole(),hfs,NULL,0,HOEF_NOINPUT);
#endif
	if (HoeGame::g_hoeengine == NULL)
		return false; 

	{
	THoeInitSettings his;
	memset(&his,0,sizeof(THoeInitSettings));
	his.hInst = NULL;

#ifdef __WXGTK__
	// ftp://biolpc22.york.ac.uk/pub/contrib/glcanvas/glcanvs2.zip
	GdkWindow * bwin = GTK_PIZZA(m_wxwindow)->bin_window;
	his.dpy = GDK_WINDOW_XDISPLAY( bwin );
	his.win = GDK_WINDOW_XWINDOW( bwin );
	his.screen = DefaultScreen(his.dpy);//GDK_WINDOW_SCREEN( m_wxwindow->window);
	//his.screen = GDK_SCREEN_XSCREEN(m_wxwindow->screen);
#else
	his.dpy = NULL;
	his.win = (HOE_WINDOW)this->GetHandle();
#endif

	wxSize s = GetClientSize();

	his.width = s.GetWidth();
	his.height = s.GetHeight();
	his.fullscreen = false;
	his.forcewnd = true; 
	his.depth = 16;

	wxLogMessage("* Init Engine with %dx%dx%d *",his.width,his.height,his.depth);

	if (!HoeGame::g_hoeengine->Init(&his))
		return false;
	}
	m_loaded = true;

	// arial
	HoeGetRef(HoeGame::g_hoeengine)->SetBackgroundColor(0xff000000);
	
	return true;
}

static wxString str; /*!!!*/ /** @todo odstranit tuhleprasarnicku */

void EngineView::Frame(float dtime)
{
	if (!this->IsLoaded())
		return;


	//BEGIN_TRY

	assert(HoeGame::GetHoeEngine());
	HoeGame::GetHoeEngine()->Process(dtime);
	HoeGame::GetHoeEngine()->Frame();

	// update fps
	// info

	//float fps = HoeGetInfo(m_engine)->GetFPS();
	str.Printf("FPS: %f", HoeGetInfo(HoeGame::GetHoeEngine())->GetFPS());
	App::Get()->GetEditor()->SetStatusText(str, 1);

	//END_TRY(exit(0));
}


void EngineView::Unload()
{
	if (!this->IsLoaded())
		return;
	
	assert(HoeGame::GetHoeEngine());

	HoeGame::GetHoeEngine()->Destroy();
	HoeGame::g_hoeengine = NULL;
#ifndef HOE_STATIC_ENGINE
	m_lib.Unload();
#endif

	m_loaded = false;
}

void EngineView::OnPaint(wxPaintEvent& event)
{
 	wxPaintDC dc(this);
	if (IsLoaded())
	{
		//this->Frame(0);
	}
	else
	{
		dc.SetTextForeground(*wxWHITE);
        dc.DrawLabel(_("Engine not loaded."), GetClientRect(), wxALIGN_CENTER);
	}
	event.Skip();
}

void EngineView::OnSize( wxSizeEvent& event)
{
	/*int x = 0;
	int y = 0;
	GetClientSize( &x, &y );
	y = (3 * x) / 4;
	SetClientSize(x,y);*/
	if (m_loaded)
	{
		assert(HoeGame::GetHoeEngine());
		int x = 0;
		int y = 0;
		GetClientSize( &x, &y );

		HoeGame::GetHoeEngine()->Resize(x,y);
		Frame(0);
	}
	else
		Refresh();
}

void EngineView::OnSizing( wxSizeEvent& event)
{
	event;
	/*if (m_loaded)
	{
		assert(m_engine);
		int x = 0;
		int y = 0;
		GetClientSize( &x, &y );

		m_engine->Resize(x,y);
		Frame(0);
	}
	else
		Refresh();*/
}


void EngineView::OnEnterWindow( wxMouseEvent& event )
{
    SetFocus();
	EngineEventHandler::Get()->MouseEnter(event.GetX(), event.GetY());
    m_lastmouseevent = event;
	event.Skip();
}

void EngineView::OnLeaveWindow( wxMouseEvent& event )
{
	EngineEventHandler::Get()->MouseLeave();
	event.Skip();
}


void EngineView::OnMouseMove(wxMouseEvent& event)
{
	EngineEventHandler::Get()->MouseMove(event.GetX()-m_lastmouseevent.GetX(),event.GetY()-m_lastmouseevent.GetY(),
			event.GetX(),event.GetY(), event);
	/*if (m_active)
	{
	if (event.ControlDown())
			flag |= HoePlugin::MControl;
		if (event.MetaDown())
			flag |= HoePlugin::MMeta;
		if (event.AltDown())
			flag |= HoePlugin::MAlt;
		if (event.ShiftDown())
			flag |= HoePlugin::MShift;
		if (event.CmdDown())
			flag |= HoePlugin::MCmd;
		if (event.MiddleIsDown())
			flag |= HoePlugin::MMiddle;
		if (event.RightIsDown())
			flag |= HoePlugin::MRight;
		if (event.LeftIsDown())
			flag |= HoePlugin::MLeft;

		if (event.GetWheelRotation() != 0)
		{
			m[0] = event.GetWheelRotation();
        	m_active->Msg(HoePlugin::EPM_EngMouseWheel,flag,m);
		}

		m_active->Msg(HoePlugin::EPM_EngMouseMove,flag,m);

	}*/
	m_lastmouseevent = event;
	event.Skip();
}

void EngineView::OnMouseUp(wxMouseEvent& event)
{
	if (event.LeftUp())
		EngineEventHandler::Get()->MouseLeftUp(event.GetX(),event.GetY(),event);
	if (event.RightUp())
		EngineEventHandler::Get()->MouseRightUp(event.GetX(),event.GetY(),event);
	
    /*if (m_active)
	{
		float m[4] = {event.GetX()-m_lastmouseevent.GetX(),event.GetY()-m_lastmouseevent.GetY(),
			event.GetX(),event.GetY()};
		int flag = 0;;

		if (event.ControlDown())
			flag |= HoePlugin::MControl;
		if (event.MetaDown())
			flag |= HoePlugin::MMeta;
		if (event.AltDown())
			flag |= HoePlugin::MAlt;
		if (event.ShiftDown())
			flag |= HoePlugin::MShift;
		if (event.CmdDown())
			flag |= HoePlugin::MCmd;
		if (event.MiddleUp())
			flag |= HoePlugin::MMiddle;
		if (event.RightUp())
			flag |= HoePlugin::MRight;
		if (event.LeftUp())
			flag |= HoePlugin::MLeft;

		m_active->Msg(HoePlugin::EPM_EngMouseUp,flag,m);

	}*/
	m_lastmouseevent = event;
	event.Skip();
}

void EngineView::OnMouseDown(wxMouseEvent& event)
{
	if (event.LeftDown())
		EngineEventHandler::Get()->MouseLeftDown(event.GetX(),event.GetY(),event);
	if (event.RightDown())
		EngineEventHandler::Get()->MouseRightDown(event.GetX(),event.GetY(),event);
    
	/*if (m_active)
	{
		float m[4] = {event.GetX()-m_lastmouseevent.GetX(),event.GetY()-m_lastmouseevent.GetY(),
			event.GetX(),event.GetY()};
		int flag = 0;;

		if (event.ControlDown())
			flag |= HoePlugin::MControl;
		if (event.MetaDown())
			flag |= HoePlugin::MMeta;
		if (event.AltDown())
			flag |= HoePlugin::MAlt;
		if (event.ShiftDown())
			flag |= HoePlugin::MShift;
		if (event.CmdDown())
			flag |= HoePlugin::MCmd;
		if (event.MiddleDown())
			flag |= HoePlugin::MMiddle;
		if (event.RightDown())
			flag |= HoePlugin::MRight;
		if (event.LeftDown())
			flag |= HoePlugin::MLeft;

		m_active->Msg(HoePlugin::EPM_EngMouseDown,flag,m);

	}*/
	m_lastmouseevent = event;
	event.Skip();
}

void EngineView::OnMouseWheel(wxMouseEvent& event)
{
	EngineEventHandler::Get()->MouseWheel(event);
}

void EngineView::OnKeyDown(wxKeyEvent& event)
{
	EngineEventHandler::Get()->KeyDown(event);

/*
        case WXK_NUMPAD_DECIMAL: key = _T("NUMPAD_DECIMAL"); break;
        case WXK_NUMPAD_DIVIDE: key = _T("NUMPAD_DIVIDE"); break;

        default:
        {
            if ( keycode < 256 )
            {
                if ( keycode == 0 )
                    key.Printf(_T("NUL"));
                else if ( keycode < 27 )
                    key.Printf(_T("Ctrl-%c"),
                                (unsigned char)(_T('A') + keycode - 1));
                else
                    key.Printf(_T("'%c'"), (unsigned char)keycode);
            }
            else
            {
                key.Printf(_T("unknown (%ld)"), keycode);
            }
        } 
*/

	//EngineEventHandler::Get()->SetTitle();
	event.Skip();
}

void EngineView::OnKeyUp(wxKeyEvent& event)
{
	EngineEventHandler::Get()->KeyUp(event);
	event.Skip();
}

void EngineView::OnChar(wxKeyEvent& event)
{
	event.Skip();
}

void EngineView::OnSetFocus(wxFocusEvent &event)
{
    //GetApp()->GetMainFrame()->SetTitle("focused");
	event.Skip();
}


void EngineView::OnKillFocus(wxFocusEvent &even)
{
    //GetApp()->GetMainFrame()->SetTitle("unfocused");
    //event.Skip();
}

};




