
#include "StdAfx.h"
#include "../include/hoe_win32.h"

BEGIN_HOEGAME 

HoeWin32::HoeWin32(HOE_INSTANCE hInst)/* : HoeBaseApp()*/
{
	this->m_hInstance = hInst;
	m_hWnd = NULL;
	m_enginstance = NULL;
}

HoeWin32::~HoeWin32()
{
}


LRESULT CALLBACK HoeWin32::WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return GET_THIS(HoeWin32)->MsgProc(hwnd, message, wParam, lParam);
}

bool HoeWin32::RegisterApp()
{
     WNDCLASS     wndclass ;
     
     wndclass.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC; //CS_HREDRAW | CS_VREDRAW ;
     wndclass.lpfnWndProc   = WndProc ;
     wndclass.cbClsExtra    = 0 ;
     wndclass.cbWndExtra    = 0 ;
     wndclass.hInstance     = m_hInstance;
     wndclass.hIcon         = LoadIcon (NULL, IDI_APPLICATION) ;
     wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
     wndclass.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH) ;
     wndclass.lpszMenuName  = NULL ;
     wndclass.lpszClassName = this->GetAppName();
     
     return RegisterClass (&wndclass) != FALSE;
}

bool HoeWin32::CreateWin(const char * title, int width, int height, bool fs)
{
	if (fs)
	{
		m_hWnd = CreateWindow (this->GetAppName(), title,
							WS_OVERLAPPEDWINDOW,
							CW_USEDEFAULT, CW_USEDEFAULT,
							640,480,//CW_USEDEFAULT, CW_USEDEFAULT,
							NULL, NULL, m_hInstance, NULL) ;
	}
	else
	{
		m_hWnd = CreateWindow (this->GetAppName(), title,
							WS_OVERLAPPEDWINDOW,
							CW_USEDEFAULT, CW_USEDEFAULT,
							width,height,//CW_USEDEFAULT, CW_USEDEFAULT,
							NULL, NULL, m_hInstance, NULL) ;
	}

	if (!m_hWnd)
	{
		return false;
	}

	 ShowWindow (m_hWnd, SW_SHOWNORMAL) ;
	 //AnimateWindow(m_hWnd,200,AW_SLIDE|AW_HOR_POSITIVE);

	 UpdateWindow (m_hWnd) ;

     return true;
}

void HoeWin32::HandleError()
{
	MSG	msg ;

	UpdateConsole();

	while (GetMessage( &msg, NULL, 0U, 0U ))
    {
		if (msg.message == WM_QUIT || (msg.message == WM_KEYUP && msg.wParam == VK_ESCAPE))
			return;

        TranslateMessage (&msg) ;
		DispatchMessage (&msg) ;
	}
}

void HoeWin32::UpdateConsole()
{
	InvalidateRect(m_hWnd,NULL,TRUE);
}

void HoeWin32::SetTitle(const char * title)
{
	assert(m_hWnd);
	SetWindowText(m_hWnd, title);
}

int HoeWin32::GetMsg(IHoe3DEngine * eng)
{
	MSG	msg ;

	m_enginstance = eng;

	while (PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ))
    {
        TranslateMessage (&msg) ;
		DispatchMessage (&msg) ;

		if (msg.message == WM_QUIT)
			return 0;
	}

	return 1;
}

int HoeWin32::ShowMsg(const char * msg)
{
	MessageBox(NULL,msg,"Error:", MB_OK);
	return 0;
}

void HoeWin32::Destroy()
{
	if (m_hWnd)
		DestroyWindow(m_hWnd);
	m_hWnd = NULL;
	
	UnregisterClass( this->GetAppName(), m_hInstance );
}

void HoeWin32::PaintConsole(Console * con)
{
	HDC         hdc ;
     PAINTSTRUCT ps ;

		 /* if (Becher::GetState() != Becher::sInit)
		 {
			 ValidateRect(hwnd,NULL);
			 break;
		 }*/
			  int numlines = (m_cyClient-5) / m_cyChar;
				if (numlines > con->GetCount())
					numlines = con->GetCount();

				int y = ((numlines-1) * m_cyChar) + 5;
				int n = 0;

			  hdc = BeginPaint (m_hWnd, &ps) ;

           // Nalezen� hranic kreslen�
			  
				while (y > 0)
				{
					const char * str = con->GetLine(n++);
					TextOut (hdc, 5, y,	str,lstrlen (str)) ;
					y -= m_cyChar;
				}

	          EndPaint (m_hWnd, &ps) ;
}

LRESULT CALLBACK HoeWin32::MsgProc (HWND hwnd,UINT message, WPARAM wParam, LPARAM lParam)
{
     HDC         hdc ;
     TEXTMETRIC  tm ;
     
     switch (message)
     {
     case WM_CREATE:
          hdc = GetDC (hwnd) ;
          
          GetTextMetrics (hdc, &tm) ;
          m_cyChar = tm.tmHeight + tm.tmExternalLeading ;
          
          ReleaseDC (hwnd, hdc) ;

          return 0 ;
     case WM_SIZE:
         m_cyClient = HIWORD (lParam);
		this->OnSize(LOWORD (lParam), HIWORD (lParam));

         break;
      case WM_PAINT :
		this->OnPaint();
          return 0 ;

     case WM_DESTROY :
		 PostQuitMessage (0) ;
          return 0 ;

     }

	if(m_enginstance)
		m_enginstance->WndProc(hwnd, message, wParam, lParam);

     return DefWindowProc (hwnd, message, wParam, lParam) ;
}


END_HOEGAME
