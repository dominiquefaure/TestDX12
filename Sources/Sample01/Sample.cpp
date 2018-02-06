// EmptyProject.cpp : Defines the entry point for the application.
//

#include "Sample.h"
#include <fstream>
#include <vector>

#include "WinResources/Resource.h"

//---------------------------------------------------------------------------------------------
Sample::Sample()
{
}
//---------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------
Sample::~Sample()
{
}
//---------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------
void Sample::OnSetWindowConfig( WinAppConfig& a_config )
{
	a_config.m_captionText									=	L"Sample 1";

	a_config.m_iconResID									=	IDI_SAMPLE;
	a_config.m_smallIconResID								=	IDI_SMALL;

	a_config.m_menuID										=	IDC_SAMPLE;
}
//---------------------------------------------------------------------------------------------

