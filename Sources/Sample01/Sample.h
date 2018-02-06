#pragma once

#include<Windows.h>


#include "BaseApplication.h"


class Sample : public BaseApplication
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