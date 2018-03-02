#ifndef __WINMAIN_LAUNCHER_H__
#define __WINMAIN_LAUNCHER_H__

#include <Windows.h>

#include "WindowsAppConfig.h"
#include "GameApplication.h"



class WinMainLauncher
{
public:
	WinMainLauncher();
	~WinMainLauncher();
	


	int Run( GameApplication* a_application , HINSTANCE a_hInstance, int a_nCmdShow );

// MEthods
private:

	BOOL Init( GameApplication* a_application , HINSTANCE a_hInstance, int a_nCmdShow );

	/*
	* Register the WNDCLASSEX Class
	*/
	BOOL RegisterWndClass( );


	BOOL CreateWindowDlg( int a_nCmdShow );

//// Fields
private:
	HINSTANCE     m_hInst;
	HWND          m_hWnd;


	TCHAR			m_strClass[MAX_PATH];
	TCHAR			m_strCaption[MAX_PATH];
	WNDCLASSEX		m_stWcex;


	WinAppConfig		m_config;

	GameApplication*	m_application;
};

#endif