#pragma once

#include <Windows.h>


#include "Application/GameApplication.h"

class Sample : public GameApplication
{
public:

	Sample();
	~Sample();



// pure Virtual methods to be overrided
protected:
	
	/*
	 * Configure the Windows Dialog
	 */
	virtual void OnSetWindowConfig( WinAppConfig& a_config );

private:



// Fields
private:


};