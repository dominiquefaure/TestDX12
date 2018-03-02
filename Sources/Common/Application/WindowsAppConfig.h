#ifndef __WINDOWS_APP_CONFIG_H__
#define __WINDOWS_APP_CONFIG_H__
#include <string>

struct WinAppConfig
{

	std::wstring	m_className;
	std::wstring	m_captionText;

	DWORD	m_dwStyle;
	DWORD	m_dwXPos;
	DWORD	m_dwYPos;
	DWORD	m_dwWidth;
	DWORD	m_dwHeight;

	int		m_iconResID;
	int		m_smallIconResID;

	int		m_menuID;

};


#endif