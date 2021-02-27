// MyPlayer.cpp : Defines the entry point for the application.
//

// This is a very basic work on creating a simple video player on windows using DirectShow and Win32 APIs
// Very basic win32 programming with directshow filter graph to play the video along with some basic commands defined in WndProc()

// Author: Mufti Md

#include "stdafx.h"
#include "MyPlayer.h"
#include <dshow.h>

#pragma comment(lib, "Strmiids.lib")

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;					// current instance
TCHAR szTitle[MAX_LOADSTRING];				// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM			MyRegisterClass(HINSTANCE hInstance);
BOOL			InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

#define APPLICATIONNAME	"DirectShow Player"
#define CLASSNAME	"DShow"
#define WM_GRAPHNOTIFY	WM_USER + 13
#define RELEASE(x) { if (x) x->Release(); x = NULL;}

IGraphBuilder	*pGraphBuilder = NULL;
IMediaControl	*pMediaControl = NULL;
IMediaEventEx	*pEvtEx = NULL;
IVideoWindow	*pVideoWindow = NULL;

HWND			hWindow = NULL;

void EnumFilters(IGraphBuilder *pGraphBuilder)
{
	HRESULT hr = S_FALSE;
	IEnumFilters *pEnumFilter = NULL;
	IBaseFilter *pBaseFilter = NULL;
	hr = pGraphBuilder->EnumFilters(&pEnumFilter);
	if(SUCCEEDED(hr))
	{
		while (pEnumFilter->Next(1, &pBaseFilter, NULL) == S_OK) 
		{
			FILTER_INFO filterInfo;
			char szName[MAX_PATH];
			pBaseFilter->QueryFilterInfo(&filterInfo);
			WideCharToMultiByte(CP_ACP, 0, filterInfo.achName, -1, szName, MAX_PATH, 0, 0);
			MessageBoxA(hWindow, szName, "Filter Name", MB_OK);
			filterInfo.pGraph->Release();
			pBaseFilter->Release();
		}

		pEnumFilter->Release();
	}
}

void PlayFile(LPWSTR szFile)
{
	WCHAR wFile[MAX_PATH];

#ifndef UNICODE
	MultiByteToWideChar(CP_ACP, 0, szFile, -1, wFile, MAX_PATH);
#else
	lstrcpy(wFile, szFile);
#endif

	CoInitialize(NULL);

	HRESULT hr = S_FALSE;
	hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, reinterpret_cast<void **>(&pGraphBuilder));
	if(SUCCEEDED(hr))
	{
		pGraphBuilder->QueryInterface(IID_IMediaControl, reinterpret_cast<void **>(&pMediaControl));
		pGraphBuilder->QueryInterface(IID_IMediaEventEx, reinterpret_cast<void **>(&pEvtEx));
		pGraphBuilder->QueryInterface(IID_IVideoWindow, reinterpret_cast<void **>(&pVideoWindow));

		pVideoWindow->put_Owner((OAHWND)hWindow);
		pVideoWindow->put_WindowStyle(WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

		// Build the graph
		hr = pGraphBuilder->RenderFile(wFile, NULL);

		// Check filters in the graph
		EnumFilters(pGraphBuilder);
		
		if(SUCCEEDED(hr))
			pMediaControl->Run();
	}

}

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_MYPLAYER, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MYPLAYER));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style		= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon		= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MYPLAYER));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_MYPLAYER);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   hWindow = hWnd;

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case ID_FILE_OPEN:
			PlayFile(L"SomeVideoFile.wmv"); // File source embedded inside the code
			break;
		case ID_FILE_PAUSE:
			pMediaControl->Pause();
			break;
		case ID_FILE_STOP:
			pMediaControl->Stop();
			break;
		case ID_FILE_PLAY:
			pMediaControl->Run();
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		RELEASE(pMediaControl);
		RELEASE(pEvtEx);
		RELEASE(pGraphBuilder);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
