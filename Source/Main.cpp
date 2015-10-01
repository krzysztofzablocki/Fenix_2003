
/* Windows + DirectX */
#include <windows.h>
#include <d3d9.h>
#include <math.h>

/* Engine */
#include "FNX_Timer.h"
#include "FNX_Logger.h"
#include "FNX_TextureManager.h"
#include "FNX_LineManager.h"
#include "FNX_Input.h" 

/* Engine GUI */
#include "FNX_GUIEditBox.h"
#include "FNX_GUIListBox.h"
#include "FNX_GUIComboBox.h"
#include "FNX_GUICheckBox.h"
#include "FNX_GUIButton.h"
#include "FNX_GUIPanel.h"
#include "FNX_GUIScrollBar.h"

/* Engine Physic */
#include "FNX_PhysicManager.h"


/*  Make the class name into a global variable  */
char szClassName[ ] = "Merowing Engin";




/* WinApi Variables*/
  WNDCLASSEX WindowClass;        /* Data structure for the windowclass */
  HWND hWnd;               /* This is the handle for our window */



/* DirectX Variables*/
	LPDIRECT3D9             g_pD3D       = NULL; // Used to create the D3DDevice
	LPDIRECT3DDEVICE9       g_pD3DDevice = NULL; // Our rendering device
	D3DPRESENT_PARAMETERS   g_pD3Dpp;			     //present parametrs




/* Engine Variables */

 /* Basic Classes */
	FNX_Timer* Timer;
	FNX_Logger* Logger;
	FNX_TextureManager* TextureManager;
	FNX_LineManager* LineManager;
	FNX_Input* Input;
	
 /* GUI Elements */
	FNX_GUIPanel* Panel       = NULL;

/* Physic */
	FNX_PhysicManager* PhysicManager = NULL;
	FNX_Actor*		   BoxActor     = NULL;	
	FNX_Actor*		   LineActor    = NULL;
	FNX_Actor*		   Map[1]	;//= NULL;
	int Counter=0;
	bool Pressed=false;
	FNX_Vector Start;

/* Initialize DirectX */
FNX_Result InitD3D()
{

	if(NULL==(g_pD3D=Direct3DCreate9(D3D_SDK_VERSION)))
		return FNX_ERROR;

	ZeroMemory(&g_pD3Dpp,sizeof(D3DPRESENT_PARAMETERS));
	g_pD3Dpp.Windowed=true;
	g_pD3Dpp.SwapEffect=D3DSWAPEFFECT_DISCARD;
	g_pD3Dpp.BackBufferFormat=D3DFMT_UNKNOWN;

	if(FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,hWnd,D3DCREATE_HARDWARE_VERTEXPROCESSING,&g_pD3Dpp,&g_pD3DDevice)))
	{
		if(FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,hWnd,D3DCREATE_SOFTWARE_VERTEXPROCESSING,&g_pD3Dpp,&g_pD3DDevice)))
			if(FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_SW,hWnd,D3DCREATE_SOFTWARE_VERTEXPROCESSING,&g_pD3Dpp,&g_pD3DDevice)))
				if(FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_REF,hWnd,D3DCREATE_SOFTWARE_VERTEXPROCESSING,&g_pD3Dpp,&g_pD3DDevice)))
					return FNX_ERROR;
	}
		

	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING,FALSE);

	return FNX_OK;
};



/* Sets Up Matrices for 2d scene */
void FNX_SetupMatrices2d(LPDIRECT3DDEVICE9 pDevice)
{
	RECT Window;
	GetClientRect(hWnd,&Window);
	D3DXMATRIX m_pixel,m_view; 
	D3DXMatrixOrthoLH(&m_pixel,(FLOAT)Window.right,(FLOAT)Window.bottom,1.0f,1000.0f);//pixel projection
	D3DXMatrixIdentity(&m_view);
	pDevice->SetTransform(D3DTS_PROJECTION,&m_pixel);
	pDevice->SetTransform(D3DTS_VIEW,&m_view);
};


#define SafeRelease(x) if(x){x->Release();x=NULL;};

/* Cleans up after application */
  void Cleanup()
  {
	  SafeRelease(g_pD3D);
	  SafeRelease(g_pD3DDevice);
	  SafeRelease(TextureManager);
	  SafeRelease(LineManager);
	  SafeRelease(Input);
	  SafeRelease(Panel);
	  SafeRelease(PhysicManager);
	  SafeRelease(Logger);
  };




/* Invalidate Resources...*/
  void Invalidate() 
  {
	  LineManager->Invalidate();

	  Panel->Invalidate();
  };





/* Restore Resources...*/
  HRESULT Restore()
  {
	  LineManager->Restore();

	  Panel->Restore();

	g_pD3DDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
    g_pD3DDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
    g_pD3DDevice->SetRenderState( D3DRS_ZENABLE,  FALSE );
	g_pD3DDevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );
	  return S_OK;
  };





/* Restore Device...*/
HRESULT RestoreDevice()
 {
	HRESULT Result=g_pD3DDevice->TestCooperativeLevel();
	if (Result==D3DERR_DEVICELOST) Invalidate();

	while(Result==D3DERR_DEVICELOST)
	{
		while(Result!=D3DERR_DEVICENOTRESET)
		{
			Sleep(1000);
			Result=g_pD3DDevice->TestCooperativeLevel();
		}

		if FAILED(g_pD3DDevice->Reset(&g_pD3Dpp)) Result=D3DERR_DEVICELOST; else
			return Restore();
	}

	return S_OK;
 };





/* Restore Window (when size is changed) */

void WindowRestore()
{
	if(!g_pD3DDevice)return;

	RECT Window;
	GetClientRect(hWnd,&Window);
	g_pD3Dpp.BackBufferHeight=Window.bottom;
	g_pD3Dpp.BackBufferWidth=Window.right;

	Invalidate();
	if SUCCEEDED(g_pD3DDevice->Reset(&g_pD3Dpp))Restore();
};





/* Take care of mouse focus in window mode */
void ProtectMouseFocus()
{

RECT WindowRect;
GetWindowRect(hWnd,&WindowRect);
float x=Input->MousePos.x;
float y=Input->MousePos.y;

if(x>(WindowRect.right-WindowRect.left)/2.0f || x<-(WindowRect.right-WindowRect.left)/2.0f || y>(WindowRect.bottom-WindowRect.top-25)/2.0f || y<-(WindowRect.bottom-WindowRect.top)/2.0f)
if(Input->IsAcquired() && !BUTTONDOWN(Input->MouseState,0) && !BUTTONDOWN(Input->MouseState,1))
	{
		Input->UnacquireMouse();
		ReleaseCapture();
		SetCursorPos((int)(FNX_Vector((float)WindowRect.left,(float)WindowRect.top+24)+FNX_TranslatePosLHToOffScreenLH(hWnd,x,y)).x,(int)(FNX_Vector((float)WindowRect.left,(float)WindowRect.top+24)+FNX_TranslatePosLHToOffScreenLH(hWnd,x,y)).y);
	};

POINT s;
GetCursorPos(&s);
x=(float)s.x;y=(float)s.y;

if(x>=WindowRect.left && x<=WindowRect.right && y<=WindowRect.bottom && y>=WindowRect.top+25)
if(!Input->IsAcquired())
	{
		Input->AcquireMouse();
		SetCapture(hWnd);
		Input->MousePos=(FNX_Vector((float)-WindowRect.left,(float)(WindowRect.top+24))+FNX_TranslatePosOffScreenLHToLH(hWnd,x,y));
	};

};

/* Add Line to map terrain */
void AddLine(int index,FNX_Vector Start, FNX_Vector End)
{
		FNX_LineShapeDesc Shape2;
		FNX_ActorDesc LineActorDesc2;
		Shape2.Start=Start;		 // LOCAL COORDINATES
		Shape2.End=End;
	
	if(Counter<=0 || (index==1 && Counter==1))
	{
	

	LineActorDesc2.Coords.T=FNX_Vector(0,0);
	LineActorDesc2.Coords.R=FNX_Rotation(0.0f);
	LineActorDesc2.Mass=0.1f;
	LineActorDesc2.Inertia=100.0f;
	
	LineActorDesc2.Shapes.push_back(&Shape2);
    LineActorDesc2.bStatic=true;

	Map[index] = new FNX_Actor(LineActorDesc2);
	PhysicManager->Add(Map[index]);

	Counter++;
	}else
	{
		Map[index]->AddShape(&Shape2);
	};
};

/* Render graphic and also used like main loop function */

HRESULT Render()
{

	float dt=Timer->GetElapsedSeconds();

		Input->Update();
		Panel->Update(Input);

	PhysicManager->AddGlobalForce(FNX_Vector(0,-90.8f),true);


	PhysicManager->Update(dt,Input->MousePos,BUTTONDOWN(Input->MouseState,0));
	

/* Mouse Cursor */
			LineManager->Add(Input->MousePos-FNX_Vector(8,0),Input->MousePos+FNX_Vector(8,0),D3DCOLOR_XRGB(255,110,0));
			LineManager->Add(Input->MousePos-FNX_Vector(0,8),Input->MousePos+FNX_Vector(0,8),D3DCOLOR_XRGB(255,110,0));



	g_pD3DDevice->Clear(0,NULL,D3DCLEAR_TARGET,D3DCOLOR_XRGB(20,20,20),1.0f,0);


	if SUCCEEDED(g_pD3DDevice->BeginScene())
	{
		
			g_pD3DDevice->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_LINEAR);
			g_pD3DDevice->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_LINEAR);
			
			g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);
			g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

			FNX_SetupMatrices2d(g_pD3DDevice);



	


	FNX_Vector s[4];
	for(int i=0;i<(int)BoxActor->Shapes.size();++i)
	{
	GetVertexsFromData(BoxActor->Shapes[i]->GetWorldCoords().R,BoxActor->Shapes[i]->GetWorldCoords().T,FNX_Vector(BoxActor->Shapes[i]->GetSize().x*2.0f,BoxActor->Shapes[i]->GetSize().y*2.0f),s);
	LineManager->Add(s[0],s[2],D3DCOLOR_XRGB(255,110,0));
	LineManager->Add(s[0],s[1],D3DCOLOR_XRGB(255,110,0));
	LineManager->Add(s[2],s[3],D3DCOLOR_XRGB(255,110,0));
	LineManager->Add(s[1],s[3],D3DCOLOR_XRGB(255,110,0));	
	};


	for(int i=0;i<(int)LineActor->Shapes.size();++i)
	{
	GetVertexsFromData(LineActor->Shapes[i]->GetWorldCoords().R,LineActor->Shapes[i]->GetWorldCoords().T,FNX_Vector(LineActor->Shapes[i]->GetSize().x*2.0f,LineActor->Shapes[i]->GetSize().y*2.0f),s);
	LineManager->Add(s[0],s[2],D3DCOLOR_XRGB(255,110,0));
	LineManager->Add(s[0],s[1],D3DCOLOR_XRGB(255,110,0));
	LineManager->Add(s[2],s[3],D3DCOLOR_XRGB(255,110,0));
	LineManager->Add(s[1],s[3],D3DCOLOR_XRGB(255,110,0));	
	};




	for(int j=0;j<Counter;++j)
	for(int i=0;i<(int)Map[j]->Shapes.size();++i)
	{
		GetVertexsFromData(Map[j]->Shapes[i]->GetWorldCoords().R,Map[j]->Shapes[i]->GetWorldCoords().T,FNX_Vector(Map[j]->Shapes[i]->GetSize().x*2.0f,2),s);
		LineManager->Add(s[0],s[2],D3DCOLOR_XRGB(255,0,110));
	};



	if(BUTTONDOWN(Input->MouseState,1))
	{
		if(!BUTTONDOWN(Input->oldMouseState,1))Start=Input->MousePos;
		LineManager->Add(Start,Input->MousePos,D3DCOLOR_XRGB(0,255,0));
	};

	if(!BUTTONDOWN(Input->MouseState,1) && BUTTONDOWN(Input->oldMouseState,1))
	{
		
		AddLine(0,Start,Input->MousePos);

	};

	LineManager->Add(FNX_Vector(-5,0),FNX_Vector(5,0),D3DCOLOR_XRGB(0,100,100));

	PhysicManager->Render();
		Panel->Render();
			LineManager->Render();
			

		g_pD3DDevice->EndScene();
	};


	ProtectMouseFocus();


	return g_pD3DDevice->Present( NULL, NULL, NULL, NULL );

};





void KeyboardFunc(char* buffer)
{

};


/* Pure basic for engine */
FNX_Result BasicSetting()
{
	Timer = new FNX_Timer();
	Logger->SetD3DDevice(g_pD3DDevice);
	
	TextureManager = new FNX_TextureManager(Logger);
	LineManager = new FNX_LineManager(Logger);
	

	Input= new FNX_Input(Logger,true,true,true);
	Input->SetKeyboardFunction(KeyboardFunc);


	return FNX_OK;
};





/* Creating and setting app basics */
FNX_Result OnAppCreate()
{
	if(BasicSetting()!=FNX_OK)return FNX_ERROR;


	

	Panel = new FNX_GUIPanel();
	Panel->SetPosition(FNX_Vector(0,0));

	
	PhysicManager = new FNX_PhysicManager(Logger);

	FNX_ActorDesc ActorDesc2;
	FNX_BoxShapeDesc BoxShape2;
	FNX_BoxShapeDesc BoxShape;
	ActorDesc2.bStatic=false;
	ActorDesc2.Coords.T.y+=50;
	ActorDesc2.Mass=1.0f;
	ActorDesc2.Inertia=1500.0f;

	BoxShape.Extents=FNX_Vector(40,10);
	BoxShape2.Extents=FNX_Vector(10,40);

	ActorDesc2.Shapes.push_back(&BoxShape);
	ActorDesc2.Shapes.push_back(&BoxShape2);

	BoxActor = new FNX_Actor(ActorDesc2);
	
	PhysicManager->Add(BoxActor);


	FNX_LineShapeDesc LineShape;
	FNX_ActorDesc ActorDesc;
	ActorDesc.Coords.Identity();
	LineShape.Start.Set(-50,0);
	LineShape.End.Set(50,0);
	LineShape.BrushSize=10.0f;

	ActorDesc.Inertia=1000.0f;
	ActorDesc.Mass=1.0f;
	ActorDesc.Shapes.push_back(&LineShape);
	
	LineActor = new FNX_Actor(ActorDesc);
	PhysicManager->Add(LineActor);
	
	PhysicManager->SetMouseDrag(true);
	return FNX_OK;
};






















  
/*  This function is called by the Windows function DispatchMessage()  */
LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)                  /* handle the messages */
    {
        case WM_DESTROY:
			Cleanup();
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;

        case WM_CHAR:
			Input->TakeOverChars(wParam);
			break;

		case WM_SIZE:
			WindowRestore();
			break;

        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}







int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nFunsterStil)
{
  
    MSG msg;            /* Here messages to the application are saved */
  

    /* The Window structure */
    WindowClass.hInstance = hThisInstance;
    WindowClass.lpszClassName = szClassName;
    WindowClass.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    WindowClass.style = CS_DBLCLKS;                 /* Catch double-clicks */
    WindowClass.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    WindowClass.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    WindowClass.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    WindowClass.hCursor = LoadCursor (NULL, IDC_ARROW);
    WindowClass.lpszMenuName = NULL;                 /* No menu */
    WindowClass.cbClsExtra = 0;                      /* No extra bytes after the window class */
    WindowClass.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default color as the background of the window */
    WindowClass.hbrBackground = (HBRUSH) COLOR_BACKGROUND;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&WindowClass))
        return 0;

    /* The class is registered, let's create the program*/
    hWnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           "Merowing Engine Test",       /* Title Text */
           WS_OVERLAPPEDWINDOW, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           800,                 /* The programs width */
           600,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hWnd, nFunsterStil);
	



Logger=new FNX_Logger();
 Logger->SethWnd(hWnd);
  Logger->SethInstance(hThisInstance);
   Logger->BeginSection("Merowing Engine");
  
if(Logger->Log("Initializing D3D",InitD3D())!=FNX_OK)return 0;///Initializing device failed



 if(OnAppCreate()!=FNX_OK)return 0;



	ZeroMemory(&msg,sizeof(msg));
    /* Run the message loop. It will run until GetMessage() returns 0 */
   	while( msg.message!=WM_QUIT )
		{
			if( PeekMessage( &msg, 0, 0U, 0U, PM_REMOVE ) )
			{
				TranslateMessage( &msg );
				DispatchMessage( &msg );
			}
			else
			{
				if(Render()==D3DERR_DEVICELOST)
				{
					///logger
					Logger->Log("Device Lost",FNX_ERROR);
					RestoreDevice();
				};


			};
		};





    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return (int)msg.wParam;
}

