
#include "StdAfx.h"
#include "shared.h"
#include "utils.h"
#include "config.h"
#include "../include/hoe3d.h"
#include "ref.h"
#include "hoe_stream.h"
#include "hoe_index.h"
#include "hoe_info.h"

RefOpenGL::RefOpenGL()
{
	Con_Print("------ OpenGL  ------");
}

bool RefOpenGL::Init(THoeInitSettings * his)
{
	m_Fullscreen = GetConfig()->IsFullscreen();
	m_Width = GetConfig()->GetWidthView();
	m_Height = GetConfig()->GetHeightView();
#ifdef _WIN32
	//
	m_hWnd = his->win;
	GLuint PixelFormat;
	dword dwExStyle;
	dword dwStyle;			
	RECT WindowRect;

	if (!his->forcewnd)
	{

		if (m_Fullscreen)
		{
			long res;
			DEVMODE dmScreenSettings;								
			memset(&dmScreenSettings,0,sizeof(dmScreenSettings));
			dmScreenSettings.dmSize=sizeof(dmScreenSettings);
			dmScreenSettings.dmPelsWidth	= m_Width;
			dmScreenSettings.dmPelsHeight	= m_Height;	
			dmScreenSettings.dmBitsPerPel	= 16;
			dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

			if ((res = ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN))!=DISP_CHANGE_SUCCESSFUL)
			{
				Con_Print("Error: The Requested Fullscreen Mode (%dx%dx%d) Is Not Supported by Your Video Card. (%d)",
					dmScreenSettings.dmPelsWidth,dmScreenSettings.dmPelsHeight,dmScreenSettings.dmBitsPerPel,res);
				return false;	
			}

			dwExStyle=WS_EX_APPWINDOW;	
			dwStyle=WS_POPUP;
			ShowCursor(FALSE);
		}
		else
		{
			dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;	
			dwStyle=WS_OVERLAPPEDWINDOW;
		}

		AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);	

		if (SetWindowLong(m_hWnd,GWL_EXSTYLE,dwExStyle) == 0)
		{
			Con_Print("error init: %d",GetLastError());
			return false;
		}
		if (SetWindowLong(his->win,GWL_STYLE,dwStyle | WS_CLIPSIBLINGS | WS_CLIPCHILDREN) == 0)
		{
			Con_Print("error init: %d",GetLastError());
			return false;
		}
		if (m_Fullscreen)
			SetWindowPos(m_hWnd,HWND_TOP,0,0,m_Width,m_Height,0);
	}

	static PIXELFORMATDESCRIPTOR pfd=
{
		sizeof(PIXELFORMATDESCRIPTOR),// Velikost struktury
		1,// ��slo verze
		PFD_DRAW_TO_WINDOW |// Podpora okna
		PFD_SUPPORT_OPENGL |// Podpora OpenGL
		PFD_DOUBLEBUFFER,// Podpora Double Bufferingu
		PFD_TYPE_RGBA,// RGBA Format
		16,// Zvol� barevnou hloubku
		0, 0, 0, 0, 0, 0,// Bity barev ignorov�ny
		0,// ��dn� alpha buffer
		0,// Ignorov�n Shift bit
		0,// ��dn� akumula�n� buffer
		0, 0, 0, 0,// Akumula�n� bity ignorov�ny
		24,// 24-bitov� hloubkov� buffer (Z-Buffer)
		0,// ��dn� Stencil Buffer
		0,// ��dn� Auxiliary Buffer
		PFD_MAIN_PLANE,// Hlavn� vykreslovac� vrstva
		0,// Rezervov�no
		0, 0, 0// Maska vrstvy ignorov�na
	};

	if (!(m_hDC=::GetDC(m_hWnd)))
	{
		Con_Print("Error: Can't Create A GL Device Context.");
		return false;
	}

	if (!(PixelFormat=ChoosePixelFormat(m_hDC,&pfd)))
	{
		Con_Print("Error: Can't Find A Suitable PixelFormat.");
		return false;
	}

	if(!SetPixelFormat(m_hDC,PixelFormat,&pfd))
	{
		Con_Print("Error: Can't Set The PixelFormat.");
		return false;
	}

	if (!(m_hRC=wglCreateContext(m_hDC)))
	{
		Con_Print("Error: Can't Create A GL Rendering Context.");
		return false;
	}

	if(!wglMakeCurrent(m_hDC,m_hRC))
	{
		Con_Print("Error: Can't Activate The GL Rendering Context.");
		return false;
	}

	ShowWindow(m_hWnd,SW_SHOW);	
	SetForegroundWindow(m_hWnd);
	SetFocus(m_hWnd);

#endif // _WIN32
#ifdef _LINUX

    XVisualInfo *vi;
    Colormap cmap;
    int dpyWidth, dpyHeight;
    int i;
    int glxMajorVersion, glxMinorVersion;
    int vidModeMajorVersion, vidModeMinorVersion;
    XF86VidModeModeInfo **modes;
    int modeNum;
    int bestMode;
    Atom wmDelete;
    Window winDummy;
    unsigned int borderDummy;
	XSetWindowAttributes attr;

    int attrListDbl[] = { GLX_RGBA,
              GLX_RED_SIZE, 1,
                GLX_GREEN_SIZE, 1,
                GLX_BLUE_SIZE, 1,
                GLX_DEPTH_SIZE, his->depth,
		GLX_DOUBLEBUFFER,
                None };
    


    /* set best mode to current */
    bestMode = 0;
    /* get a connection */
    XF86VidModeQueryVersion(his->dpy, &vidModeMajorVersion,
        &vidModeMinorVersion);
	Con_Print("XF86VidModeExtension-Version %d.%d", vidModeMajorVersion,
        vidModeMinorVersion);

	if (!his->forcewnd)
	{ 

		XF86VidModeGetAllModeLines(his->dpy, his->screen, &modeNum, &modes);
		/* save desktop-resolution before switching modes */
		//GLWin.deskMode = *modes[0];
		/* look for mode with requested resolution */
		for (i = 0; i < modeNum; i++)
		{
			if ((modes[i]->hdisplay == m_Width) && (modes[i]->vdisplay == m_Height))
			{
				bestMode = i;
			}
		}

	}
    /* get an appropriate visual */
    vi = glXChooseVisual(his->dpy, his->screen, attrListDbl);
	if (vi == NULL)
    {
        Con_Print("Error with get double buffered visual.");
		return false;
    }
    glXQueryVersion( his->dpy, &glxMajorVersion, &glxMinorVersion);
    Con_Print("glX-Version %d.%d", glxMajorVersion, glxMinorVersion);
    /* create a GLX context */
	
	m_Ctx = glXCreateContext(his->dpy, vi, None, GL_TRUE);
	if (m_Ctx == NULL)
	{
	        Con_Print("Error create context.");
                return false;
	}
		
	if (!his->forcewnd)
	{ 
		/* create a color map */
		cmap = XCreateColormap(his->dpy, RootWindow(his->dpy, vi->screen),
			vi->visual, AllocNone);
		attr.colormap = cmap;
		attr.border_pixel = 0;		

		if (m_Fullscreen)
		{
			XF86VidModeSwitchToMode(his->dpy, his->screen, modes[bestMode]);
			XF86VidModeSetViewPort(his->dpy, his->screen, 0, 0);
			dpyWidth = modes[bestMode]->hdisplay;
			dpyHeight = modes[bestMode]->vdisplay;
			Con_Print("Resolution %dx%d\n", dpyWidth, dpyHeight);
			XFree(modes);
	    
			/* create a fullscreen window */
			attr.override_redirect = True;
			attr.event_mask = ExposureMask | KeyPressMask | ButtonPressMask |
				StructureNotifyMask;
			his->win = XCreateWindow(his->dpy, RootWindow(his->dpy, vi->screen),
				0, 0, dpyWidth, dpyHeight, 0, vi->depth, InputOutput, vi->visual,
				CWBorderPixel | CWColormap | CWEventMask | CWOverrideRedirect,
				&attr);
			/*XWarpPointer(his->dpy, None, his->win, 0, 0, 0, 0, 0, 0);
			XMapRaised(his->dpy, his->win);
			XGrabKeyboard(his->dpy, his->win, True, GrabModeAsync,
				GrabModeAsync, CurrentTime);
			XGrabPointer(his->dpy, his->win, True, ButtonPressMask,
				GrabModeAsync, GrabModeAsync, his->win, None, CurrentTime);*/
		}
		else
		{
			/* create a window in window mode*/
			attr.event_mask = ExposureMask | KeyPressMask | ButtonPressMask | KeyReleaseMask | ButtonReleaseMask | PointerMotionMask | StructureNotifyMask;
			his->win = XCreateWindow(his->dpy, RootWindow(his->dpy, vi->screen),
				0, 0, m_Width, m_Height, 0, vi->depth, InputOutput, vi->visual,
				CWBorderPixel | CWColormap | CWEventMask, &attr);
			/* only set window title and handle wm_delete_events if in windowed mode */
			XMapRaised(his->dpy, his->win);
		}    
	} // konci force wnd
    /* connect the glx-context to the window */
    if (!glXMakeCurrent(his->dpy, his->win, m_Ctx))
	{
		Con_Print("Error connect context to window");
		return false;
	}
	
	int x,y;
    //XGetGeometry(m_Dpy, his->win, &winDummy, &x, &y,
    //    &his->width, &his->height, &borderDummy, &his->depth);
    
    m_Dpy = his->dpy;
    m_Win = his->win;

#endif // _LINUX

#ifdef _MACOSX

	_tp;
	// mac nahravani
	// his->win
	CGLPixelFormatAttribute ATTRIBUTES[5] =
	{
		kCGLPFAFullScreen,
	kCGLPFASingleRenderer,
kCGLPFADisplayMask, 0,
0
};

CGLError cglError;
ATTRIBUTES[3] = CGDisplayIDToOpenGLDisplayMask(kCGDirectMainDisplay);
CGLPixelFormatObj pixelFormat;
long numberOfPixelFormats;
cglError = CGLChoosePixelFormat(ATTRIBUTES,
&pixelFormat,
&numberOfPixelFormats);
if (cglError != 0 || pixelFormat == NULL)
{
fprintf(stderr, "Can't find pixel format\n");
return false;
}
CGLContextObj context;
cglError = CGLCreateContext(pixelFormat, NULL, &context);
if (cglError != 0 || context == NULL)
{
fprintf(stderr, "Can't create context\n");
CGLDestroyPixelFormat(pixelFormat);
return false;
}

CGLDestroyPixelFormat(pixelFormat);
CGCaptureAllDisplays();
cglError = CGLSetFullScreen(context);
if (cglError != 0)
{
fprintf(stderr, "Can't set context's drawable to screen\n");
CGLDestroyContext(context);
CGReleaseAllDisplays();
return false;
}
cglError = CGLSetCurrentContext(context);
if (cglError != 0)
{
fprintf(stderr, "Can't make context current\n");
CGLClearDrawable(context);
CGLDestroyContext(context);
CGReleaseAllDisplays();
return false;
}
	_tp;
#endif // _MACOSX

	Con_Print("%s",(char *)glGetString(GL_VENDOR));// V�pis v�robce
	Con_Print("%s",(char *)glGetString(GL_RENDERER));// V�pis typu grafick� karty
	Con_Print("GL ver. %s",(char *)glGetString(GL_VERSION));// V�pis verze

	glViewport(0,0,m_Width,m_Height);// Resetuje aktu�ln� nastaven�
	glMatrixMode(GL_PROJECTION);// Zvol� projek�n� matici
	glLoadIdentity();// Reset matice
	glMatrixMode(GL_MODELVIEW);// Zvol� matici Modelview
	glLoadIdentity();// Reset matice

	glDisable(GL_TEXTURE_2D);// Zapne mapov�n� textur
	glShadeModel(GL_SMOOTH);// Jemn� st�nov�n�
	SetBackgroundColor(m_BackColor);
	glClearDepth(1.0f);// Nastaven� hloubkov�ho bufferu
	glEnable(GL_DEPTH_TEST);// Zapne hloubkov� testov�n�
	glDepthFunc(GL_LESS);// Typ hloubkov�ho testov�n�
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);// Nejlep�� perspektivn� korekce

    glGetIntegerv(GL_MAX_ELEMENTS_INDICES, &param.max_indices);
    glGetIntegerv(GL_MAX_ELEMENTS_VERTICES, &param.max_vertices);
    param.max_indices = 500;
    Con_Print("GL: Max drawed vert = %d, indices = %d", param.max_vertices, param.max_indices);
    
	// extensions 
	PrintGlExt();
	LoadExtensions();
	//glPixelStorei(GL_UNPACK_SWAP_BYTES,GL_FALSE);

	return true;
}


bool RefOpenGL::Begin()
{
	return true;
}

void RefOpenGL::End()
{
	// Prohozen� buffer� (Double Buffering)
#ifdef _LINUX
	glXSwapBuffers(m_Dpy, m_Win);

#elif defined (_WIN32)
	SwapBuffers(m_hDC);
#endif
}

void RefOpenGL::ClearBuffers(bool target, bool depth)
{
	glClear((target?GL_COLOR_BUFFER_BIT:0) | (depth?GL_DEPTH_BUFFER_BIT:0));
}

void RefOpenGL::SetBackgroundColor(unsigned long color)
{
	glClearColor((color >> 16 & 0xff) / 255.f,
		(color >> 8 & 0xff) / 255.f,
		(color >> 0 & 0xff) / 255.f,
		0.f);
}

void RefOpenGL::Destroy()
{
#ifdef _LINUX
	if (m_Ctx)
    {
        if (!glXMakeCurrent(m_Dpy, None, NULL))
        {
			Con_Print("Could not release drawing context.");   
        }
        glXDestroyContext(m_Dpy, m_Ctx);
        m_Ctx = NULL;
    }
#endif // _LINUX

#ifdef _WIN32
	if (m_hRC)
	{
        if (!wglMakeCurrent(m_hDC,NULL))
		{
			Con_Print("Error: Release Of DC And RC Failed.");
        }
		if (!wglDeleteContext(m_hRC))
		{
			Con_Print("Error: Release Rendering Context Failed.");
		}
		m_hRC=NULL;
	}
	if (m_hDC && !ReleaseDC(m_hWnd,m_hDC))
	{
		DWORD err = GetLastError();
		Con_Print("Error: Release Device Context Failed. error=%d",err);
	}
    m_hDC=NULL;
	
#endif // _WIN32
	Con_Print("Ref destroyed.");

}

/** check gl buffer */
void checkglbuffer(HoeStream * stream, HoeIndex * index)
{
	// check for index
	for (unsigned short i=0; i < index->GetNumIndices();i++)
	{
		GLvoid* d = (GLvoid*)index->GetIndexBuffer();
		unsigned short *ud = (unsigned short*)d; 
		if (ud[i] < 0 || ud[i] > stream->GetNumVert())
			glerr(__FILE__, __LINE__, "check index data",__FUNCTION__, GL_INVALID_OPERATION);
	}
}

void RefOpenGL::DrawStdObject(HoeStream * stream, HoeIndex * index)
{
	checkglbuffer(stream, index);
	stream->Set(0);
	if (stream->IsDynamic() && GetRef()->ext.EXT_compiled_vertex_array)
	{
		glLockArraysEXT(0, (GLsizei)stream->GetNumVert());
		checkgl("glLockArraysEXT");
	}
	
	glDrawElements(GL_TRIANGLES,index->GetNumIndices(),GL_UNSIGNED_SHORT,(GLvoid*)index->GetIndexBuffer());
	checkgl("glDrawElements");
	if (stream->IsDynamic() && GetRef()->ext.EXT_compiled_vertex_array)
	{
		glUnlockArraysEXT();
		checkgl("glUnlockArraysEXT");
	}
	GetInfo()->AddStatTriangles(index->GetNumIndices()/3);
}

void RefOpenGL::DrawStdObject(HoeStream * stream, HoeIndex * index, dword vert, dword ind)
{
	stream->Set(0);
	if (stream->IsDynamic() && GetRef()->ext.EXT_compiled_vertex_array)
	{
		glLockArraysEXT(0, (GLsizei)vert);
		checkgl("glLockArraysEXT");
	}
	glDrawElements(GL_TRIANGLES,ind,GL_UNSIGNED_SHORT,(GLvoid*)index->GetIndexBuffer());
	checkgl("glDrawElements");
	if (stream->IsDynamic() && GetRef()->ext.EXT_compiled_vertex_array)
	{
		glUnlockArraysEXT();
		checkgl("glUnlockArraysEXT");
	}
	GetInfo()->AddStatTriangles(ind/3);
}

void RefOpenGL::DrawStdObjectFT(HoeStream * stream, HoeIndex * index, dword start, dword num)
{
	stream->Set(0);
	if (stream->IsDynamic() && GetRef()->ext.EXT_compiled_vertex_array)
	{
		glLockArraysEXT(0, (GLsizei)stream->GetNumVert());
		checkgl("glLockArraysEXT");
	}
    // vypisovani po castech
    while (num > 0)
    {
        const dword proc = GetRef()->param.max_indices;
	    word * w = ((word*)index->GetIndexBuffer())+start;
	    glDrawElements(GL_TRIANGLES,num > proc ? proc:num,GL_UNSIGNED_SHORT,(GLvoid*)w);
	    checkgl("glDrawElements");
        start += proc;
        if (num > proc) num -= proc; else num = 0;
    }
	if (stream->IsDynamic() && GetRef()->ext.EXT_compiled_vertex_array)
	{
		glUnlockArraysEXT();
		checkgl("glUnlockArraysEXT");
	}
	GetInfo()->AddStatTriangles(num/3);
}

void RefOpenGL::DrawIndex(HoeIndex * index, dword offset, dword count)
{
	byte * ui = reinterpret_cast<byte*>(index->GetIndexBuffer());
	ui += offset * 2;
	glDrawElements(GL_TRIANGLES,count,GL_UNSIGNED_SHORT,(GLvoid*)ui);
	checkgl("glDrawElements");
	GetInfo()->AddStatTriangles(count/3);
}

void RefOpenGL::DrawFanObject(dword offset, dword count)
{
	glDrawArrays(GL_TRIANGLE_FAN, offset, count);
	GetInfo()->AddStatTriangles(count-2);
}

void RefOpenGL::DrawPointObject(class HoeStream * stream, int vertCount)
{
	stream->Set(0);
	glDrawArrays(GL_POINTS, 0, vertCount);
}

void RefOpenGL::DrawLineObject(class HoeStream * stream, int lineCount)
{
	stream->Set(0);
	glDrawArrays(GL_LINES, 0, lineCount);

}








